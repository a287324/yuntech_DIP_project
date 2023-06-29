#include <iostream>
#include <iomanip>
#include <cmath>
#include "bmp.h"

#define PI 3.14159265359
//旋轉矩陣 
#define Rotation_X(X, Y, R) X * cos(R) - Y * sin(R)
#define Rotation_Y(X, Y, R) X * sin(R) + Y * cos(R)
//旋轉反矩陣
#define Rotation_invert_X(X, Y, R) X * cos(R) + Y * sin(R)
#define Rotation_invert_Y(X, Y, R)  - X * sin(R) + Y * cos(R)
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

const double Rotation_angle = 45 * PI / 180; //unit: degree
void Image_rotation(int [][MaxBMPSizeY], int&, int&, int [][MaxBMPSizeY], int, int, double);

int main(int argc, char *argv[])
{
	int width, height;
	int width_new, height_new;
	//開啟檔案 
	open_bmp("1.bmp", R, G, B, width, height);
		//計算旋轉後的邊框大小
	Image_rotation(r, width_new, height_new, R, width, height, Rotation_angle);
	Image_rotation(g, width_new, height_new, G, width, height, Rotation_angle);
	Image_rotation(b, width_new, height_new, B, width, height, Rotation_angle);
	
	//儲存檔案 
	save_bmp("1new.bmp", r, g, b, width_new, height_new); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
void Image_rotation(int result[][MaxBMPSizeY], int &width_new, int &height_new, int origin[][MaxBMPSizeY], int width, int height, double rotation)
{
	int *width_rotation, *height_rotation;
	int width_origin, height_origin;
	const int coordinate_amount = 4;
	float p1, p2;
	
	int i, j, m, n;
	
	width_rotation = new int[coordinate_amount];
	height_rotation = new int[coordinate_amount];
	
	width_rotation[0] = 0;
	width_rotation[1] = Rotation_X( 0, height, rotation);
	width_rotation[2] = Rotation_X( width, height, rotation);
	width_rotation[3] = Rotation_X( width, 0, rotation);
	
	height_rotation[0] = 0;
	height_rotation[1] = Rotation_Y( 0, height, rotation);
	height_rotation[2] = Rotation_Y( width, height, rotation);
	height_rotation[3] = Rotation_Y( width, 0, rotation);
	
		//排序 
	for(i = coordinate_amount - 1; i > 0; i--)
	{
		for(j = 0; j < i; j++)
		{
			if(width_rotation[j] > width_rotation[j + 1])
			{
				m = width_rotation[j];
				width_rotation[j] = width_rotation[j + 1];
				width_rotation[j + 1] = m;
			}
		}
	}
	for(i = coordinate_amount - 1; i > 0; i--)
	{
		for(j = 0; j < i; j++)
		{
			if(height_rotation[j] > height_rotation[j + 1])
			{
				m = height_rotation[j];
				height_rotation[j] = height_rotation[j + 1];
				height_rotation[j + 1] = m;
			}
		}
	}
	
	width_new = width_rotation[coordinate_amount - 1] - width_rotation[0];
	height_new = height_rotation[coordinate_amount - 1] - height_rotation[0];
	
	if( width_new > MaxBMPSizeX || height_new > MaxBMPSizeY)
	{
		cout << "旋轉後的圖檔大小超出程式可處理的大小." << endl;
		exit(1);
	}
	
	//旋轉 
	
	for(i = width_rotation[0], m = 0; i < width_rotation[coordinate_amount - 1]; i++, m++)
	{
		for(j = height_rotation[0], n = 0; j < height_rotation[coordinate_amount - 1]; j++, n++)
		{
			width_origin = Rotation_invert_X(i, j, rotation);
			height_origin = Rotation_invert_Y(i, j, rotation);
			if(width_origin >= 0 && width_origin < width && height_origin >= 0 && height_origin < height)
			{
				if( (int(height_origin) != (height - 1)) && (int(width_origin) != (width - 1)) )
				{
					p1 = origin[int(width_origin)][int(height_origin)] * (int(height_origin + 1) - height_origin) +
						 origin[int(width_origin)][int(height_origin + 1)] * (height_origin - int(height_origin));
					p2 = origin[int(width_origin + 1)][int(height_origin)] * (int(height_origin + 1) - height_origin) +
						 origin[int(width_origin + 1)][int(height_origin + 1)] * (height_origin - int(height_origin));
					result[m][n] =  p1 * (int(width_origin + 1) - width_origin) + 
									p2 * (width_origin - int(width_origin));
				}
				else if( (int(height_origin) == (height - 1)) && (int(width_origin) != (width - 1)) )
				{
					result[m][n] =  origin[int(width_origin)][int(height_origin)] * (int(width_origin + 1) - width_origin) +
									origin[int(width_origin + 1)][int(height_origin)] * (width_origin - int(width_origin));
				}
				else if( (int(width_origin) == (width - 1)) && (int(height_origin) != (height - 1)) )
				{
					result[m][n] =  origin[int(width_origin)][int(height_origin)] * (int(height_origin + 1) - height_origin) +
									origin[int(width_origin)][int(height_origin + 1)] * (height_origin - int(height_origin));
				}
				else
				{
					result[m][n] = origin[int(width_origin)][int(height_origin)];
				}
			}
			
		}
	}
	
	delete [] width_rotation;
	delete [] height_rotation;
}
