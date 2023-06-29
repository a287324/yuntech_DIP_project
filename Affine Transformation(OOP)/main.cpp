#include <iostream>
#include "MyClass.h" 
#include "bmp.h"

using namespace std;
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

template <class T1, class T2>
void Get_affine_matrix(Matrix<T1>&, Coordinate<T2> [], Coordinate<T2> [], const int);

template <class T>
void Affine_transformation(Matrix<T>, int [][MaxBMPSizeY], int [][MaxBMPSizeY], int, int);

int main(int argc, char *argv[])
{
	set_new_handler(LackMemory);
	int width, height;
	int i, j;
	
	Matrix<float> Affine_matrix(3, 3);
	const int control_point_amount = 4;
	Coordinate<float> control_point_o[control_point_amount];
	Coordinate<float> control_point_r[control_point_amount];
	
	//開啟檔案 
	open_bmp("2.bmp", R, G, B, width, height);
	
	//設定原始控制點及轉換後的控制點 
	control_point_o[0] = Coordinate<float>(100, 100);
	control_point_o[1] = Coordinate<float>(width-100, 100);
	control_point_o[2] = Coordinate<float>(100,  height-100);
	control_point_o[3] = Coordinate<float>(width-100, height-100);
	
	control_point_r[0] = Coordinate<float>(0, 0);
	control_point_r[1] = Coordinate<float>(width, 0);
	control_point_r[2] = Coordinate<float>(0, height);
	control_point_r[3] = Coordinate<float>(width, height);
	
	//計算affine_matrix 
	Get_affine_matrix(Affine_matrix, control_point_o, control_point_r, control_point_amount);
	
	//affine transformation
	Affine_transformation( Affine_matrix, r, R, width, height);
	Affine_transformation( Affine_matrix, g, G, width, height);
	Affine_transformation( Affine_matrix, b, B, width, height);
	
	//儲存檔案 
	save_bmp("2_new.bmp", r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}

template <class T1, class T2>
void Get_affine_matrix(Matrix<T1>& Affine_matrix, Coordinate<T2> point_o[], Coordinate<T2> point_r[], const int amount)
{
	int i, j, k;
	Matrix<T1> A((amount * 2), 6), AT(6, (amount * 2)), b((amount * 2), 1), X_Ls(6, 1);
	
	//將原始控制點變成矩陣 
	for(i = 1; i <= amount * 2; i+=2)
	{
		A.at(i, 1) = point_o[(i >> 1)].at(Px);
		A.at(i, 2) = point_o[(i >> 1)].at(Py);
		A.at(i, 3) = 1;
		A.at(i, 4) = 0;
		A.at(i, 5) = 0;
		A.at(i, 6) = 0;
	}
	for(i = 2; i <= amount * 2; i+=2)
	{
		A.at(i, 1) = 0;
		A.at(i, 2) = 0;
		A.at(i, 3) = 0;
		A.at(i, 4) = point_o[(i >> 1) - 1].at(Px);
		A.at(i, 5) = point_o[(i >> 1) - 1].at(Py);
		A.at(i, 6) = 1;
	}
	//將轉換後的控制點變成矩陣 
	for(int i = 1; i <= amount * 2; i+=2)
	{
		b.at(i, 1) = point_r[i >> 1].at(Px);
	}
	for(int i = 2; i <= amount * 2; i+=2)
	{
		b.at(i, 1) = point_r[(i >> 1) - 1].at(Py);
	}
	
	//做least-square estimate,取得affine matrix參數 
	AT = ~A;
	X_Ls = ((!(AT * A)) * AT) * b;
	
	//將affine matrix參數轉成3x3的矩陣 
	for(i = 1, k = 1; i <= 2; i++)
	{
		for(int j = 1; j <= 3; j++, k++)
		{
			Affine_matrix.at(i, j) = X_Ls.at(k, 1);
		}
	}
	Affine_matrix.at(3, 1) = 0;
	Affine_matrix.at(3, 2) = 0;
	Affine_matrix.at(3, 3) = 1;
}

template <class T>
void Affine_transformation(Matrix<T> A, int result[][MaxBMPSizeY], int origin[][MaxBMPSizeY], int width, int height)
{
	Matrix<T> AT;
	Matrix<T> Point_o(3, 1), Point_r(3,1);
	int width_o, height_o;
	float p1, p2;
	int i, j;
	
	AT = !A;
	
	Point_r.at(3,1) = 1;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			//結果座標 
			Point_r.at(1,1) = i;
			Point_r.at(2,1) = j;
			
			//透過affine matrix的反矩陣取得原圖座標 
			Point_o = AT * Point_r;
			
			//找原圖近似像素值(bilinear interpolation) 
			width_o = Point_o.at(1, 1);
			height_o = Point_o.at(2, 1);
			if(width_o >= 0 && width_o < width && height_o >=0 && height_o < height)
			{
				if( (int(height_o) != (height - 1)) && (int(width_o) != (width - 1)) )
				{
					p1 = origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
						 origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
					p2 = origin[int(width_o + 1)][int(height_o)] * (int(height_o + 1) - height_o) +
						 origin[int(width_o + 1)][int(height_o + 1)] * (height_o - int(height_o));
					result[i][j] =  p1 * (int(width_o + 1) - width_o) + 
									p2 * (width_o - int(width_o));
				}
				else if( (int(height_o) == (height - 1)) && (int(width_o) != (width - 1)) )
				{
					result[i][j] =  origin[int(width_o)][int(height_o)] * (int(width_o + 1) - width_o) +
									origin[int(width_o + 1)][int(height_o)] * (width_o - int(width_o));
				}
				else if( (int(width_o) == (width - 1)) && (int(height_o) != (height - 1)) )
				{
					result[i][j] =  origin[int(width_o)][int(height_o)] * (int(height_o + 1) - height_o) +
									origin[int(width_o)][int(height_o + 1)] * (height_o - int(height_o));
				}
				else
				{
					result[i][j] = origin[int(width_o)][int(height_o)];
				}
			}
		}
	}
}
