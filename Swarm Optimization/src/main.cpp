#pragma warning(disable:4996)

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include "robot.h"
#include "kilobot.cpp"
#include <chrono>
#include <thread>

#define SIMPLEBMP_OPENGL 
#include "simplebmp.h"
using namespace std;

#define buffer_size 1000000
#define channels 2 
//#define delay 10 //delay between time steps, use if program is too fast
#define windowWidth 600 //display window
#define windowHeight 700 //display window
#define comm_noise_std 5 //standard dev. of sensor noise
#define PI 3.14159265358979324
#define twicePi  2 * PI
#define radius 16 //radius of a robot
#define p_control_execute .99 // probability of a controller executing its time step
#define SKIPFRAMES 0
#define shuffles 20
#define circledef 30
// Global vars.
double time_sim;  //simulation time
double zoom, view_x, view_y; //var. for zoom and scroll

int num_robots = 25; //number of robots running

void Measure_metric();

robot** robots;//creates an array of robots
int* safe_distance;
int* order;

int delay = 0;
int draw_delay=100;
FILE *results;

char log_buffer[255];
char log_file_buffer[buffer_size];


bool log_debug_info = true;
char log_file_name[255] = "simulation.log";
bool showscene = true;

char shape_file_name[255] = "";

int total_secs;
int timelimit = 180 * 60;
char rt[100];

int light_center[2];

int food_num = 0;
int food_P = 100;
int food_color_ratio = 50;
#define food_max 20
int food_x[food_max];
int food_y[food_max];
int food_type[food_max];
int food_index = 0;

int yellow_yield_avg = 20;
int blue_yield_avg = 20;

#define food_collection 1000;
#define food_decay 1
int food_storage = 0;
int max_food_storage = 1000000;

double ch[radius];

int arena_width = 2400;
int arena_height = 2400;

int snapshot = 60;
int snapshotcounter = 0;

bool last = false;
bool write_final = false;

unsigned int seed = 0;

int test22[4] = {2, 3,4,5};

// 0-25 hourly per-kilobot collection rate
float rate20[26] = 
{0.00,25.00,14.00,13.50,12.00,10.00,7.17,6.87,6.13,5.55,4.90,4.87,4.67,3.23,3.04,2.93,2.92,2.91,2.90,2.90,2.88,2.86,2.82,2.78,2.57,2.40};
float rate70[26] = 
{0.00,31.00,29.00,25.78,22.75,21.78,20.67,17.32,14.88,13.65,12.90,12.32,11.58,10.00,9.89,9.77,9.70,9.65,8.55,7.58,7.25,6.95,6.90,6.80,6.55,6.25};
float rate130[26] ={0.00,32.00,31.00,28.59,27.00,24.98,22.83,21.37,19.38,18.98,18.40,18.02,17.67,15.61,16.43,15.07,14.12,13.06,12.76,12.58,12.48,12.12,12.02,11.91,11.55,10.76};
float rate180[26] = {0.00,35.00,32.50,29.23,26.25,25.82,24.17,21.58,22.50,21.71,20.00,19.88,19.42,18.69,18.15,17.67,16.72,15.94,15.27,14.84,14.81,14.76,14.06,13.65,13.43,13.16};


void strcpy_safe(char *m, int l, char *s)

{
	for (int i = 0; i <= l && s; i++)
		*m = *s;
}

