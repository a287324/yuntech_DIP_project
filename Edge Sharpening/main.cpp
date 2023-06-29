#include <iostream>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	float mask[3][3] = { -1.0/3, -1.0/3, -1.0/3,
						 -1.0/3, 11.0/3, -1.0/3,
						 -1.0/3, -1.0/3, -1.0/3,};
	int max_r, min_r;
	int max_g, min_g;
	int max_b, min_b;
	
	/*Open Image*/ 
	open_bmp("1.bmp", R, G, B, width, height);
	
	max_r = 0;
	min_r = 255;
	max_g = 0;
	min_g = 255;
	max_b = 0;
	min_b = 255;
	/*Edge Sharpening*/ 
    for (i = 1; i < width; i++)
	{
		for (j = 1; j < height; j++)
		{
			r[i][j] = R[i - 1][j - 1] * mask[0][0] + 
					  R[i - 1][j + 0] * mask[0][1] + 
					  R[i - 1][j + 1] * mask[0][2] +
	   				  R[i + 0][j - 1] * mask[1][0] + 
					  R[i + 0][j + 0] * mask[1][1] + 
					  R[i + 0][j + 1] * mask[1][2] +
	   				  R[i + 1][j - 1] * mask[2][0] + 
					  R[i + 1][j + 0] * mask[2][1] + 
					  R[i + 1][j + 1] * mask[2][2];
			if(r[i][j] > max_r)
				max_r = r[i][j];
			if(r[i][j] < min_r)
				min_r = r[i][j];
				
	   		g[i][j] = G[i - 1][j - 1] * mask[0][0] + 
					  G[i - 1][j + 0] * mask[0][1] + 
					  G[i - 1][j + 1] * mask[0][2] +
	   				  G[i + 0][j - 1] * mask[1][0] + 
					  G[i + 0][j + 0] * mask[1][1] + 
					  G[i + 0][j + 1] * mask[1][2] +
	   				  G[i + 1][j - 1] * mask[2][0] + 
					  G[i + 1][j + 0] * mask[2][1] + 
					  G[i + 1][j + 1] * mask[2][2];
			if(g[i][j] > max_g)
				max_g = g[i][j];
			if(g[i][j] < min_g)
				min_g = g[i][j];
						  
	   		b[i][j] = B[i - 1][j - 1] * mask[0][0] + 
					  B[i - 1][j + 0] * mask[0][1] + 
					  B[i - 1][j + 1] * mask[0][2] +
	   				  B[i + 0][j - 1] * mask[1][0] + 
					  B[i + 0][j + 0] * mask[1][1] + 
					  B[i + 0][j + 1] * mask[1][2] +
	   				  B[i + 1][j - 1] * mask[2][0] + 
					  B[i + 1][j + 0] * mask[2][1] + 
					  B[i + 1][j + 1] * mask[2][2];
			if(b[i][j] > max_b)
				max_b = b[i][j];
			if(b[i][j] < min_b)
				min_b = b[i][j];
		}
	}
	for(i = 1; i < width; i++)
	{
		for(j = 1; j < height; j++)
		{
			r[i][j] = 255.0 * (r[i][j] - min_r) / (max_r - min_r);
			g[i][j] = 255.0 * (g[i][j] - min_g) / (max_g - min_g);
			b[i][j] = 255.0 * (b[i][j] - min_b) / (max_b - min_b);
		}
	}
	cout << "max_r = " << max_r << endl;
    cout << "min_r = " << min_r << endl;
    cout << "max_g = " << max_g << endl;
    cout << "min_g = " << min_g << endl;
    cout << "max_b = " << max_b << endl;
    cout << "min_b = " << min_b << endl;
    /*Save Image*/
	save_bmp("1_new.bmp", r, g, b);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
