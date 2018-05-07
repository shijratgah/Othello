#include<stdio.h>
#include<windows.h>

//�}�X�̏�Ԓ�`
#define NONE 0
#define BLACK 1
#define WHITE 2

//�{�[�h�i�����[0][0])
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

//�}�X�̏�Ԃ̐�
static int NONE_NUMBER;			//�󂫃}�X
static int BLACK_NUMBER = 2;	//����
static int WHITE_NUMBER = 2;	//����
int FLAG = 1;					//�p������ 1:���s 0:�I��
int Date[7];					//���Ԃ��̔���ޗ� ������������͖���
bool Turn_FLAG = false;			//�ǂ���1��ł����Ԃ��邩�̔��� 1:�� 0:�s
bool PassFlag = false;			//�p�X���ł��邩�̔��� 1:�� 0:�s
static int y, x;				//���W

int turn = BLACK;				//1��BLACK�A2��WHITE 
int Player1, Player2;			//�v���C���[

void Disp(void);														//��ʂ̍X�V�E��̍ĕ`��
void CanPut(void);														//��u���邩�̏���
int CanPut_sub(int place);												//CanPut�ɓ��͂����ʒu�ɋ�u���Ă邩��Ԃ� 1:�Ȃ� 0:����
void PassCheck(void);													//�p�X����@������͎c���Ă��邯�ǒu���Ȃ����̏���
void TurnOver(int Date[7],int place,int j,int sub_count);				//���Ԃ����� turn����
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER,int NONE_NUMBER);		//�Q�[�������s�ł��邩�̔��� 1�͑��s 0�͏I��
/*---------------------------------MAIN----------------------------------------------*/
int main(void) {
	while(FLAG){//�p������
		Disp();
		PassCheck();
		CanPut();
	}
	if (BLACK_NUMBER > WHITE_NUMBER) {//��������
		printf("���̏���!!\n");
	}
	else if (WHITE_NUMBER > BLACK_NUMBER) {
		printf("���̏���!!\n");
	}
	else if (BLACK_NUMBER == WHITE_NUMBER) {
		printf("��������");
	}
	return 0;
}

//��ʂ̍X�V�E��̍ĕ`��
void Disp() {
	NONE_NUMBER = 0;	//�󂫃}�X
	BLACK_NUMBER = 0;	//����
	WHITE_NUMBER = 0;	//����
	//system("cls");		//��ʃN���A
	for (int i = 0; i < 9; i++) {		//�����-1�����ď�����
		board[i][8] = -1;
		for (int j = 0; j < 9; j++) {
			board[8][i] = -1;
		}
	}
	printf(" ");
	for (int i = 1; i <= 8; i++) {		//�s��`��
		printf("%2d", i);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		
		printf("%d", ((i + 1) * 10));	//���`��
		for (int j = 0; j < 8; j++) {
			switch (board[i][j]) {
			case NONE: printf("��"); NONE_NUMBER++; break;			//�����������'��'			
			case BLACK: printf("��"); BLACK_NUMBER++; break;		//1(��)�������Ă�����'��'	
			case WHITE: printf("��"); WHITE_NUMBER++; break;		//2(��)�������Ă�����'��'	
			default: break;
			}
		}
		printf("\n");
	}
	printf("������̐�:%2d\n", BLACK_NUMBER);
	printf("������̐�:%2d\n", WHITE_NUMBER);
	FLAG = FinishCheck(BLACK_NUMBER, WHITE_NUMBER, NONE_NUMBER);	//�p�����f
}

//��u���邩�̏���
void CanPut() { 
	char pl[3];			//�O���̕������i�[
	int place;			//char�^���L���X�g���邽�߂̐���
	if (FLAG) {			//�p���\�̂Ƃ������\������
		printf("---------------------------\n");
		printf("�ꏊ��I�����ĉ�����\n");
		printf("�� 40 + 2 -> 42\n");
		printf("---------------------------\n");
		if (turn == BLACK) {
			printf("���̔Ԃł�\n");
		}
		else {
			printf("���̔Ԃł�\n");
		}
		printf("�ꏊ:");
	}
	else {
		return;
	}
	while (1) {					//���͏���
		scanf_s("%s", pl, 3);	//�u���ꏊ�̑I��
		if (isdigit(*pl)) {		//�����ȊO�������Ă��邩�`�F�b�N
			place = atoi(pl);	//������𐮐��^�ɃL���X�g
			if (place >= 11 && place <= 88 && (place % 10) != 9 && (place % 10) != 0) {	//�u����͈̓`�F�b�N
				if (CanPut_sub(place)) {	//���łɋ�u���Ă��邩
					break;
				}
				else {
					printf("\n���͂����ꏊ�͖����ł�\n");
					printf("�ꏊ:");
				}
			}
			else {
				printf("\n���͂��������͖����ł�\n");
				printf("�ꏊ:");
			}
		}
		else {
			printf("\n���͂��������͖����ł�\n");
			printf("�ꏊ:");
		}
	}
}

