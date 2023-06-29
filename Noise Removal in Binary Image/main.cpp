#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "bmp.h"

#define Structuring_size 5
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
/* 
int B[Structuring_size][Structuring_size] = {{ 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255}};
int B[Structuring_size][Structuring_size] = {{   0,   0, 255, 255, 255,   0,   0},
											 {   0, 255, 255, 255, 255, 255,   0},
											 { 255, 255, 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255, 255, 255},
											 { 255, 255, 255, 255, 255, 255, 255},
											 {   0, 255, 255, 255, 255, 255,   0},
											 {   0,   0, 255, 255, 255,   0,   0}};
			*/								 
int B[Structuring_size][Structuring_size] = {{255, 255, 255},
											 {255, 255, 255},
											 {255, 255, 255}};
											 
void Dilation(int R[][MaxBMPSizeY], int width, int height, int B[][Structuring_size], int B_size);	//B必須是正方形,B_size必須是奇數 
void Erosion(int R[][MaxBMPSizeY], int width, int height, int B[][Structuring_size], int B_size);
				
int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	const int Erosion_times_f = 1;
	const int Dilation_times = 5;
	const int Erosion_times_b = 3;
	
	open_bmp("noise_remov_morph_gray.bmp", R, R, R, width, height);
    
    for(i = 0; i < Erosion_times_f; i++)
    	Erosion(R, width, height, B, Structuring_size);
    
    for(i = 0; i < Dilation_times; i++)
    	Dilation(R, width, height, B, Structuring_size);
    
    for(i = 0; i < Erosion_times_b; i++)
    	Erosion(R, width, height, B, Structuring_size);
    
	save_bmp("noise_remov_morph_gray_1.bmp", R, R, R, width, height);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
void Dilation(int R[][MaxBMPSizeY], int width, int height, int B[][Structuring_size], int B_size)
{
	int i, j, m, n;
	int **r;
	int B_center;
	B_center = (B_size >> 1);
	//創建一個比影像大一點的影像，並把影像複製再中間 
	r = new int* [width + (B_center << 1)];
	r[0] = new int [(width + (B_center << 1)) * (height + (B_center << 1))];
	for(i = 1; i < (width + (B_center << 1)); i++)
		r[i] = r[i-1] + height + (B_center << 1);
	
	for(i = B_center; i < (width + B_center); i++)
	{
		for(j = B_center; j < (height + B_center); j++)
		{
			r[i][j] = R[i - B_center][j - B_center];
		}
	}
	//膨脹 
	for(i = B_center; i < (width + B_center); i++)
	{
		for(j = B_center; j < (height + B_center); j++)
		{
			if(r[i][j] == 255)
			{
				for(m = 0; m < B_size; m++)
				{
					for(n = 0; n < B_size; n++)
					{
						if(B[m][n] == 255 && r[i - B_center + m][j - B_center + n] == 0)
						{
							if((i - B_center + m) >= B_center && (j - B_center + n) >= B_center)
							{
								R[i - 2 * B_center + m][j - 2 * B_center + n] = 255;
							}
						}
					}
				}
			}
		}
	}
	
	delete [] r[0];
	delete [] r;
} 

void Erosion(int R[][MaxBMPSizeY], int width, int height, int B[][Structuring_size], int B_size)
{
	int i, j, m, n;
	int flag;
	int B_center = B_size >> 1;
	int **r;
	//創建一個比影像大一點的影像，並把影像複製再中間 
	r = new int* [width];
	r[0] = new int [width * height];
	for(i = 1; i < width; i++)
		r[i] = r[i - 1] + height;

	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			r[i][j] = R[i][j];
		}
	}
	//侵蝕 
	for(i = B_center; i < (width - B_center); i++)
	{
		for(j = B_center; j < (height - B_center); j++)
		{
			flag = 1;
			for(m = 0; m < B_size; m++)
			{
				for(n = 0; n < B_size; n++)
				{
					if(B[m][n] == 255)
					{
						if(r[i - B_center + m][j - B_center + n] == 0)
						{
							flag = 0;
						}
					}
				}
			}
			R[i][j] = (flag == 1) ? (255) : (0);
		}
	}

	delete [] r[0];
	delete [] r;
}