void log_info(char *s)
{
	static char *m = log_file_buffer;
	if (s)
	{
		int l = strlen(s) + 1;
		strcpy(m, s);
		m += l - 1;
	}
	if (m - log_file_buffer >= buffer_size-255 || !s)
	{
		results = fopen(log_file_name, "a");
		fprintf(results, "%s", log_file_buffer);
		fclose(results);
		m = log_file_buffer;
	}
}
//check to see if motion causes robots to collide
int find_collisions(int id, double x, double y)
{
	double two_r = 2 * radius;
	int i;
	if (x <= radius || x >= arena_width - radius || y <= radius || y >= arena_height - radius) return 1;
	double x_ulim = x + two_r;
	double x_llim = x - two_r;
	double y_ulim = y + two_r;
	double y_llim = y - two_r;
	for (i = 0;i < num_robots;i++)
	{
		if (i != id)
		{
			if (safe_distance[id*num_robots+i])
			{
				safe_distance[id*num_robots + i]--;
			}
			else
			{
				double dist_x = x - robots[i]->pos[0];
				double dist_y = y - robots[i]->pos[1];
				if (x_ulim > robots[i]->pos[0] && x_llim<robots[i]->pos[0] &&
					y_ulim>robots[i]->pos[1] && y_llim < robots[i]->pos[1]) //if not in the sqare limits, i dont even check the circular ones
				{

					double distance = sqrt(dist_x*dist_x + dist_y * dist_y);
					if (distance < two_r)
					{
						return 1;
					}
				}
				else
				{
					double bd = 0;
					if (fabs(dist_x)>fabs(dist_y))
					{
						bd = fabs(dist_x);
					}
					else
					{
						bd = fabs(dist_y);
					}
					if (bd > two_r+20)
					{
						double speed = robots[id]->speed + robots[i]->speed;
						if (speed > 0)
						{
							safe_distance[id*num_robots + i] = (int)((bd - (two_r + 20)) / speed);
						}
						else
						{
							safe_distance[id*num_robots + i] = 1000000;
						}
						safe_distance[i*num_robots + id] = safe_distance[id*num_robots + i];
					}
				}
			}
		}
	}
	return 0;
}

void save_bmp(const char *fileName)
{

	// The width and the height, would be the width
	// and height of your current scene.

	SimpleBMP bmp(windowWidth, windowHeight);

	bmp.glReadPixels();
	bmp.save(fileName);
}

