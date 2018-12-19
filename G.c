#include <stdio.h>
#include <stdlib.h>
#define WIDTH 15
#define HEIGHT 15
#define WHITE 0
#define BLACK 1
#define WIN 2

char WHITE_FLAG = 'O';
char BLACK_FLAG = 'X';
char map[WIDTH][HEIGHT];
int qinxingPC[WIDTH][HEIGHT] = { 0 };
int qinxingPlayer[WIDTH][HEIGHT] = { 0 };

void init_map();
void draw_pc_map();
void draw_player_map();
void draw_map();
int horizontal(int row, int col, char whoFlag);
int vertical(int row, int col, char whoFlag);
int zuoxie(int row, int col, char whoFlag);
int youxie(int row, int col, char whoFlag);
int jieguo(int left, int right, int count, int k, char num);
void pcLoad();
void Scorer();

int playerLoad();
int is_win();

int player = BLACK;
int main() {
	int mode = 0, result, nums;
	
	do {
		printf("\nplease choose mode\n");
		printf("play with human please input 1\n");
		printf("play with computer please input 2\n");
		printf("exit please input 3\n");
		scanf("%d", &mode);

		init_map();
		draw_map();

		if (mode == 1) {
			for (nums = 0; nums < 225; nums++) {
				if (player == BLACK) {    //1下子
					printf("please white(%c) play\n", WHITE_FLAG);
					player = WHITE;
					result = playerLoad();
					if (!result) break;
				}
				else {    //2下子            
					printf("please black(%c) play\n", BLACK_FLAG);
					player = BLACK;
					result = playerLoad();
					if (!result) break;
				}

				system("clear");
				draw_map();
				if (is_win()) {
					if (player == WHITE)
						printf("WHITE(%c) is winner.\n", WHITE_FLAG);
					else
						printf("BLACK(%c) is winner.\n", BLACK_FLAG);
					break;
				}
			}
		}

		else if (mode == 2) {
			for (nums = 0; nums < 225; nums++) {
				if (player == BLACK) {
					//玩家下子
					printf("please white(%c) play\n", WHITE_FLAG);
					player = WHITE;
					result = playerLoad();
					if (!result) break;
				}
				else {
					//电脑下子
					player = BLACK;
					pcLoad();
				}

				draw_map();
				if (is_win()) {
					if (player == WHITE) {
						printf("WHITE(%c) is winner.\n", WHITE_FLAG);
					}
					else {
						printf("BLACK(%c) is winner.\n", BLACK_FLAG);
					}
					break;
				}
			}
		}
	} while (mode != 3);
	return 1;
}

void init_map() {
	int i, j = 0;

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			map[i][j] = '+';
}

void draw_pc_map() {
	int i, j, k;

	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < WIDTH; j++) {
			printf("%d", qinxingPC[i][j]);
			if (j<(WIDTH - 1)) printf("-");
		}
		putchar('\n');
	}
}

void draw_player_map() {
	int i = 0, j = 0;

	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < WIDTH; j++) {
			printf("%d", qinxingPlayer[i][j]);
			if (j<(WIDTH - 1)) printf("-");
		}
		putchar('\n');
	}
}


void draw_map() {
	int i = 0, j = 0, k = 0;

	printf("    X--------------------------->\n");
	printf("    ");
	for (k = 0; k < HEIGHT; k++) {
		if (k >= 0 && k <= 8)
			printf("%d ", k + 1);
		else
			printf("%d", k + 1);
	}
	putchar('\n');

	for (i = 0; i < WIDTH; i++) {
		printf("Y");
		if (i >= 0 && i <= 8)
			printf("0%d ", i + 1);
		else
			printf("%d ", i + 1);

		for (j = 0; j < WIDTH; j++) {
			putchar(map[i][j]);
			if (j<(WIDTH - 1)) putchar('-');
		}
		putchar('\n');
	}
}
int horizontal(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int count = 1;//几连，包含当前要下的子
	int leftHad = 0;//左边是否有同子
	int x = row;
	int y = col;
	int liveLeft = 0;
	int liveRight = 0;

	if (map[row][col] != '+') {
		return 0;
	}
	while (y>0 && (map[x][y - 1] == '+' || map[x][y - 1] == whoFlag)) {
		if (map[x][y - 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x][y - 2] != whoFlag) {
				liveLeft = 1;
				break;
			}
			spaceNum++;
			y--;
		}
		else if (map[x][y - 1] == whoFlag) {
			leftHad = 1;
			y--;
			count++;
		}
		else {//第2个空白
			liveLeft = 1;
			break;
		}
	}

	//如果左边没有同色子，设置空白数为0
	if (!leftHad) {
		spaceNum = 0;
	}

	y = col;
	while (y<14 && (map[x][y + 1] == '+' || map[x][y + 1] == whoFlag)) {
		if (map[x][y + 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x][y + 2] != whoFlag) {
				liveRight = 1;
				break;
			}
			spaceNum++;
			y++;
		}
		else if (map[x][y + 1] == '+' && spaceNum>0) {//第2个空白
			liveRight = 1;
			break;
		}
		else {
			y++;
			count++;
		}
	}
	return jieguo(liveLeft, liveRight, count, spaceNum, whoFlag);
}

