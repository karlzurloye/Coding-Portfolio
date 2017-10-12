#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <signal.h>
#include <curses.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

#define A 0.003 // complementary filter constant
#define NEUTRAL_P 1300 // starting point for motor PWM
#define KP 7 // proportional constant
#define KD 500 // differential constant
#define KI 0.01 // integral control

#define KP_Y 4
#define KI_Y 0.005

// 0.01, 1250, 1, 100, 0.0025
// 0.003, 1300, 7, 500, 0.01
// 3, 300, 0.01
// 1300, 4, 300, 0.005

#define frequency 25000000.0
#define LED0 0x6			//LED0 start register
#define LED0_ON_L 0x6		//LED0 output and brightness control byte 0
#define LED0_ON_H 0x7		//LED0 output and brightness control byte 1
#define LED0_OFF_L 0x8		//LED0 output and brightness control byte 2
#define LED0_OFF_H 0x9		//LED0 output and brightness control byte 3
#define LED_MULTIPLYER 4	// For the other 15 channels
int execute,pwm;

//init the pwm board
void init_pwm(int pwm)
{
    float freq =400.0*.95;
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;
    uint8_t prescale = floor(prescaleval+0.5);
    int settings = wiringPiI2CReadReg8(pwm, 0x00) & 0x7F;
    int sleep	= settings | 0x10;
    int wake 	= settings & 0xef;
    int restart = wake | 0x80;
    wiringPiI2CWriteReg8(pwm, 0x00, sleep);
    wiringPiI2CWriteReg8(pwm, 0xfe, prescale);
    wiringPiI2CWriteReg8(pwm, 0x00, wake);
    delay(10);
    wiringPiI2CWriteReg8(pwm, 0x00, restart|0x20);
}

//turn on the motor
void init_motor(int pwm,uint8_t channel)
{
	int on_value=0;

	int time_on_us=900;
	uint16_t off_value=round((time_on_us*4096.f)/(1000000.f/400.0));

	wiringPiI2CWriteReg8(pwm, LED0_ON_L + LED_MULTIPLYER * channel, on_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_ON_H + LED_MULTIPLYER * channel, on_value >> 8);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_L + LED_MULTIPLYER * channel, off_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_H + LED_MULTIPLYER * channel, off_value >> 8);
	delay(100);

	 time_on_us=1200;
	 off_value=round((time_on_us*4096.f)/(1000000.f/400.0));

	wiringPiI2CWriteReg8(pwm, LED0_ON_L + LED_MULTIPLYER * channel, on_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_ON_H + LED_MULTIPLYER * channel, on_value >> 8);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_L + LED_MULTIPLYER * channel, off_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_H + LED_MULTIPLYER * channel, off_value >> 8);
	delay(100);

	 time_on_us=1000;
	 off_value=round((time_on_us*4096.f)/(1000000.f/400.0));

	wiringPiI2CWriteReg8(pwm, LED0_ON_L + LED_MULTIPLYER * channel, on_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_ON_H + LED_MULTIPLYER * channel, on_value >> 8);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_L + LED_MULTIPLYER * channel, off_value & 0xFF);
	wiringPiI2CWriteReg8(pwm, LED0_OFF_H + LED_MULTIPLYER * channel, off_value >> 8);
	delay(100);

}

//set the pwm value of the motor
void set_PWM(int pwm, uint8_t channel, float time_on_us)
{
	uint16_t off_value=round((time_on_us*4096.f)/(1000000.f/400.0));
	wiringPiI2CWriteReg16(pwm, LED0_OFF_L + LED_MULTIPLYER * channel,off_value);
}

//kill motors
void kill_motor(int pwm,uint8_t channel)
{
    int on_value=0;
    
    int time_on_us=1000;
    uint16_t off_value=round((time_on_us*4096.f)/(1000000.f/400.0));
    
    wiringPiI2CWriteReg8(pwm, LED0_ON_L + LED_MULTIPLYER * channel, on_value & 0xFF);
    wiringPiI2CWriteReg8(pwm, LED0_ON_H + LED_MULTIPLYER * channel, on_value >> 8);
    wiringPiI2CWriteReg8(pwm, LED0_OFF_L + LED_MULTIPLYER * channel, off_value & 0xFF);
    wiringPiI2CWriteReg8(pwm, LED0_OFF_H + LED_MULTIPLYER * channel, off_value >> 8);
    
}

