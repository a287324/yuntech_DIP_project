// character encoding: Big5
// ���]
// ���I:���U��
// x�b:�����b
// y�b:�����b
// �y��, ex: R[x][y]
// �����G�o�ӵ{���y�{�W��BUG�A�L�L�k�����P�߶�A�]���ڵ{�ǤW�O�]�w���u���A���S�Ҽ{��P�߶ꪺ���p�A�]���y�{�ݭn���A142���211��ݭn��令�������(a,b)��b�H�~�A�]�ݭn������b�|

#include <iostream>
#include <cmath>
#include <new>
#include "bmp.h" 
/******************************
 �ܼƵ��c
*******************************/
typedef struct HoughTable {
	int b;
	double r;
} HoughTable;
typedef struct CircleTable {
	int a = 0;
	int b = 0;
	double r = 0;
	CircleTable *pre = NULL;
	CircleTable *next = NULL;
} CircleTable;
/******************************
 �����ܼ�
*******************************/
// �v������
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];
// �ɦW
char fileSrc[] = "coins.bmp";
char fileEdge[] = "coins_edge.bmp";
char fileDst[] = "coins_new.bmp";
// ��t����
int edgeMaskX[3][3] = { {-1, -2, -1},
						{ 0,  0,  0},
						{ 1,  2,  1}};
int edgeMaskY[3][3] = { {-1, 0, 1},
						{-2, 0, 2},
						{-1, 0, 1} };
