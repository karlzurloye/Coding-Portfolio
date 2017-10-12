
#include <math.h>

int counter;
bool domain[9][9][9] = {true};

bool valid(int **sudoku)
{
    // checking row and column validity
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            int num = sudoku[jj][ii];
            if (num != 0) {
                for (int kk=0; kk<9; kk++) {
                    if ((sudoku[jj][kk] == num) && (kk != ii)) return false;
                    if ((sudoku[kk][ii] == num) && (kk != jj)) return false;
                }
            }
        }
    }
    
    // checking box validity
    for (int ii=0; ii<9; ii+=3) {
        for (int jj=0; jj<9; jj+=3) {
            for (int kk=ii; kk<ii+3; kk++) {
                for (int hh=jj; hh<jj+3; hh++) {
                    int num = sudoku[hh][kk];
                    if (num != 0) {
                        for (int xx=ii; xx<ii+3; xx++) {
                            for (int yy=jj; yy<jj+3; yy++) {
                                if ((sudoku[yy][xx] == num) && !((xx == kk) && (yy == hh))) return false;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool sudoku_backtracking_helper(int **sudoku)
{
    counter++;
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            if (sudoku[jj][ii] == 0) {
                for (int kk=1; kk<10; kk++) {
                    sudoku[jj][ii] = kk;
                    if (valid(sudoku)) {
                        if (sudoku_backtracking_helper(sudoku)) return true;
                    }
                }
                sudoku[jj][ii] = 0;
                return false;
            }
        }
    }
    return true;
}

int sudoku_backtracking(int **sudoku)
{
    counter = 0;
    sudoku_backtracking_helper(sudoku);
    return counter;
}

void update_domain(int x, int y, int **sudoku) {
    
    // resetting domain to all true
    int num;
    for (int ii=0; ii<9; ii++) {
        domain[x][y][ii] = true;
    }
    
    // checking row and column
    for (int kk=0; kk<9; kk++) {
        if (kk != y) {
            num = sudoku[kk][x];
            if (num != 0) domain[x][y][num-1] = false;
        }
        if (kk != x) {
            num = sudoku[y][kk];
            if (num != 0) domain[x][y][num-1] = false;
        }
    }
    
    // checking box
    int corner_x = 3*floor(x/3);
    int corner_y = 3*floor(y/3);
    for (int ii=corner_x; ii<corner_x+3; ii++) {
        for (int jj=corner_y; jj<corner_y+3; jj++) {
            if (!((ii == x) && (jj == y))) {
                num = sudoku[jj][ii];
                if (num != 0) domain[x][y][num-1] = false;
            }
        }
    }
}

void repair_domain(int x, int y, int **sudoku) {
    
    // repairing domain of row and column
    for (int kk=0; kk<9; kk++) {
        if (kk != y) update_domain(x,kk,sudoku);
        if (kk != x) update_domain(kk,y,sudoku);
    }
    
    // repairing domain of box
    int corner_x = 3*floor(x/3);
    int corner_y = 3*floor(y/3);
    for (int ii=corner_x; ii<corner_x+3; ii++) {
        for (int jj=corner_y; jj<corner_y+3; jj++) {
            if (!((ii == x) && (jj == y))) update_domain(ii,jj,sudoku);
        }
    }
}

void trim_domain(int x, int y, int num) {
    
    // trimming domain of row and column
    for (int kk=0; kk<9; kk++) {
        if (kk != y) domain[x][kk][num-1] = false;
        if (kk != x) domain[kk][y][num-1] = false;
    }

    // trimming domain of box
    int corner_x = 3*floor(x/3);
    int corner_y = 3*floor(y/3);
    for (int ii=corner_x; ii<corner_x+3; ii++) {
        for (int jj=corner_y; jj<corner_y+3; jj++) {
            if (!((ii == x) && (jj == y))) domain[ii][jj][num-1] = false;
        }
    }
}

bool check_domain(int x, int y) {
    
    // checking single cell for empty domain
    for (int ii=0; ii<9; ii++) {
        if (domain[x][y][ii]) return true;
    }
    return false;
}

bool empty_domain(int x, int y, int **sudoku) {

    // checking domain of row and column
    for (int kk=0; kk<9; kk++) {
        if (sudoku[kk][x] == 0) {
            if (!check_domain(x,kk)) {
                return true;
            }
        }
        if (sudoku[y][kk] == 0) {
            if (!check_domain(kk,y)) {
                return true;
            }
        }
    }
    
    // checking domain of box
    int corner_x = 3*floor(x/3);
    int corner_y = 3*floor(y/3);
    for (int ii=corner_x; ii<corner_x+3; ii++) {
        for (int jj=corner_y; jj<corner_y+3; jj++) {
            if (sudoku[jj][ii] == 0) {
                if (!check_domain(ii,jj)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool sudoku_forwardchecking_helper(int **sudoku) {
    
    counter++;
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            if (sudoku[jj][ii] == 0) {
                for (int kk=1; kk<10; kk++) {
                    if (domain[ii][jj][kk-1]) {
                        sudoku[jj][ii] = kk;
                        trim_domain(ii,jj,kk);
                        if (!empty_domain(ii,jj,sudoku)) {
                            if (sudoku_forwardchecking_helper(sudoku)) return true;
                        }
                        sudoku[jj][ii] = 0;
                        repair_domain(ii,jj,sudoku);
                    }
                }
                return false;
            }
        }
    }
    return true;
}

int sudoku_forwardchecking(int **sudoku)
{
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            if (sudoku[jj][ii] == 0) update_domain(ii,jj,sudoku);
        }
    }
    
    counter = 0;
    sudoku_forwardchecking_helper(sudoku);
    return counter;
}

int domain_counter(int x, int y) {
    
    int count = 0;
    for (int ii=0; ii<9; ii++) {
        if (domain[x][y][ii]) count++;
    }
    return count;
}

bool sudoku_mrv_helper(int **sudoku) {
    
    counter++;
    int min = 10;
    int min_x, min_y;
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            if (sudoku[jj][ii] == 0) {
                int count = domain_counter(ii,jj);
                if (count < min) {
                    min = count;
                    min_x = ii;
                    min_y = jj;
                }
            }
        }
    }
    if (min == 10) return true;
    
    for (int kk=1; kk<10; kk++) {
        if (domain[min_x][min_y][kk-1]) {
            sudoku[min_y][min_x] = kk;
            trim_domain(min_x,min_y,kk);
            if (!empty_domain(min_x,min_y,sudoku)) {
                if (sudoku_mrv_helper(sudoku)) return true;
            }
            sudoku[min_y][min_x] = 0;
            repair_domain(min_x,min_y,sudoku);
        }
    }
    return false;
}

int sudoku_mrv(int **sudoku)
{
    for (int ii=0; ii<9; ii++) {
        for (int jj=0; jj<9; jj++) {
            if (sudoku[jj][ii] == 0) update_domain(ii,jj,sudoku);
        }
    }
    
    counter = 0;
    sudoku_mrv_helper(sudoku);
    return counter;
}

