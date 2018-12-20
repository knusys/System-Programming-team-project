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
int PC_map[WIDTH][HEIGHT] = { 0 };
int Player_map[WIDTH][HEIGHT] = { 0 };

void init_map();
void draw_pc_map();
void draw_player_map();
void draw_map();
int horizontal(int row, int col, char whoFlag);
int vertical(int row, int col, char whoFlag);
int LeftObli(int row, int col, char whoFlag);   // Left oblique
int RightObl(int row, int col, char whoFlag);   // Right oblique
int result(int left, int right, int cnt, int k, char num);
int Player_opp();   //Player opportunities
void PC_opp();   // Pc opportunities
void Scorer();
int win();

int player = BLACK;
int main() {
	int mode = 0, result, nums;

	do {
		printf("-------------------------------------\n");
		printf("+                                   +\n");
		printf("+    ( White is O, Black is X. )    +\n");
		printf("+                                   +\n");
		printf("+         Please choose mode.       +\n");
		printf("+                                   +\n");
		printf("+                                   +\n");
		printf("+  Play with human, please input 1. +\n");
		printf("+         ( First hand is O )       +\n");
		printf("+                                   +\n");
		printf("+    Play with PC, please input 2.  +\n");
		printf("+      ( First hand is O , and      +\n");
		printf("+           you will first )        +\n");
		printf("+                                   +\n");
		printf("+         Exit please input 3.      +\n");
		printf("+                                   +\n");
		printf("-------------------------------------\n");
		scanf("%d", &mode);

		system("clear");
		init_map();

		if (mode != 3)
			draw_map();

		if (mode == 1) {
			for (nums = 0; nums < 225; nums++) {
				if (player == BLACK) {
					printf("Please White(%c) play.\n", WHITE_FLAG);
					player = WHITE;
					result = Player_opp();
					if (!result) break;
				}
				else {
					printf("Please Black(%c) play.\n", BLACK_FLAG);
					player = BLACK;
					result = Player_opp();
					if (!result) break;
				}
				system("clear");
				draw_map();
				if (win()) {
					if (player == WHITE)
						printf("\n       White(%c) is the winner.\n", WHITE_FLAG);
					else
						printf("\n       Black(%c) is the winner.\n", BLACK_FLAG);
					break;
				}
			}
		}

		else if (mode == 2) {
			for (nums = 0; nums < 225; nums++) {
				if (player == BLACK) {
					printf("Please player(%c) play\n", WHITE_FLAG);
					player = WHITE;
					result = Player_opp();
					if (!result) break;
				}
				else {
					player = BLACK;
					PC_opp();
				}
				draw_map();
				if (win()) {
					if (player == WHITE) {
						printf("\n       Player(%c) is the winner.\n", WHITE_FLAG);
					}
					else
						printf("\n       PC(%c) is the winner.\n", BLACK_FLAG);

					break;
				}
			}
		}
	} while (mode != 3);
	return 0;
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
			printf("%d", PC_map[i][j]);
			if (j<(WIDTH - 1))
				printf("-");
		}
		putchar('\n');
	}
}

void draw_player_map() {
	int i = 0, j = 0;

	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < WIDTH; j++) {
			printf("%d", Player_map[i][j]);
			if (j<(WIDTH - 1))
				printf("-");
		}
		putchar('\n');
	}
}


void draw_map() {
	int i = 0, j = 0, k = 0;

	printf("    X axis----------------------->\n");
	printf("    ");
	for (k = 0; k < HEIGHT; k++) {
		if (k >= 0 && k <= 8)
			printf("%d ", k + 1);
		else
			printf("%d", k + 1);
	}
	putchar('\n');

	for (i = 0; i < WIDTH; i++) {
		if(i == 0)
			printf("Y");
		else if(i == WIDTH - 1)
			printf("V");
		else printf("|");
			
		if (i >= 0 && i <= 8)
			printf("0%d ", i + 1);
		else
			printf("%d ", i + 1);

		for (j = 0; j < WIDTH; j++) {
			putchar(map[i][j]);
			if (j<(WIDTH - 1))
				putchar('-');
		}
		putchar('\n');
	}
}
int horizontal(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int cnt = 1;//几连，包含当前要下的子
	int leftHad = 0;//左边是否有同子
	int x = row, y = col, liveLeft = 0, liveRight = 0;

	if (map[row][col] != '+')
		return 0;

	while (y > 0 && (map[x][y - 1] == '+' || map[x][y - 1] == whoFlag)) {
		if (map[x][y - 1] == '+' && spaceNum<1) {  //第一个空白
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
			cnt++;
		}
		else {  //第2个空白
			liveLeft = 1;
			break;
		}
	}

	//如果左边没有同色子，设置空白数为0
	if (!leftHad)
		spaceNum = 0;

	y = col;
	while (y<14 && (map[x][y + 1] == '+' || map[x][y + 1] == whoFlag)) {
		if (map[x][y + 1] == '+' && spaceNum < 1) {//第一个空白
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
			cnt++;
		}
	}
	return result(liveLeft, liveRight, cnt, spaceNum, whoFlag);
}

