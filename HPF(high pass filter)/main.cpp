#include <iostream>
#include <stdlib.h>
#include "bmp.hpp"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	float HPF_mask[3][3] = { -1.0/9, -1.0/9, -1.0/9,
							 -1.0/9,  8.0/9, -1.0/9,
							 -1.0/9, -1.0/9, -1.0/9};
	
	/*load image*/
	open_bmp("boat.bmp", R, R, R, width, height);
	
	/*highpass filtering*/ 
    for (i = 1; i < width - 1; i++)
	{
       for (j = 1; j < height - 1; j++)
	   {
	   		r[i][j] = R[i - 1][j - 1] * HPF_mask[0][0] + 
					  R[i - 1][j + 0] * HPF_mask[0][1] + 
					  R[i - 1][j + 1] * HPF_mask[0][2] +
	   				  R[i + 0][j - 1] * HPF_mask[1][0] + 
					  R[i + 0][j + 0] * HPF_mask[1][1] + 
					  R[i + 0][j + 1] * HPF_mask[1][2] +
	   				  R[i + 1][j - 1] * HPF_mask[2][0] + 
					  R[i + 1][j + 0] * HPF_mask[2][1] + 
					  R[i + 1][j + 1] * HPF_mask[2][2];
       }
    }
	
	/*Save image*/ 
	save_bmp("boat_HPF.bmp", r, r, r, width, height); // for gray images
		
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
