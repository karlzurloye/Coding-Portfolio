
#include <math.h>

float prev_values[6][6] = {1000};

void print_values(float **values) {
    
    for (int y=0; y<6; y++) {
        for (int x=0; x<6; x++) {
            printf("%s%6.4f\t",GREEN,values[y][x]);
        }
        printf("\n");
    }
    printf("\n\n");
}

bool converged (float **values) {

    for (int x=0; x<6; x++) {
        for (int y=0; y<6; y++) {
            float avg = (values[y][x]+prev_values[y][x]) / 2;
            if (avg != 0) {
                float percent_diff = (values[y][x]-prev_values[y][x]) / avg;
                if (fabs(percent_diff) > 0.001) return false;
            }
        }
    }
    return true;
}

void update_values (float **values) {

    for (int x=0; x<6; x++) {
        for (int y=0; y<6; y++) {
            prev_values[y][x] = values[y][x];
        }
    }
}

float value (int x, int y, float parent_value, float **values) {
    
    if ((x<0) || (x>5) || (y<0) || (y>5)) return parent_value;
    else return values[y][x];
}

float expectimax (int x, int y, int a, int **map, float **values, float battery_drop_cost, float discount_per_cycle) {

    float reward;
    switch (a) {
        case 1 : // south, no boost
            reward = 0.15*value(x-1,y,values[y][x],values) + 0.15*value(x+1,y,values[y][x],values) + 0.7*value(x,y+1,values[y][x],values);
            break;
        case 2 : // west, no boost
            reward = 0.15*value(x,y+1,values[y][x],values) + 0.15*value(x,y-1,values[y][x],values) + 0.7*value(x-1,y,values[y][x],values);
            break;
        case 3 : // north, no boost
            reward = 0.15*value(x-1,y,values[y][x],values) + 0.15*value(x+1,y,values[y][x],values) + 0.7*value(x,y-1,values[y][x],values);
            break;
        case 4 : // east, no boost
            reward = 0.15*value(x,y+1,values[y][x],values) + 0.15*value(x,y-1,values[y][x],values) + 0.7*value(x+1,y,values[y][x],values);
            break;
        case 5 : // south, with boost
            reward = 0.1*value(x-1,y,values[y][x],values) + 0.1*value(x+1,y,values[y][x],values) + 0.8*value(x,y+1,values[y][x],values);
            break;
        case 6 : // west, with boost
            reward = 0.1*value(x,y+1,values[y][x],values) + 0.1*value(x,y-1,values[y][x],values) + 0.8*value(x-1,y,values[y][x],values);
            break;
        case 7 : // north, with boost
            reward = 0.1*value(x-1,y,values[y][x],values) + 0.1*value(x+1,y,values[y][x],values) + 0.8*value(x,y-1,values[y][x],values);
            break;
        case 8 : // east, with boost
            reward = 0.1*value(x,y+1,values[y][x],values) + 0.1*value(x,y-1,values[y][x],values) + 0.8*value(x+1,y,values[y][x],values);
            break;
    }
    
    if (a > 4) return -2*battery_drop_cost + (1-discount_per_cycle)*reward;
    else return -battery_drop_cost + (1-discount_per_cycle)*reward;
}

void drone_flight_planner_helper (int **map, int **policies, float **values, float delivery_fee, float battery_drop_cost, float dronerepair_cost, float discount_per_cycle) {
 
    while(!converged(values)) {
        update_values(values);
        //print_values(values);
        for (int x=0; x<6; x++) {
            for (int y=0; y<6; y++) {
                if (map[y][x] == 2) {
                    values[y][x] = delivery_fee;
                    policies[y][x] = 0;
                } else if (map[y][x] == 3) {
                    values[y][x] = -dronerepair_cost;
                    policies[y][x] = 0;
                } else {
                    float max_reward = -10000000;
                    int optimal_action;
                    for (int a=1; a<9; a++) {
                        float reward = expectimax(x,y,a,map,values,battery_drop_cost,discount_per_cycle);
                        if (reward > max_reward) {
                            max_reward = reward;
                            optimal_action = a;
                        }
                    }
                    values[y][x] = max_reward;
                    policies[y][x] = optimal_action;
                }
            }
        }
    }
}

float drone_flight_planner (int **map, int **policies, float **values, float delivery_fee, float battery_drop_cost, float dronerepair_cost, float discount_per_cycle) {
    
    drone_flight_planner_helper (map,policies,values,delivery_fee,battery_drop_cost, dronerepair_cost,discount_per_cycle);
    
    for (int x=0; x<6; x++) {
        for (int y=0; y<6; y++) {
            if (map[y][x] == 1) return values[y][x];
        }
    }
}





