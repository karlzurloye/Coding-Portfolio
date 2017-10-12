#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 12
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define NORMAL  "\x1b[0m"

void print_map(int **map);

#include "student_code.c"

int **init_map();
void set_map(int **, char *);
bool check_result(char *title, int **map1, int **map2);

int main(int n, char **args)
{
	char data1[] = "2000000000"
                   "0101111111"
                   "0100000000"
                   "0101111111"
                   "0101000001"
                   "0101010110"
                   "0101010000"
                   "0100011110"
                   "0011111110"
                   "1011111110"
                   "1011111111"
                   "1000000003";
				  
	char gold_df1[] = "5444444444"
                      "5141111111"
                      "5144444444"
                      "5141111111"
                      "5141444441"
                      "5141414114"
                      "5141414444"
                      "5144411114"
                      "5511111114"
                      "1511111114"
                      "1511111111"
                      "1555555555";
					 
	char gold_bf1[] = "5444444444"
                      "5141111111"
                      "5144444444"
                      "5141111111"
                      "5141444441"
                      "5141414110"
                      "5141414440"
                      "5144411110"
                      "5511111110"
                      "1511111110"
                      "1511111111"
                      "1555555555";
					 

	char data2[] = "0000000000"
                   "1111110101"
                   "0300010101"
                   "1111010101"
                   "0001010101"
                   "0100010101"
                   "1111010101"
                   "0000000101"
                   "0111111100"
                   "0000000101"
                   "0111111120"
                   "0000000010";
				  
	char gold_df2[] = "0000005554"
                      "1111115151"
                      "0555515151"
                      "1111515151"
                      "4441515151"
                      "4144515151"
                      "1111515151"
                      "4444555151"
                      "4111111154"
                      "4444444151"
                      "4111111154"
                      "4444444414";
					 
	char gold_bf2[] = "4444445554"
                      "1111115151"
                      "0555515151"
                      "1111515151"
                      "4441515151"
                      "4144515151"
                      "1111515151"
                      "4444555151"
                      "4111111154"
                      "4440000151"
                      "4111111154"
                      "4000000014";

	char data3[] = "0000000000"
                   "0010111111"
                   "0210100000"
                   "1110101110"
                   "0000101010"
                   "0010101010"
                   "0010001010"
                   "0011111010"
                   "0000000010"
                   "0011111110"
                   "0010001031"
                   "1000101001";
				  
	char gold_df3[] = "4444444444"
                      "4414111111"
                      "4414144444"
                      "1114141114"
                      "4444141414"
                      "4414141414"
                      "4414441414"
                      "4411111414"
                      "4444444414"
                      "4411111114"
                      "4414441031"
                      "1444141001";
					 
	char gold_bf3[] = "4444444444"
                      "4414111111"
                      "4414144444"
                      "1114141114"
                      "4444141414"
                      "4414141414"
                      "4414441414"
                      "4411111414"
                      "4444444414"
                      "4411111114"
                      "4414441031"
                      "1444141001";



	int **dfmap1, **bfmap1, **gold_dfmap1, **gold_bfmap1;
	gold_dfmap1 = init_map();
	set_map(gold_dfmap1, gold_df1);
	gold_bfmap1 = init_map();
	set_map(gold_bfmap1, gold_bf1);
	
	dfmap1 = init_map();
	set_map(dfmap1, data1);
	bool df1 = df_search(dfmap1);
	char tdf1[] = "Simple maze first depth search results:";
	bool cdf1 = check_result(tdf1,dfmap1,gold_dfmap1);
	
	bfmap1 = init_map();
	set_map(bfmap1, data1);
	bool bf1 = bf_search(bfmap1);
	char tbf1[] = "Simple maze first breadth search results:";
	bool cbf1 = check_result(tbf1,bfmap1,gold_bfmap1);

	bool exp1 =!( cdf1 && cbf1 && df1 && bf1 );
	
	int **dfmap2, **bfmap2, **gold_dfmap2, **gold_bfmap2;
	gold_dfmap2 = init_map();
	set_map(gold_dfmap2, gold_df2);
	gold_bfmap2 = init_map();
	set_map(gold_bfmap2, gold_bf2);
	
	dfmap2 = init_map();
	set_map(dfmap2, data2);
	bool df2 = df_search(dfmap2);
	char tdf2[] = "Empty map first depth search results:";
	bool cdf2 = check_result(tdf2,dfmap2,gold_dfmap2);
	
	bfmap2 = init_map();
	set_map(bfmap2, data2);
	bool bf2 = bf_search(bfmap2);
	char tbf2[] = "Empty map first breadth search results:";
	bool cbf2 = check_result(tbf2,bfmap2,gold_bfmap2);

	bool exp2 =!( cdf2 && cbf2 && df2 && bf2 );
	
	int **dfmap3, **bfmap3, **gold_dfmap3, **gold_bfmap3;
	gold_dfmap3 = init_map();
	set_map(gold_dfmap3, gold_df3);
	gold_bfmap3 = init_map();
	set_map(gold_bfmap3, gold_bf3);
	
	dfmap3 = init_map();
	set_map(dfmap3, data3);
	bool df3 = df_search(dfmap3);
	char tdf3[] = "Unreachable goal first depth search results:";
	bool cdf3 = check_result(tdf3,dfmap3,gold_dfmap3);
	
	bfmap3 = init_map();
	set_map(bfmap3, data3);
	bool bf3 = bf_search(bfmap3);
	char tbf3[] = "Unreachable goal first breadth search results:";
	bool cbf3 = check_result(tbf3,bfmap3,gold_bfmap3);

	bool exp3 =!( cdf3 && cbf3 && !df3 && !bf3 );
	

	return exp1 || exp2 || exp3;
	}

bool check_result(char *title, int **map1, int **map2)
{
	bool result=true;
	printf("%s\n",title);
	for (int y=0;y<MAP_HEIGHT;y++)
	{
		for (int x=0;x<MAP_WIDTH;x++)
		{
			if (map1[y][x]==map2[y][x])
			{
				printf("%s%d",GREEN,map1[y][x]);
			}
			else
			{
				result = false;
				printf("%s%d",RED,map1[y][x]);
			}
				
		}
		printf("%s\n",NORMAL);
	}
	if (result)
	{
		printf("Test Result: %sPassed",GREEN);
	}
	else
	{
		printf("Test Result: %sFail",RED);
	}
	printf("%s\n",NORMAL);
	return result;
}

int **init_map()
{
	int **map;
	map = (int **) malloc(sizeof(int*)*MAP_HEIGHT);
	for (int i=0 ; i < MAP_HEIGHT; i++)
	{
		map[i]=(int *) malloc(sizeof(int) * MAP_WIDTH);
	}
	return map;
}

void set_map(int **map, char *data)
{
	for (int y=0;y<MAP_HEIGHT;y++)
	{
		for (int x=0;x<MAP_WIDTH;x++)
		{
			map[y][x]=data[y*MAP_WIDTH+x]-'0';
		}
	}
}

void print_map(int **map)
{
	printf("\n");
	for (int y=0;y<MAP_HEIGHT;y++)
	{
		for (int x=0;x<MAP_WIDTH;x++)
		{
			printf("%d",map[y][x]);
		}
		printf("\n");
	}
}
