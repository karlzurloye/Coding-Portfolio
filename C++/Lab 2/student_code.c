

int inv(int turn) {
    if (turn == X) return O;
    else return X;
}

int minmax_tictactoe(int *board, int turn)
{ 
    int result = game_status(board);
    if (result != 0) return result;
    
    int temp;
    bool full = true;
    result = inv(turn);
    
    for (int ii=0; ii<9; ii++) {
        if (board[ii] == 0) {
            full = false;
            board[ii] = turn;
            temp = minmax_tictactoe(board, inv(turn));
            board[ii] = 0;

            if (temp == turn) result = turn;
            if ((temp == 0) && (result != turn)) result = temp;
            if (result == inv(turn)) result == temp;
            }
    }
    if (full) return 0;
	return result;
}

int abprun_tictactoe(int *board, int turn)
{ 
    int result = game_status(board);
    if (result != 0) return result;
    
    int temp;
    bool full = true;
    result = inv(turn);
    
    for (int ii=0; ii<9; ii++) {
        if (board[ii] == 0) {
            full = false;
            board[ii] = turn;
            temp = abprun_tictactoe(board, inv(turn));
            board[ii] = 0;
            
            if (temp == turn) {
                result = turn;
                break;
            }
            if ((temp == 0) && (result != turn)) result = temp;
            if (result == inv(turn)) result == temp;
        }
    }
    if (full) return 0;
    return result;
}

float st_tictactoe_win(int *board, int turn)
{
    //nah
}

float st_tictactoe_winortie(int *board, int turn)
{
    //nah
}



