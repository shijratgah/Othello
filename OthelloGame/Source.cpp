#include<stdio.h>
#include<windows.h>

//マスの状態定義
#define NONE 0
#define BLACK 1
#define WHITE 2

/*
//ボード（左上を[0][0])
int board[8][8] = 
{
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,2,1,0,0,0 },
	{ 0,0,0,1,2,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 }
};
*/

//ボード（左上を[0][0])
int board[9][9] =
{

	{ 0,0,0,2,0,0,1,0,-1 },
	{ 0,1,0,1,0,2,0,0,-1 },
	{ 0,0,1,1,2,0,0,0,-1 },
	{ 2,0,2,0,1,1,2,0,-1 },
	{ 0,0,2,1,2,0,0,0,-1 },
	{ 0,2,0,2,0,1,0,0,-1 },
	{ 2,0,0,2,0,0,1,0,-1 },
	{ 0,0,0,2,0,0,0,2,-1 },
{-1,-1,-1,-1,-1,-1,-1,-1,-1}

};


//マスの状態の数
static int NONE_NUMBER;  //空きマス
static int BLACK_NUMBER = 2; //黒駒
static int WHITE_NUMBER = 2; //白駒
int FLAG = 1;//継続判定 1:続行 0:終了
int Date[7];//裏返しの判定材料 自分がいる情報は無視
static int sub_count = 0; //関数の呼び出し回数
static int y, x;//座標

int turn = WHITE;			//1が先手、2が後手 
int Player1, Player2;//プレイヤー

