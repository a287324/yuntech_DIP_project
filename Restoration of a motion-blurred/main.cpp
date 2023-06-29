#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "bmp.h"
#include "FFT.h"

#define PI 3.14159265359
#define H_re(T, A, U) ((T) / ((PI)*(U)*(A)) * sin((PI)*(U)*(A)) * cos((PI)*(U)*(A)))
#define H_im(T, A, U) -((T) / ((PI)*(U)*(A)) * sin((PI)*(U)*(A)) * sin((PI)*(U)*(A)))
 
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
float data[MaxBMPSizeX*MaxBMPSizeY*2];
float sp_re[MaxBMPSizeX][MaxBMPSizeY]; 
float sp_im[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j, k;
	//Motion blur 
	int mm;
	int sp_re_tmp, sp_im_tmp;
	float H_re_inv, H_im_inv;
	float H_re_tmp, H_im_tmp;
	float H_mag_2;
	const float T = 1.0;
	const float a = 0.14;
	const float H_mini = 0.0001;
	
	//load image
	open_bmp("CarPlate2_blur2.bmp", R, R, R, width, height);
	mm = width/2;
    
    //image to spectrum  
    pixel_to_data(R, data, width, height);
    fft2(data,width,1);
	data_to_spectrum_2D(data, sp_re, sp_im, width, height);
	  
	//spectrum shift
	spectrum_shift(sp_re, sp_im, mm);
	
	//G(u,v) / H(u,v)
	for(i = 0, k = -mm; i < width; i++, k++)
	{
		//計算H(u,v) 
		if(k == 0)
		{
			//透過羅必達法則計算 
			H_re_tmp = T;
			H_im_tmp = 0;
		}
		else
		{
			H_re_tmp = H_re(T, a, k);
			H_im_tmp = H_im(T, a, k);
		}
		 
		for(j = 0; j < height; j++)
		{
			//計算 1/H(u,v)	
			H_mag_2 = H_re_tmp*H_re_tmp + H_im_tmp*H_im_tmp;
			if(H_mag_2 >= H_mini)
			{
				H_re_inv = H_re_tmp / H_mag_2;
				H_im_inv = (0-H_im_tmp) / H_mag_2;
			}
			else
			{
				H_re_inv = 0;
				H_im_inv = 0;
			}
			// G(u,v) / H(u,v) 
			sp_re_tmp = sp_re[i][j] * H_re_inv - sp_im[i][j] * H_im_inv;
			sp_im_tmp = sp_re[i][j] * H_im_inv + sp_im[i][j] * H_re_inv;
			sp_re[i][j] = sp_re_tmp;
			sp_im[i][j] = sp_im_tmp;
		}
	}
	
	// shift the 2-D spectrum back
	spectrum_shift(sp_re, sp_im, mm);
   
    //spectrum to image
    spectrum_to_data_2D(sp_re, sp_im, data, width, height);
    fft2(data,width,-1);
    data_to_pixel(data, sp_re, width, height);
    
	//Clipping
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(sp_re[i][j] > 255)
				R[i][j] = 255;
			else if(sp_re[i][j] < 0)
				R[i][j] = 0;
			else
				R[i][j] = sp_re[i][j];
		}
	}
	
	//儲存影像 
	save_bmp("CarPlate2_blur2_1.bmp", R, R, R, width, height);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}


