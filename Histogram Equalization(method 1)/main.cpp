#include <iostream>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	float gray_level[256] = {};
	
	/*Load image*/ 
	open_bmp("boat.bmp", R, R, R, width, height);
	
	/*統計影像的像素出現的次數*/ 
    for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			gray_level[ (R[i][j]) ]++;
			
	/*各像素在影像出現機率*/ 
	for(i = 0; i < 256; i++)
		gray_level[i] /= (width * height);
		
	/*各像素的CDF(cumulative distribution function)*/ 
	for(i = 1; i < 256; i++)
		gray_level[i] += gray_level[i - 1];
	
	/*像素出現的次數均等化*/
	for(i = 0, j = gray_level[0]; i < 256; i++)
		gray_level[i] = 255 * (gray_level[i] - j) / (1 - j);
	
	/*以均等化結果去取代原值*/ 
	for(i = 0; i < width; i++)
		for(j = 0; j < height; j++)
			r[i][j] = gray_level[ R[i][j] ];
	
    /*Save image*/
	save_bmp("boat_new.bmp", r, r, r);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
