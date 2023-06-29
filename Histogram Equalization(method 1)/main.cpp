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
	
	/*�έp�v���������X�{������*/ 
    for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			gray_level[ (R[i][j]) ]++;
			
	/*�U�����b�v���X�{���v*/ 
	for(i = 0; i < 256; i++)
		gray_level[i] /= (width * height);
		
	/*�U������CDF(cumulative distribution function)*/ 
	for(i = 1; i < 256; i++)
		gray_level[i] += gray_level[i - 1];
	
	/*�����X�{�����Ƨ�����*/
	for(i = 0, j = gray_level[0]; i < 256; i++)
		gray_level[i] = 255 * (gray_level[i] - j) / (1 - j);
	
	/*�H�����Ƶ��G�h���N���*/ 
	for(i = 0; i < width; i++)
		for(j = 0; j < height; j++)
			r[i][j] = gray_level[ R[i][j] ];
	
    /*Save image*/
	save_bmp("boat_new.bmp", r, r, r);
	printf("Job Finished!\n");

	// ���� bmp �v������
	close_bmp();

	return 0;
}