void Disp(void); //画面の更新・駒数の再描画
void CanPut(void); //置けるかの処理　パス判定　両方駒は残っているけど置けない
void TurnOver(int Date[7],int place,int j); //裏返す処理 turn処理
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER,int NONE_NUMBER); //ゲームが続行できるかの判定 1は続行 0は終了
/*---------------------------------MAIN----------------------------------------------*/
int main(void) {
	//継続判定
	while(FLAG){
		Disp();
		CanPut();
	}
	//勝利判定
	if (BLACK_NUMBER > WHITE_NUMBER) {

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
	NONE_NUMBER = 0;  //空きマス
	BLACK_NUMBER = 0; //黒駒
	WHITE_NUMBER = 0; //白駒
	//画面クリア
	//system("cls");
	//行を描画
	printf(" ");
	for (int i = 1; i <= 8; i++) {
		printf("%2d", i);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		//列を描写
		printf("%d", ((i + 1) * 10));
		for (int j = 0; j < 8; j++) {
			switch (board[i][j]) {
			case NONE: printf("□"); NONE_NUMBER++; break; //駒が無かったら'□'			
			case BLACK: printf("○"); BLACK_NUMBER++; break;//1(黒)が入っていたら'○'	
			case WHITE: printf("●"); WHITE_NUMBER++; break;//2(白)が入っていたら'●'	
			default: break;
			}
		}
		printf("\n");
	}
	printf("○黒駒の数:%2d\n", BLACK_NUMBER);
	printf("●白駒の数:%2d\n", WHITE_NUMBER);
	//継続判断
	FLAG = FinishCheck(BLACK_NUMBER, WHITE_NUMBER, NONE_NUMBER);
}


//置けるかの処理
void CanPut() {
	char pl[3];//外部の文字を格納
	int place;//char型をキャストするための整数
	//継続可能のときだけ表示する
	if (FLAG) {
		printf("---------------------------\n");
		printf("場所を選択して下さい\n");
		printf("例 40 + 2 -> 42\n");
		printf("---------------------------\n");
		printf("場所:");
	}
	else {
		return;
	}
	while (1) {
		//置く場所の選択
		scanf_s("%s", pl, 3);
		//数字以外が入っているかチェック
		if (isdigit(*pl)) {
			//文字列を整数型にキャスト
			place = atoi(pl);
			//置ける範囲チェック
			if (place >= 11 && place <= 88 && (place % 10) != 9 && (place % 10) != 0) {
				break;
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
	
	//渡された整数を分割
	y = place / 10;
	x = place % 10;
	y--;
	x--;
	
	//駒がすでにある場合は置けない
	if (board[y][x] == 0) {
		int j;		//何回回るかの値
		//1:上ok
		j = y - 1;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[--y][x];
		}
		TurnOver(Date,place,j);
		//2:右上
		j = y - 1;
		for (int i = 0; i <= j; i++) {
			if (board[--y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j);
		//3:右ok
		j = 6;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][++x];
		}
		TurnOver(Date, place, j);
		//4:右下
		j = 6;
		for (int i = 0; i <= j; i++) {
			if (board[++y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j);
		//5:下ok
		j = 7 - y;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[++y][x];
		}
		TurnOver(Date, place, j);
		//6:左下
		j = x - 1;
		for (int i = 0; i <= j; i++) {
			if (board[++y][--x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j);
		//7:左ok
		j = x - 1;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][--x];
		}
		TurnOver(Date, place, j);
		//8:左上
		j = x - 1;
		for (int i = 0; i <= j; i++) {
			if (board[--y][--x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j);
	}
	else {
		printf("置けません\n");
	}
}



//裏返す処理
void TurnOver(int Date[7],int place, int j){
	int TurnFlag = 0; //ひっくり返すかの判定 1:続行 0:終了
	int  count = 0;
	sub_count++;

	//渡された整数を分割
	y = place / 10;
	x = place % 10;
	y--;
	x--;
	printf("count: %d\n", sub_count);
	for (int i = 0; i <= j; i++) {
		printf("%d", Date[i]);
	}
	printf("\n");
	printf("%d", j);
	printf("\n");




		//int judge;//裏返せるかの判定
	if (Date[0] <= 0 || Date[0] == turn) {
		printf("返せない\n\n");
	}
	else {
		TurnFlag = 1;
		printf("返せるmaybe\n");
		for (int i = 0; i <= j; i++) {
			if (turn == BLACK) {
				if (Date[++i] == WHITE) {
					count++;
					printf("//////////////BLACK///////////////////\n");
					printf("%d\n",count);
					printf("/////////////////////////////////\n");
				}
				else {
					break;
				}
			}
			else if (turn == WHITE) {
				if (Date[++i] == BLACK) {
					count++;
					printf("/////////////////////////////////\n");
					printf("%d\n", count);
					printf("/////////////////////////////////\n");
				}
				else {
					break;
				}
			}
			else if (Date[++i] == 0) {
				break;
			}
		}
		printf("\n");


	}


	//裏返しの処理
	switch (sub_count) {
	case 1:
		//上
		for (int i = 1; i <= count; i++) {
			board[y - i][x] = turn;
		}
		break;
		//右上
	case 2:
		for (int i = 1; i <= count; i++) {
			board[y - i][x + i] = turn;
		}
		break;
		//右
	case 3:
		for (int i = 1; i <= count; i++) {
			board[y][x + i] = turn;
		}
		break;
		//右下
	case 4:
		for (int i = 1; i <= count; i++) {
			board[y + i][x + i] = turn;
		}
		break;
		//下
	case 5:
		for (int i = 1; i <= count; i++) {
			board[y + i][x] = turn;
		}
		break;
		//左下
	case 6:
		for (int i = 1; i <= count; i++) {
			board[y + i][x - i] = turn;
		}
		break;
		//左
	case 7:
		for (int i = 1; i <= count; i++) {
			board[y][x - i] = turn;
		}
		break;
		//左上
	case 8:
		for (int i = 1; i <= count; i++) {
			board[y - i][x - i] = turn;
		}
		sub_count = 0;
		if (TurnFlag) {
			printf("裏返せれません\n");
		}
		break;
	default:
		break;
	}

	

	/*
	//ターン変更処理 8回とも置けなかったら置けませんでした出力
	if (sub_count == 8) {
		if (turn == BLACK) {
			turn = WHITE;//ホワイトは後攻
		}
		else {
			turn = BLACK;//ブラックは先攻
		}
	}
	*/




	
}




//ゲームが続行できるかの判定 1は続行 0は終了
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER, int NONE_NUMBER) {
	//どちらかの色が一色になってないか
	if (BLACK_NUMBER == 0) {
		printf("すべての駒が白になりました。\n");
		FLAG = 0;
	}
	else if (WHITE_NUMBER == 0) {
		printf("すべての駒が黒になりました。\n");
		FLAG = 0;
	}
	else if (NONE_NUMBER == 0) {//盤面が埋まっているか
		FLAG = 0;
	}
	
	return FLAG;
}