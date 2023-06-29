#include <iostream>
#include "bmp.hpp"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

enum Direction {horizontal = 0, vertical, right_oblique, left_oblique};

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	float direction_R[4];
	float direction_G[4];
	float direction_B[4];
	
	float diff_R[4];
	float diff_G[4];
	float diff_B[4];
	
	float min_R, min_G, min_B;
	int flag_min_R, flag_min_G, flag_min_B;
	
	/*讀取影像*/ 
	open_bmp("1.bmp", R, G, B, width, height);
	
	/*影像邊緣：保留原影像*/
	for(i = 0; i < width; i++)
	{
		r[i][0] = R[i][0];
		g[i][0] = G[i][0];
		b[i][0] = B[i][0];
		
		r[i][height - 1] = R[i][height - 1];
		g[i][height - 1] = G[i][height - 1];
		b[i][height - 1] = B[i][height - 1];
	}
	for(j = 0; j < height; j++)
	{
		r[0][j] = R[0][j];
		g[0][j] = G[0][j];
		b[0][j] = B[0][j];
		
		r[width - 1][j] = R[width - 1][j];
		g[width - 1][j] = G[width - 1][j];
		b[width - 1][j] = B[width - 1][j];
	}
	
	/*Directional smoothing*/ 
    for (i = 1; i < width - 1; i++)
	{
		for (j = 1; j < height - 1; j++)
		{
			/*計算4個方向的像素平均值*/
			direction_R[horizontal] = (R[i][j - 1] + R[i][j] + R[i][j + 1]) / 3;
			direction_R[vertical] = (R[i - 1][j] + R[i][j] + R[i + 1][j]) / 3;
			direction_R[right_oblique] = (R[i + 1][j + 1] + R[i][j] + R[i - 1][j - 1]) / 3;
			direction_R[left_oblique] = (R[i - 1][j - 1] + R[i][j] + R[i + 1][j + 1]) / 3;
			
			direction_G[horizontal] = (G[i][j - 1] + G[i][j] + G[i][j + 1]) / 3;
			direction_G[vertical] = (G[i - 1][j] + G[i][j] + G[i + 1][j]) / 3;
			direction_G[right_oblique] = (G[i + 1][j + 1] + G[i][j] + G[i - 1][j - 1]) / 3;
			direction_G[left_oblique] = (G[i - 1][j - 1] + G[i][j] + G[i + 1][j + 1]) / 3;
			
			direction_B[horizontal] = (B[i][j - 1] + B[i][j] + B[i][j + 1]) / 3;
			direction_B[vertical] = (B[i - 1][j] + B[i][j] + B[i + 1][j]) / 3;
			direction_B[right_oblique] = (B[i + 1][j + 1] + B[i][j] + B[i - 1][j - 1]) / 3;
			direction_B[left_oblique] = (B[i - 1][j - 1] + B[i][j] + B[i + 1][j + 1]) / 3;
			
			/*計算4個方向的差值(  = |平均值 - 原值| ) */
			diff_R[horizontal] = fabs( direction_R[horizontal] - R[i][j] );
			diff_R[vertical] = fabs( direction_R[vertical] - R[i][j] );
			diff_R[right_oblique] = fabs( direction_R[right_oblique] - R[i][j] );
			diff_R[left_oblique] = fabs( direction_R[left_oblique] - R[i][j] );
			
			diff_G[horizontal] = fabs( direction_G[horizontal] - G[i][j] );
			diff_G[vertical] = fabs( direction_G[vertical] - G[i][j] );
			diff_G[right_oblique] = fabs( direction_G[right_oblique] - G[i][j] );
			diff_G[left_oblique] = fabs( direction_G[left_oblique] - G[i][j] );
			
			diff_B[horizontal] = fabs( direction_B[horizontal] - B[i][j] );
			diff_B[vertical] = fabs( direction_B[vertical] - B[i][j] );
			diff_B[right_oblique] = fabs( direction_B[right_oblique] - B[i][j] );
			diff_B[left_oblique] = fabs( direction_B[left_oblique] - B[i][j] );
			
			/*找出最小差值的方向,並將其平均值代替原值*/
			min_R = diff_R[horizontal];
			flag_min_R = horizontal;
			if(min_R > diff_R[vertical])
			{
				min_R = diff_R[vertical];
				flag_min_R = vertical;
			}
			if(min_R > diff_R[right_oblique])
			{
				min_R = diff_R[right_oblique];
				flag_min_R = right_oblique;
			}
			if(min_R > diff_R[left_oblique])
			{
				min_R = diff_R[left_oblique];
				flag_min_R = left_oblique;
			}
			r[i][j] = direction_R[flag_min_R];
			
			min_G = diff_G[horizontal];
			flag_min_G = horizontal;
			if(min_G > diff_G[vertical])
			{
				min_G = diff_G[vertical];
				flag_min_G = vertical;
			}
			if(min_G > diff_G[right_oblique])
			{
				min_G = diff_G[right_oblique];
				flag_min_G = right_oblique;
			}
			if(min_G > diff_G[left_oblique])
			{
				min_G = diff_G[left_oblique];
				flag_min_G = left_oblique;
			}
			g[i][j] = direction_G[flag_min_G];
			
			min_B = diff_B[horizontal];
			flag_min_B = horizontal;
			if(min_B > diff_B[vertical])
			{
				min_B = diff_B[vertical];
				flag_min_B = vertical;
			}
			if(min_B > diff_B[right_oblique])
			{
				min_B = diff_B[right_oblique];
				flag_min_B = right_oblique;
			}
			if(min_B > diff_B[left_oblique])
			{
				min_B = diff_B[left_oblique];
				flag_min_B = left_oblique;
			}
			b[i][j] = direction_B[flag_min_B];
		}
	}
    
	/*儲存影像*/ 
	save_bmp("1_new.bmp", r, g, b, width, height);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
