#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define NORMAL  "\x1b[0m"

void print_sudoku(int **sudoku);
bool can_yx_be_z(int **sudoku, int y,int x,int z);

#include "student_code.c"

int **init_sudoku();
void set_sudoku(int **, char *);
bool check_result(int **sudoku, bool show);
bool run_experiment(char data[], int btlimit, int fclimit, int mrvlimit);

int main(int n, char **args)
{
	char data1[] = "900670000"
                   "006800470"
                   "800010003"
                   "003000001"
                   "005406900"
                   "600000300"
                   "300060008"
                   "068005200"
                   "000082006";

	char data2[] = "006100050"
                   "200605008"
                   "000090002"
                   "000019300"
                   "002000800"
                   "003570000"
                   "900040000"
                   "800301009"
                   "040006100";

	char data3[] = "530070000"
                   "600195000"
                   "098000060"
                   "800060003"
                   "400803001"
                   "700020006"
                   "060000280"
                   "000419005"
                   "000080079";

	char data4[] = "009000400"
                   "600400020"
                   "840031090"
                   "008007041"
                   "500060003"
                   "160800700"
                   "070290065"
                   "020005004"
                   "005000900";

	printf("\nBoard 1:\n");
	bool exp1 = run_experiment(data1, 35000, 4000, 2500);
	printf("\nBoard 2:\n");
	bool exp2 = run_experiment(data2, 200000, 30000, 3500);
	printf("\nBoard 3:\n");
	bool exp3 = run_experiment(data3, 40000, 4500, 300);
	printf("\nBoard 4\n");
	bool exp4 = run_experiment(data4, 6000, 800, 650);
	return !(exp1 && exp2 && exp3 && exp4);
}

bool run_experiment(char data[], int btlimit, int fclimit, int mrvlimit)
{
	bool result=true;
    
	int **sudoku;
	sudoku = init_sudoku();
	set_sudoku(sudoku, data);
	int bt1 = sudoku_backtracking(sudoku);
	if (!check_result(sudoku,false))
	{
		printf("Backtracking results: %sFAIL%s\n", RED, NORMAL);
		check_result(sudoku,true);
		result=false;
	}
	else
	{
		printf("Backtracking results:%sSUCCESS%s\n", GREEN, NORMAL);
	}
	if (bt1>btlimit)
	{
		printf("Backtracking count: %d (%sFAIL%s)\n", bt1, RED, NORMAL);
		result=false;
	}
	else
	{
		printf("Backtracking count: %d (%sSUCCESS%s)\n", bt1, GREEN, NORMAL);
	}

	set_sudoku(sudoku, data);
	int fc1 = sudoku_forwardchecking(sudoku);
	if (!check_result(sudoku,false))
	{
		printf("Forwardchecking results: %sFAIL%s\n", RED, NORMAL);
		check_result(sudoku,true);
		result=false;
	}
	else
	{
		printf("Forwardchecking results:%sSUCCESS%s\n", GREEN, NORMAL);
	}
	if (fc1>fclimit)
	{
		printf("Forwardchecking count: %d (%sFAIL%s)\n", fc1, RED, NORMAL);
		result=false;
	}
	else
	{
		printf("Forwardchecking count: %d (%sSUCCESS%s)\n", fc1, GREEN, NORMAL);
	}

	set_sudoku(sudoku, data);
	int mrv1 = sudoku_mrv(sudoku);
	if (!check_result(sudoku,false))
	{
		printf("MRV results: %sFAIL%s\n", RED, NORMAL);
		check_result(sudoku,true);
		result=false;
	}
	else
	{
		printf("MRV results:%sSUCCESS%s\n", GREEN, NORMAL);
	}
	if (mrv1>mrvlimit)
	{
		printf("MRV count: %d (%sFAIL%s)\n", mrv1, RED, NORMAL);
		result=false;
	}
	else
	{
		printf("MRV count: %d (%sSUCCESS%s)\n", mrv1, GREEN, NORMAL);
	}
	return result;
}


bool can_yx_be_z(int **sudoku, int y,int x,int z)
{
	for (int i=0;i<9;i++)
	{
		if (sudoku[y][i]==z) return false;
		if (sudoku[i][x]==z)  return false;
		if(sudoku[(y/3)*3+i/3][(x/3)*3+i%3]==z) return false;
	}
	return true;
}

bool check_result(int **sudoku, bool show)
{
	bool result=true;
	for (int y=0;y<9;y++)
	{
		for (int x=0;x<9;x++)
		{
			int value = sudoku[y][x];
			sudoku[y][x]=0;
			if (value!=0 && can_yx_be_z(sudoku,y,x,value))
			{
				if (show)
					printf("%s%d",GREEN,value);
			}
			else
			{
				result = false;
				if (show)
					printf("%s%d",RED,value);
			}
			sudoku[y][x]=value;
				
		}
		if (show)
			printf("%s\n",NORMAL);
	}
	return result;
}

int **init_sudoku()
{
	int **sudoku;
	sudoku = (int **) malloc(sizeof(int*)*9);
	for (int i=0 ; i < 9; i++)
	{
		sudoku[i]=(int *) malloc(sizeof(int) * 9);
	}
	return sudoku;
}

void set_sudoku(int **sudoku, char *data)
{
	for (int y=0;y<9;y++)
	{
		for (int x=0;x<9;x++)
		{
			sudoku[y][x]=data[y*9+x]-'0';
		}
	}
}

void print_sudoku(int **sudoku)
{
	printf("\n");
	for (int y=0;y<9;y++)
	{
		for (int x=0;x<9;x++)
		{
			printf("%d",sudoku[y][x]);
		}
		printf("\n");
	}
}