int vertical(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int count = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row;
	int y = col;
	int liveLeft = 0;
	int liveRight = 0;

	if (map[row][col] != '+') {
		return 0;
	}
	while (x>0 && (map[x - 1][y] == '+' || map[x - 1][y] == whoFlag)) {
		if (map[x - 1][y] == '+' && spaceNum<1) {//第一个空白
			if (map[x - 2][y] != whoFlag) {
				liveLeft = 1;
				break;
			}
			spaceNum++;
			x--;
		}
		else if (map[x - 1][y] == whoFlag) {
			topHad = 1;
			x--;
			count++;
		}
		else {//第2个空白
			liveLeft = 1;
			break;
		}
	}
	//如果左边没有同色子，设置空白数为0
	if (!topHad) {
		spaceNum = 0;
	}
	x = row;
	while (x<14 && (map[x + 1][y] == '+' || map[x + 1][y] == whoFlag)) {
		if (map[x + 1][y] == '+' && spaceNum<1) {//第一个空白
			if (map[x + 2][y] != whoFlag) {
				liveRight = 1;
				break;
			}
			spaceNum++;
			x++;
		}
		else if (map[x + 1][y] == '+' && spaceNum>0) {//第2个空白
			liveRight = 1;
			break;
		}
		else {
			x++;
			count++;
		}
	}
	return jieguo(liveLeft, liveRight, count, spaceNum, whoFlag);
}


// +-+-+-@-+
// +-+-@-+-+
// +-@-+-+-+
int zuoxie(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int count = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row;
	int y = col;
	int liveLeft = 0;
	int liveRight = 0;

	if (map[row][col] != '+') {
		return 0;
	}
	//向下
	while (x<14 && y>0 && (map[x + 1][y - 1] == '+' || map[x + 1][y - 1] == whoFlag)) {
		if (map[x + 1][y - 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x + 2][y - 2] != whoFlag) {
				liveLeft = 1;
				break;
			}
			spaceNum++;
			x++;
			y--;
		}
		else if (map[x + 1][y - 1] == whoFlag) {
			topHad = 1;
			x++;
			y--;
			count++;
		}
		else {//第2个空白
			liveLeft = 1;
			break;
		}
	}
	//如果上边没有同色子，设置空白数为0
	if (!topHad) {
		spaceNum = 0;
	}

	x = row;
	y = col;
	//向上
	while (x>0 && y<14 && (map[x - 1][y + 1] == '+' || map[x - 1][y + 1] == whoFlag)) {
		if (map[x - 1][y + 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x - 2][y + 2] != whoFlag) {
				liveRight = 1;
				break;
			}
			spaceNum++;
			x--;
			y++;
		}
		else if (map[x - 1][y + 1] == '+' && spaceNum>0) {//第2个空白
			liveRight = 1;
			break;
		}
		else {
			x--;
			y++;
			count++;
		}
	}
	return jieguo(liveLeft, liveRight, count, spaceNum, whoFlag);
}

int youxie(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int count = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row;
	int y = col;
	int liveLeft = 0;
	int liveRight = 0;

	if (map[row][col] != '+') {
		return 0;
	}
	//向上
	while (x>0 && y>0 && (map[x - 1][y - 1] == '+' || map[x - 1][y - 1] == whoFlag)) {
		if (map[x - 1][y - 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x - 2][y - 2] != whoFlag) {
				liveLeft = 1;
				break;
			}
			spaceNum++;
			x--;
			y--;
		}
		else if (map[x - 1][y - 1] == whoFlag) {
			topHad = 1;
			x--;
			y--;
			count++;
		}
		else {//第2个空白
			liveLeft = 1;
			break;
		}
	}

	//如果上边没有同色子，设置空白数为0
	if (!topHad) {
		spaceNum = 0;
	}
	x = row;
	y = col;
	//向下
	while (x<14 && y<14 && (map[x + 1][y + 1] == '+' || map[x + 1][y + 1] == whoFlag)) {
		if (map[x + 1][y + 1] == '+' && spaceNum<1) {//第一个空白
			if (map[x + 2][y + 2] != whoFlag) {
				liveRight = 1;
				break;
			}
			spaceNum++;
			x++;
			y++;
		}
		else if (map[x + 1][y + 1] == '+' && spaceNum>0) {//第2个空白
			liveRight = 1;
			break;
		}
		else {
			x++;
			y++;
			count++;
		}
	}
	return jieguo(liveLeft, liveRight, count, spaceNum, whoFlag);
}

