#define PI 3.14159265359
#include "bmp.h"
void Canny_edge_detector(int [][MaxBMPSizeY], float, int, int, int[][MaxBMPSizeY], int, int);
void connet_weak_edge(int*, int, int, int&, int&);

void Canny_edge_detector(int result[][MaxBMPSizeY], float var, int threshold_h, int threshold_l, int origin[][MaxBMPSizeY], int width, int height)
{
	int **img_reg;
	double **img_dir;
	//Gaussian LPF 
	int mask[3][3], mask_sum;
	int mask_offset = 256;
	//Sobel
	int Sobel_h1[3][3] = {  {-1, 0, 1},
							{-2, 0, 2},
							{-1, 0, 1} };
	int Sobel_h2[3][3] = {  {-1, -2, -1},
							{ 0,  0,  0},
							{ 1,  2,  1}};
	int h1_reg, h2_reg;
	//
	int i, j, k, m, n;
	
	//Gaussian LPF	//輸入: int origin[width][height](原image), float var, int width, int height, 輸出: int img_reg[width][height](經過LPF的img) 
		//產生高斯遮罩 
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			mask[i][j] = ((exp( -( ( ( pow((i - 1),2) + pow((j - 1), 2) ) ) / (2 * var * var) ) )) * mask_offset);
			mask_sum += mask[i][j];
		}
	}
		//暫存Gaussian LPF 
	img_reg = new int* [width];
	img_reg[0] = new int [width * height];
	for(i = 1; i < width; i++)
		img_reg[i] = img_reg[i - 1] + height;
	
	for(i = 1; i < (width - 1); i++)	//image的中心區塊 
	{
		for(j = 1; j < (height - 1); j++)
		{
			img_reg[i][j] = mask[0][0] * origin[i - 1][j - 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j + 1] +
							mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
							mask[2][0] * origin[i + 1][j - 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j + 1];
			img_reg[i][j] = img_reg[i][j] / mask_sum;
		}
	}
	for(i = 1; i < (width - 1); i++)	//image的上下邊緣 
	{
		j = 0;
		img_reg[i][j] = mask[0][0] * origin[i - 1][j + 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j + 1] +
						mask[1][0] * origin[  i  ][j + 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
						mask[2][0] * origin[i + 1][j + 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j + 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
		
		j = height - 1;
		img_reg[i][j] = mask[0][0] * origin[i - 1][j - 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j - 1] +
						mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j - 1] +
						mask[2][0] * origin[i + 1][j - 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j - 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
	}
	for(j = 1; j < (height - 1); j++)	//image的左右邊緣 
	{
		i = 0;
		img_reg[i][j] = mask[0][0] * origin[i + 1][j - 1] + mask[0][1] * origin[i + 1][  j  ] + mask[0][2] * origin[i + 1][j + 1] +
						mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
						mask[2][0] * origin[i + 1][j - 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j + 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
		i = width - 1;
		img_reg[i][j] = mask[0][0] * origin[i - 1][j - 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j + 1] +
						mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
						mask[2][0] * origin[i - 1][j - 1] + mask[2][1] * origin[i - 1][  j  ] + mask[2][2] * origin[i - 1][j + 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
	}
	{									//image的四個角 
		i = 0;
		j = 0;
		img_reg[i][j] = mask[0][0] * origin[i + 1][j + 1] + mask[0][1] * origin[i + 1][  j  ] + mask[0][2] * origin[i + 1][j + 1] +
						mask[1][0] * origin[  i  ][j + 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
						mask[2][0] * origin[i + 1][j + 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j + 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
		
		i = width - 1;
		j = 0;
		img_reg[i][j] = mask[0][0] * origin[i - 1][j + 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j + 1] +
						mask[1][0] * origin[  i  ][j + 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
						mask[2][0] * origin[i - 1][j + 1] + mask[2][1] * origin[i - 1][  j  ] + mask[2][2] * origin[i - 1][j + 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
		
		i = 0;
		j = height - 1;
		img_reg[i][j] = mask[0][0] * origin[i + 1][j - 1] + mask[0][1] * origin[i + 1][  j  ] + mask[0][2] * origin[i + 1][j - 1] +
						mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j - 1] +
						mask[2][0] * origin[i + 1][j - 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j - 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
		
		i = width - 1;
		j = height - 1;
		img_reg[i][j] = mask[0][0] * origin[i - 1][j - 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j - 1] +
						mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j - 1] +
						mask[2][0] * origin[i - 1][j - 1] + mask[2][1] * origin[i - 1][  j  ] + mask[2][2] * origin[i - 1][j - 1];
		img_reg[i][j] = img_reg[i][j] / mask_sum;
	}
	
	//Sobel: 輸入:int img_reg[width][height](經過LPF的image) , 輸出: int result[width][height](經過sobel的image) int img_dir[width][height](image的gradient direction) 
	img_dir = new double* [width];
	img_dir[0] = new double [width * height];
	for(i = 1; i < width; i++)
		img_dir[i] = img_dir[i - 1] + height;
		
	for(i = 1; i < (width - 1); i++)	//image的中心區塊 
	{
		for(j = 1; j < (height - 1); j++)
		{
			h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j - 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j + 1] +
					 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
					 Sobel_h1[2][0] * img_reg[i + 1][j - 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j + 1];
			h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j - 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j + 1] +
					 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
					 Sobel_h2[2][0] * img_reg[i + 1][j - 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j + 1];
			result[i][j] = (abs(h1_reg) + abs(h2_reg));
			img_dir[i][j] = atan2(h2_reg, h1_reg);
		}
	}
	for(i = 1; i < (width - 1); i++)	//image的上下邊緣 
	{
		j = 0;
		h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j + 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j + 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h1[2][0] * img_reg[i + 1][j + 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j + 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j + 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j + 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h2[2][0] * img_reg[i + 1][j + 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j + 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
		
		j = height - 1;
		h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j - 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j - 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h1[2][0] * img_reg[i + 1][j - 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j - 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j - 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j - 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h2[2][0] * img_reg[i + 1][j - 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j - 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
	}
	for(j = 1; j < (height - 1); j++)	//image的左右邊緣 
	{
		i = 0;
		h1_reg = Sobel_h1[0][0] * img_reg[i + 1][j - 1] + Sobel_h1[0][1] * img_reg[i + 1][j] + Sobel_h1[0][2] * img_reg[i + 1][j + 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h1[2][0] * img_reg[i + 1][j - 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j + 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i + 1][j - 1] + Sobel_h2[0][1] * img_reg[i + 1][j] + Sobel_h2[0][2] * img_reg[i + 1][j + 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h2[2][0] * img_reg[i + 1][j - 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j + 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
			
		i = width - 1;
		h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j - 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h1[2][0] * img_reg[i - 1][j - 1] + Sobel_h1[2][1] * img_reg[i - 1][j] + Sobel_h1[2][2] * img_reg[i - 1][j + 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j - 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h2[2][0] * img_reg[i - 1][j - 1] + Sobel_h2[2][1] * img_reg[i - 1][j] + Sobel_h2[2][2] * img_reg[i - 1][j + 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
	}
	{									//image的四個角 
		i = 0;
		j = 0;
		h1_reg = Sobel_h1[0][0] * img_reg[i + 1][j + 1] + Sobel_h1[0][1] * img_reg[i + 1][j] + Sobel_h1[0][2] * img_reg[i + 1][j + 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j + 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h1[2][0] * img_reg[i + 1][j + 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j + 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i + 1][j + 1] + Sobel_h2[0][1] * img_reg[i + 1][j] + Sobel_h2[0][2] * img_reg[i + 1][j + 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j + 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h2[2][0] * img_reg[i + 1][j + 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j + 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
		
		i = width - 1;
		j = 0;
		h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j + 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j + 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h1[2][0] * img_reg[i - 1][j + 1] + Sobel_h1[2][1] * img_reg[i - 1][j] + Sobel_h1[2][2] * img_reg[i - 1][j + 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j + 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j + 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j + 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j + 1] +
				 Sobel_h2[2][0] * img_reg[i - 1][j + 1] + Sobel_h2[2][1] * img_reg[i - 1][j] + Sobel_h2[2][2] * img_reg[i - 1][j + 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
		
		i = 0;
		j = height - 1;
		h1_reg = Sobel_h1[0][0] * img_reg[i + 1][j - 1] + Sobel_h1[0][1] * img_reg[i + 1][j] + Sobel_h1[0][2] * img_reg[i + 1][j - 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h1[2][0] * img_reg[i + 1][j - 1] + Sobel_h1[2][1] * img_reg[i + 1][j] + Sobel_h1[2][2] * img_reg[i + 1][j - 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i + 1][j - 1] + Sobel_h2[0][1] * img_reg[i + 1][j] + Sobel_h2[0][2] * img_reg[i + 1][j - 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h2[2][0] * img_reg[i + 1][j - 1] + Sobel_h2[2][1] * img_reg[i + 1][j] + Sobel_h2[2][2] * img_reg[i + 1][j - 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
		
		i = width - 1;
		j = height - 1;
		h1_reg = Sobel_h1[0][0] * img_reg[i - 1][j - 1] + Sobel_h1[0][1] * img_reg[i - 1][j] + Sobel_h1[0][2] * img_reg[i - 1][j - 1] +
				 Sobel_h1[1][0] * img_reg[  i  ][j - 1] + Sobel_h1[1][1] * img_reg[  i  ][j] + Sobel_h1[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h1[2][0] * img_reg[i - 1][j - 1] + Sobel_h1[2][1] * img_reg[i - 1][j] + Sobel_h1[2][2] * img_reg[i - 1][j - 1];
		h2_reg = Sobel_h2[0][0] * img_reg[i - 1][j - 1] + Sobel_h2[0][1] * img_reg[i - 1][j] + Sobel_h2[0][2] * img_reg[i - 1][j - 1] +
				 Sobel_h2[1][0] * img_reg[  i  ][j - 1] + Sobel_h2[1][1] * img_reg[  i  ][j] + Sobel_h2[1][2] * img_reg[  i  ][j - 1] +
				 Sobel_h2[2][0] * img_reg[i - 1][j - 1] + Sobel_h2[2][1] * img_reg[i - 1][j] + Sobel_h2[2][2] * img_reg[i - 1][j - 1];
		result[i][j] = (abs(h1_reg) + abs(h2_reg));
		img_dir[i][j] = atan2(h2_reg, h1_reg);
	}
	delete img_reg[0];
	delete img_reg;
	//nonmaximum suppression	輸入:result[width][height](sobel後的image) img_dir[width][height](image的gradient direction) 輸出: result[width][height](經過nonmaximum suppression) 
		//gradient direction的分類 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if((img_dir[i][j] >= (PI / 8) && img_dir[i][j] < (3 * PI / 8)) || (img_dir[i][j] >= (-7 * PI / 8) && img_dir[i][j] < (-5 * PI / 8)))
				img_dir[i][j] = 45;
			else if((img_dir[i][j] >= (3 * PI / 8) && img_dir[i][j] < (5 * PI / 8)) || (img_dir[i][j] >= (-5 * PI / 8) && img_dir[i][j] < (-3 * PI / 8)))
				img_dir[i][j] = 90;
			else if((img_dir[i][j] >= (5 * PI / 8) && img_dir[i][j] < (7 * PI / 8)) || (img_dir[i][j] >= (-3 * PI / 8) && img_dir[i][j] < (-1 * PI / 8)))
				img_dir[i][j] = 135;
			else
				img_dir[i][j] = 0;
		}
	}
		//在有相同的gradient direction的邊緣上進行 
	for(i = 1; i < (width - 1); i++)
	{
		for(j = 1; j < (height - 1); j++)
		{
			switch (int(img_dir[i][j]))
			{
				case 0:
					if(result[i][j] < result[i][j - 1] || result[i][j] < result[i][j + 1])
						result[i][j] = 0;
					break;
				case 45:
					if(result[i][j] < result[i - 1][j - 1] || result[i][j] < result[i + 1][j + 1])
						result[i][j] = 0;
					break;
				case 90:
					if(result[i][j] < result[i - 1][j] || result[i][j] < result[i + 1][j])
						result[i][j] = 0;
					break;
				case 135:
					if(result[i][j] < result[i + 1][j - 1] || result[i][j] < result[i - 1][j + 1])
						result[i][j] = 0;
					break;
				default:
					cerr << "img_dir出現問題" << endl;
					exit(1);
			} 
		}
	}
	
	delete img_dir[0];
	delete img_dir;
	//double threshold	輸入: result[width][height](經過nonmaximum suppression) 輸出: result[width][height](經double threshold)
	int width_reg = width + 2;
	int height_reg = height + 2;
	img_reg = new int* [width_reg];
	img_reg[0] = new int [(width_reg) * (height_reg)];
	for(i = 1; i < width_reg; i++)
		img_reg[i] = img_reg[i - 1] + (height_reg);
		//分類
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(result[i][j] <= threshold_l)
				img_reg[i + 1][j + 1] = 0;
			else if(result[i][j] > threshold_h)
			{
				img_reg[i + 1][j + 1] = 0xff;
			}
			else
			{
				img_reg[i + 1][j + 1] = 0x7f;
			}
		}
	}
		//連接弱邊緣 
	for(i = 1; i < (width + 1); i++)
	{
		for(j = 1; j < (height + 1); j++)
		{
			if(img_reg[i][j] == 0xff)
			{
				connet_weak_edge(&img_reg[0][0], i, j, width_reg, height_reg);
			}
		}
	}
	
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			//去除沒被連接的弱邊緣 
			if(img_reg[i + 1][j + 1] != 0x7f)
				result[i][j] = img_reg[i + 1][j + 1];
			else
				result[i][j] = 0;
		}
	}
	delete img_reg[0];
	delete img_reg;
}
void connet_weak_edge(int* img, int center_w, int center_h, int& width, int& height)
{
	int i, j;
	for(i = (center_w - 1); i <= (center_w + 1); i++)
	{
		for(j = (center_h - 1); j <= (center_h + 1); j++)
		{
			if(*(img + (i * height) + j) == 0x7f)
			{
				*(img + (i * height) + j) = 0xff;
				connet_weak_edge(img, i, j, width, height);
			}
		}
	}
}