int vertical(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int cnt = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row,y = col, liveLeft = 0, liveRight = 0;

	if (map[row][col] != '+')
		return 0;

	while (x > 0 && (map[x - 1][y] == '+' || map[x - 1][y] == whoFlag)) {
		if (map[x - 1][y] == '+' && spaceNum < 1) {//第一个空白
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
			cnt++;
		}
		else {//第2个空白
			liveLeft = 1;
			break;
		}
	}
	//如果左边没有同色子，设置空白数为0
	if (!topHad)
		spaceNum = 0;

	x = row;
	while (x < 14 && (map[x + 1][y] == '+' || map[x + 1][y] == whoFlag)) {
		if (map[x + 1][y] == '+' && spaceNum < 1) {//第一个空白
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
			cnt++;
		}
	}
	return result(liveLeft, liveRight, cnt, spaceNum, whoFlag);
}


// +-+-+-X-+
// +-+-X-+-+
// +-X-+-+-+
int LeftObli(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int count = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row, y = col, liveLeft = 0, liveRight = 0;

	if (map[row][col] != '+')
		return 0;
	//向下
	while (x<14 && y>0 && (map[x + 1][y - 1] == '+' || map[x + 1][y - 1] == whoFlag)) {
		if (map[x + 1][y - 1] == '+' && spaceNum<1) {  //第一个空白
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
		else {  //第2个空白
			liveLeft = 1;
			break;
		}
	}
	//如果上边没有同色子，设置空白数为0
	if (!topHad)
		spaceNum = 0;

	x = row;
	y = col;
	//向上
	while (x>0 && y<14 && (map[x - 1][y + 1] == '+' || map[x - 1][y + 1] == whoFlag)) {
		if (map[x - 1][y + 1] == '+' && spaceNum<1) {  //第一个空白
			if (map[x - 2][y + 2] != whoFlag) {
				liveRight = 1;
				break;
			}
			spaceNum++;
			x--;
			y++;
		}
		else if (map[x - 1][y + 1] == '+' && spaceNum>0) {   //第2个空白
			liveRight = 1;
			break;
		}
		else {
			x--;
			y++;
			count++;
		}
	}
	return result(liveLeft, liveRight, count, spaceNum, whoFlag);
}

int RightObl(int row, int col, char whoFlag) {
	int spaceNum = 0;//空白数
	int cnt = 1;//几连，包含当前要下的子
	int topHad = 0;//上边是否有同子
	int x = row, y = col, liveLeft = 0, liveRight = 0;

	if (map[row][col] != '+')
		return 0;
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
			cnt++;
		}
		else {   //第2个空白
			liveLeft = 1;
			break;
		}
	}

	//如果上边没有同色子，设置空白数为0
	if (!topHad)
		spaceNum = 0;

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
			cnt++;
		}
	}
	return result(liveLeft, liveRight, cnt, spaceNum, whoFlag);
}