bool run_simulation_step()
{
	static int lastrun = 0;
	lastrun++;

	total_secs = lastrun / SECOND;
	
	int secs = total_secs % 60;
	int mins = (total_secs / 60) % 60;
	int hours = total_secs / 3600;
	sprintf(rt, "%02d:%02d:%02d", hours, mins, secs);

	int i, j;

	double rotation_step = .05;//motion step size
							   //run a step of most or all robot controllers
	for (i = 0;i < num_robots;i++)
	{
		//run controller this time step with p_control_execute probability
		if ((rand())<(int)(p_control_execute*RAND_MAX))
		{
			robots[i]->robot_controller();
		}
	}

	//update angle to light for robots
	for(i=0;i<num_robots;i++)
	{
		robots[i]->angle_to_light=atan2(light_center[1]-robots[i]->pos[1],light_center[0]-robots[i]->pos[0])-robots[i]->pos[2];
	}

	//update distance to light for robots
	for(i=0;i<num_robots;i++)
	{
		robots[i]->dist_to_light=sqrt(pow(light_center[1]-robots[i]->pos[1],2)+pow(light_center[0]-robots[i]->pos[0],2));
	}
	
	//check for eaten food
	for(i=0;i<num_robots;i++) {
		if (robots[i]->food_eaten) {	
			robots[i]->food_eaten = false;
			if (food_x[robots[i]->food_num] != 10000) {
				food_num--;
				food_x[robots[i]->food_num] = 10000;
				food_y[robots[i]->food_num] = 10000;
			}
		}
	}

	//identify nearest food + distance and angle for robots
	for(i=0;i<num_robots;i++)
	{
		int food_num = 0;
		int food_num_closest = 0;
		float dist_min = 10000; 
		for(j=0;j<food_max;j++) {	
			float food_dist = sqrt(pow(food_y[j]-robots[i]->pos[1],2)+pow(food_x[j]-robots[i]->pos[0],2));
			if ((food_dist < dist_min) && (robots[i]->goal_type == food_type[j])) {
				dist_min = food_dist;
				food_num_closest = j;
			}
		}
		robots[i]->food_num = food_num_closest;
		robots[i]->dist_to_food = dist_min;
		robots[i]->angle_to_food=atan2(food_y[food_num_closest]-robots[i]->pos[1],
			food_x[food_num_closest]-robots[i]->pos[0])-robots[i]->pos[2];
	}

	//make new food
	int food_dice = rand() % 30000;
	if((food_num <= food_max) && (food_dice < food_P)) {
		food_index++;
		if (food_index >= food_max) food_index = 0;
		if (food_x[food_index] == 10000) {
			food_num++;
		}
		food_x[food_index] = rand() % 2200 + 100;
		food_y[food_index] = rand() % 2200 + 100;
		
		int food_color_dice = rand() % 100;
		if (food_color_dice < food_color_ratio) food_type[food_index] = 0;
		else food_type[food_index] = 1;
	}
	
	//update food storage for robots
	for(i=0;i<num_robots;i++) {
		robots[i]->food_storage_count = food_storage;
		robots[i]->food_storage_max = max_food_storage;
	}
	
	//check for returned food
	for(i=0;i<num_robots;i++) {
		if (robots[i]->food_returned) {	
			robots[i]->food_returned = false;
			food_storage += food_collection;
		}
	}
	
	//food eaten by swarm
	food_storage -= food_decay;
	
	int seed;
	seed = (rand() % shuffles) * num_robots;
	//let robots communicate
	for (i = 0;i < num_robots;i++)
	{
		int index = order[seed + i];
		robot *rs = robots[index];
		//if robot wants to communicate, send message to all robots within distance comm_range
		void *msg = rs->get_message();
		if (msg)
		{
			for (j = 0;j < num_robots;j++)
			{
				robot *rd = robots[j];
				if (j != index)
				{
					double range = rs->comm_out_criteria(rd->pos[0], rd->pos[1], safe_distance[index * num_robots + j]);
					//printf("rz\n\r");

					if (range)
					{
						float theta=0;
						theta=atan2(rs->pos[1]-rd->pos[1],rs->pos[0]-rd->pos[0])-rd->pos[2];


						if (rd->comm_in_criteria(rs->pos[0], rs->pos[1], range, theta ,msg))
						{
							rs->received();
							//printf("rs\n\r");
							//break;
						}
					}
				}
			}
		}
	}

	seed = (rand() % shuffles) * num_robots;
	//move robots
	for (i = 0;i < num_robots;i++)
	{
		int index = order[seed + i];
		robot *r = robots[index];

		double t = r->pos[2];
		double s = 0;
		switch (r->motor_command)
		{
		case 1:
		{
			t += r->motor_error;
			s = r->speed;
			break;
		}
		case 2:
		{
			t += rotation_step;
			s = r->speed;
			if (r->pos[2] > twicePi)
			{
				r->pos[2] -= twicePi;
			}
			break;
		}
		case 3:
		{
			t -= rotation_step;
			s = r->speed;
			if (r->pos[2] < 0)
			{
				r->pos[2] += twicePi;
			}
			break;
		}
		}
		double temp_x = s*cos(t) + r->pos[0];
		double temp_y = s*sin(t) + r->pos[1];
		if (find_collisions(index, temp_x, temp_y) == 0)
		{
			r->pos[0] = temp_x;
			r->pos[1] = temp_y;
		}
		r->pos[2] = t;
	}
	static int lastsec =- 1;
	bool result = false;
/*
	if ((lastsec!=secs && lastrun>1 && snapshot )|| last)
	{
		if (last)
			cout << "ended\n";
		else
			cout << rt << endl;

		lastsec = secs;
		if (!snapshotcounter || last)
		{
			result = true;
			if (log_debug_info || last)
			{
				char finalMSG[] = "final";
				char buffer[255];
				if (last)
				{
					for (int i = 0;i < num_robots;i++)
						log_info(robots[i]->get_debug_info(buffer, finalMSG));
				}else
				{
					for (int i = 0;i < num_robots;i++)
						log_info(robots[i]->get_debug_info(buffer, rt));
				}
			}
			snapshotcounter = snapshot;
		}
		snapshotcounter--;
	}*/
	
	if(lastrun%draw_delay==0)
		return true;
	return false;
}

