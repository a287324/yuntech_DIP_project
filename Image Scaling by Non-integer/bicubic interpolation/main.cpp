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

double Magnification = 0.58;
 
void Image_scaling(int[][MaxBMPSizeY], int[][MaxBMPSizeY] , int, int, double);
float Bicubic_formula(float X)
{
	float x = fabs(X);
	if(x <= 1)
		return (1.5 * pow(x, 3) - 2.5 * pow(x, 2) + 1);
	else if(x <= 2)
		return (-0.5 * pow(x, 3) + 2.5 * pow(x, 2) - 4 * x + 2);
	else
	{
		cerr << "Bicubic_formula的參數大於2" << endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	if(Magnification == 0 || Magnification < 0)
	{
		cout << "倍率不可為零或負數" << endl;
		exit(1);
	}
	
	//開啟檔案 
	open_bmp("1.bmp", R, G, B, width, height);
	
	//判斷程式是否支援 
	if( (width * Magnification) > MaxBMPSizeX || (height * Magnification) > MaxBMPSizeY )
	{
		cout << "倍率過大,程式無法支援" << endl;
		exit(2);
	}
	//cout << "Bicubic_formula(5/3) = " << Bicubic_formula(5.0/3) << endl;
	//Image scaling 
	
	Image_scaling(r, R, width, height, Magnification);
	Image_scaling(g, G, width, height, Magnification);
	Image_scaling(b, B, width, height, Magnification);
	
	width *= Magnification;
	height *= Magnification;
	
	//儲存檔案 
	save_bmp("1_new.bmp", r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();
	return 0;
}
void Image_scaling(int result[][MaxBMPSizeY], int origin[][MaxBMPSizeY] , int width, int height, double Magnification)
{
	int i, j;
	int width_r = width * Magnification;
	int height_r = height * Magnification;
	int** origin_extension;
	float width_e, height_e;
	float p1, p2, p3, p4;
	
	//動態分配一個新的擴大的原始圖像的空間 
	origin_extension = new int* [width + 4];
	origin_extension[0] = new int [(width + 4) * (height + 4)];
	for(i = 1; i < (width + 4); i++)
		origin_extension[i] = origin_extension[i - 1] + (height + 4);
	//複製原圖到擴大的原始圖像空間 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			origin_extension[i + 2][j + 2] = origin[i][j];
		}
	}
	//分配擴大的原始圖像空間邊界值
	for(i = 2; i < (width + 2); i++)
	{
		origin_extension[i][0] = origin_extension[i][4];
		origin_extension[i][1] = origin_extension[i][3];
		origin_extension[i][height + 2] = origin_extension[i][height];
		origin_extension[i][height + 3] = origin_extension[i][height - 1];
	}
	for(j = 2; j < (height + 2); j++)
	{
		origin_extension[0][j] = origin_extension[4][j];
		origin_extension[1][j] = origin_extension[3][j];
		origin_extension[width + 2][j] = origin_extension[width][j];
		origin_extension[width + 3][j] = origin_extension[width - 1][j];
	}
	{
		origin_extension[0][0] = origin_extension[3][3];
		origin_extension[0][1] = origin_extension[2][3];
		origin_extension[1][0] = origin_extension[3][2];
		origin_extension[1][1] = origin_extension[2][2];
		
		origin_extension[width + 2][0] = origin_extension[width][2];
		origin_extension[width + 2][1] = origin_extension[width + 1][2];
		origin_extension[width + 3][0] = origin_extension[width][3];
		origin_extension[width + 3][1] = origin_extension[width + 1][3];
		
		origin_extension[0][height + 2] = origin_extension[2][height];
		origin_extension[1][height + 2] = origin_extension[2][height + 1];
		origin_extension[0][height + 3] = origin_extension[3][height];
		origin_extension[1][height + 3] = origin_extension[3][height + 1];
		
		origin_extension[width + 2][height + 2] = origin_extension[width + 1][height + 1];
		origin_extension[width + 2][height + 3] = origin_extension[width][height + 1];
		origin_extension[width + 3][height + 2] = origin_extension[width + 1][height];
		origin_extension[width + 3][height + 3] = origin_extension[width][height];
	}
	
	//bilinear interpolation
	for(i = 0; i < width_r; i++)
	{
		for(j = 0; j < height_r; j++)
		{
			width_e = float(i) / width_r * width + 2;
			height_e = float(j) / height_r * height + 2;
			
			p1 = origin_extension[int(width_e - 1)][int(height_e - 1)] * Bicubic_formula(height_e - int(height_e - 1)) +
				 origin_extension[int(width_e - 1)][int(height_e)]     * Bicubic_formula(height_e - int(height_e))     +
				 origin_extension[int(width_e - 1)][int(height_e + 1)] * Bicubic_formula(int(height_e + 1) - height_e) +
				 origin_extension[int(width_e - 1)][int(height_e + 2)] * Bicubic_formula(int(height_e + 2) - height_e);
			p2 = origin_extension[int(width_e)][int(height_e - 1)]     * Bicubic_formula(height_e - int(height_e - 1)) +
				 origin_extension[int(width_e)][int(height_e)]         * Bicubic_formula(height_e - int(height_e))     +
				 origin_extension[int(width_e)][int(height_e + 1)]     * Bicubic_formula(int(height_e + 1) - height_e) +
				 origin_extension[int(width_e)][int(height_e + 2)]     * Bicubic_formula(int(height_e + 2) - height_e);
			p3 = origin_extension[int(width_e + 1)][int(height_e - 1)] * Bicubic_formula(height_e - int(height_e - 1)) +
				 origin_extension[int(width_e + 1)][int(height_e)]     * Bicubic_formula(height_e - int(height_e))     +
				 origin_extension[int(width_e + 1)][int(height_e + 1)] * Bicubic_formula(int(height_e + 1) - height_e) +
				 origin_extension[int(width_e + 1)][int(height_e + 2)] * Bicubic_formula(int(height_e + 2) - height_e);
			p4 = origin_extension[int(width_e + 2)][int(height_e - 1)] * Bicubic_formula(height_e - int(height_e - 1)) +
				 origin_extension[int(width_e + 2)][int(height_e)]     * Bicubic_formula(height_e - int(height_e))     +
				 origin_extension[int(width_e + 2)][int(height_e + 1)] * Bicubic_formula(int(height_e + 1) - height_e) +
				 origin_extension[int(width_e + 2)][int(height_e + 2)] * Bicubic_formula(int(height_e + 2) - height_e);
			result[i][j] =  p1 * Bicubic_formula(width_e - int(width_e - 1)) +
							p2 * Bicubic_formula(width_e - int(width_e))     +
							p3 * Bicubic_formula(int(width_e + 1) - width_e) +
							p4 * Bicubic_formula(int(width_e + 2) - width_e);
			
			if(result[i][j] > 255)
				result[i][j] = 255;
			if(result[i][j] < 0)
				result[i][j] = 0;
		}
	}
	delete [] origin_extension[0];
	delete [] origin_extension;
}
