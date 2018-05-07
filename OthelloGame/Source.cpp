#include<stdio.h>
#include<windows.h>

//マスの状態定義
#define NONE 0
#define BLACK 1
#define WHITE 2

//ボード（左上を[0][0])
int board[9][9] =
{
	{ 0,0,0,0,0,0,0,0,-1 },
	{ 0,0,0,0,0,0,0,0,-1 },
	{ 0,0,0,0,0,0,0,0,-1 },
	{ 0,0,0,2,1,0,0,0,-1 },
	{ 0,0,0,1,2,0,0,0,-1 },
	{ 0,0,0,0,0,0,0,0,-1 },
	{ 0,0,0,0,0,0,0,0,-1 },
	{ 0,0,0,0,0,0,0,0,-1 },
{-1,-1,-1,-1,-1,-1,-1,-1,-1}
};

//マスの状態の数
static int NONE_NUMBER;			//空きマス
static int BLACK_NUMBER = 2;	//黒駒
static int WHITE_NUMBER = 2;	//白駒
int FLAG = 1;					//継続判定 1:続行 0:終了
int Date[7];					//裏返しの判定材料 自分がいる情報は無視
bool Turn_FLAG = false;			//どこか1列でも裏返せるかの判定 1:可 0:不
bool PassFlag = false;			//パスができるかの判定 1:可 0:不
static int y, x;				//座標

int turn = BLACK;				//1がBLACK、2がWHITE 
int Player1, Player2;			//プレイヤー

void Disp(void);														//画面の更新・駒数の再描画
void CanPut(void);														//駒が置けるかの処理
int CanPut_sub(int place);												//CanPutに入力した位置に駒が置いてるかを返す 1:ない 0:ある
void PassCheck(void);													//パス判定　両方駒は残っているけど置けない時の処理
void TurnOver(int Date[7],int place,int j,int sub_count);				//裏返す処理 turn処理
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER,int NONE_NUMBER);		//ゲームが続行できるかの判定 1は続行 0は終了
/*---------------------------------MAIN----------------------------------------------*/
int main(void) {
	while(FLAG){//継続判定
		Disp();
		PassCheck();
		CanPut();
	}
	if (BLACK_NUMBER > WHITE_NUMBER) {//勝利判定
		printf("黒の勝利!!\n");
	}
	else if (WHITE_NUMBER > BLACK_NUMBER) {
		printf("白の勝利!!\n");
	}
	else if (BLACK_NUMBER == WHITE_NUMBER) {
		printf("引き分け");
	}
	return 0;
}

//画面の更新・駒数の再描画
void Disp() {
	NONE_NUMBER = 0;	//空きマス
	BLACK_NUMBER = 0;	//黒駒
	WHITE_NUMBER = 0;	//白駒
	//system("cls");		//画面クリア
	for (int i = 0; i < 9; i++) {		//周りに-1を入れて初期化
		board[i][8] = -1;
		for (int j = 0; j < 9; j++) {
			board[8][i] = -1;
		}
	}
	printf(" ");
	for (int i = 1; i <= 8; i++) {		//行を描画
		printf("%2d", i);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		
		printf("%d", ((i + 1) * 10));	//列を描写
		for (int j = 0; j < 8; j++) {
			switch (board[i][j]) {
			case NONE: printf("□"); NONE_NUMBER++; break;			//駒が無かったら'□'			
			case BLACK: printf("○"); BLACK_NUMBER++; break;		//1(黒)が入っていたら'○'	
			case WHITE: printf("●"); WHITE_NUMBER++; break;		//2(白)が入っていたら'●'	
			default: break;
			}
		}
		printf("\n");
	}
	printf("○黒駒の数:%2d\n", BLACK_NUMBER);
	printf("●白駒の数:%2d\n", WHITE_NUMBER);
	FLAG = FinishCheck(BLACK_NUMBER, WHITE_NUMBER, NONE_NUMBER);	//継続判断
}

//駒が置けるかの処理
void CanPut() { 
	char pl[3];			//外部の文字を格納
	int place;			//char型をキャストするための整数
	if (FLAG) {			//継続可能のときだけ表示する
		printf("---------------------------\n");
		printf("場所を選択して下さい\n");
		printf("例 40 + 2 -> 42\n");
		printf("---------------------------\n");
		if (turn == BLACK) {
			printf("黒の番です\n");
		}
		else {
			printf("白の番です\n");
		}
		printf("場所:");
	}
	else {
		return;
	}
	while (1) {					//入力処理
		scanf_s("%s", pl, 3);	//置く場所の選択
		if (isdigit(*pl)) {		//数字以外が入っているかチェック
			place = atoi(pl);	//文字列を整数型にキャスト
			if (place >= 11 && place <= 88 && (place % 10) != 9 && (place % 10) != 0) {	//置ける範囲チェック
				if (CanPut_sub(place)) {	//すでに駒が置いてあるか
					break;
				}
				else {
					printf("\n入力した場所は無効です\n");
					printf("場所:");
				}
			}
			else {
				printf("\n入力した文字は無効です\n");
				printf("場所:");
			}
		}
		else {
			printf("\n入力した文字は無効です\n");
			printf("場所:");
		}
	}
}

