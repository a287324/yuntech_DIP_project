#include <iostream>
#include <stdlib.h>
#include "bmp.hpp"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j, m, k;
	int Array_median[9];
	int Reg;
	
	/*load image*/
	open_bmp("lena.bmp", R, R, R, width, height);
	
	/*Median filtering*/ 
    for (i = 1; i < width - 1; i++)
	{
       for (j = 1; j < height - 1; j++)
	   {
	   		/*取出Mask遮罩住的pixel*/
	   		Array_median[0] = R[i - 1][j - 1];
	   		Array_median[1] = R[i - 1][j + 0];
	   		Array_median[2] = R[i - 1][j + 1];
	   		Array_median[3] = R[i + 0][j - 1];
	   		Array_median[4] = R[i + 0][j + 0];
	   		Array_median[5] = R[i + 0][j + 1];
	   		Array_median[6] = R[i + 1][j - 1];
	   		Array_median[7] = R[i + 1][j + 0];
	   		Array_median[8] = R[i + 1][j + 1];
	   		/*排列Mask的pixel*/
	   		for(m = 0; m < 9; m++)
	   		{
	   			for(k = m + 1; k < 9; k++)
	   			{
	   				if(Array_median[m] > Array_median[k])
	   				{
	   					Reg = Array_median[m];
	   					Array_median[m] = Array_median[k];
	   					Array_median[k] = Reg;
					}
				}
			}
			/*中位數取代原值*/ 
	   		r[i][j] = Array_median[4]; 
       }
    }
	/**/
	
	/*Save image*/ 
	save_bmp("lena_new.bmp", r, r, r, width, height); // for gray images
		
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
