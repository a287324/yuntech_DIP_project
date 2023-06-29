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
	
	//Gaussian LPF	//��J: int origin[width][height](��image), float var, int width, int height, ��X: int img_reg[width][height](�g�LLPF��img) 
		//���Ͱ����B�n 
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			mask[i][j] = ((exp( -( ( ( pow((i - 1),2) + pow((j - 1), 2) ) ) / (2 * var * var) ) )) * mask_offset);
			mask_sum += mask[i][j];
		}
	}
		//�ȦsGaussian LPF 
	img_reg = new int* [width];
	img_reg[0] = new int [width * height];
	for(i = 1; i < width; i++)
		img_reg[i] = img_reg[i - 1] + height;
	
	for(i = 1; i < (width - 1); i++)	//image�����߰϶� 
	{
		for(j = 1; j < (height - 1); j++)
		{
			img_reg[i][j] = mask[0][0] * origin[i - 1][j - 1] + mask[0][1] * origin[i - 1][  j  ] + mask[0][2] * origin[i - 1][j + 1] +
							mask[1][0] * origin[  i  ][j - 1] + mask[1][1] * origin[  i  ][  j  ] + mask[1][2] * origin[  i  ][j + 1] +
							mask[2][0] * origin[i + 1][j - 1] + mask[2][1] * origin[i + 1][  j  ] + mask[2][2] * origin[i + 1][j + 1];
			img_reg[i][j] = img_reg[i][j] / mask_sum;
		}
	}
	for(i = 1; i < (width - 1); i++)	//image���W�U��t 
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
	for(j = 1; j < (height - 1); j++)	//image�����k��t 
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
	{									//image���|�Ө� 
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
	
	//Sobel: ��J:int img_reg[width][height](�g�LLPF��image) , ��X: int result[width][height](�g�Lsobel��image) int img_dir[width][height](image��gradient direction) 
	img_dir = new double* [width];
	img_dir[0] = new double [width * height];
	for(i = 1; i < width; i++)
		img_dir[i] = img_dir[i - 1] + height;
		
	for(i = 1; i < (width - 1); i++)	//image�����߰϶� 
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
	for(i = 1; i < (width - 1); i++)	//image���W�U��t 
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
	for(j = 1; j < (height - 1); j++)	//image�����k��t 
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
	{									//image���|�Ө� 
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
	//nonmaximum suppression	��J:result[width][height](sobel�᪺image) img_dir[width][height](image��gradient direction) ��X: result[width][height](�g�Lnonmaximum suppression) 
		//gradient direction������ 
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
		//�b���ۦP��gradient direction����t�W�i�� 
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
					cerr << "img_dir�X�{���D" << endl;
					exit(1);
			} 
		}
	}
	
	delete img_dir[0];
	delete img_dir;
	//double threshold	��J: result[width][height](�g�Lnonmaximum suppression) ��X: result[width][height](�gdouble threshold)
	int width_reg = width + 2;
	int height_reg = height + 2;
	img_reg = new int* [width_reg];
	img_reg[0] = new int [(width_reg) * (height_reg)];
	for(i = 1; i < width_reg; i++)
		img_reg[i] = img_reg[i - 1] + (height_reg);
		//����
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
		//�s���z��t 
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
			//�h���S�Q�s�����z��t 
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