//CanPutに入力した位置に駒が置いてるかを返す 1:ない 0:ある
int CanPut_sub(int place) {
	int sub_count = 1;	//TurnOver関数の呼び出し回数
	y = place / 10;		//渡された整数を分割
	x = place % 10;
	y--;
	x--;
	if (board[y][x] == 0) {		//駒がすでにある場合は置けない
		int j;				//何回回るかの値
							//入力された位置からの全方位のデータを関数TurnOverに値を渡している				
		j = y - 1;								//1:上
		for (int i = 0; i <= j; i++) {
			if (board[--y][x]) {
				Date[i] = board[y][x];
			}
			else {
				Date[i] = 0;
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = y - 1;								//2:右上
		for (int i = 0; i <= j; i++) {
			if (board[--y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = 6;									//3:右
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][++x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = 6;									//4:右下
		for (int i = 0; i <= j; i++) {
			if (board[++y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = 7 - y;								//5:下
		for (int i = 0; i <= j; i++) {
			Date[i] = board[++y][x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//6:左下
		for (int i = 0; i <= j; i++) {
			if (board[++y][--x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//7:左							
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][--x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//8:左上
		for (int i = 0; i <= j; i++) {
			if (board[--y][--x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		return 1;
	}
	else {
		return 0;
	}
}

//パス判定　両方駒は残っているけど置けない時の処理
void PassCheck() {
	int place = 0;
	for (int i = 0; board[i][y] != -1;i++) {
		for (int y = 0; board[i][y] != -1; y++) {
			if (board[i][y] == turn) {
				place = ((i + 1) * 10) + (y + 1);
				//printf("%2d", place);
				CanPut_sub(place);
			}
			//printf("%2d", board[i][y]);
		}
		//printf("\n");
	}
}

//裏返す処理
void TurnOver(int Date[7],int place, int j,int sub_count){
	int count_FLAG;				//その1列を裏返せるかの判定　1:可 0:不
	int  count;					//駒を裏返す回数
	count = count_FLAG = 0;		//初期化
	y = place / 10;				//渡された整数を分割
	x = place % 10;
	y--;
	x--;
	
	//何回裏返せるかの判定
	if (Date[0] <= 0 || Date[0] == turn) {			//1列目の0番目の要素が0,-1,自分のターンの色でないときの処理
	}
	else {
		if (turn == BLACK) {						//黒番の処理
			for (int i = 0; i <= j; i++) {
				if (Date[i] == WHITE) {				//1列目のi番目の要素が白の間回る
					count++;
				}
				else if (Date[i] == BLACK) {
					count_FLAG = 1;					//白駒を黒駒で挟めた時の判定
					break;
				}
				else if (Date[i] <= 0) {
					break;
				}
			}
		}
		else if (turn == WHITE) {					//白番の処理
			for (int i = 0; i <= j; i++) {
				if (Date[i] == BLACK) {				//1列目のi番目の要素が黒の間回る
					count++;
				}
				else if (Date[i] == WHITE) {
					count_FLAG = 1;					//白駒を黒駒で挟めた時の判定
					break;
				}
				else if (Date[i] <= 0) {
					break;
				}
			}
		}
	}

	//裏返しの処理
	switch (sub_count) {							//sub_countの数字によって処理する　例）1,上　4,右下
	case 1:											//上
		if (count_FLAG) {							
			for (int i = 1; i <= count; i++) {		//countの回数分だけ回る
				board[y - i][x] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 2:											//右上
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y - i][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 3:											//右
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 4:											//右下
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 5:											//下
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 6:											//左下
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x - i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 7:											//左
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y][x - i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 8:											//左上
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y - i][x - i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	default:
		break;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (sub_count == 8) {					//ターンチェンジ
		sub_count = 0;
		if (Turn_FLAG != true) {			//どの方位も裏返らない
			printf("置けません\n");
		}
		else {
			board[y][x] = turn;				//入力した場所に駒を置く
			if (turn == BLACK) {
				turn = WHITE;				//ホワイトは後攻
			}
			else {
				turn = BLACK;				//ブラックは先攻
			}
		}
		Turn_FLAG = false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//ゲームが続行できるかの判定 1は続行 0は終了
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER, int NONE_NUMBER) {
	if (BLACK_NUMBER == 0) {										//どちらかの色が一色になってないか
		printf("すべての駒が白になりました。\n");
		FLAG = 0;
	}
	else if (WHITE_NUMBER == 0) {
		printf("すべての駒が黒になりました。\n");
		FLAG = 0;
	}
	else if (NONE_NUMBER == 0) {									//盤面が埋まっているか
		FLAG = 0;
	}
	return FLAG;
}