// Drawing routine.
void draw_scene(void)
{
	static int snapshottaken = 0;
	static bool takesnapshot = false;
	//draws the arena

	takesnapshot = run_simulation_step();

	
	if(takesnapshot)
	{
		glColor4f(0, 0, 0, 0);
		glRectd(0, 0, arena_width, arena_height);

		glutSetWindowTitle(rt);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1.0);
		glBegin(GL_LINES);
		for (int i = 0; i <= radius; i++)
		{
			for (int j = 0;j < num_robots;j++)
			{
				glColor4f((GLfloat)robots[j]->color[0], (GLfloat)robots[j]->color[1], (GLfloat)robots[j]->color[2], 1.0);
				glVertex2f((GLfloat)(robots[j]->pos[0]-i), (GLfloat)(robots[j]->pos[1]-ch[i]));
				glVertex2f((GLfloat)(robots[j]->pos[0] -i), (GLfloat)(robots[j]->pos[1] + ch[i]));
				glVertex2f((GLfloat)(robots[j]->pos[0] + i), (GLfloat)(robots[j]->pos[1] - ch[i]));
				glVertex2f((GLfloat)(robots[j]->pos[0] + i), (GLfloat)(robots[j]->pos[1] + ch[i]));
			}

			// drawing foods
			for (int j=0; j<food_max; j++)
			{
				if (food_type[j] == 0) glColor4f(1.0, 1.0, 0, 1.0);
				else glColor4f(0, 0, 1.0, 1.0);
				glVertex2f((GLfloat)(food_x[j]-i), (GLfloat)(food_y[j]-ch[i]));
				glVertex2f((GLfloat)(food_x[j] -i), (GLfloat)(food_y[j] + ch[i]));
				glVertex2f((GLfloat)(food_x[j] + i), (GLfloat)(food_y[j] - ch[i]));
				glVertex2f((GLfloat)(food_x[j] + i), (GLfloat)(food_y[j] + ch[i]));
			}

		}
		for (int j = 0;j < num_robots;j++)
		{
			glBegin(GL_LINES);
			glColor4f(0, 0, 0, 1.0);
			glVertex2f((GLfloat)robots[j]->pos[0], (GLfloat)robots[j]->pos[1]);
			glVertex2f((GLfloat)(robots[j]->pos[0] + cos(robots[j]->pos[2])*radius), (GLfloat)(robots[j]->pos[1] + sin(robots[j]->pos[2])*radius));
			if (robots[j]->dest[0] != -1)
			{
				glBegin(GL_LINES);
				glColor4f(1, 1, 1, 1.0);
				glVertex2f((GLfloat)robots[j]->pos[0], (GLfloat)robots[j]->pos[1]);
				glVertex2f((GLfloat)robots[j]->dest[0], (GLfloat)robots[j]->dest[1]);
			}
		}

		glEnd();
		glFlush();

		/*if (takesnapshot)
		{
			snapshottaken++;
			char file[100];
			if (last)
			{
				sprintf(file, "%s.final.bmp", log_file_name);
			}
			else 
			{
				sprintf(file, "%s.%04d.bmp", log_file_name, snapshottaken);
			}
			save_bmp(file);
		}*/

		glutSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	}

	/*if (last)
	{
		log_info(NULL);
		exit(0);
	}
	if (total_secs >= timelimit)
	{
		last = true;
	}*/
}

// Initialization routine.
void setup(void)
{
	for (int i = 0;i < num_robots;i++)
		for (int j = 0;j < shuffles;j++)
			order[i + num_robots*j] = i;

	for (int i = 0;i < num_robots - 1;i++)
		for (int j = 0;j < shuffles;j++)
		{
			int index = j*num_robots + i;
			int r = index + rand() % (num_robots - i);
			int p = order[index];
			order[index] = order[r];
			order[r] = p;
		}
	for (int i = 0;i < num_robots;i++)
		for (int j = 0;j < num_robots;j++)
			safe_distance[i * num_robots + j] = 0;
}

