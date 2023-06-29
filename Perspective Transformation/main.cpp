#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "bmp.hpp"

using namespace std;

// ����(Macro)
#define epsilon 1e-15

// ���w�q��ƫ��O(redefine the datatype)
typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

// �����ܼ�(global variable)
	// �ɦW
char fileSrc[] = "switch.bmp";
char fileDst[] = "switch_new.bmp";
	// �v���I
Matrix ctrlSrc{{185,328},{484,272},{127,182},{453,111}};
Matrix ctrlDst{{0,412},{620,412},{0,0},{620,0}};
// Matrix ctrlSrc{{0,0},{1,0},{1,1},{0,1}};
// Matrix ctrlDst{{2,0},{1,1},{-2,2},{-4,0}};
	// �v������
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

// �Ƶ{��(subfunction)
Matrix matrixTranspose(Matrix src) {
	Matrix dst(src[0].size(), Vector(src.size(), 0));
	for(int i = 0; i < src.size(); i++) {
		for(int j = 0; j < src[i].size(); j++) {
			dst[j][i] = src[i][j];
		}
	}
	return dst;
}
Matrix matrixMultiplication(Matrix srcA, Matrix srcB) {
	// �ˬd�ӷ��x�}����C�O�_�ǰt
	if(srcA[0].size() != srcB.size()) {
		printf("Erros�G�x�}�j�p���ǰt,�L�k���x�}���k\n");
		exit(1);
	}
	// �x�}���k
	Matrix dst(srcA.size(), Vector(srcB[0].size(), 0));
	for(int i = 0; i < srcA.size(); i++) {
		for(int j = 0; j < srcB[i].size(); j++) {
			dst[i][j] = 0;
			for(int k = 0; k < srcB.size(); k++) {
				dst[i][j] += srcA[i][k]*srcB[k][j];
			}
		}
	}
	return dst;
}
double matrixDeterminant(Matrix src) {
	// �ˬd�x�}�O�_����}
	int n = src.size();
	for(int i = 0; i < n; i++) {
		if(src[i].size() != n) {
			printf("Error�GmatrixDeterminant����J�x�}���O��}\n");
			exit(1);
		}
	}
	// determinant
	Matrix reg;
	Matrix::iterator itMat;
	double det = 0;
	// �P�_�O�_�ಣ�;l�]�l�x�}
	if(n == 1) {
		return src[0][0];
	} else {
		det = 0;
		for(int i = 0; i < n; i ++) {
			// ���;l�]�l�x�}
			reg = src;
			itMat = reg.begin() + i;
			reg.erase(itMat);
			for(int k = 0; k < reg.size(); k++) {
				reg[k].erase(reg[k].begin());
			}
			// �p��l�]�l�x�}����C��
			if(i % 2 != 1) {
				det += src[i][0]*matrixDeterminant(reg);
			} else {
				det -= src[i][0]*matrixDeterminant(reg);
			}
		}
		return det;
	}
}
Matrix matrixAdjugate(Matrix src) {
	// �ˬd�x�}�O�_����}
	int n = src.size();
	for(int i = 0; i < n; i++) {
		if(src[i].size() != n) {
			printf("Error�GmatrixAdjugate����J�x�}���O��}\n");
			exit(1);
		}
	}
	// �p����H�x�}
	Matrix adj(src.size(), Vector(src.size(), 0));
	Matrix reg;
	Matrix::iterator itMat;
	Vector::iterator itVec;
	for(int i = 0; i < src.size(); i++) {
		for(int j = 0; j < src[0].size(); j++) {
			// �ƻs��x�}
			reg = src;
			// ���;l�]�l�x�}
			itMat = reg.begin() + i;
			reg.erase(itMat);
			for(int k = 0; k < reg.size(); k++) {
				itVec = reg[k].begin() + j;
				reg[k].erase(itVec);
			}
			// �p����H�x�}������
			if((i+j)%2 == 0) {
				adj[i][j] = matrixDeterminant(reg);
			} else {
				adj[i][j] = -matrixDeterminant(reg);
			}
		}
	}
	adj = matrixTranspose(adj);
	return adj;
}
Matrix matrixInverse(Matrix src) {
	// �p���C��(det(A))
	double det = matrixDeterminant(src);
	if(fabs(det) < epsilon) {
		printf("Error�GmatrixInverse�X�{���i�f���x�}\n");
		exit(1);
	}
	// �p����H�x�}(adj(A))
	Matrix adj = matrixAdjugate(src);
	// �ϯx�}(inv(A) = adj(A)/det(A))
	Matrix dst = adj;
	for(int i = 0; i < adj.size(); i++) {
		for(int j = 0; j < adj[i].size(); j++) {
			dst[i][j] /= det;
		}
	}
	return dst;
}
Matrix getPerspectiveMatrix(Matrix src, Matrix dst) {
	// �ˬd�v�������I���ƶq�M�W��
	if(src.size() != dst.size()) {
		printf("Error�G �����I�ƶq���ǰt\n");
		exit(1);
	} else {
		for(int i = 0; i < src.size(); i++) {
			if(src[i].size() != 2) {
				printf("Error�Gsrc[i]�������I���O2��\n");
				exit(1);
			}
			if(dst[i].size() != 2) {
				printf("Error�Gdst[i]�������I���O2��\n");
				exit(1);
			}
		}
	}
	
	// �ظm�x�}(perspective transformation)
	const int ctrlNum = src.size();
	Matrix AM(ctrlNum*2, Vector(8, 0));
	Matrix bM(ctrlNum*2, Vector(1, 0));
		// A
	for(int i = 0; i < ctrlNum; i++) {
		AM[i][0] = dst[i][0];
		AM[i][1] = dst[i][1];
		AM[i][2] = 1;
		AM[i][6] = -dst[i][0]*src[i][0];
		AM[i][7] = -dst[i][1]*src[i][0];
		
		AM[i+ctrlNum][3] = dst[i][0];
		AM[i+ctrlNum][4] = dst[i][1];
		AM[i+ctrlNum][5] = 1;
		AM[i+ctrlNum][6] = -dst[i][0]*src[i][1];
		AM[i+ctrlNum][7] = -dst[i][1]*src[i][1];
	}
		// b
	for(int i = 0; i < ctrlNum; i++) {
		bM[i][0] = src[i][0];
		bM[i+ctrlNum][0] = src[i][1];
	}
	
	// Least-squares estimate
	Matrix PMatrix;
	PMatrix = matrixMultiplication(
			matrixMultiplication(
				matrixInverse(
					matrixMultiplication(
						matrixTranspose(AM), 
						AM))
				, matrixTranspose(AM)), 
			bM);
	
	// ��ܯx�}
	// {
		// Matrix dst = PMatrix;
		// printf("PMatrix\n");
		// for(int i = 0; i < dst.size(); i++) {
			// for(int j = 0; j < dst[i].size(); j++) {
				// printf("% .4f\t", dst[i][j]);
			// }
			// printf("\n");
		// }
	// }
	return PMatrix;
}
void PerspectiveTransfrom(int dst[][MaxBMPSizeY], int src[][MaxBMPSizeY], int width, int height, Matrix PMatrix) {
	double x, y, k;
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			k = PMatrix[6][0]*i+PMatrix[7][0]*j+1;
			x = (PMatrix[0][0]*i+PMatrix[1][0]*j+PMatrix[2][0]) / k;
			y = (PMatrix[3][0]*i+PMatrix[4][0]*j+PMatrix[5][0]) / k;
			if((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
				// Linear interpolation
				int x1 = int(x);
				int x2 = x1 + 1;
				int y1 = int(y);
				int y2 = y1 + 1;
				dst[i][j] = src[x1][y1] * (x2-x) * (y2-y) + 
							src[x2][y1] * (x-x1) * (y2-y) + 
							src[x1][y2] * (x2-x) * (y-y1) + 
							src[x2][y2] * (x-x1) * (y-y1);
			} else {
				dst[i][j] = 0;
			}
		}
	}
}

// �D�{��(main function)
int main(void) {
	int width, height;
	
	// Ū���v��
	open_bmp(fileSrc, R, G, B, width, height);
	
	// �p�� Perspective Transfrom���x�}
	Matrix PMatrix = getPerspectiveMatrix(ctrlSrc, ctrlDst);
	// Perspective Transfrom
	PerspectiveTransfrom(r, R, width, height, PMatrix);
	PerspectiveTransfrom(g, G, width, height, PMatrix);
	PerspectiveTransfrom(b, B, width, height, PMatrix);
	
    // �x�s�v��
	save_bmp(fileDst, r, g, b); // for gray images
	
	// ���ܻy
	printf("Job Finished!\n");

	// ���� bmp �v������
	close_bmp();
	return 0;
}
