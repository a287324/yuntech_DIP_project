#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "bmp.h"
#include "FFT.h"

#define dist_cent(X, Y) sqrt(X*X + Y*Y)
#define Butterworth_FWF(D0, N, R) 1 / (1 + pow((R/D0), 2*N)) 

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; 
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
float spectrum[MaxBMPSizeX*MaxBMPSizeY*2] = {0};
float sp_re[MaxBMPSizeX][MaxBMPSizeY];
float sp_im[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[]) {
	int width, height;
	int i, j;
	int mm;
	//參數 
	float cent_R;
	float H;
	const int Cutoff_freq_low = 3;
	const int Cufoff_freq_high = 5;
	const int filter_order = 1000;
	
	open_bmp("periodic_noise2.bmp", R, R, R, width, height);
	mm = width/2;
    //image to spectrum  
    pixel_to_data(R, spectrum, width, height);
    fft2(spectrum,width,1);
	data_to_spectrum_2D(spectrum, sp_re, sp_im, width, height);
	  
	//spectrum shift
	spectrum_shift(sp_re, sp_im, mm);
	
	for(i = 0; i < width; i++) {
		for(j = 0; j < height; j++) {
			//計算去除特定頻率的頻率權重函式 
			cent_R = dist_cent((i-mm), (j-mm));
			H = Butterworth_FWF(Cutoff_freq_low, filter_order, cent_R) + (1 - Butterworth_FWF(Cufoff_freq_high, filter_order, cent_R));
			//頻譜與頻率權重函式相乘 
			sp_re[i][j] *= H;
			sp_im[i][j] *= H;
		}
	}
	
	// shift the 2-D spectrum back
	spectrum_shift(sp_re, sp_im, mm);
    //spectrum to image
    spectrum_to_data_2D(sp_re, sp_im, spectrum, width, height);
    fft2(spectrum,width,-1);
    data_to_pixel(spectrum, sp_re, width, height);
    
    // overflow and underflow handling (by linear scaling)
    float min = 1E99;
    float max = -1E99;
    for (i = 0; i < height; i++) {
       for (j = 0; j < width; j++) {
          if (sp_re[i][j] > max) max = sp_re[i][j]; 
          if (sp_re[i][j] < min) min = sp_re[i][j]; 
       }
    }
    float sf;
    sf = 255/(max-min);
    printf("max = %f \t min = %f \t sf = %f \n",max,min,sf);
    for (i = 0; i < height; i++) {
       for (j = 0; j < width; j++) {
          R[i][j] = (int)((sp_re[i][j]-min)*sf); 
       }
    }
	
	//儲存影像 
	save_bmp("periodic_noise2_1.bmp", R, R, R, width, height);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}