// OpenGL window reshape routine.
void resize_window(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void key_input(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w'://up 
		view_y += 100;
		break;
	case 'a'://up 
		view_x -= 100;
		break;
	case 's'://up 
		view_y -= 100;
		break;
	case 'd'://up 
		view_x += 100;
		break;
	case '-':
		zoom = zoom*1.1;
		break;
	case '+':
		zoom = zoom*0.9;
		break;
	case '1':
		if (delay>0)
			delay--;
		printf("busy delay %d\n\r",delay);
		break;
	case '2':
		delay++;
		printf("busy delay %d\n\r",delay);
		break;
	case '3':
		if (draw_delay>1)
			draw_delay--;
		printf("draw delay %d\n\r",draw_delay);
		break;
	case '4':
		draw_delay++;
		printf("draw delay %d\n\r",draw_delay);
		break;
	case 'k':
		Measure_metric();
		//exit(0);
		break;
	case 'f':
		max_food_storage += 50000;
		printf("max food storage %d\n\r",max_food_storage/10000);
		break;
	case 'g':
		max_food_storage -= 50000;
		if (max_food_storage <= 0) max_food_storage = 50000;
		printf("max food storage %d\n\r",max_food_storage/10000);
		break;	
	case 'r':
		food_P += 10;
		printf("food spawn rate %d\n\r",food_P);
		break;
	case 't':
		food_P -= 10;
		if (food_P < 0) food_P = 0;
		printf("food spawn rate %d\n\r",food_P);
		break;
	case 'b':
		food_color_ratio -= 10;
		if (food_color_ratio < 0) food_color_ratio = 0;
		printf("yellow food : blue food = %d : %d\n\r",food_color_ratio/10, (100-food_color_ratio)/10);
		break;	
	case 'v':
		food_color_ratio += 10;
		if (food_color_ratio > 100) food_color_ratio = 100;
		printf("yellow food : blue food = %d : %d\n\r",food_color_ratio/10, (100-food_color_ratio)/10);
		break;		
	case '0':
		draw_delay = 100;
		printf("draw delay %d\n\r",draw_delay);
		break;
	default:
		break;
	}
}

void on_idle(void) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-zoom + view_x, view_x, -zoom + view_y, view_y, 0.0f, 1.0f);
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	glutPostRedisplay();
}

void setup_positions()
{
	//srand(2);

	light_center[0]=1200;
	light_center[1]=1200;

	int k = 0;
	for (int ii = 0; ii < 5; ii++) {
		for (int jj = 0; jj < 5; jj++) {
			int x = (ii * 240) + 600;
			int y = (jj * 240) + 600; 
			robots[k] = new mykilobot();
			robots[k]->robot_init(x,y,0);
			robots[k]->goal_type = rand() % 2;
			robots[k]->yellow_yield = rand() % yellow_yield_avg + 1;
			robots[k]->blue_yield = rand() % blue_yield_avg + 1;
			robots[k]->id = k;
			k++;
		}
	}	
}

float yellow_rate(int yellow_count)
{
	float yellow_rate;
	if (food_P*2*food_color_ratio/100 < 20) yellow_rate = rate20[yellow_count];
	else if (food_P*2*food_color_ratio/100 < 70) yellow_rate = rate20[yellow_count]*(70-food_P)/50 + rate70[yellow_count]*(food_P-20)/50;
	else if (food_P*2*food_color_ratio/100 < 130) yellow_rate = rate70[yellow_count]*(130-food_P)/60 + rate130[yellow_count]*(food_P-70)/60;
	else if (food_P*2*food_color_ratio/100 < 180) yellow_rate = rate130[yellow_count]*(180-food_P)/50 + rate180[yellow_count]*(food_P-130)/50;
	else yellow_rate = rate180[yellow_count];
	return yellow_rate;
}

