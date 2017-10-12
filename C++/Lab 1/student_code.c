bool df_helper(int**, int, int);

bool df_search(int **map)
{ 	
	for (int i=0; i<MAP_WIDTH; i++) {
		for (int j=0; j<MAP_HEIGHT; j++) {
			if (map[j][i] == 2) {
				return df_helper(map, i, j);
			}
		}	
	}
}

bool df_helper(int **map, int  x, int  y)
{
	if ((x<0) || (y<0) || (x>=MAP_WIDTH) || (y>=MAP_HEIGHT)) return false;

	if (map[y][x] == 3) {
		map[y][x] = 5;
		return true;
	}

	if (map[y][x] == 1) return false;
	if (map[y][x] == 4) return false;

	map[y][x] = 4;
	
	if (df_helper(map, x+1, y)) {
		map[y][x] = 5;
		return true;
	} else if (df_helper(map, x, y+1)) {
		map[y][x] = 5;
		return true;
	} else if (df_helper(map, x-1, y)) {
		map[y][x] = 5;
		return true;
	} else if (df_helper(map, x, y-1)) {
		map[y][x] = 5;
		return true;
	} else {
		return false;
	}
}

bool bf_helper(int**);
void bf_final_path_maker(int**, int, int);

int queue[1000][2];
int parents[MAP_WIDTH][MAP_HEIGHT][2];
int queue_start = 0;
int queue_end = 0;

bool bf_search(int **map)
{
	queue_start = 0;
	queue_end = 0;

	for (int i=0; i<MAP_WIDTH; i++) {
		for (int j=0; j<MAP_HEIGHT; j++) {
			if (map[j][i] == 2) {
				queue[queue_end][0] = i;
				queue[queue_end][1] = j;
				parents[i][j][0] = -1;
				parents[i][j][1] = -1;
				queue_end++;
				map[j][i] = 4;

				return bf_helper(map);
			}
		}
	}
}

bool bf_helper(int **map)
{
	int node;
	while (queue_start <  queue_end) {
		int x = queue[queue_start][0];
		int y = queue[queue_start][1];

		if (!((x+1<0) || (y<0) || (x+1>=MAP_WIDTH) || (y>=MAP_HEIGHT))) {
		node = map[y][x+1];
		if (node == 3) {
			map[y][x+1] = 5;
			bf_final_path_maker(map,x,y);
			return true;
		}
		if (node == 0) {
			queue[queue_end][0] = x+1;
			queue[queue_end][1] = y;
			queue_end++;
			parents[x+1][y][0] = x;
			parents[x+1][y][1] = y;
			map[y][x+1] = 4;
		}
		}

		if (!((x<0) || (y+1<0) || (x>=MAP_WIDTH) || (y+1>=MAP_HEIGHT))) {		
		node = map[y+1][x];
		if (node == 3) {
			map[y+1][x] = 5;
			bf_final_path_maker(map,x,y);
			return true;
		}
		if (node == 0) {
			queue[queue_end][0] = x;
			queue[queue_end][1] = y+1;
			queue_end++;
			parents[x][y+1][0] = x;
			parents[x][y+1][1] = y;
			map[y+1][x] = 4;
		}
		}

		if (!((x-1<0) || (y<0) || (x-1>=MAP_WIDTH) || (y>=MAP_HEIGHT))) {
		node = map[y][x-1];
		if (node == 3) {
			map[y][x-1] = 5;
			bf_final_path_maker(map,x,y);
			return true;
		}
		if (node == 0) {
			queue[queue_end][0] = x-1;
			queue[queue_end][1] = y;
			queue_end++;
			parents[x-1][y][0] = x;
			parents[x-1][y][1] = y;
			map[y][x-1] = 4;
		}
		}

		if (!((x<0) || (y-1<0) || (x>=MAP_WIDTH) || (y>=MAP_HEIGHT))) {
		node = map[y-1][x];
		if (node == 3) {
			map[y-1][x] = 5;
			bf_final_path_maker(map,x,y);
			return true;
		}
		if (node == 0) {
			queue[queue_end][0] = x;
			queue[queue_end][1] = y-1;
			queue_end++;
			parents[x][y-1][0] = x;
			parents[x][y-1][1] = y;
			map[y-1][x] = 4;
		}
		}

		queue_start++;
	}

	return false;
}

void bf_final_path_maker(int **map, int x, int y) 
{
	map[y][x] = 5;
	int x_next = parents[x][y][0];
	int y_next = parents[x][y][1];
	if (x_next != -1) bf_final_path_maker(map,x_next,y_next);
}