int edgeThreshold = 360;
// Hough transform
int circleThreshold = 40;	// �Q�����ꪺthreshold
int circleOffset = 5;		// �ꪺ��t�I���~�t
/******************************
 subfunction
*******************************/
void LackMemory() {
	std::cerr << "�O���餣��" << std::endl;
	exit(1);
}
/******************************
 main function
*******************************/
int main(int argc, char *argv[]) {
	int width, height;
	
	//�����ʺA���t���S���X���D 
	set_new_handler(LackMemory);
	
	// Ū���v��
	open_bmp(fileSrc, R, G, B, width, height);
	
	// edge detector(��t���B�z)
	double dirImg[width][height];
	int edgeImg[width][height];
	int dx,dy;
	int edgePoint = 0;
	for(int x = 1; x < width-1; x++) {
		for(int y = 1; y < height-1; y++) {
			dx = edgeMaskX[0][0] * R[x-1][y-1] + edgeMaskX[0][1] * R[x-1][y] + edgeMaskX[0][2] * R[x-1][y+1] +
				 edgeMaskX[1][0] * R[ x ][y-1] + edgeMaskX[1][1] * R[ x ][y] + edgeMaskX[1][2] * R[ x ][y+1] +
				 edgeMaskX[2][0] * R[x+1][y-1] + edgeMaskX[2][1] * R[x+1][y] + edgeMaskX[2][2] * R[x+1][y+1];
			dy = edgeMaskY[0][0] * R[x-1][y-1] + edgeMaskY[0][1] * R[x-1][y] + edgeMaskY[0][2] * R[x-1][y+1] +
				 edgeMaskY[1][0] * R[ x ][y-1] + edgeMaskY[1][1] * R[ x ][y] + edgeMaskY[1][2] * R[ x ][y+1] +
				 edgeMaskY[2][0] * R[x+1][y-1] + edgeMaskY[2][1] * R[x+1][y] + edgeMaskY[2][2] * R[x+1][y+1];
			dirImg[x][y] = atan(double(dy)/dx);
			if((abs(dx) + abs(dy)) > edgeThreshold) {
				edgeImg[x][y] = 255;
				edgePoint++;	// �p����t�I���`�ơA�Ω�᭱��Hough Table
			} else {
				edgeImg[x][y] = 0;
			}
		}
	}
	// �x�s��t�v��
	for(int x = 0, i = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			r[x][y] = edgeImg[x][y];
		}
	}
	save_bmp(fileEdge, r, r, r, width, height); // for gray images
	
	// �إ�Hough Table
		// ��l��
	HoughTable **houghT; // ����:�H�����]���a, ����:��t�I, ��椺�e: ��t�I���y��(x,y)/���(theta)/���b
	houghT = new HoughTable* [edgePoint];
	houghT[0] = new HoughTable [edgePoint * width];
	for(int i = 1; i < edgePoint; i++) {
		houghT[i] = houghT[i - 1] + width;
	}
		// ���
	for(int x = 0, i = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			if(edgeImg[x][y] == 255) {	// �M����t�I
				// ��������t�I���ƾ�
				for(int a = 0; a < width; a++) {
					houghT[i][a].b = y-(x-a)*tan(dirImg[x][y]);
					houghT[i][a].r = sqrt(pow(x-a, 2) + pow(y-houghT[i][a].b, 2));
				}
				// ��t�I���s��+1
				i++;
			}
		}
	}
	
	// �M��Hough Equation
		// �ھڶ��(a,b)��b��Hough Table�Ƨ�
	for(int a = 0; a < width; a++) {
		// Bubble Sort
		bool flagExchanged = true;
		for (int i = 0; flagExchanged && (i < edgePoint-1); i++){ // �Ƨǽ�ơAflagExchanged�ˬd�ƧǱ��p,�p�G flagExchanged�Ofalse�A�h�N��Ӧ��ƧǵL�洫,�Y�Ƨǧ���,�i��������
			flagExchanged = false;
			for (int j = 0; j < (edgePoint-1-i); j++) { // �C����������
				if (houghT[j][a].b > houghT[j+1][a].b) {
					HoughTable houghReg;
					memcpy((void*)&houghReg, (void*)&houghT[j][a], sizeof(HoughTable));
					memcpy((void*)&houghT[j][a], (void*)&houghT[j+1][a], sizeof(HoughTable));
					memcpy((void*)&houghT[j+1][a], (void*)&houghReg, sizeof(HoughTable));
					flagExchanged = true; // �p�G�ƭȤ����L, exchanged�~�|�qfalse�ܦ�true,�_�h�ƦC�w�g�Ƨǧ���,exchanged�Ȥ��M��false,�S���n�Ƨ� */
				}
			}
		}
	}
		// �����X�{���ƶW�L���󪺶��
	CircleTable *circleTablePointer = new CircleTable;
	for(int a = 0; a < width; a++) {
		int flagb = houghT[0][a].b;
		int countb = 1;
		for(int i = 1; i < edgePoint; i++) {
			// �p��(a,flagb)�ۦP���Ӽ�
			if(houghT[i][a].b == flagb) {
				countb++;
			} else {
				if(countb >= circleThreshold) {
					circleTablePointer->next = new CircleTable;
					circleTablePointer->next->pre = circleTablePointer;
					circleTablePointer = circleTablePointer->next;
					circleTablePointer->a = a;
					circleTablePointer->b = flagb;
					// �ꪺ�b�|�����H�K���@��,�]���|��ļ�I
						// ���X���@�˶�ߪ��ꪺ�b�|
					double rT[countb];
					for(int m = 0; m < countb; m++) {
						rT[m] = houghT[i-m-1][a].r;
					}
						// Bubble Sort�Ƨ�
					bool flagExchanged = true;
					for(int m = 0; flagExchanged && m<countb-1; m++){ // �Ƨǽ��
						flagExchanged = false;
						for (int n=0; n<countb-1-m; n++) { // �C����������
							if (rT[n] > rT[n+1]) {
								double temp = rT[n];
								rT[n] = rT[n+1];
								rT[n+1] = temp;
								flagExchanged = true;
							}
						}
					}
					circleTablePointer->r = rT[int(countb/2)];	// �N����Ʒ��b�|(�]��ļ�I���b�|�O���ݭ�,�ҥH�~�Τ���Ʒ�N��)
				}
				flagb = houghT[i][a].b;
				countb = 1;
			}
		}
		if(countb >= circleThreshold) {
			int i = edgePoint;
			circleTablePointer->next = new CircleTable;
			circleTablePointer->next->pre = circleTablePointer;
			circleTablePointer = circleTablePointer->next;
			circleTablePointer->a = a;
			circleTablePointer->b = flagb;
			// �ꪺ�b�|�����H�K���@��,�]���|��ļ�I
				// ���X���@�˶�ߪ��ꪺ�b�|
			double rT[countb];
			for(int m = 0; m < countb; m++) {
				rT[m] = houghT[i-m-1][a].r;
			}
				// Bubble Sort�Ƨ�
			bool flagExchanged = true;
			for(int m = 0; flagExchanged && m<countb-1; m++){ // �Ƨǽ��
				flagExchanged = false;
				for(int n=0; n<countb-1-m; n++) { /* ���j�鬰�C���������ơA��i����len-i��  */
					if (rT[n] > rT[n+1]) { /* �۾F��������A�Y�f�ǫh�����]�ɧǬ����j��k�A�f�ǤϤ��^ */
						double temp = rT[n];
						rT[n] = rT[n+1];
						rT[n+1] = temp;
						flagExchanged = true; /*�u���ƭȤ����L, exchanged�~�|�qfalse�ܦ�true,�_�h�ƦC�w�g�Ƨǧ���,exchanged�Ȥ��M��false,�S���n�Ƨ� */
					}
				}
			}
			circleTablePointer->r = rT[int(countb/2)];
		}
	}
	// ø�s�ŦX���{������t�I
	memcpy((void*)&r[0][0], (void*)&R[0][0], sizeof(r[0][0])*MaxBMPSizeX*MaxBMPSizeY);
	memcpy((void*)&g[0][0], (void*)&R[0][0], sizeof(g[0][0])*MaxBMPSizeX*MaxBMPSizeY);
	memcpy((void*)&b[0][0], (void*)&R[0][0], sizeof(b[0][0])*MaxBMPSizeX*MaxBMPSizeY);
	while(circleTablePointer->pre != NULL) {
		for(int x = 0; x < width; x++) {
			for(int y = 0; y < height; y++) {
				if(edgeImg[x][y] == 255) {
					if(fabs(circleTablePointer->r - sqrt(pow(x-circleTablePointer->a,2) + pow(y-circleTablePointer->b,2))) < circleOffset) {
						r[x][y] = 0;
						g[x][y] = 255;
						b[x][y] = 0;
					}
				}
			}
		}
		circleTablePointer = circleTablePointer->pre;
	}
	
	// �x�s�v��
	save_bmp(fileDst, r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// �����ɮ�
	delete [] houghT[0];
	delete [] houghT;
	close_bmp();

	return 0;
}