int jieguo(int left, int right, int count, int k, char num) {
	if (count == 1) {
		return 1;
	}
	else if (count == 2) {
		if (left && right) {//左右两边都是空的
			if (k == 0) {
				//电脑60
				return num == BLACK_FLAG ? 60 : 50;
			}
			else {
				return num == BLACK_FLAG ? 40 : 35;
			}
		}
		else if (!left && !right) {
			return 1;
		}
		else {
			return 10;
		}
	}
	else if (count == 3) {

		if (left && right) {//左右两边都是空的
			if (k == 0) {
				//电脑950
				return num == BLACK_FLAG ? 950 : 700;
			}
			else {
				return num == BLACK_FLAG ? 900 : 650;
			}
		}
		else if (!left && !right) {
			return 1;
		}
		else {
			return 100;
		}
	}
	else if (count == 4) {
		if (left && right) {//左右两边都是空的
			if (k == 0) {
				return num == BLACK_FLAG ? 6000 : 3500;
			}
			else {
				return num == BLACK_FLAG ? 5000 : 3000;
			}
		}
		else if (!left && !right) {
			return 1;
		}
		else {
			if (k == 0) {
				return num == BLACK_FLAG ? 4000 : 800;
			}
			else {
				return num == BLACK_FLAG ? 3600 : 750;
			}
		}
	}
	else {
		if (k == 0) {
			return num == BLACK_FLAG ? 20000 : 15000;
		}
		else {
			return num == BLACK_FLAG ? 10000 : 3300;
		}
	}

}

void pcLoad() {
	Scorer();

	int count = 0, row = 0, col = 0, i = 0, j = 0;

	for (i = 0; i< 15; i++) {
		for (j = 0; j< 15; j++) {
			if (qinxingPC[i][j] > count) {
				count = qinxingPC[i][j];
				row = i;
				col = j;
			}
			if (qinxingPlayer[i][j] > count) {
				count = qinxingPlayer[i][j];
				row = i;
				col = j;
			}
		}
	}
	system("clear");
	printf("PC[%d][%d]\n", col + 1, row + 1);
	if (map[row][col] == '+') {
		map[row][col] = BLACK_FLAG;
	}

}

void Scorer() {     //记分
	int n = 0;
	int m = 0;
	for (n = 0; n<15; n++) {
		for (m = 0; m<15; m++) {
			qinxingPC[n][m] = horizontal(n, m, BLACK_FLAG) + vertical(n, m, BLACK_FLAG) + zuoxie(n, m, BLACK_FLAG) + youxie(n, m, BLACK_FLAG);
			qinxingPlayer[n][m] = horizontal(n, m, WHITE_FLAG) + vertical(n, m, WHITE_FLAG) + zuoxie(n, m, WHITE_FLAG) + youxie(n, m, WHITE_FLAG);
		}
	}

}

int playerLoad() {  //玩家下子
	int x, y;
	int res;

	printf("please input x y:");
	scanf("%d %d", &y, &x);

	if (x < 0 || y < 0 || x > HEIGHT || y > WIDTH) {
		printf("input error,again\n");
		while ((getchar()) != '\n');
		res = playerLoad();
		if (res == 1) return 1;
	}
	x--;
	y--;

	if (map[x][y] == '+') {
		if (player == WHITE)
			map[x][y] = WHITE_FLAG;
		else if (player == BLACK)
			map[x][y] = BLACK_FLAG;
	}
	else {
		printf("had it,again\n");
		while ((getchar()) != '\n');
		playerLoad();
		if (res == 1) return 1;
	}
	return 1;
}

int is_win() {
	char m;
	int i, j;

	if (player == WHITE)
		m = WHITE_FLAG;
	else m = BLACK_FLAG;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (map[i][j] == m) {
				if ((i + 4) < HEIGHT)
					if (map[i + 1][j] == m && map[i + 2][j] == m && map[i + 3][j] == m && map[i + 4][j] == m)
						return 1;
				if ((j + 4) < WIDTH)
					if (map[i][j + 1] == m && map[i][j + 2] == m && map[i][j + 3] == m && map[i][j + 4] == m)
						return 1;
				if ((i + 4) < HEIGHT && (j + 4) < WIDTH)
					if (map[i + 1][j + 1] == m && map[i + 2][j + 2] == m && map[i + 3][j + 3] == m && map[i + 4][j + 4] == m)
						return 1;
				if ((i + 4) < HEIGHT && (j - 4) >= 0)
					if (map[i + 1][j - 1] == m && map[i + 2][j - 2] == m && map[i + 3][j - 3] == m && map[i + 4][j - 4] == m)
						return 1;
			}
		}
	}
	return 0;
}