int result(int left, int right, int cnt, int k, char num) {

	if (cnt == 1)
		return 1;
	else if (cnt == 2) {
		if (left && right) {   //左右两边都是空的
			if (k == 0)
				if (num == BLACK_FLAG)
					return 60;       //电脑60
				else
					return 50;
			else
				if (num == BLACK_FLAG)
					return 40;
				else
					return 35;
		}
		else if (!left && !right)
			return 1;
		else
			return 10;
	}
	else if (cnt == 3) {

		if (left && right) {  //左右两边都是空的
			if (k == 0)
				if (num == BLACK_FLAG)
					return 950;
				else
					return 700;
			else
				if (num == BLACK_FLAG)
					return 900;
				else
					return 650;;
		}
		else if (!left && !right)
			return 1;
		else
			return 100;
	}
	else if (cnt == 4) {
		if (left && right) {  //左右两边都是空的
			if (k == 0)
				if (num == BLACK_FLAG)
					return 6000;
				else
					return 3500;
			else
				if (num == BLACK_FLAG)
					return 5000;
				else
					return 3000;
		}
		else if (!left && !right)
			return 1;
		else {
			if (k == 0)
				if (num == BLACK_FLAG)
					return 4000;
				else
					return 800;
			else
				if (num == BLACK_FLAG)
					return 3600;
				else
					return 750;
		}
	}
	else {
		if (k == 0)
			if (num == BLACK_FLAG)
				return 20000;
			else
				return 15000;
		else
			if (num == BLACK_FLAG)
				return 10000;
			else
				return 3300;
	}
}

int Player_opp() {

	int x, y, res;

	printf("Please input numeric coordinates(x y):");
	scanf("%d %d", &y, &x);

	if (x > HEIGHT || y > WIDTH || x < 0 || y < 0) {
		printf("Input error,please enter numeric coordinates again.\n");
		while ((getchar()) != '\n');
		res = Player_opp();
		if (res == 1)
			return 1;
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
		printf("Already have a chess,please enter again.\n");
		while ((getchar()) != '\n');
		Player_opp();
		if (res == 1)
			return 1;
	}
	return 1;
}

void PC_opp() {

	int cnt = 0, row = 0, col = 0, i = 0, j = 0;

	Scorer();

	for (i = 0; i< 15; i++) {
		for (j = 0; j< 15; j++) {
			if (PC_map[i][j] > cnt) {
				cnt = PC_map[i][j];
				row = i;
				col = j;
			}
			if (Player_map[i][j] > cnt) {
				cnt = Player_map[i][j];
				row = i;
				col = j;
			}
		}
	}

	system("clear");
	printf("\n        PC is put on [%d][%d]\n\n", col + 1, row + 1);

	if (map[row][col] == '+')  map[row][col] = BLACK_FLAG;
}

void Scorer() {

	int x = 0, y = 0;
	for (x = 0; x < 15; x++) {
		for (y = 0; y < 15; y++) {
			PC_map[x][y] = horizontal(x, y, BLACK_FLAG) + vertical(x, y, BLACK_FLAG) + LeftObli(x, y, BLACK_FLAG) + RightObl(x, y, BLACK_FLAG);
			//
			Player_map[x][y] = horizontal(x, y, WHITE_FLAG) + vertical(x, y, WHITE_FLAG) + LeftObli(x, y, WHITE_FLAG) + RightObl(x, y, WHITE_FLAG);
			//
		}
	}
}

int win() {
	char cp;   //chess piece
	int i, j;

	if (player == WHITE)
		cp = WHITE_FLAG;
	else cp = BLACK_FLAG;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (map[i][j] == cp) {
				if ((i + 4) < HEIGHT)
					if (map[i + 1][j] == cp && map[i + 2][j] == cp && map[i + 3][j] == cp && map[i + 4][j] == cp)
						// Are there five chess pieces attached to the horizontal line?
						return 1;
				if ((j + 4) < WIDTH)
					if (map[i][j + 1] == cp && map[i][j + 2] == cp && map[i][j + 3] == cp && map[i][j + 4] == cp)
						// Are there five chess pieces attached to the vertical line?
						return 1;
				if ((i + 4) < HEIGHT && (j + 4) < WIDTH)
					if (map[i + 1][j + 1] == cp && map[i + 2][j + 2] == cp && map[i + 3][j + 3] == cp && map[i + 4][j + 4] == cp)
						// Are there five chess pieces attached to the left oblique line?
						return 1;
				if ((i + 4) < HEIGHT && (j - 4) >= 0)
					if (map[i + 1][j - 1] == cp && map[i + 2][j - 2] == cp && map[i + 3][j - 3] == cp && map[i + 4][j - 4] == cp)
						// Are there five chess pieces attached to the right oblique line?
						return 1;
			}
		}
	}
	return 0;
}