float blue_rate(int blue_count) 
{
	float blue_rate;
	if (food_P*2*(100-food_color_ratio)/100 < 20) blue_rate = rate20[blue_count];
	else if (food_P*2*(100-food_color_ratio)/100 < 70) blue_rate = rate20[blue_count]*(70-food_P)/50 + rate70[blue_count]*(food_P-20)/50;
	else if (food_P*2*(100-food_color_ratio)/100 < 130) blue_rate = rate70[blue_count]*(130-food_P)/60 + rate130[blue_count]*(food_P-70)/60;
	else if (food_P*2*(100-food_color_ratio)/100 < 180) blue_rate = rate130[blue_count]*(180-food_P)/50 + rate180[blue_count]*(food_P-130)/50;
	else blue_rate = rate180[blue_count];
	return blue_rate;
}

float reward(int yellow_count) 
{
	int blue_count = 25 - yellow_count;
	float current_reward = 0;
	for (int ii = 0; ii < 25; ii++) {
		if (robots[ii]->yellow_search_P_metric > 50) {
			current_reward += yellow_rate(yellow_count) * robots[ii]->yellow_yield;
		} else {
			current_reward += blue_rate(blue_count) * robots[ii]->blue_yield;
		}
	}
	return current_reward;
}

int wrong_counter(int wrong)
{
	if (wrong >= 5) return wrong;
	int min_wrong = 26;
	
	int yellow_count = 0;
	for (int ii = 0; ii < 25; ii++) if (robots[ii]->yellow_search_P_metric > 50) yellow_count++;
			
	float current_reward = reward(yellow_count);
	for (int ii = 0; ii < 25; ii++) {
		if (robots[ii]->search_P_metric > 15) {
			robots[ii]->yellow_search_P_metric = 100 - robots[ii]->yellow_search_P_metric;
			yellow_count = 0;
			for (int jj = 0; jj < 25; jj++) if (robots[jj]->yellow_search_P_metric > 50) yellow_count++;
			
			if (current_reward < reward(yellow_count)) {
				int test_wrong = wrong_counter(wrong + 1);
				if (test_wrong < min_wrong) min_wrong = test_wrong;
			}
			
			robots[ii]->yellow_search_P_metric = 100 - robots[ii]->yellow_search_P_metric;
			yellow_count = 0;
			for (int jj = 0; jj < 25; jj++) if (robots[jj]->yellow_search_P_metric > 50) yellow_count++;
		}
	}
	if (min_wrong < 26) return min_wrong;
	return wrong;
}

void Measure_metric()
{
			float swarm_blue_avg = 0;
			float swarm_yellow_avg = 0;
			float max_yellow = 0;
			float min_yellow = 100000;
			float max_blue = 0;
			float min_blue = 100000;
					
			for (int ii = 0; ii < 25; ii++) {
				swarm_blue_avg += robots[ii]->blue_yield;
				swarm_yellow_avg += robots[ii]->yellow_yield;
				if (robots[ii]->blue_yield > max_blue) max_blue = robots[ii]->blue_yield;
				if (robots[ii]->blue_yield < min_blue) min_blue = robots[ii]->blue_yield;
				if (robots[ii]->yellow_yield > max_yellow) max_yellow = robots[ii]->yellow_yield;
				if (robots[ii]->yellow_yield < min_yellow) min_yellow = robots[ii]->yellow_yield;
			}
			swarm_blue_avg /= 25;
			swarm_yellow_avg /= 25;	
			
			float fitness[25];
			for (int ii = 0; ii < 25; ii++) {
				float yellow_percentage = (robots[ii]->yellow_yield - swarm_yellow_avg) / (max_yellow - min_yellow);
				float blue_percentage = (robots[ii]->blue_yield - swarm_blue_avg) / (max_blue - min_blue);
				fitness[ii] = std::max(yellow_percentage,blue_percentage);
			}
			
			int loafer_count = 0;
			for (int ii = 0; ii < 25; ii++) if (robots[ii]->search_P_metric < 15) loafer_count++;
			
			int correct_loafer_count = 0;
			for (int ii = 0; ii < 25; ii++) {
				if (robots[ii]->search_P_metric < 15) {
					int worse_count = 0;
					for (int jj = 0; jj < 25; jj++) if (fitness[jj] < fitness[ii]) worse_count++;
					if (worse_count < loafer_count) correct_loafer_count++;
				}	
			}
			
			int yellow_count = 0;
			for (int ii = 0; ii < 25; ii++) if (robots[ii]->yellow_search_P_metric > 50) yellow_count++;
			
			int correct_color_count = 25 - wrong_counter(0);
				
			printf("yellow : blue = %d : %d\n\r", yellow_count, 25-yellow_count);
			
			printf("correct color choices = %d / 25\n\r", correct_color_count);
			
			printf("correct loafer choices = %d / %d\n\r", correct_loafer_count, loafer_count);
			
			printf("food storage capacity = %d / %d (%d%%)\n\r",food_storage/10000,max_food_storage/10000, 100*food_storage/max_food_storage);
			
			printf("yellow probabilities: ");
			for (int ii = 0; ii < 25; ii++) printf("%d ",robots[ii]->yellow_search_P_metric);
			printf("\n\r");
			
			printf("search probabilities: ");
			for (int ii = 0; ii < 25; ii++) printf("%d ",robots[ii]->search_P_metric);
			printf("\n\r");			
			
			printf("\n\r");
}

