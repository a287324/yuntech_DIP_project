// character encoding: Big5
// 公設
// 原點:左下角
// x軸:水平軸
// y軸:垂直軸
// 座標, ex: R[x][y]
// 附註：這個程式流程上有BUG，他無法偵測同心圓，因為我程序上是設定成只找圓，但沒考慮到同心圓的情況，因此流程需要更改，142行到211行需要更改成偵測圓心(a,b)的b以外，也需要偵測圓半徑

#include <iostream>
#include <cmath>
#include <new>
#include "bmp.h" 
/******************************
 變數結構
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
 全域變數
*******************************/
// 影像像素
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];
// 檔名
char fileSrc[] = "coins.bmp";
char fileEdge[] = "coins_edge.bmp";
char fileDst[] = "coins_new.bmp";
// 邊緣偵測
int edgeMaskX[3][3] = { {-1, -2, -1},
						{ 0,  0,  0},
						{ 1,  2,  1}};
int edgeMaskY[3][3] = { {-1, 0, 1},
						{-2, 0, 2},
						{-1, 0, 1} };
int edgeThreshold = 360;
// Hough transform
int circleThreshold = 40;	// 被視為圓的threshold
int circleOffset = 5;		// 圓的邊緣點的誤差
/******************************
 subfunction
*******************************/
void LackMemory() {
	std::cerr << "記憶體不足" << std::endl;
	exit(1);
}
/******************************
 main function
*******************************/
int main(int argc, char *argv[]) {
	int width, height;
	
	//偵測動態分配有沒有出問題 
	set_new_handler(LackMemory);
	
	// 讀取影像
	open_bmp(fileSrc, R, G, B, width, height);
	
	// edge detector(邊緣不處理)
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
				edgePoint++;	// 計算邊緣點的總數，用於後面的Hough Table
			} else {
				edgeImg[x][y] = 0;
			}
		}
	}
	// 儲存邊緣影像
	for(int x = 0, i = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			r[x][y] = edgeImg[x][y];
		}
	}
	save_bmp(fileEdge, r, r, r, width, height); // for gray images
	
	// 建立Hough Table
		// 初始化
	HoughTable **houghT; // 水平:隨機假設圓心a, 垂直:邊緣點, 表格內容: 邊緣點的座標(x,y)/梯度(theta)/圓心b
	houghT = new HoughTable* [edgePoint];
	houghT[0] = new HoughTable [edgePoint * width];
	for(int i = 1; i < edgePoint; i++) {
		houghT[i] = houghT[i - 1] + width;
	}
		// 填表
	for(int x = 0, i = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			if(edgeImg[x][y] == 255) {	// 尋找邊緣點
				// 紀錄此邊緣點的數據
				for(int a = 0; a < width; a++) {
					houghT[i][a].b = y-(x-a)*tan(dirImg[x][y]);
					houghT[i][a].r = sqrt(pow(x-a, 2) + pow(y-houghT[i][a].b, 2));
				}
				// 邊緣點的編號+1
				i++;
			}
		}
	}
	
	// 尋找Hough Equation
		// 根據圓心(a,b)的b對Hough Table排序
	for(int a = 0; a < width; a++) {
		// Bubble Sort
		bool flagExchanged = true;
		for (int i = 0; flagExchanged && (i < edgePoint-1); i++){ // 排序趟數，flagExchanged檢查排序情況,如果 flagExchanged是false，則代表該次排序無交換,即排序完成,可提早結束
			flagExchanged = false;
			for (int j = 0; j < (edgePoint-1-i); j++) { // 每趟比較的次數
				if (houghT[j][a].b > houghT[j+1][a].b) {
					HoughTable houghReg;
					memcpy((void*)&houghReg, (void*)&houghT[j][a], sizeof(HoughTable));
					memcpy((void*)&houghT[j][a], (void*)&houghT[j+1][a], sizeof(HoughTable));
					memcpy((void*)&houghT[j+1][a], (void*)&houghReg, sizeof(HoughTable));
					flagExchanged = true; // 如果數值互換過, exchanged才會從false變成true,否則數列已經排序完成,exchanged值仍然為false,沒必要排序 */
				}
			}
		}
	}
		// 紀錄出現次數超過條件的圓心
	CircleTable *circleTablePointer = new CircleTable;
	for(int a = 0; a < width; a++) {
		int flagb = houghT[0][a].b;
		int countb = 1;
		for(int i = 1; i < edgePoint; i++) {
			// 計算(a,flagb)相同的個數
			if(houghT[i][a].b == flagb) {
				countb++;
			} else {
				if(countb >= circleThreshold) {
					circleTablePointer->next = new CircleTable;
					circleTablePointer->next->pre = circleTablePointer;
					circleTablePointer = circleTablePointer->next;
					circleTablePointer->a = a;
					circleTablePointer->b = flagb;
					// 圓的半徑不能隨便取一個,因為會有躁點
						// 取出有一樣圓心的圓的半徑
					double rT[countb];
					for(int m = 0; m < countb; m++) {
						rT[m] = houghT[i-m-1][a].r;
					}
						// Bubble Sort排序
					bool flagExchanged = true;
					for(int m = 0; flagExchanged && m<countb-1; m++){ // 排序趟數
						flagExchanged = false;
						for (int n=0; n<countb-1-m; n++) { // 每趟比較的次數
							if (rT[n] > rT[n+1]) {
								double temp = rT[n];
								rT[n] = rT[n+1];
								rT[n+1] = temp;
								flagExchanged = true;
							}
						}
					}
					circleTablePointer->r = rT[int(countb/2)];	// 將中位數當成半徑(因為躁點的半徑是極端值,所以才用中位數當代表)
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
			// 圓的半徑不能隨便取一個,因為會有躁點
				// 取出有一樣圓心的圓的半徑
			double rT[countb];
			for(int m = 0; m < countb; m++) {
				rT[m] = houghT[i-m-1][a].r;
			}
				// Bubble Sort排序
			bool flagExchanged = true;
			for(int m = 0; flagExchanged && m<countb-1; m++){ // 排序趟數
				flagExchanged = false;
				for(int n=0; n<countb-1-m; n++) { /* 內迴圈為每趟比較的次數，第i趟比較len-i次  */
					if (rT[n] > rT[n+1]) { /* 相鄰元素比較，若逆序則互換（升序為左大於右，逆序反之） */
						double temp = rT[n];
						rT[n] = rT[n+1];
						rT[n+1] = temp;
						flagExchanged = true; /*只有數值互換過, exchanged才會從false變成true,否則數列已經排序完成,exchanged值仍然為false,沒必要排序 */
					}
				}
			}
			circleTablePointer->r = rT[int(countb/2)];
		}
	}
	// 繪製符合圓方程式的邊緣點
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
	
	// 儲存影像
	save_bmp(fileDst, r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// 關閉檔案
	delete [] houghT[0];
	delete [] houghT;
	close_bmp();

	return 0;
}