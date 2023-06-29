#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "bmp.hpp"

using namespace std;

// 巨集(Macro)
#define epsilon 1e-15

// 重定義資料型別(redefine the datatype)
typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

// 全域變數(global variable)
	// 檔名
char fileSrc[] = "switch.bmp";
char fileDst[] = "switch_new.bmp";
	// 影像點
Matrix ctrlSrc{{185,328},{484,272},{127,182},{453,111}};
Matrix ctrlDst{{0,412},{620,412},{0,0},{620,0}};
// Matrix ctrlSrc{{0,0},{1,0},{1,1},{0,1}};
// Matrix ctrlDst{{2,0},{1,1},{-2,2},{-4,0}};
	// 影像像素
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

// 副程式(subfunction)
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
	// 檢查來源矩陣的行列是否匹配
	if(srcA[0].size() != srcB.size()) {
		printf("Erros：矩陣大小不匹配,無法做矩陣乘法\n");
		exit(1);
	}
	// 矩陣乘法
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
	// 檢查矩陣是否為方陣
	int n = src.size();
	for(int i = 0; i < n; i++) {
		if(src[i].size() != n) {
			printf("Error：matrixDeterminant的輸入矩陣不是方陣\n");
			exit(1);
		}
	}
	// determinant
	Matrix reg;
	Matrix::iterator itMat;
	double det = 0;
	// 判斷是否能產生餘因子矩陣
	if(n == 1) {
		return src[0][0];
	} else {
		det = 0;
		for(int i = 0; i < n; i ++) {
			// 產生餘因子矩陣
			reg = src;
			itMat = reg.begin() + i;
			reg.erase(itMat);
			for(int k = 0; k < reg.size(); k++) {
				reg[k].erase(reg[k].begin());
			}
			// 計算餘因子矩陣的行列式
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
	// 檢查矩陣是否為方陣
	int n = src.size();
	for(int i = 0; i < n; i++) {
		if(src[i].size() != n) {
			printf("Error：matrixAdjugate的輸入矩陣不是方陣\n");
			exit(1);
		}
	}
	// 計算伴隨矩陣
	Matrix adj(src.size(), Vector(src.size(), 0));
	Matrix reg;
	Matrix::iterator itMat;
	Vector::iterator itVec;
	for(int i = 0; i < src.size(); i++) {
		for(int j = 0; j < src[0].size(); j++) {
			// 複製原矩陣
			reg = src;
			// 產生餘因子矩陣
			itMat = reg.begin() + i;
			reg.erase(itMat);
			for(int k = 0; k < reg.size(); k++) {
				itVec = reg[k].begin() + j;
				reg[k].erase(itVec);
			}
			// 計算伴隨矩陣的元素
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
	// 計算行列式(det(A))
	double det = matrixDeterminant(src);
	if(fabs(det) < epsilon) {
		printf("Error：matrixInverse出現不可逆的矩陣\n");
		exit(1);
	}
	// 計算伴隨矩陣(adj(A))
	Matrix adj = matrixAdjugate(src);
	// 反矩陣(inv(A) = adj(A)/det(A))
	Matrix dst = adj;
	for(int i = 0; i < adj.size(); i++) {
		for(int j = 0; j < adj[i].size(); j++) {
			dst[i][j] /= det;
		}
	}
	return dst;
}
Matrix getPerspectiveMatrix(Matrix src, Matrix dst) {
	// 檢查影像控制點的數量和規格
	if(src.size() != dst.size()) {
		printf("Error： 控制點數量不匹配\n");
		exit(1);
	} else {
		for(int i = 0; i < src.size(); i++) {
			if(src[i].size() != 2) {
				printf("Error：src[i]的控制點不是2維\n");
				exit(1);
			}
			if(dst[i].size() != 2) {
				printf("Error：dst[i]的控制點不是2維\n");
				exit(1);
			}
		}
	}
	
	// 建置矩陣(perspective transformation)
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
	
	// 顯示矩陣
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

// 主程式(main function)
int main(void) {
	int width, height;
	
	// 讀取影像
	open_bmp(fileSrc, R, G, B, width, height);
	
	// 計算 Perspective Transfrom的矩陣
	Matrix PMatrix = getPerspectiveMatrix(ctrlSrc, ctrlDst);
	// Perspective Transfrom
	PerspectiveTransfrom(r, R, width, height, PMatrix);
	PerspectiveTransfrom(g, G, width, height, PMatrix);
	PerspectiveTransfrom(b, B, width, height, PMatrix);
	
    // 儲存影像
	save_bmp(fileDst, r, g, b); // for gray images
	
	// 提示語
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();
	return 0;
}