// Main routine.
int main(int argc, char **argv)
{
	std::ofstream myfile;
     	myfile.open ("data.csv",std::ofstream::trunc);
     	myfile.close();
     	
     	std::fill_n(food_x, food_max, 10000);
     	std::fill_n(food_y, food_max, 10000);

	for (int i = 0;i < argc-1;i++)
	{
		if (strcmp(argv[i],"/r")==0)
		{
			num_robots = stoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "/l") == 0)
		{
			log_debug_info = argv[i + 1][0]=='y';
		}
		if (strcmp(argv[i], "/d") == 0)
		{
			showscene = argv[i + 1][0] == 'y';
		}
		if (strcmp(argv[i], "/aw") == 0)
		{
			arena_width = stoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "/ah") == 0)
		{
			arena_height = stoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "/t") == 0)
		{
			timelimit = stoi(argv[i + 1]) ;
		}
		if (strcmp(argv[i], "/f") == 0)
		{
			strcpy_safe(log_file_name,255, argv[i + 1]);
		}
		if (strcmp(argv[i], "/ss") == 0)
		{
			snapshot = stoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "/seed") == 0)
		{
			seed = stoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "/shape") == 0)
		{
			strcpy_safe(shape_file_name, 255, argv[i + 1]);
		}
	}

	robots = (robot **)malloc(num_robots * sizeof(robot *));//creates an array of robots
	safe_distance = (int *) malloc(num_robots * num_robots * sizeof(int));
	order = (int *) malloc(shuffles * num_robots * sizeof(int));
	//seed random variable for different random behavior every time
	unsigned int t = 0;
	
	if (seed)
	{
		t = seed;
	}
	else
	{
		t= (unsigned int) time(NULL);
	}

	sprintf(log_buffer, "random seed: %d\n", t);
	
	log_info(log_buffer);
	srand(t);
	
	//set the simulation time to 0
	time_sim = 0;

	//inital zoom and scroll positions
	zoom = arena_width;
	view_x = arena_width;
	view_y = arena_height;

	//place robots
	//setup_positions_gradient();
	setup_positions();

	setup();

	//do some open gl stuff

	for (int i = 0;i < radius;i++)
	{
		ch[i] = sqrt(radius*radius - i*i);
	}

	if (showscene)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(0, 0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, 1000, 1000, 0.0f, 0.0f, 1.0f);
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glutCreateWindow("Kilobot simulator");

		glutDisplayFunc(draw_scene);
		glutReshapeFunc(resize_window);
		glutIdleFunc(on_idle);
		glutKeyboardFunc(key_input);
		glutMainLoop();
	}
	else {
		while (total_secs<timelimit)
		{
			run_simulation_step();
		}
		
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(0, 0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, 1000, 1000, 0.0f, 0.0f, 1.0f);
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glutCreateWindow("Kilobot simulator");

		glutDisplayFunc(draw_scene);
		glutReshapeFunc(resize_window);
		glutIdleFunc(on_idle);
		glutKeyboardFunc(key_input);
		glutMainLoop();
	}
	return 0;
}
