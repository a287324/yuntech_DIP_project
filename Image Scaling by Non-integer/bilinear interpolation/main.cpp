#include <iostream>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

double Magnification = 0.5;
 
void Image_scaling(int[][MaxBMPSizeY], int[][MaxBMPSizeY] , int, int, double);

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
	open_bmp("5.bmp", R, G, B, width, height);
	
	//判斷程式是否支援 
	if( (width * Magnification) > MaxBMPSizeX || (height * Magnification) > MaxBMPSizeY )
	{
		cout << "倍率過大,程式無法支援" << endl;
		exit(2);
	}
	
	//Image scaling 
	Image_scaling(r, R, width, height, Magnification);
	Image_scaling(g, G, width, height, Magnification);
	Image_scaling(b, B, width, height, Magnification);
	
	width *= Magnification;
	height *= Magnification;
	
	//儲存檔案 
	save_bmp("5_new.bmp", r, g, b, width, height); // for gray images
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
	float width_o, height_o;
	float p1, p2;
	//bilinear interpolation
	for(i = 0; i < width_r; i++)
	{
		for(j = 0; j < height_r; j++)
		{
			width_o = float(i) / width_r * width;
			height_o = float(j) / height_r * height;
			 
			if( (int(height_o) != (height - 1)) && (int(width_o) != (width - 1)) ) 	//中心區塊 
			{
				p1 = origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
					 origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
				p2 = origin[int(width_o + 1)][int(height_o)] * (int(height_o + 1) - height_o) +
					 origin[int(width_o + 1)][int(height_o + 1)] * (height_o - int(height_o));
				result[i][j] =  p1 * (int(width_o + 1) - width_o) + 
								p2 * (width_o - int(width_o));
			}
			else if( (int(height_o) == (height - 1)) && (int(width_o) != (width - 1)) )	//圖的上面邊緣 
			{
				result[i][j] =  origin[int(width_o)][int(height_o)] * (int(width_o + 1) - width_o) +
								origin[int(width_o + 1)][int(height_o)] * (width_o - int(width_o));
			}
			else if( (int(height_o) != (height - 1)) && (int(width_o) == (width - 1)) )	//圖的側邊邊緣 
			{
				result[i][j] =  origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
								origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
			}
			else	//圖的角落 
			{
				result[i][j] = origin[int(width_o)][int(height_o)];
			}
		}
	}
}
