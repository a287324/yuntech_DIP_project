#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <new>
#include "bmp.h"
#include "FFT.h"

#define D_2(U, V) ((U)*(U) + (V)*(V))
#define Gaussian_LPF_FWF(U, V, SIGMA) exp(-(D_2(U,V)/(2*(SIGMA)*(SIGMA))))
#define Gaussian_HPF_FWF(U, V, SIGMA) (1.0 - Gaussian_LPF_FWF(U, V, SIGMA))

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
float data[MaxBMPSizeX*MaxBMPSizeY*2];
float sp_re[MaxBMPSizeX][MaxBMPSizeY];
float sp_im[MaxBMPSizeX][MaxBMPSizeY];
float sp_re_LPF[MaxBMPSizeX][MaxBMPSizeY];
float sp_im_LPF[MaxBMPSizeX][MaxBMPSizeY];
float sp_re_HPF[MaxBMPSizeX][MaxBMPSizeY];
float sp_im_HPF[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	int mm;
	//Gaussian
	const float Sigma = 10;
	
	open_bmp("lena.bmp", R, R, R, width, height);
	mm = width/2;
	
    //image to spectrum  
    pixel_to_data(R, data, width, height);
    fft2(data,width,1);
	data_to_spectrum_2D(data, sp_re, sp_im, width, height);
	
	//spectrum shift
	spectrum_shift(sp_re, sp_im, mm);
	
	//Gaussian_LPF and Gaussian_HPF
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			sp_re_LPF[i][j] = sp_re[i][j] * Gaussian_LPF_FWF(i-mm, j-mm, Sigma);
			sp_im_LPF[i][j] = sp_im[i][j] * Gaussian_LPF_FWF(i-mm, j-mm, Sigma);
			
			sp_re_HPF[i][j] = sp_re[i][j] * Gaussian_HPF_FWF(i-mm, j-mm, Sigma);
			sp_im_HPF[i][j] = sp_im[i][j] * Gaussian_HPF_FWF(i-mm, j-mm, Sigma);
		}
	}
	
	// shift the 2-D spectrum back
	spectrum_shift(sp_re_LPF, sp_im_LPF, mm);
	spectrum_shift(sp_re_HPF, sp_im_HPF, mm);
	
    //spectrum to image
    //LPF
    spectrum_to_data_2D(sp_re_LPF, sp_im_LPF, data, width, height);
    fft2(data,width,-1);
    data_to_pixel(data, sp_re_LPF, width, height);
	//HPF
	spectrum_to_data_2D(sp_re_HPF, sp_im_HPF, data, width, height);
    fft2(data,width,-1);
    data_to_pixel(data, sp_re_HPF, width, height);
    
    // overflow and underflow handling (by linear scaling)
    float min_LPF, min_HPF;
    float max_LPF, max_HPF;
    min_LPF = min_HPF =  1E99;
    max_LPF = max_HPF = -1E99;
    for (i = 0; i < width; i++) {
       for (j = 0; j < height; j++) {
          if (sp_re_LPF[i][j] > max_LPF) max_LPF = sp_re_LPF[i][j]; 
          if (sp_re_LPF[i][j] < min_LPF) min_LPF = sp_re_LPF[i][j];
		  
		  if (sp_re_HPF[i][j] > max_HPF) max_HPF = sp_re_HPF[i][j]; 
          if (sp_re_HPF[i][j] < min_HPF) min_HPF = sp_re_HPF[i][j];
       }
    }
    
    float sf_LPF, sf_HPF;
    sf_LPF = 255/(max_LPF-min_LPF);
	sf_HPF = 255/(max_HPF-min_HPF);
    printf("max_LPF = %f \t min_LPF = %f \t sf_LPF = %f \n",max_LPF,min_LPF,sf_LPF);
	printf("max_HPF = %f \t min_HPF = %f \t sf_HPF = %f \n",max_HPF,min_HPF,sf_HPF);
	
    for (i = 0; i < width; i++) {
       for (j = 0; j < height; j++) {
          r[i][j] = (int)((sp_re_LPF[i][j]-min_LPF)*sf_LPF);
       }
    }
    save_bmp("lena_LPF.bmp", r, r, r, width, height);
    
    for (i = 0; i < width; i++) {
       for (j = 0; j < height; j++) {
          r[i][j] = (int)((sp_re_HPF[i][j]-min_HPF)*sf_HPF);
       }
    }
    save_bmp("lena_HPF.bmp", r, r, r, width, height);
	
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}


