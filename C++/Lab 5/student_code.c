
#include <math.h>
#define PI 3.14159265359

int count[1000][13760] = {0};

void detect_line (unsigned char ***image, float *m, float *b)
{
    for (int i=0; i<1000; i++) {
        for (int j=0; j<13760; j++) {
            count[i][j] = 0;
        }
    }
    
    for (int x=0; x<640; x++) {
        for (int y=0; y<480; y++) {
            if (image[0][y][x] + image[1][y][x] + image[2][y][x] < 0.1*(3*255)) {
                for (int k=-500; k<500; k++) {
                    float m0 = (float) k*10/500;
                    int b0 = y - m0*x;
                    count[k+500][b0+6880]++;
                }
            }
        }
    }
    
    int max_count = 0;
    float m_max;
    int b_max;
    for (int i=0; i<1000; i++) {
        for (int j=0; j<13760; j++) {
            if (count[i][j] > max_count) {
                max_count = count[i][j];
                *m = (float) -(i-500)/500*10;
                *b = 480-(j-6880);
            }
        }
    }
}

unsigned char edge_image[480][640];
int count2[1000][1600] = {0};

int detect_polygon (unsigned char ***image)
{
    for (int i=0; i<1000; i++) {
        for (int j=0; j<1600; j++) {
            count2[i][j] = 0;
        }
    }
    
    for (int x=0; x<640; x++) {
        for (int y=0; y<480; y++) {
            if ((x==0) || (y==0) || (x==639) || (y==479)) {
                edge_image[y][x] = 0;
            } else {
                int color = image[0][y][x] + image[1][y][x] + image[2][y][x];
                bool different = false;
                for (int i=-1; i<2; i++) {
                    for (int j=-1; j<2; j++) {
                        int adj_color = image[0][y+j][x+i] + image[1][y+j][x+i] + image[2][y+j][x+i];
                        if ((color != adj_color) && (!different)) {
                            edge_image[y][x] = 1;
                            bool different = true;
                        } else {
                            edge_image[y][x] = 0;
                        }
                    }
                }
            }
        }
    }
    
    for (int x=0; x<640; x++) {
        for (int y=0; y<480; y++) {
            if (edge_image[y][x] == 1) {
                for (int k=0; k<1000; k++) {
                    float theta = (float) k*PI/1000;
                    float w0 = (float) x*cos(theta) - y*sin(theta);
                    int w = round(w0);
                    count2[k][w+800]++;
                }
            }
        }
    }
    
    int max_count = 0;
    for (int i=0; i<1000; i++) {
        for (int j=0; j<1600; j++) {
            if (count2[i][j] > max_count) {
                max_count = count2[i][j];
            }
        }
    }
    
    float lines[50][2];
    for (int i=0; i<20; i++) {
        lines[i][0] = 100000;
        lines[i][1] = 100000;
    }
    
    int index = 0;
    for (int i=0; i<1000; i++) {
        for (int j=0; j<1600; j++) {
            if (count2[i][j] > 0.25*max_count) {
                float theta = (float) i*PI/1000;
                int w = j-800;
                bool duplicate = false;
                for (int k=0; k<index+1; k++) {
                    float theta_diff = fabs(theta - lines[k][0]);
                    int w_diff = fabs(w - lines[k][1]);
                    int w_diff_inv = fabs(w + lines[k][1]);
                    if (((theta_diff < 20.0*PI/180) && (w_diff < 20)) ||
                        ((theta_diff > PI-20.0*PI/180) && (w_diff_inv < 20))) {
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate) {
                    lines[index][0] = theta;
                    lines[index][1] = w;
                    index++;
                }
            }
        }
    }

	return index;
}

int circle_on_top (unsigned char ***image)
{
	// PUT YOUR CODE HERE
	// access the image using "image[chanel][y][x]"
	// return RED or BLUE
	// to create a bidimentional structure you can use "int **space=init_space(heigh, width);"
	return RED;
}







