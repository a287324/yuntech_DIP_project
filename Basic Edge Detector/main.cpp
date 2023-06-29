#include <iostream>
#include <cmath>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int Roberts_h1[2][2] = { { 0, 1},
						 {-1, 0}};
int Roberts_h2[2][2] = { { 1,  0},
						 { 0, -1}};

int Prewitt_h1[3][3] = { {-1, 0, 1},
						 {-1, 0, 1},
						 {-1, 0, 1}};
int Prewitt_h2[3][3] = { {-1, -1, -1},
						 { 0,  0,  0},
						 { 1,  1,  1}};

int Sobel_h1[3][3] = {  {-1, 0, 1},
						{-2, 0, 2},
						{-1, 0, 1} };
int Sobel_h2[3][3] = {  {-1, -2, -1},
						{ 0,  0,  0},
						{ 1,  2,  1}};

int Isotropic_h1[3][3] = {{-256, 0, 256},
						  {-362, 0, 362},
						  {-256, 0, 256}};
int Isotropic_h2[3][3] = {{-256, -362, -256},
						  {   0,    0,    0},
						  { 256,  362,  256}};
							
void Edge_detection(int [][MaxBMPSizeY], int, int, int [][MaxBMPSizeY], int, int);
enum Mask_operator {Roberts = 0, Prewitt, Sobel, Isotropic};
int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	const int threshold = 130;
	
	//開啟檔案 
	open_bmp("1.bmp", R, R, R, width, height);
	
	//使用Roberts operator的edge detection
	Edge_detection( r, Roberts, threshold, R, width, height);
	save_bmp("2_new_Roberts.bmp", r, r, r, width, height);
	
	//使用Prewiit operator的edge detection
	Edge_detection( r, Prewitt, threshold, R, width, height);
	save_bmp("2_new_Prewitt.bmp", r, r, r, width, height);
	
	//使用Sobel operator的edge detection
	Edge_detection( r, Sobel, threshold, R, width, height);
	save_bmp("2_new_Sobel.bmp", r, r, r, width, height); 
	
	//使用Isotropic operator的edge detection
	Edge_detection( r, Isotropic, threshold, R, width, height);
	save_bmp("2_new_Isotropic.bmp", r, r, r, width, height);
	
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
void Edge_detection(int result[][MaxBMPSizeY], int Mask_option, int threshold, int origin[][MaxBMPSizeY], int width, int height)
{
	int (*Mask_h1)[3], (*Mask_h2)[3];
	int offset;
	int i, j, h1_reg, h2_reg;
	
	//遮罩選擇 
	switch (Mask_option)
	{
		case Roberts:
			break;
		case Prewitt:
			Mask_h1 = Prewitt_h1;
			Mask_h2 = Prewitt_h2;
			offset = 0;
			break;
		case Sobel:
			Mask_h1 = Sobel_h1;
			Mask_h2 = Sobel_h2;
			offset = 0;
			break;
		case Isotropic:
			Mask_h1 = Isotropic_h1;
			Mask_h2 = Isotropic_h2;
			offset = 8;
			break;
		default:
			cerr << "ERROR: 找不到匹配的Mask" << endl;
			exit(1);
	}
	//Gradient operator的卷積 
	if(Mask_option != Roberts)
	{
		for(i = 1; i < (width - 1); i++)	//image的中心區塊 
		{
			for(j = 1; j < (height - 1); j++)
			{
				h1_reg = Mask_h1[0][0] * origin[i - 1][j - 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j + 1] +
						 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
						 Mask_h1[2][0] * origin[i + 1][j - 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j + 1];
				h2_reg = Mask_h2[0][0] * origin[i - 1][j - 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j + 1] +
						 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
						 Mask_h2[2][0] * origin[i + 1][j - 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j + 1];
				result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
			}
		}
		for(j = 1; j < (height - 1); j++)	//image的左右邊緣 
		{
			i = 0;
			h1_reg = Mask_h1[0][0] * origin[i + 1][j - 1] + Mask_h1[0][1] * origin[i + 1][j] + Mask_h1[0][2] * origin[i + 1][j + 1] +
					 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
					 Mask_h1[2][0] * origin[i + 1][j - 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j + 1];
			h2_reg = Mask_h2[0][0] * origin[i + 1][j - 1] + Mask_h2[0][1] * origin[i + 1][j] + Mask_h2[0][2] * origin[i + 1][j + 1] +
					 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
					 Mask_h2[2][0] * origin[i + 1][j - 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
			
			i = width - 1;
			h1_reg = Mask_h1[0][0] * origin[i - 1][j - 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j + 1] +
					 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
					 Mask_h1[2][0] * origin[i - 1][j - 1] + Mask_h1[2][1] * origin[i - 1][j] + Mask_h1[2][2] * origin[i - 1][j + 1];
			h2_reg = Mask_h2[0][0] * origin[i - 1][j - 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j + 1] +
					 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
					 Mask_h2[2][0] * origin[i - 1][j - 1] + Mask_h2[2][1] * origin[i - 1][j] + Mask_h2[2][2] * origin[i - 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
		for(i = 1; i < (width - 1); i++)	//image的上下邊緣 
		{
			j = 0;
			h1_reg = Mask_h1[0][0] * origin[i - 1][j + 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j + 1] +
					 Mask_h1[1][0] * origin[  i  ][j + 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
					 Mask_h1[2][0] * origin[i + 1][j + 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j + 1];
			h2_reg = Mask_h2[0][0] * origin[i - 1][j + 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j + 1] +
					 Mask_h2[1][0] * origin[  i  ][j + 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
					 Mask_h2[2][0] * origin[i + 1][j + 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
			
			j = height - 1;
			h1_reg = Mask_h1[0][0] * origin[i - 1][j - 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j - 1] +
					 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j - 1] +
					 Mask_h1[2][0] * origin[i + 1][j - 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j - 1];
			h2_reg = Mask_h2[0][0] * origin[i - 1][j - 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j - 1] +
					 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j - 1] +
					 Mask_h2[2][0] * origin[i + 1][j - 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j - 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
		{									//image的左下角 
			i = 0;
			j = 0;
			h1_reg = Mask_h1[0][0] * origin[i + 1][j + 1] + Mask_h1[0][1] * origin[i + 1][j] + Mask_h1[0][2] * origin[i + 1][j + 1] +
					 Mask_h1[1][0] * origin[  i  ][j + 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
					 Mask_h1[2][0] * origin[i + 1][j + 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j + 1];
			h2_reg = Mask_h2[0][0] * origin[i + 1][j + 1] + Mask_h2[0][1] * origin[i + 1][j] + Mask_h2[0][2] * origin[i + 1][j + 1] +
					 Mask_h2[1][0] * origin[  i  ][j + 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
					 Mask_h2[2][0] * origin[i + 1][j + 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
		{									//image的右下角 
			i = width - 1;
			j = 0;
			h1_reg = Mask_h1[0][0] * origin[i - 1][j + 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j + 1] +
					 Mask_h1[1][0] * origin[  i  ][j + 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j + 1] +
					 Mask_h1[2][0] * origin[i - 1][j + 1] + Mask_h1[2][1] * origin[i - 1][j] + Mask_h1[2][2] * origin[i - 1][j + 1];
			h2_reg = Mask_h2[0][0] * origin[i - 1][j + 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j + 1] +
					 Mask_h2[1][0] * origin[  i  ][j + 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j + 1] +
					 Mask_h2[2][0] * origin[i - 1][j + 1] + Mask_h2[2][1] * origin[i - 1][j] + Mask_h2[2][2] * origin[i - 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
		{									//image的左上角 
			i = 0;
			j = height - 1;
			h1_reg = Mask_h1[0][0] * origin[i + 1][j - 1] + Mask_h1[0][1] * origin[i + 1][j] + Mask_h1[0][2] * origin[i + 1][j - 1] +
					 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j - 1] +
					 Mask_h1[2][0] * origin[i + 1][j - 1] + Mask_h1[2][1] * origin[i + 1][j] + Mask_h1[2][2] * origin[i + 1][j - 1];
			h2_reg = Mask_h2[0][0] * origin[i + 1][j - 1] + Mask_h2[0][1] * origin[i + 1][j] + Mask_h2[0][2] * origin[i + 1][j - 1] +
					 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j - 1] +
					 Mask_h2[2][0] * origin[i + 1][j - 1] + Mask_h2[2][1] * origin[i + 1][j] + Mask_h2[2][2] * origin[i + 1][j - 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
		{									//image的右上角 
			i = width - 1;
			j = height - 1;
			h1_reg = Mask_h1[0][0] * origin[i - 1][j - 1] + Mask_h1[0][1] * origin[i - 1][j] + Mask_h1[0][2] * origin[i - 1][j - 1] +
					 Mask_h1[1][0] * origin[  i  ][j - 1] + Mask_h1[1][1] * origin[  i  ][j] + Mask_h1[1][2] * origin[  i  ][j - 1] +
					 Mask_h1[2][0] * origin[i - 1][j - 1] + Mask_h1[2][1] * origin[i - 1][j] + Mask_h1[2][2] * origin[i - 1][j - 1];
			h2_reg = Mask_h2[0][0] * origin[i - 1][j - 1] + Mask_h2[0][1] * origin[i - 1][j] + Mask_h2[0][2] * origin[i - 1][j - 1] +
					 Mask_h2[1][0] * origin[  i  ][j - 1] + Mask_h2[1][1] * origin[  i  ][j] + Mask_h2[1][2] * origin[  i  ][j - 1] +
					 Mask_h2[2][0] * origin[i - 1][j - 1] + Mask_h2[2][1] * origin[i - 1][j] + Mask_h2[2][2] * origin[i - 1][j - 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg)) >> offset;
		}
	}
	else
	{
		for(i = 0; i < (width - 1); i++)	//image的中心區塊 
		{
			for(j = 0; j < (height - 1); j++)
			{
				h1_reg = Roberts_h1[0][0] * origin[  i  ][j] + Roberts_h1[0][1] * origin[  i  ][j + 1] +
						 Roberts_h1[1][0] * origin[i + 1][j] + Roberts_h1[1][1] * origin[i + 1][j + 1];
				h2_reg = Roberts_h2[0][0] * origin[  i  ][j] + Roberts_h2[0][1] * origin[  i  ][j + 1] +
						 Roberts_h2[1][0] * origin[i + 1][j] + Roberts_h2[1][1] * origin[i + 1][j + 1];
				result[i][j] = abs(h1_reg) + abs(h2_reg);
			}
		}
		for(j = 0; j < (height - 1); j++)	//image的右緣 
		{
			i = width - 1;
			h1_reg = Roberts_h1[0][0] * origin[  i  ][j] + Roberts_h1[0][1] * origin[  i  ][j + 1] +
					 Roberts_h1[1][0] * origin[i - 1][j] + Roberts_h1[1][1] * origin[i - 1][j + 1];
			h2_reg = Roberts_h2[0][0] * origin[  i  ][j] + Roberts_h2[0][1] * origin[  i  ][j + 1] +
					 Roberts_h2[1][0] * origin[i - 1][j] + Roberts_h2[1][1] * origin[i - 1][j + 1];
			result[i][j] = abs(h1_reg) + abs(h2_reg);
		}
		for(i = 0; i < (width - 1); i++)	//image的下緣 
		{
			j = height - 1;
			h1_reg = Roberts_h1[0][0] * origin[  i  ][j] + Roberts_h1[0][1] * origin[  i  ][j - 1] +
					 Roberts_h1[1][0] * origin[i + 1][j] + Roberts_h1[1][1] * origin[i + 1][j - 1];
			h2_reg = Roberts_h2[0][0] * origin[  i  ][j] + Roberts_h2[0][1] * origin[  i  ][j - 1] +
					 Roberts_h2[1][0] * origin[i + 1][j] + Roberts_h2[1][1] * origin[i + 1][j - 1];
			result[i][j] = abs(h1_reg) + abs(h2_reg);
		}
		{									//image的右上角 
			i = width - 1;
			j = height - 1;
			h1_reg = Roberts_h1[0][0] * origin[  i  ][j] + Roberts_h1[0][1] * origin[  i  ][j - 1] +
					 Roberts_h1[1][0] * origin[i - 1][j] + Roberts_h1[1][1] * origin[i - 1][j - 1];
			h2_reg = Roberts_h2[0][0] * origin[  i  ][j] + Roberts_h2[0][1] * origin[  i  ][j - 1] +
					 Roberts_h2[1][0] * origin[i - 1][j] + Roberts_h2[1][1] * origin[i - 1][j - 1];
			result[i][j] = abs(h1_reg) + abs(h2_reg);
		}
	}
	
	//二值化 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			r[i][j] = (r[i][j] > threshold) ? (255) : (0);
		}
	}
}