//when cntrl+c pressed, kill motors
void trap(int signal)
{
    delay(10);
    set_PWM(pwm,0,1000);
    delay(10);
    set_PWM(pwm,1,1000);
    delay(10);
    set_PWM(pwm,2,1000);
    delay(10);
    set_PWM(pwm,3,1000);
    delay(100);
    delay(10);
    set_PWM(pwm,0,1000);
    delay(100);
    set_PWM(pwm,1,1000);
    delay(100);
    set_PWM(pwm,2,1000);
    delay(100);
    set_PWM(pwm,3,1000);
    delay(100);
    
    kill_motor(pwm,0);
    delay(100);
    kill_motor(pwm,1);
    delay(100);
    kill_motor(pwm,2);
    delay(100);
    kill_motor(pwm,3);
    delay(100);
    
    execute=0;
    printf("ending program\n\r");
}

struct data
{
    int keypress;
    float pitch;
    float roll;
    float yaw;
    float thrust;
    int sequence_num;
    bool b; //heartbeat
};

int main (int argc, char *argv[])
{
    //open file for writing to if needed
	FILE *f;
	f=fopen("data.csv","w");
	execute=1;

	signal(SIGINT, &trap);
    struct timespec te;
    int address;
    int mag,imu;
    //int data;
	int display;
    
    //shared memory init
    int segment_id;
    data* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;
    int smhkey=33222;

    // Basic initialization of curses lib
    initscr();
    cbreak();
    noecho(); // Set this for interactive programs.
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    
    // my initializations
    nodelay(stdscr,TRUE);
    
    // Allocate a shared memory segment.
    segment_id = shmget (smhkey, shared_segment_size,IPC_CREAT | 0666);
    // Attach the shared memory segment.
    shared_memory = (data*) shmat(segment_id, 0, 0);
    // Determine the segment's size.
    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    
    int frame=0;
    int counter = 0; // counts up over time (used for heartbeat kill command)
    bool ref_bool = TRUE; // reference for checking if heartbeat has toggled
    //end of shared memory init
    
    wiringPiSetup();

    //setup for pwm
    pwm=wiringPiI2CSetup(0x40);  //connect pwm board to imu
    imu=wiringPiI2CSetup(0x6B) ; //accel/gyro address

    if(imu==-1||pwm==-1)
    {
            printf("cant connect to I2C device %d %d\n",imu,pwm);
            // Detach the shared memory segment.
            shmdt(shared_memory);
            return -1;
    }
    else
    {
    	//turn on imu by writing 0xC0 to address 0x10 and 0x20
        wiringPiI2CWriteReg8(imu,0x20,0xC0); //turn on and set accell output
        wiringPiI2CWriteReg8(imu,0x10,0xC0);  //turn on and set gyro output
    	
        //start the pwm
        init_pwm(pwm);

        //init motors
        init_motor(pwm,0);
        init_motor(pwm,1);
	init_motor(pwm,2);
	init_motor(pwm,3);
		
		//declarations and initializations for imu
        float pitch_angle=0;
        float roll_angle=0;
        float yaw_angle=0;
        float roll_angle_accel=0;
        float pitch_angle_accel=0;
        long time_curr;
        long time_prev;
        float delta_x_rotate=0;
        float delta_y_rotate=0;
        float delta_z_rotate=0;
        float z_rate_average = 0;
        float roll_filtered = 0;
        float prev_roll_filtered = 0;
        float prev_pitch_filtered = 0;
        float pitch_filtered = 0;
        float i_term_roll = 0;
        float i_term_pitch = 0;
        float i_term_yaw = 0;
        float roll_desired = 0;
        float pitch_desired = 0;
        float yaw_rate_desired = 0;
        float neutral = NEUTRAL_P;
        bool motors_on = true;
        bool calibrate = false;
        int calibration_counter = 0;
        float roll_angle_accel_cal = 0;
        float roll_angle_cal = 0;
        float pitch_angle_accel_cal = 0;
        float pitch_angle_cal = 0;
        float yaw_angle_cal = 0;

        while(execute==1)
        {
            //command reading
            data s=*shared_memory;
            
            //update desired roll, pitch, yaw rate
            roll_desired = s.roll;
            pitch_desired = s.pitch;
            yaw_rate_desired = s.yaw;
	    neutral = s.thrust;
            
            //keyboard commands
            if(s.sequence_num!=frame)
            {
                //printf("data received %d\n\r",s.a);
                if (s.keypress == 97) {   // ascii a (decrease roll)
                    roll_desired -= 2.5;
                } else if(s.keypress == 100) { // ascii d (increase roll)
                    roll_desired += 2.5;
                } else if(s.keypress == 119) { // ascii w (increase pitch)
                    pitch_desired += 2.5;
                } else if(s.keypress == 115) { // ascii s (decrease pitch)
                    pitch_desired -= 2.5;
                } else if(s.keypress == 32) { // joystick a (kill motors and program)
                    execute = 0;
                } else if(s.keypress == 104) { // ascii h (increase thrust)
                    neutral += 10;
                } else if(s.keypress == 110) { // ascii n (decrease thrust)
                    neutral -= 10;
                } else if(s.keypress == 120) { // ascii x (set pitch and roll and yaw rate to 0)
                    pitch_desired = 0;
                    roll_desired = 0;
                    yaw_rate_desired = 0;
                } else if(s.keypress == 122) { // ascii z (set thurst to default)
                    neutral = NEUTRAL_P;
                } else if(s.keypress == 113) { // ascii q (decrease yaw rate)
                    yaw_rate_desired -= 3;
                } else if(s.keypress == 101) { // ascii e (increase yaw rate)
                    yaw_rate_desired += 3;
                } else if(s.keypress == 33) { // joystick b (turn off motors)
                    motors_on = false;
                } else if(s.keypress == 34) { // joystick x (turn on motors to default)
                    float roll_desired = 0;
                    float pitch_desired = 0;
                    neutral = NEUTRAL_P;
                    motors_on = true;
                } else if(s.keypress == 35) { // joystick y (IMU calibration)
                    calibrate = true;
                    calibration_counter = 0;
                    motors_on = false;
                }
    
                frame=s.sequence_num;
            }
            
            if(counter >= 200){ // kills motors and program if controller stops responding
                printf("Exiting! (client heartbeat)");
                execute = 0;
                shmdt (shared_memory);
            }
            
            if(ref_bool != s.b){ // resets kill counter if hearbeat toggle is detected
                counter = 0;
                ref_bool = s.b;
            } else {
                counter ++;
            }
            
            //read in gyro
            int x_rate=wiringPiI2CReadReg16(imu,0x18);
            int y_rate=wiringPiI2CReadReg16(imu,0x1A);
            int z_rate=wiringPiI2CReadReg16(imu,0x1C);
            //read in accelerometer
            int x_accel=wiringPiI2CReadReg16(imu,0x28);
            int y_accel=wiringPiI2CReadReg16(imu,0x2A);
            int z_accel=wiringPiI2CReadReg16(imu,0x2C);
            //convert all to 2's complement
            if(x_rate>0x8000)
            {
                x_rate=x_rate ^ 0xffff;
                x_rate=-x_rate-1;

            }
            if(y_rate>0x8000)
            {
                y_rate=y_rate ^ 0xffff;
                y_rate=-y_rate-1;

            }
            if(z_rate>0x8000)
            {
                z_rate=z_rate ^ 0xffff;
                z_rate=-z_rate-1;

            }
            if(x_accel>0x8000)
            {
                x_accel=x_accel ^ 0xffff;
                x_accel=-x_accel-1;

            }
            if(y_accel>0x8000)
            {
                y_accel=y_accel ^ 0xffff;
                y_accel=-y_accel-1;

            }
            if(z_accel>0x8000)
            {
                z_accel=z_accel ^ 0xffff;
                z_accel=-z_accel-1;

            }
            // convert accelerometer to g's
            // this was done for debugging purposes, but not necessary for atan2 input
            float x = x_accel*(2.0/32757.0);
            float y = y_accel*(2.0/32757.0);
            float z = z_accel*(2.0/32757.0);
            
            // yaw calibration value
            z_rate-=z_rate_average;
            // convert gyro data to dps
            float x_angle_dps = (float) x_rate*(245.0/32767.0);
            float y_angle_dps = (float) y_rate*(245.0/32767.0);
            float z_angle_dps = (float) z_rate*(245.0/32767.0);
			// convert linear accel data to roll and pitch angles in degrees
			roll_angle_accel = -180.0/3.14*atan2(y_accel,z_accel); // negative so in same direction as gyro
			pitch_angle_accel = 180.0/3.14*atan2(x_accel,z_accel);

            //get current time in nanoseconds
            timespec_get(&te,TIME_UTC);
            time_curr=te.tv_nsec;

            //compute time since last execution
            float diff=time_curr-time_prev;
            //check for rollover
            if(diff<=0)
            {
                diff+=1000000000;
            }
            //convert to seconds
            diff=diff/1000000000;
            
            //calibration
            if (calibrate) {
                roll_angle_cal += x_angle_dps;
                pitch_angle_cal += y_angle_dps;
                yaw_angle_cal += z_angle_dps;
                roll_angle_accel_cal += roll_angle_accel;
                pitch_angle_accel_cal += pitch_angle_accel;
                
                calibration_counter++;
                
                if (calibration_counter > 1000) {
                    calibrate = false;
                    roll_angle_cal /= 1000;
                    pitch_angle_cal /= 1000;
                    yaw_angle_cal /= 1000;
                    roll_angle_accel_cal /= 1000;
                    pitch_angle_accel_cal /= 1000;
                    
                    //motors_on = true;
                    
                    roll_angle = 0;
                    pitch_angle = 0;
                    yaw_angle = 0;
                    roll_filtered = 0;
                    pitch_filtered = 0;

		    printf("calibration complete\n");
                }
            }
            
            //calibrating data
            x_angle_dps -= roll_angle_cal;
            y_angle_dps -= pitch_angle_cal;
            z_angle_dps -= yaw_angle_cal;
            roll_angle_accel -= roll_angle_accel_cal;
            pitch_angle_accel -= pitch_angle_accel_cal;

            //compute amount of rotation since last execution
            delta_x_rotate=x_angle_dps*diff;
            delta_y_rotate=y_angle_dps*diff;
            delta_z_rotate=z_angle_dps*diff;

            //compute cumulative yaw, pitch, roll
            roll_angle += delta_x_rotate;
            pitch_angle += delta_y_rotate;
            yaw_angle += delta_z_rotate;
            
            //complementary filter for roll and pitch
            roll_filtered = roll_angle_accel*A + (1-A)*(delta_x_rotate + roll_filtered);
            pitch_filtered = pitch_angle_accel*A + (1-A)*(delta_y_rotate + pitch_filtered);

			signal(SIGINT, &trap);

            if(execute==1)//if cntrl+c has not been pressed
            {
                
                if(motors_on) {
                
                //kill command sent after this angle of tilt or program hang-up
                int roll_limit = 30;
		int yaw_rate_limit = 400;
                if ((roll_filtered > roll_limit) || (roll_filtered < -roll_limit) || (pitch_filtered > roll_limit) || (pitch_filtered < -roll_limit) || (z_angle_dps > yaw_rate_limit) || (z_angle_dps < -yaw_rate_limit) || (diff > 1E9)) {
		    printf("tilt: %f\n",pitch_filtered);
		    printf("yaw rate: %f\n",z_angle_dps);
		    printf("loop: %f\n",diff);
                    execute = 0;
                }
                else {
                    
                    //velocity calculations for D control
                    float roll_velocity = roll_filtered - prev_roll_filtered;
                    float pitch_velocity = pitch_filtered - prev_pitch_filtered;
                    
                    //integral calculations for I control
                    i_term_roll += KI*(roll_filtered-roll_desired);
                    i_term_pitch += KI*(pitch_filtered-pitch_desired);
                    int i_limit = 50;
                    if (i_term_roll > i_limit) i_term_roll = i_limit;
                    if (i_term_roll < -i_limit) i_term_roll = -i_limit;
                    if (i_term_pitch > i_limit) i_term_pitch = i_limit;
                    if (i_term_pitch < -i_limit) i_term_pitch = -i_limit;
                    
                    //yaw PI control
                    i_term_yaw += KI_Y*(z_angle_dps-yaw_rate_desired);
                    int yaw_i_limit = 25;
                    if (i_term_yaw > yaw_i_limit) i_term_yaw = yaw_i_limit;
                    if (i_term_yaw < -yaw_i_limit) i_term_yaw = -yaw_i_limit;
                    float yaw_term = KP_Y*(z_angle_dps-yaw_rate_desired) + i_term_yaw;
                    
                    //PID controller
                    int PWM0 = neutral + KP*(roll_filtered-roll_desired) + KD*roll_velocity + i_term_roll - yaw_term + 50;
                    int PWM1 = neutral - KP*(roll_filtered-roll_desired) - KD*roll_velocity - i_term_roll - yaw_term;
                    int PWM2 = neutral + KP*(pitch_filtered-pitch_desired) + KD*pitch_velocity + i_term_pitch + yaw_term - 100;
                    int PWM3 = neutral - KP*(pitch_filtered-pitch_desired) - KD*pitch_velocity - i_term_pitch + yaw_term + 100;
                    
                    //motor PWM limits
                    if (PWM0 > 1500)
                        PWM0 = 1500;
                    if (PWM0 < 1000)
                        PWM0 = 1000;
                    if (PWM1 > 1500)
                        PWM1 = 1500;
                    if (PWM1 < 1000)
                        PWM1 = 1000;
                    if (PWM2 > 1500)
                        PWM2 = 1500;
                    if (PWM2 < 1000)
                        PWM2 = 1000;
                    if (PWM3 > 1700)
                        PWM3 = 1700;
                    if (PWM3 < 1000)
                        PWM3 = 1000;
                		
                    set_PWM(pwm,0,PWM0); 
                    set_PWM(pwm,1,PWM1); 
                    set_PWM(pwm,2,PWM2);
                    set_PWM(pwm,3,PWM3);

        	 //set_PWM(pwm,0,1200); 
                   // set_PWM(pwm,1,1400); 
                    //set_PWM(pwm,2,1100); //edited
                    //set_PWM(pwm,3,1200); //edited
                    
                    prev_roll_filtered = roll_filtered;
                    prev_pitch_filtered = pitch_filtered;
                }
                    
                } else {
                    set_PWM(pwm,0,1000);
                    set_PWM(pwm,1,1000);
                    set_PWM(pwm,2,1000);
                    set_PWM(pwm,3,1000);
                }
            }
            else
            {
				trap(0);
                // Detach the shared memory segment.
                shmdt(shared_memory);
				return 0;
            }
            
            //remember the current time
            time_prev=time_curr;
        }
        if(execute==0)//put at end of main loop
        {
            set_PWM(pwm,0,1000);
            delay(100);
            set_PWM(pwm,1,1000);
            delay(100);
            set_PWM(pwm,2,1000);
            delay(100);
            set_PWM(pwm,3,1000);
            delay(100);
            printf("motors set to 1000 for exit\n\r");
        }
  	 }
  	 	trap(0);
        // Detach the shared memory segment.
        shmdt(shared_memory);
        return 0;
}
