#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define NORMAL  "\x1b[0m"

#define PI 3.14159
#define TRAINING_SIZE 1000
#define TEST_SIZE 100
#define NUM_CLASSES 9
#define NUM_FEATURES 3
#define DATA_DIM 2

float **init_data(int items, int columns);

#include "student_code.c"


void read_data(float **training_data,float **test_data1,float *gold_data1, char *filename)
{
	float data[DATA_DIM+1];
	FILE *fd;
	fd = fopen(filename, "rb");
	for (int i=0;i<TRAINING_SIZE;i++)
	{
		fread(data,sizeof(float),DATA_DIM+1,fd);
		for (int j=0;j<DATA_DIM+1;j++)
			training_data[i][j]=data[j];
	}
	for (int i=0;i<TEST_SIZE;i++)
	{
		fread(data,sizeof(float),DATA_DIM+1,fd);
		for (int j=0;j<DATA_DIM;j++)
		{
			test_data1[i][j]=data[j];
		}
		test_data1[i][DATA_DIM]=-1;
		gold_data1[i]=data[DATA_DIM];
	}
	fclose(fd);
}

bool run_experiment1(char *filename)
{
	float **training_data = init_data(TRAINING_SIZE,DATA_DIM+1);
	float **test_data = init_data(TEST_SIZE,DATA_DIM+1);
	float gold_data [TEST_SIZE];
	//generating data should be hidden from students!
	read_data(training_data,test_data,gold_data,filename);

	//this is one of the two student functions
	part_one_classifier(training_data,test_data);
	//part 1 grading
	int i;
	int error=0;
	for(i=0;i<TEST_SIZE;i++)
	{
		if(test_data[i][DATA_DIM]!=gold_data[i])
		{   
			error++;
		}
	}
	printf("Incorrect classificattions is %d out of  %d", error,TEST_SIZE);	

	bool success=true;
	
	if (error<=(float)TEST_SIZE*.05)
	{
		printf("(%sSUCCESS%s)\n", GREEN, NORMAL);	
	}
	else
	{
		success=false;
		printf("(%sFAIL%s) maximum %f\n", RED, NORMAL, (float)TEST_SIZE*.05);
	}
	printf("\n");
	return success;

}

bool run_experiment2(char *filename)
{
	float **training_data = init_data(TRAINING_SIZE,DATA_DIM+1);
	float **test_data = init_data(TEST_SIZE,DATA_DIM+1);
	float gold_data [TEST_SIZE];
	printf("Linear Classifier : Dataset 1");
	//generating data should be hidden from students!
	read_data(training_data,test_data,gold_data,filename);

	//this is one of the two student functions
	part_two_classifier(training_data,test_data);
	//part 1 grading
	int i;
	int error=0;
	for(i=0;i<TEST_SIZE;i++)
	{
		if(test_data[i][DATA_DIM]!=gold_data[i])
		{   
			error++;
		}
	}
	printf("Incorrect classificattions is %d out of  %d", error,TEST_SIZE);	

	bool success=true;
	
	if (error<=(float)TEST_SIZE*.05)
	{
		printf("(%sSUCCESS%s)\n", GREEN, NORMAL);	
	}
	else
	{
		success=false;
		printf("(%sFAIL%s) maximum %f\n", RED, NORMAL, (float)TEST_SIZE*.05);
	}
	printf("\n");
}

int main(int n, char **args)
{
	////////////
	char filename1[] = "Labs/Lab6/data1.dat";
	printf("Linear Classifier : Dataset 1\n");
	bool exp1=run_experiment1(filename1);

	char filename2[] = "Labs/Lab6/data2.dat";
	printf("Linear Classifier : Dataset 2\n");
	bool exp2=run_experiment1(filename2);

	char filename3[] = "Labs/Lab6/data3.dat";
	printf("Linear Classifier : Dataset 3\n");
	bool exp3=run_experiment1(filename3);
	
	char filename4[] = "Labs/Lab6/data4.dat";
	printf("Linear Classifier : Dataset 4\n");
	bool exp4=run_experiment1(filename4);

	char filename5[] = "Labs/Lab6/datar1.dat";
	printf("Accelerometer : Dataset 1\n");
	bool exp5=run_experiment2(filename5);

	char filename6[] = "Labs/Lab6/datar2.dat";
	printf("Accelerometer : Dataset 2\n");
	bool exp6=run_experiment2(filename6);

	char filename7[] = "Labs/Lab6/datar3.dat";
	printf("Accelerometer : Dataset 3\n");
	bool exp7=run_experiment2(filename7);
	
	return exp1 | exp2 | exp3 | exp4 | exp5 | exp6 | exp7;
}

float **init_data(int items, int columns)
{
	float **map;
	map = (float **) malloc(sizeof(float*)*items);
	for (int i=0 ; i < items; i++)
	{
		map[i]=(float *) malloc(sizeof(float) * columns);
	}
	return map;
}