#include "FFT.cpp"

#define MaxBMPSizeX	4096
#define MaxBMPSizeY	4096

float tmp[MaxBMPSizeX][MaxBMPSizeY];

void fft1(float data[],int nn,int isign);
void fft2(float data[],int nn,int isign);

void pixel_to_data(int pixel[][MaxBMPSizeY], float data[], int width, int height)
{
	int ptr = 0; // pointer for the data array
	int i, j;
    for (i = 0; i < height; i++){
       for (j = 0; j < width; j++){
          data[ptr] = pixel[i][j];     // real part of the input data
          data[ptr+1] = 0;         // imaginary part of the input data
          ptr += 2;
       }
    }
}
void data_to_pixel(float data[], float sp_re[][MaxBMPSizeY], int width, int height)
{
	int ptr = 0; // pointer for the data array
	int i, j;
    for (i = 0; i < height; i++){
       for (j = 0; j < width; j++){
          sp_re[i][j] = data[ptr];
          ptr += 2;
       }
    }
}
void data_to_spectrum_2D(float data[], float sp_re[][MaxBMPSizeY], float sp_im[][MaxBMPSizeY], int width, int height)
{
	int ptr = 0;
	int i, j;
    for (i=0; i<height; i++) {
       for (j=0; j<width; j++) {
          sp_re[i][j] = data[ptr];    // real part of the 2D spectrum
          sp_im[i][j] = data[ptr+1];  // imaginary part of the 2D spectrum
          ptr += 2;
       }
    }
}
void spectrum_to_data_2D(float sp_re[][MaxBMPSizeY], float sp_im[][MaxBMPSizeY], float data[], int width, int height)
{
	int ptr = 0; // pointer for the data array
	int i, j;
    for (i = 0; i < height; i++){
       for (j = 0; j < width; j++){
          data[ptr] = sp_re[i][j];
          data[ptr+1] = sp_im[i][j];
          ptr += 2;
       }
    }
}
void spectrum_shift(float sp_re[][MaxBMPSizeY], float sp_im[][MaxBMPSizeY], int mm){
   for (int i=0; i<mm; i++) {
      for (int j=0; j<mm; j++) {
         //------------------------ the real part
         tmp[i][j] = sp_re[i][j]; // upper-left <-> lower-right
         sp_re[i][j] = sp_re[mm+i][mm+j];
         sp_re[mm+i][mm+j] = tmp[i][j];
         tmp[i][j] = sp_re[i][mm+j]; // upper-right <-> lower-left
         sp_re[i][mm+j] = sp_re[mm+i][j];
         sp_re[mm+i][j] = tmp[i][j];
         //--------------------------- the imag. part
         tmp[i][j] = sp_im[i][j]; // upper-left <-> lower-right
         sp_im[i][j] = sp_im[mm+i][mm+j];
         sp_im[mm+i][mm+j] = tmp[i][j];
         tmp[i][j] = sp_im[i][mm+j]; // upper-right <-> lower-left
         sp_im[i][mm+j] = sp_im[mm+i][j];
         sp_im[mm+i][j] = tmp[i][j];
      }
   }
}
