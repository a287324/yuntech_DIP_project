#include <iostream>
#include <stdlib.h>
#include "bmp.hpp"

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
	float LPF_separate_mask_row[1][3] = { 1.0/3, 1.0/3, 1.0/3};
	float LPF_separate_mask_col[3][1] = { 1.0/3, 
										  1.0/3, 
										  1.0/3};
	int threshold = 30;
	
	/*load image*/
	open_bmp("1.bmp", R, G, B, width, height);
	/**/ 
	
	/*Adaptive smoothing filtering*/ 
	/*General Lowpass filtering*/ 
	/*column方向的LPF*/ 
    for (i = 0; i < width; i++)
	{
		/*邊緣的LPF，邊緣採取：mirroring補值*/ 
		r[i][0] = R[i][1] * LPF_separate_mask_col[0][0] + 
				  R[i][0] * LPF_separate_mask_col[1][0] + 
	   			  R[i][1] * LPF_separate_mask_col[2][0];
	   	r[i][height - 1] = R[i][height - 2] * LPF_separate_mask_col[0][0] + 
						   R[i][height - 1] * LPF_separate_mask_col[1][0] + 
	   					   R[i][height - 2] * LPF_separate_mask_col[2][0];
	   					   
	   	g[i][0] = G[i][1] * LPF_separate_mask_col[0][0] + 
				  G[i][0] * LPF_separate_mask_col[1][0] + 
	   			  G[i][1] * LPF_separate_mask_col[2][0];
	   	g[i][height - 1] = G[i][height - 2] * LPF_separate_mask_col[0][0] + 
						   G[i][height - 1] * LPF_separate_mask_col[1][0] + 
	   					   G[i][height - 2] * LPF_separate_mask_col[2][0];
	   					
	   	b[i][0] = B[i][1] * LPF_separate_mask_col[0][0] + 
				  B[i][0] * LPF_separate_mask_col[1][0] + 
	   			  B[i][1] * LPF_separate_mask_col[2][0];
	   	b[i][height - 1] = B[i][height - 2] * LPF_separate_mask_col[0][0] + 
						   B[i][height - 1] * LPF_separate_mask_col[1][0] + 
	   					   B[i][height - 2] * LPF_separate_mask_col[2][0];
	   					  
       for (j = 1; j < height - 1; j++)
	   {
	   		r[i][j] = R[i][j - 1] * LPF_separate_mask_col[0][0] + 
	   				  R[i][j] * LPF_separate_mask_col[1][0] + 
	   				  R[i][j + 1] * LPF_separate_mask_col[2][0];
	   		g[i][j] = G[i][j - 1] * LPF_separate_mask_col[0][0] + 
	   				  G[i][j] * LPF_separate_mask_col[1][0] + 
	   				  G[i][j + 1] * LPF_separate_mask_col[2][0];
	   		b[i][j] = B[i][j - 1] * LPF_separate_mask_col[0][0] + 
	   				  B[i][j] * LPF_separate_mask_col[1][0] + 
	   				  B[i][j + 1] * LPF_separate_mask_col[2][0];
       }
    }
    /*Row方向的LPF*/
    for (j = 0; j < height; j++)
	{
		/*邊緣的LPF，邊緣採取：mirroring補值*/  
		r[0][j] = r[1][j] * LPF_separate_mask_row[0][0] + 
	   			  r[0][j] * LPF_separate_mask_row[0][1] +
	   			  r[1][j] * LPF_separate_mask_row[0][2];
	   	r[width -1][j] = r[width - 2][j] * LPF_separate_mask_row[0][0] + 
	   					 r[width - 1][j] * LPF_separate_mask_row[0][1] +
	   					 r[width - 2][j] * LPF_separate_mask_row[0][2];
	   	
	   	g[0][j] = g[1][j] * LPF_separate_mask_row[0][0] + 
	   			  g[0][j] * LPF_separate_mask_row[0][1] +
	   			  g[1][j] * LPF_separate_mask_row[0][2];
	   	g[width -1][j] = g[width - 2][j] * LPF_separate_mask_row[0][0] + 
	   					 g[width - 1][j] * LPF_separate_mask_row[0][1] +
	   					 g[width - 2][j] * LPF_separate_mask_row[0][2];
	   	b[0][j] = b[1][j] * LPF_separate_mask_row[0][0] + 
	   			  b[0][j] * LPF_separate_mask_row[0][1] +
	   			  b[1][j] * LPF_separate_mask_row[0][2];
	   	b[width -1][j] = b[width - 2][j] * LPF_separate_mask_row[0][0] + 
	   					 b[width - 1][j] * LPF_separate_mask_row[0][1] +
	   					 b[width - 2][j] * LPF_separate_mask_row[0][2];
	   					 
		for (i = 1; i < width - 1; i++)
		{
	   		r[i][j] = r[i - 1][j] * LPF_separate_mask_row[0][0] + 
	   				  r[i][j] * LPF_separate_mask_row[0][1] +
	   				  r[i + 1][j] * LPF_separate_mask_row[0][2];
	   		g[i][j] = g[i - 1][j] * LPF_separate_mask_row[0][0] + 
	   				  g[i][j] * LPF_separate_mask_row[0][1] +
	   				  g[i + 1][j] * LPF_separate_mask_row[0][2];
	   		b[i][j] = b[i - 1][j] * LPF_separate_mask_row[0][0] + 
	   				  b[i][j] * LPF_separate_mask_row[0][1] +
	   				  b[i + 1][j] * LPF_separate_mask_row[0][2];
		}
    }
    /*判斷LPF與原值差異，並決定是否保留原值*/ 
    for(i = 0; i < width; i++)
    {
    	for(j = 0; j < height; j++)
    	{
    		r[i][j] = ( abs(r[i][j] - R[i][j]) > threshold ) ? (r[i][j]) : (R[i][j]);
    		g[i][j] = ( abs(g[i][j] - G[i][j]) > threshold ) ? (g[i][j]) : (G[i][j]);
    		b[i][j] = ( abs(b[i][j] - B[i][j]) > threshold ) ? (b[i][j]) : (B[i][j]);
		}
	}
	/**/
	
	/*Save image*/ 
	save_bmp("1_new.bmp", r, g, b, width, height); // for gray images
	/**/ 
		
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
