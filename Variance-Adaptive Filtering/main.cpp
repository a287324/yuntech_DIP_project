#include <iostream>
#include <stdlib.h>
#include <cmath>
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
	float mask_ave, mask_var;
	float img_var_R, img_ave_R;
	float img_var_G, img_ave_G;
	float img_var_B, img_ave_B;
	
	/*load image*/
	open_bmp("1.bmp", R, G, B, width, height);
	
	/*Variance-adaptive filtering*/ 
	/*影像像素平均值*/
    for (i = 0, img_ave_R = 0; i < width; i++)
	{
       for (j = 0; j < height; j++)
	   {
	   		img_ave_R += R[i][j];
       }
    }
    img_ave_R /= (width * height);
    
    for (i = 0, img_ave_G = 0; i < width; i++)
	{
       for (j = 0; j < height; j++)
	   {
	   		img_ave_G += G[i][j];
       }
    }
    img_ave_G /= (width * height);
    
    for (i = 0, img_ave_B = 0; i < width; i++)
	{
       for (j = 0; j < height; j++)
	   {
	   		img_ave_B += B[i][j];
       }
    }
    img_ave_B /= (width * height);
    /**/
    /*影像變異數*/ 
    for(i = 0, img_var_R = 0; i < width; i++)
    {
    	for(j = 0;j < height; j++)
    	{
    		img_var_R += pow( R[i][j] , 2);
		}
	}
	img_var_R = img_var_R / (width * height) - pow(img_ave_R, 2);
	for(i = 0, img_var_G = 0; i < width; i++)
    {
    	for(j = 0;j < height; j++)
    	{
    		img_var_G += pow( G[i][j] , 2);
		}
	}
	img_var_G = img_var_G / (width * height) - pow(img_ave_G, 2);
	for(i = 0, img_var_B = 0; i < width; i++)
    {
    	for(j = 0;j < height; j++)
    	{
    		img_var_B += pow( B[i][j] , 2);
		}
	}
	img_var_B = img_var_B / (width * height) - pow(img_ave_B, 2);
	/**/
	for(i = 1; i < width - 1; i++)
	{
		for(j = 1; j < height - 1; j++)
		{
			/*mask的平均值*/
			mask_ave = R[i - 1][j - 1] + R[i][j - 1] + R[i + 1][j - 1] +
					   R[i - 1][j + 0] + R[i][j + 0] + R[i + 1][j + 0] +
					   R[i - 1][j + 1] + R[i][j + 1] + R[i + 1][j + 1];
			mask_ave /= 9;
			/*mask的變異數*/ 
			mask_var = pow( R[i - 1][j - 1] , 2) + pow( R[i][j - 1] , 2) + pow( R[i + 1][j - 1] , 2) +
					   pow( R[i - 1][j + 0] , 2) + pow( R[i][j + 0] , 2) + pow( R[i + 1][j + 0] , 2) +
					   pow( R[i - 1][j + 1] , 2) + pow( R[i][j + 1] , 2) + pow( R[i + 1][j + 1] , 2);
			mask_var = mask_var / 9 - pow(mask_ave , 2);
			/*公式計算*/ 
			r[i][j] = mask_ave + (mask_var) / (mask_var + img_var_R) * (R[i][j] - mask_ave);
			
			/*mask的平均值*/
			mask_ave = G[i - 1][j - 1] + G[i][j - 1] + G[i + 1][j - 1] +
					   G[i - 1][j + 0] + G[i][j + 0] + G[i + 1][j + 0] +
					   G[i - 1][j + 1] + G[i][j + 1] + G[i + 1][j + 1];
			mask_ave /= 9;
			/*mask的變異數*/ 
			mask_var = pow( G[i - 1][j - 1] , 2) + pow( G[i][j - 1] , 2) + pow( G[i + 1][j - 1] , 2) +
					   pow( G[i - 1][j + 0] , 2) + pow( G[i][j + 0] , 2) + pow( G[i + 1][j + 0] , 2) +
					   pow( G[i - 1][j + 1] , 2) + pow( G[i][j + 1] , 2) + pow( G[i + 1][j + 1] , 2);
			mask_var = mask_var / 9 - pow(mask_ave , 2);
			/*公式計算*/ 
			g[i][j] = mask_ave + (mask_var) / (mask_var + img_var_G) * (G[i][j] - mask_ave);
			
			/*mask的平均值*/
			mask_ave = B[i - 1][j - 1] + B[i][j - 1] + B[i + 1][j - 1] +
					   B[i - 1][j + 0] + B[i][j + 0] + B[i + 1][j + 0] +
					   B[i - 1][j + 1] + B[i][j + 1] + B[i + 1][j + 1];
			mask_ave /= 9;
			/*mask的變異數*/ 
			mask_var = pow( B[i - 1][j - 1] , 2) + pow( B[i][j - 1] , 2) + pow( B[i + 1][j - 1] , 2) +
					   pow( B[i - 1][j + 0] , 2) + pow( B[i][j + 0] , 2) + pow( B[i + 1][j + 0] , 2) +
					   pow( B[i - 1][j + 1] , 2) + pow( B[i][j + 1] , 2) + pow( B[i + 1][j + 1] , 2);
			mask_var = mask_var / 9 - pow(mask_ave , 2);
			/*公式計算*/ 
			b[i][j] = mask_ave + (mask_var) / (mask_var + img_var_B) * (B[i][j] - mask_ave);
		}
	}
	
	/*Save image*/ 
	save_bmp("1_new.bmp", r, g, b, width, height); // for gray images
	/**/ 
		
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
