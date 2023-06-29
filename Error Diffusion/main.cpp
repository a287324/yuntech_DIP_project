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
	int Error;
	
	/*Load image*/ 
	open_bmp("boat.bmp", R, R, R, width, height); 
	
	/*Error Diffusion*/ 
    for (j = 0; j < height; j++)
	{
       for (i = 0; i < width; i++)
	   {
	   		/*§PÂ_»~®t­È*/ 
	   		if(R[i][j] < 128)
	   		{
	   			Error = R[i][j];
	   			R[i][j] = 0;
			}
			else
			{
				Error = R[i][j] - 255;
				R[i][j] = 255;
			}
			
			/*»~®tÂX´²*/ 
			if(j < height - 1)
			{
       			if(i > 0)
					R[i - 1][j + 1] += Error * 3 / 16;
       				
				R[i][j + 1] += Error * 5 / 16;
       			
				if(i < width - 1)
       				R[i + 1][j + 1] += Error / 16;
			}
			if(i < width - 1)
				R[i + 1][j] += Error * 7 / 16;
       }
    }
    
	/*save image*/ 
	save_bmp("boat_new.bmp", R, R, R, width, height); // for gray images
	
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
