#pragma once
#include "kilolib.h"

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;
	float theta;
	
	int motion=0;
	long int motion_timer=0;

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;
	};

	float light_vector[2] = {0, 0}; // controller vector initialization
	float food_vector[2] = {0, 0};
	float random_vector[2] = {0, 0};
	float repulsion_vector[2] = {0, 0};
	float goal_vector[2] = {0, 0};

	int blue_count[20] = {0};
	int yellow_count[20] = {0};
	int blue_search_timer[20] = {0};
	int yellow_search_timer[20] = {0};
	int search_timer = 0;
	int blue_index = 0;
	int yellow_index = 0;
	bool blue_index_maxed = false;
	bool yellow_index_maxed = false;
	float blue_avg;
	float yellow_avg;
	float blue_time_avg;
	float yellow_time_avg;
	float blue_rate;
	float yellow_rate;
	float blue_percentile;
	float yellow_percentile;
	
	float swarm_blue_count[25] = {0};
	float swarm_yellow_count[25] = {0};

	float rand_weight = 0.3; // strength of random motion
	
	#define leave_prob 150
	int search_P = leave_prob;
	int leave_step = 10;
	
	#define search_prob 50
	int yellow_search_P = search_prob; // ratio of food choices
	int blue_search_P = search_prob;
	int search_step = 5;

	bool search = true; // robot is either searching or resting
	bool homebound = false; // when robot is returning to the nest

	int reset_count = 100; // repulsion vector reset to {0, 0} every 100 cycles
	
	float yellow;
	float blue;

	//main loop
	void loop() {

		float theta_random = rand() % 2*PI; // random vector
 		random_vector[0] = rand_weight*cos(theta_random);
 		random_vector[1] = rand_weight*sin(theta_random);

		float magnitude = sqrt(repulsion_vector[0]*repulsion_vector[0] + 
			repulsion_vector[1]*repulsion_vector[1]);
		if (magnitude > 6.4) { // magnitude of repulsion vector capped at 6.4 
			repulsion_vector[0] = repulsion_vector[0] * 6.4 / magnitude;
			repulsion_vector[1] = repulsion_vector[1] * 6.4 / magnitude;
		}
	
		if (search) {
			search_timer++;
			if (dist_to_food < 5000) {
				if (dist_to_food < 35) {
					if (goal_type == 0) {
						yellow_count[yellow_index] = rand() % yellow_yield + 1;
						yellow_search_timer[yellow_index] = search_timer;
						search_timer = 0;
						yellow_index++;
						if (yellow_index > 19) {
							yellow_index = 0;
							yellow_index_maxed = true;
						}
						search = false;
						food_eaten = true;
						homebound = true;
					} else {
						blue_count[blue_index] = rand() % blue_yield + 1;
						blue_search_timer[blue_index] = search_timer;
						search_timer = 0;
						blue_index++;
						if (blue_index > 19) {
							blue_index = 0;
							blue_index_maxed = true;
						}
						search = false;
						food_eaten = true;
						homebound = true;
					}
				} else {	
					food_vector[0] = cos(angle_to_food); 
					food_vector[1] = sin(angle_to_food);
				
					goal_vector[0] = food_vector[0] + repulsion_vector[0] + random_vector[0];
					goal_vector[1] = food_vector[1] + repulsion_vector[1] + random_vector[1];
				}
			} else {
				goal_vector[0] = repulsion_vector[0] + random_vector[0];
				goal_vector[1] = repulsion_vector[1] + random_vector[1];
			}
		} else if (homebound) {
			if (dist_to_light < 200) {
				homebound = false;
			
				blue_avg = 0;
				yellow_avg = 0;
				blue_time_avg = 0;
				yellow_time_avg = 0;
				for (int ii = 0; ii < 20; ii++) {
					blue_avg += blue_count[ii];
					yellow_avg += yellow_count[ii];
					blue_time_avg += blue_search_timer[ii];
					yellow_time_avg += yellow_search_timer[ii];
				}
			
				if (blue_index_maxed) blue_avg /= 20;
				else if (blue_index == 0) blue_avg = 0;
				else blue_avg /= blue_index;
			
				if (yellow_index_maxed) yellow_avg /= 20;
				else if (yellow_index == 0) yellow_avg = 0;
				else yellow_avg /= yellow_index;
				
				if (blue_index_maxed) blue_time_avg /= 20;
				else if (blue_index == 0) blue_time_avg = 0;
				else blue_time_avg /= blue_index;
				
				if (yellow_index_maxed) yellow_time_avg /= 20;
				else if (yellow_index == 0) yellow_time_avg = 0;
				else yellow_time_avg /= yellow_index;
			
				blue_rate = blue_avg / blue_time_avg;
				yellow_rate = yellow_avg / yellow_time_avg;
		
		
				swarm_blue_count[id] = blue_avg;
				swarm_yellow_count[id] = yellow_avg;
			
				float swarm_blue_avg = 0;
				float swarm_yellow_avg = 0;
				for (int ii = 0; ii < 25; ii++) {
					swarm_blue_avg += swarm_blue_count[ii];
					swarm_yellow_avg += swarm_yellow_count[ii];
				}
				swarm_blue_avg /= 25;
				swarm_yellow_avg /= 25;
			
				float max_blue = 0;
				float min_blue = 100000;
				float max_yellow = 0;
				float min_yellow = 100000;
				for (int ii = 0; ii < 25; ii++) {
					if (swarm_blue_count[ii] > max_blue) max_blue = swarm_blue_count[ii];
					if (swarm_blue_count[ii] < min_blue) min_blue = swarm_blue_count[ii];
					if (swarm_yellow_count[ii] > max_yellow) max_yellow = swarm_yellow_count[ii];
					if (swarm_yellow_count[ii] < min_yellow) min_yellow = swarm_yellow_count[ii];				
				}
			
				/*
				float yellow_adj, blue_adj;
				if ((yellow_time_avg == 0) || (blue_time_avg == 0)) {
					yellow_adj = 1;
					blue_adj = 1;
				} else {
					blue_adj = ((yellow_time_avg/blue_time_avg) + 1) / 2;
					yellow_adj = ((blue_time_avg/yellow_time_avg) + 1) / 2;
				}
				*/
				
				float yellow_scale = max_yellow - min_yellow;
				float blue_scale = max_blue - min_blue;
			
				if (yellow_scale == 0) yellow_percentile = 0;
				else yellow_percentile = (yellow_avg - swarm_yellow_avg)/(max_yellow - min_yellow);
				//else yellow_percentile = (yellow_adj*((yellow_avg - swarm_yellow_avg) + yellow_scale/2) - yellow_scale/2) / yellow_scale;

				if (blue_scale == 0) blue_percentile = 0;
				else blue_percentile = (blue_avg - swarm_blue_avg)/(max_blue - min_blue);
				//else blue_percentile = (blue_adj*((blue_avg - swarm_blue_avg) + blue_scale/2) - blue_scale/2) / blue_scale;
			
				/*
				if (yellow_percentile > blue_percentile) {
					if (yellow_search_P < 95) {
						yellow_search_P += search_step;
						blue_search_P -= search_step;
					}
				} else {
					if (blue_search_P < 95) {
						yellow_search_P -= search_step;
						blue_search_P += search_step;
					}
				}
				*/
				
				if (yellow_rate > blue_rate) {
					if (yellow_search_P < 95) {
						yellow_search_P += search_step;
						blue_search_P -= search_step;
					}
				} else {
					if (blue_search_P < 95) {
						yellow_search_P -= search_step;
						blue_search_P += search_step;
					}
				}
				
				if (food_storage_count >= food_storage_max) {
					search_P -= leave_step * (0.5 - std::max(blue_percentile, yellow_percentile));
					if (search_P <= 0) search_P = 1;
				} else {
					food_returned = true;
					search_P += leave_step * (0.5 + std::max(blue_percentile, yellow_percentile));
					if (search_P >= 2*leave_prob) search_P = 2*leave_prob;
				}
				
				search_P_metric = search_P;
				yellow_search_P_metric = yellow_search_P;
				
			} else {
				light_vector[0] = cos(angle_to_light);
				light_vector[1] = sin(angle_to_light);

				goal_vector[0] = light_vector[0] + repulsion_vector[0] + random_vector[0];
				goal_vector[1] = light_vector[1] + repulsion_vector[1] + random_vector[1];
			}
		} else { // rest
			int search_dice = rand() % (3000 * leave_prob) + 1;
			if (search_dice < search_P) {
				search_dice = rand() % (2*search_prob) + 1;
				if (search_dice < yellow_search_P) {
					goal_type = 0;
				} else {
					goal_type = 1;
				}
				search = true;
			} else if (dist_to_light > 200) {
				light_vector[0] = cos(angle_to_light);
				light_vector[1] = sin(angle_to_light);

				goal_vector[0] = light_vector[0] + repulsion_vector[0] + random_vector[0];
				goal_vector[1] = light_vector[1] + repulsion_vector[1] + random_vector[1];
			} else {
				goal_vector[0] = repulsion_vector[0] + random_vector[0];
				goal_vector[1] = repulsion_vector[1] + random_vector[1];
			}
		}
		
		theta = atan2(goal_vector[1], goal_vector[0]); // theta to goal direction

		if (fabs(theta) < .3) { // moving in direction of goal
			spinup_motors();
			set_motors(50,50);
		} else if (theta < 0) {
			spinup_motors();
			set_motors(kilo_turn_left,0);
		} else {
			spinup_motors();
			set_motors(0,kilo_turn_right);			
		}

		if (search_P < 15) set_color(RGB(0.2,0.2,0.2));
		else if ((yellow_search_P < 60) && (yellow_search_P > 40)) {
			set_color(RGB(0.8,0.8,0.8));
		} else if (yellow_search_P >= blue_search_P) {
			yellow = (float) (yellow_search_P - 55) / 40;
			set_color(RGB(1,1,1-yellow));
		} else {
			blue = (float) (blue_search_P - 55) / 40;
			set_color(RGB(1-blue,1-blue,1));
		}

		if (reset_count == 0) { // resetting repulsion vector every 100 cycles
			repulsion_vector[0] = 0;
			repulsion_vector[1] = 0;
			reset_count = 100;
		} else {
			reset_count--;
		}
		
		out_message.data[0] = id;
		out_message.data[1] = (short) (20 * yellow_avg) >> 8 & 0xFF;
		out_message.data[2] = (short) (20 * yellow_avg) & 0xFF;
		out_message.data[3] = (short) (20 * blue_avg) >> 8 & 0xFF;
		out_message.data[4] = (short) (20 * blue_avg) & 0xFF;
		out_message.crc = message_crc(&out_message);
	}

	//executed once at start
	void setup()
	{	
		out_message.type = NORMAL;
		out_message.crc = message_crc(&out_message);
		set_color(RGB(1,1,1)); //starting color doesn't matter
	}

	//executed on successfull message send
	void message_tx_success()
	{
		//set_color(RGB(1,0,0));	
	}

	//sends message at fixed rate
	message_t *message_tx()
	{
		static int count = rand();
		count--;
		if (!(count % 10))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement,float t)
	{
		distance = estimate_distance(distance_measurement);
		theta=t;
		
		short incoming_id = message->data[0];
		swarm_yellow_count[incoming_id] = (float) ((message->data[1] << 8) | (message->data[2] & 0xFF)) / 20;
		swarm_blue_count[incoming_id] = (float) ((message->data[3] << 8) | (message->data[4] & 0xFF)) / 20;
		
		// modeled after brazil nut paper 
		if (distance < 4*radius) {
			float k = 0.2; // repulsion coefficient 
			float mag = k*(4*radius - distance);
			float v_x = mag*cos(theta);
			float v_y = mag*sin(theta);
			repulsion_vector[0] = repulsion_vector[0] - v_x;
			repulsion_vector[1] = repulsion_vector[1] - v_y;
		}
	}
};