//CanPut�ɓ��͂����ʒu�ɋ�u���Ă邩��Ԃ� 1:�Ȃ� 0:����
int CanPut_sub(int place) {
	int sub_count = 1;	//TurnOver�֐��̌Ăяo����
	y = place / 10;		//�n���ꂽ�����𕪊�
	x = place % 10;
	y--;
	x--;
	if (board[y][x] == 0) {		//����łɂ���ꍇ�͒u���Ȃ�
		int j;				//�����邩�̒l
							//���͂��ꂽ�ʒu����̑S���ʂ̃f�[�^���֐�TurnOver�ɒl��n���Ă���				
		j = y - 1;								//1:��
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
		j = y - 1;								//2:�E��
		for (int i = 0; i <= j; i++) {
			if (board[--y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = 6;									//3:�E
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][++x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = 6;									//4:�E��
		for (int i = 0; i <= j; i++) {
			if (board[++y][++x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = 7 - y;								//5:��
		for (int i = 0; i <= j; i++) {
			Date[i] = board[++y][x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//6:����
		for (int i = 0; i <= j; i++) {
			if (board[++y][--x] != -1) {
				Date[i] = board[y][x];
			}
			else {
				break;
			}
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//7:��							
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][--x];
		}
		TurnOver(Date, place, j, sub_count++);
		j = x - 1;								//8:����
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

//�p�X����@������͎c���Ă��邯�ǒu���Ȃ����̏���
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

//���Ԃ�����
void TurnOver(int Date[7],int place, int j,int sub_count){
	int count_FLAG;				//����1��𗠕Ԃ��邩�̔���@1:�� 0:�s
	int  count;					//��𗠕Ԃ���
	count = count_FLAG = 0;		//������
	y = place / 10;				//�n���ꂽ�����𕪊�
	x = place % 10;
	y--;
	x--;
	
	//���񗠕Ԃ��邩�̔���
	if (Date[0] <= 0 || Date[0] == turn) {			//1��ڂ�0�Ԗڂ̗v�f��0,-1,�����̃^�[���̐F�łȂ��Ƃ��̏���
	}
	else {
		if (turn == BLACK) {						//���Ԃ̏���
			for (int i = 0; i <= j; i++) {
				if (Date[i] == WHITE) {				//1��ڂ�i�Ԗڂ̗v�f�����̊ԉ��
					count++;
				}
				else if (Date[i] == BLACK) {
					count_FLAG = 1;					//���������ŋ��߂����̔���
					break;
				}
				else if (Date[i] <= 0) {
					break;
				}
			}
		}
		else if (turn == WHITE) {					//���Ԃ̏���
			for (int i = 0; i <= j; i++) {
				if (Date[i] == BLACK) {				//1��ڂ�i�Ԗڂ̗v�f�����̊ԉ��
					count++;
				}
				else if (Date[i] == WHITE) {
					count_FLAG = 1;					//���������ŋ��߂����̔���
					break;
				}
				else if (Date[i] <= 0) {
					break;
				}
			}
		}
	}

	//���Ԃ��̏���
	switch (sub_count) {							//sub_count�̐����ɂ���ď�������@��j1,��@4,�E��
	case 1:											//��
		if (count_FLAG) {							
			for (int i = 1; i <= count; i++) {		//count�̉񐔕��������
				board[y - i][x] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 2:											//�E��
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y - i][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 3:											//�E
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 4:											//�E��
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x + i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 5:											//��
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 6:											//����
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y + i][x - i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 7:											//��
		if (count_FLAG) {
			for (int i = 1; i <= count; i++) {
				board[y][x - i] = turn;
			}
			Turn_FLAG = true;
		}
		break;
	case 8:											//����
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
	if (sub_count == 8) {					//�^�[���`�F���W
		sub_count = 0;
		if (Turn_FLAG != true) {			//�ǂ̕��ʂ����Ԃ�Ȃ�
			printf("�u���܂���\n");
		}
		else {
			board[y][x] = turn;				//���͂����ꏊ�ɋ��u��
			if (turn == BLACK) {
				turn = WHITE;				//�z���C�g�͌�U
			}
			else {
				turn = BLACK;				//�u���b�N�͐�U
			}
		}
		Turn_FLAG = false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//�Q�[�������s�ł��邩�̔��� 1�͑��s 0�͏I��
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER, int NONE_NUMBER) {
	if (BLACK_NUMBER == 0) {										//�ǂ��炩�̐F����F�ɂȂ��ĂȂ���
		printf("���ׂĂ̋���ɂȂ�܂����B\n");
		FLAG = 0;
	}
	else if (WHITE_NUMBER == 0) {
		printf("���ׂĂ̋���ɂȂ�܂����B\n");
		FLAG = 0;
	}
	else if (NONE_NUMBER == 0) {									//�Ֆʂ����܂��Ă��邩
		FLAG = 0;
	}
	return FLAG;
}