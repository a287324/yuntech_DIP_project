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
	
	/*�έp�v���������X�{������*/ 
    for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			gray_level[ (R[i][j]) ]++;
			
	/*�v�������X�{���ֿn����*/
	for( i = 1; i < 256; i++)
			gray_level[i] += gray_level[i-1];
	
	/*�����X�{�����Ƨ�����*/
	for(i = 0, origin = gray_level[0], end = gray_level[255]; i < 256; i++)
		gray_level[i] = 255.0 * (gray_level[i] - origin) / (end - origin);
	
	/*���N���*/ 
	for(i = 0; i < width; i++)
		for(j = 0; j < height; j++)
			r[i][j] = gray_level[ R[i][j] ];
	
    /*Save image*/
	save_bmp("1_new.bmp", r, r, r, width, height);
	printf("Job Finished!\n");

	// ���� bmp �v������
	close_bmp();

	return 0;
}
