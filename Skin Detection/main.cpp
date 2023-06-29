#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; 	//range : [0, 255]
int G[MaxBMPSizeX][MaxBMPSizeY];	//range : [0, 255]
int B[MaxBMPSizeX][MaxBMPSizeY];	//range : [0, 255]
float H[MaxBMPSizeX][MaxBMPSizeY];	//range : [0, 1)
float S[MaxBMPSizeX][MaxBMPSizeY];	//range : [0, 1]
int V[MaxBMPSizeX][MaxBMPSizeY];	//range : [0, 255]

void RGB_to_HSV(int R[][MaxBMPSizeY], int G[][MaxBMPSizeY], int B[][MaxBMPSizeY], float H[][MaxBMPSizeY], float S[][MaxBMPSizeY], int V[][MaxBMPSizeY], int width, int height);

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	//膚色參數 
	const float H_threshold_low = 20 / 360.0;
	const float H_threshold_high = 40 / 360.0;
	const float S_threshold_low = 0.11;
	const float S_threshold_high =  0.45;
	const int V_threshold_low = int(0.75 * 255);
	
	//開啟影像 
	open_bmp("skin_det_1.bmp", R, G, B, width, height);
	
	//將影像從RGB轉成HSV 
	RGB_to_HSV(R, G, B, H, S, V, width, height);
	
	//二值化(條件:膚色) 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(H[i][j] >= H_threshold_low && H[i][j] <= H_threshold_high && S[i][j] >= S_threshold_low && S[i][j] <= S_threshold_high && V[i][j] >= V_threshold_low)
			{
				R[i][j] = G[i][j] = B[i][j] = 255;
			}
			else
			{
				R[i][j] = G[i][j] = B[i][j] = 0;
			}
		}
	}
	
	//儲存影像 
	save_bmp("skin_det_1_1.bmp", R, G, B, width, height);
	
	printf("Job Finished!\n");

	//關閉影像 
	close_bmp();

	return 0;
}
//R.G.B [0, 255]
//H:[0,1] S:[0,1] V:[0,255]
void RGB_to_HSV(int R[][MaxBMPSizeY], int G[][MaxBMPSizeY], int B[][MaxBMPSizeY], float H[][MaxBMPSizeY], float S[][MaxBMPSizeY], int V[][MaxBMPSizeY], int width, int height)
{
	int i, j;
	int max, min;
	float tmp;
	
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			max = R[i][j];
			if(max < G[i][j])	max = G[i][j];
			if(max < B[i][j])	max = B[i][j];
			
			min = R[i][j];
			if(min > G[i][j])	min = G[i][j];
			if(min > B[i][j])	min = B[i][j];
			
			V[i][j] = max;
			tmp = V[i][j] - min;
			S[i][j] = tmp / V[i][j];
			
			if(V[i][j] == R[i][j])		H[i][j] = (G[i][j] - B[i][j]) / tmp / 6;
			else if(V[i][j] == G[i][j])	H[i][j] = ((B[i][j] - R[i][j]) / tmp + 2) / 6;
			else if(V[i][j] == B[i][j]) H[i][j] = ((R[i][j] - G[i][j]) / tmp + 4) / 6;
			else
			{
				std::cerr << "error: RGB_to_HSV" << std::endl;
				exit(-1);
			}					
			
			if(H[i][j] < 0)	H[i][j] += 1;
		}
	}
}
