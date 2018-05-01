#include<stdio.h>
#include<windows.h>

//�}�X�̏�Ԓ�`
#define NONE 0
#define BLACK 1
#define WHITE 2

/*
//�{�[�h�i�����[0][0])
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

//�{�[�h�i�����[0][0])
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


//�}�X�̏�Ԃ̐�
static int NONE_NUMBER;  //�󂫃}�X
static int BLACK_NUMBER = 2; //����
static int WHITE_NUMBER = 2; //����
int FLAG = 1;//�p������ 1:���s 0:�I��
int Date[7];//���Ԃ��̔���ޗ� ������������͖���
static int sub_count = 0; //�֐��̌Ăяo����
static int y, x;//���W

int turn = WHITE;			//1�����A2����� 
int Player1, Player2;//�v���C���[

void Disp(void); //��ʂ̍X�V�E��̍ĕ`��
void CanPut(void); //�u���邩�̏����@�p�X����@������͎c���Ă��邯�ǒu���Ȃ�
void TurnOver(int Date[7],int place,int j); //���Ԃ����� turn����
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER,int NONE_NUMBER); //�Q�[�������s�ł��邩�̔��� 1�͑��s 0�͏I��
/*---------------------------------MAIN----------------------------------------------*/
int main(void) {
	//�p������
	while(FLAG){
		Disp();
		CanPut();
	}
	//��������
	if (BLACK_NUMBER > WHITE_NUMBER) {

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
	NONE_NUMBER = 0;  //�󂫃}�X
	BLACK_NUMBER = 0; //����
	WHITE_NUMBER = 0; //����
	//��ʃN���A
	//system("cls");
	//�s��`��
	printf(" ");
	for (int i = 1; i <= 8; i++) {
		printf("%2d", i);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		//���`��
		printf("%d", ((i + 1) * 10));
		for (int j = 0; j < 8; j++) {
			switch (board[i][j]) {
			case NONE: printf("��"); NONE_NUMBER++; break; //�����������'��'			
			case BLACK: printf("��"); BLACK_NUMBER++; break;//1(��)�������Ă�����'��'	
			case WHITE: printf("��"); WHITE_NUMBER++; break;//2(��)�������Ă�����'��'	
			default: break;
			}
		}
		printf("\n");
	}
	printf("������̐�:%2d\n", BLACK_NUMBER);
	printf("������̐�:%2d\n", WHITE_NUMBER);
	//�p�����f
	FLAG = FinishCheck(BLACK_NUMBER, WHITE_NUMBER, NONE_NUMBER);
}


//�u���邩�̏���
void CanPut() {
	char pl[3];//�O���̕������i�[
	int place;//char�^���L���X�g���邽�߂̐���
	//�p���\�̂Ƃ������\������
	if (FLAG) {
		printf("---------------------------\n");
		printf("�ꏊ��I�����ĉ�����\n");
		printf("�� 40 + 2 -> 42\n");
		printf("---------------------------\n");
		printf("�ꏊ:");
	}
	else {
		return;
	}
	while (1) {
		//�u���ꏊ�̑I��
		scanf_s("%s", pl, 3);
		//�����ȊO�������Ă��邩�`�F�b�N
		if (isdigit(*pl)) {
			//������𐮐��^�ɃL���X�g
			place = atoi(pl);
			//�u����͈̓`�F�b�N
			if (place >= 11 && place <= 88 && (place % 10) != 9 && (place % 10) != 0) {
				break;
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
	
	//�n���ꂽ�����𕪊�
	y = place / 10;
	x = place % 10;
	y--;
	x--;
	
	//����łɂ���ꍇ�͒u���Ȃ�
	if (board[y][x] == 0) {
		int j;		//�����邩�̒l
		//1:��ok
		j = y - 1;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[--y][x];
		}
		TurnOver(Date,place,j);
		//2:�E��
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
		//3:�Eok
		j = 6;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][++x];
		}
		TurnOver(Date, place, j);
		//4:�E��
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
		//5:��ok
		j = 7 - y;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[++y][x];
		}
		TurnOver(Date, place, j);
		//6:����
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
		//7:��ok
		j = x - 1;
		for (int i = 0; i <= j; i++) {
			Date[i] = board[y][--x];
		}
		TurnOver(Date, place, j);
		//8:����
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
		printf("�u���܂���\n");
	}
}



//���Ԃ�����
void TurnOver(int Date[7],int place, int j){
	int TurnFlag = 0; //�Ђ�����Ԃ����̔��� 1:���s 0:�I��
	int  count = 0;
	sub_count++;

	//�n���ꂽ�����𕪊�
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




		//int judge;//���Ԃ��邩�̔���
	if (Date[0] <= 0 || Date[0] == turn) {
		printf("�Ԃ��Ȃ�\n\n");
	}
	else {
		TurnFlag = 1;
		printf("�Ԃ���maybe\n");
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


	//���Ԃ��̏���
	switch (sub_count) {
	case 1:
		//��
		for (int i = 1; i <= count; i++) {
			board[y - i][x] = turn;
		}
		break;
		//�E��
	case 2:
		for (int i = 1; i <= count; i++) {
			board[y - i][x + i] = turn;
		}
		break;
		//�E
	case 3:
		for (int i = 1; i <= count; i++) {
			board[y][x + i] = turn;
		}
		break;
		//�E��
	case 4:
		for (int i = 1; i <= count; i++) {
			board[y + i][x + i] = turn;
		}
		break;
		//��
	case 5:
		for (int i = 1; i <= count; i++) {
			board[y + i][x] = turn;
		}
		break;
		//����
	case 6:
		for (int i = 1; i <= count; i++) {
			board[y + i][x - i] = turn;
		}
		break;
		//��
	case 7:
		for (int i = 1; i <= count; i++) {
			board[y][x - i] = turn;
		}
		break;
		//����
	case 8:
		for (int i = 1; i <= count; i++) {
			board[y - i][x - i] = turn;
		}
		sub_count = 0;
		if (TurnFlag) {
			printf("���Ԃ���܂���\n");
		}
		break;
	default:
		break;
	}

	

	/*
	//�^�[���ύX���� 8��Ƃ��u���Ȃ�������u���܂���ł����o��
	if (sub_count == 8) {
		if (turn == BLACK) {
			turn = WHITE;//�z���C�g�͌�U
		}
		else {
			turn = BLACK;//�u���b�N�͐�U
		}
	}
	*/




	
}




//�Q�[�������s�ł��邩�̔��� 1�͑��s 0�͏I��
int FinishCheck(int BLACK_NUMBER,int WHITE_NUMBER, int NONE_NUMBER) {
	//�ǂ��炩�̐F����F�ɂȂ��ĂȂ���
	if (BLACK_NUMBER == 0) {
		printf("���ׂĂ̋���ɂȂ�܂����B\n");
		FLAG = 0;
	}
	else if (WHITE_NUMBER == 0) {
		printf("���ׂĂ̋���ɂȂ�܂����B\n");
		FLAG = 0;
	}
	else if (NONE_NUMBER == 0) {//�Ֆʂ����܂��Ă��邩
		FLAG = 0;
	}
	
	return FLAG;
}