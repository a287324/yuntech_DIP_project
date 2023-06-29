#include <iostream>
#include <iomanip>
#include <cmath>
#include "bmp.hpp"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
typedef struct {
	int maxima;
	int minima;
	int median;
}Mask_info;
Mask_info Mask_sort(int width, int height, int col_cen, int row_cen, int mask_size);

int main(int argc, char *argv[]) {
	int width, height;
	int i, j;
	
	int mask_size;
	int mask_max_size = 21; 
	Mask_info mask_info;
	
	/*load image*/
	open_bmp("circuit.bmp", R, R, R, width, height);
	
	/*Adaptive Median Filtering*/ 
    for (i = 0; i < width; i++)
    {
		for (j = 0; j < height; j++)
		{	
			mask_size = 3;
			while(1)
			{
				/*遮罩排列*/
				mask_info = Mask_sort(width, height, i, j, mask_size);
				/*檢查中心點是否可靠?*/ 
				if( R[i][j] == mask_info.maxima || R[i][j] == mask_info.minima)	//中心點不可靠 
				{
					/*檢查中位數是否可靠*/ 
					if( mask_info.median == mask_info.maxima || mask_info.median == mask_info.minima )	//中位數不可靠 
					{
						mask_size += 2;
						if(mask_size > mask_max_size)
						{
							r[i][j] = mask_info.median;
							break;
						}
					}
					else	//中位數可靠就以中位數取代中心值 
					{
						r[i][j] = mask_info.median;
						break;
					}
					
				}
				else	//中心點可靠就保留 
				{
					r[i][j] = R[i][j];
					break;
				} 
			}
			
		}
    }
	/*Save image*/ 
	save_bmp("circuit_new.bmp", r, r, r, width, height); // for gray images
		
	printf("Job Finished!\n");
	close_bmp();

	return 0;
}
Mask_info Mask_sort(int width, int height, int col_cen, int row_cen, int mask_size)
{
	Mask_info mask_info;
	
	int* Mask_seq = new int[mask_size * mask_size];
	int offset = mask_size / 2;
	int col_mask_min, col_mask_max;
	int row_mask_min, row_mask_max;
	
	int col_mask, row_mask;
	int i, j;
	int n;
	int Reg;
	
	col_mask_min = col_cen - offset;
	col_mask_max = col_cen + offset;
	row_mask_min = row_cen - offset;
	row_mask_max = row_cen + offset;

	col_mask = col_mask_min;
	row_mask = row_mask_min;

	/*取出被遮罩罩住的pixel值*/
	for(i = 0, n = 0; i < (mask_size * mask_size); i++)
	{
		if(col_mask >= 0 && col_mask < width && row_mask >= 0 && row_mask < height)
		{
			Mask_seq[n] = R[col_mask][row_mask];
			n++;
		}
		if(row_mask == row_mask_max)
		{
			if(col_mask == col_mask_max)
			{
				break;
			}
			else
			{
				row_mask = row_mask_min;
				col_mask++;
			}
		}
		else
		{
			row_mask++;
		}
	}
	/*排列*/ 
	for(i = 0; i < n; i++)
	{
		for(j = i + 1; j < n; j++)
		{
			if(Mask_seq[i] > Mask_seq[j])
			{
				Reg = Mask_seq[i];
				Mask_seq[i] = Mask_seq[j];
				Mask_seq[j] = Reg;
			}
		}
	}
	
	mask_info.maxima = Mask_seq[n - 1];
	mask_info.minima = Mask_seq[0];
	mask_info.median = (n % 2 == 1) ? (Mask_seq[n / 2]) : ( (Mask_seq[n / 2] + Mask_seq[n / 2 - 1]) / 2 );
	
	delete [] Mask_seq;
	return mask_info;
}
