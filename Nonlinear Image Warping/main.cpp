#include <iostream>
#include "bmp.h"

#define PI 3.14159265359

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

void Image_warping(int [][MaxBMPSizeY], int [][MaxBMPSizeY], int, int);

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	//開啟檔案 
	open_bmp("2.bmp", R, G, B, width, height);
	
	Image_warping(r, R, width, height);
	Image_warping(g, G, width, height);
	Image_warping(b, B, width, height);
	
	//儲存檔案 
	save_bmp("2_new.bmp", r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
void Image_warping(int result[][MaxBMPSizeY], int origin[][MaxBMPSizeY], int width, int height)
{
	
	float Win = width / 2;
	float Wout = width / 2;
	float Center[2] = { (width - 1.0) / 2, (height - 1.0) / 2};	//計算中心點 
	float Dout,Din;
	float Xin, Yin;
	float Xout, Yout; 
	float width_o, height_o;
	float p1, p2;
	int i, j;
	
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			Xout = i - Center[0];
			Yout = j - Center[1];
			Dout = sqrt( (pow(Xout, 2) + pow(Yout, 2)) );
			if(Dout > Wout)		//如果不在映射範圍內，則不做運算 
				continue;
			Din = 2 * Win / PI * asin(Dout / Wout);
			Xin = Din * Xout / Dout;
			Yin = Din * Yout / Dout;
			width_o = Xin + Center[0];
			height_o = Yin + Center[1];
			/*線性內插*/
			if( (int(height_o) != (height - 1)) && (int(width_o) != (width - 1)) )
			{
				p1 = origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
					 origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
				p2 = origin[int(width_o + 1)][int(height_o)] * (int(height_o + 1) - height_o) +
					 origin[int(width_o + 1)][int(height_o + 1)] * (height_o - int(height_o));
				result[i][j] =  p1 * (int(width_o + 1) - width_o) + 
								p2 * (width_o - int(width_o));
			}
			else if( (int(height_o) == (height - 1)) && (int(width_o) != (width - 1)) )
			{
				result[i][j] =  origin[int(width_o)][int(height_o)] * (int(width_o + 1) - width_o) +
								origin[int(width_o + 1)][int(height_o)] * (width_o - int(width_o));
			}
			else if( (int(width_o) == (width - 1)) && (int(height_o) != (height - 1)) )
			{
				result[i][j] =  origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
								origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
			}
			else
			{
				result[i][j] = origin[int(width_o)][int(height_o)];
			}
		}
	}
}
