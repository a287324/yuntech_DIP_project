#include <iostream>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	
	int gray_level[256] = {};
	int origin, end;
	
	/*Load image*/ 
	open_bmp("1.bmp", R, R, R, width, height);
	
	/*統計影像的像素出現的次數*/ 
    for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			gray_level[ (R[i][j]) ]++;
			
	/*影像像素出現的累積次數*/
	for( i = 1; i < 256; i++)
			gray_level[i] += gray_level[i-1];
	
	/*像素出現的次數均等化*/
	for(i = 0, origin = gray_level[0], end = gray_level[255]; i < 256; i++)
		gray_level[i] = 255.0 * (gray_level[i] - origin) / (end - origin);
	
	/*替代原值*/ 
	for(i = 0; i < width; i++)
		for(j = 0; j < height; j++)
			r[i][j] = gray_level[ R[i][j] ];
	
    /*Save image*/
	save_bmp("1_new.bmp", r, r, r, width, height);
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}
