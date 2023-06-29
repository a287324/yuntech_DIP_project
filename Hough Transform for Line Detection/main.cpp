#include <iostream>
#include <new>
#include "MyCanny.h" 

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

void Line_detection(int [][MaxBMPSizeY], int [][MaxBMPSizeY], int [][MaxBMPSizeY], int [][MaxBMPSizeY], int [][MaxBMPSizeY], int [][MaxBMPSizeY], int, int, int);
void LackMemory() {
	std::cerr << "記憶體不足" << std::endl;
	exit(1);
}

typedef struct S_and_angle
{
	int S;
	int angle;
	int count;
}S_angle;

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	const int threshold_count = 60;
	//開啟檔案 
	open_bmp("3.bmp", R, G, B, width, height);
	
	Line_detection(r, g, b, R, G, B, width, height, threshold_count);
	
	//儲存檔案 
	save_bmp("1_1.bmp", r, g, b, width, height); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	return 0;
}

void Line_detection(int result_r[][MaxBMPSizeY], int result_g[][MaxBMPSizeY], int result_b[][MaxBMPSizeY], int origin_r[][MaxBMPSizeY], int origin_g[][MaxBMPSizeY], int origin_b[][MaxBMPSizeY], int width, int height, int threshold_count)
{
	//Canny
	int **binary_img;
	float Gaussian_var = 1.5;
	int threshold_h = 300, threshold_l = 200;
	//s,theta的表格 
	int feature_point, angle_interval = 180;
	int** List;
	
	//表格中最常出現的組合
	int Reg;
	int count;
	
	int i, j, k, m, n;
	
	//偵測動態分配有沒有出問題 
	set_new_handler(LackMemory);
	
	//Canny_edge_detector
	Canny_edge_detector(result_r, Gaussian_var, threshold_h, threshold_l, origin_r, width, height);
	
	//將經過Canny處理後的影像複製，並同時計算有多少個邊緣點 
	binary_img = new int* [width];
	binary_img[0] = new int [width * height];
	for(i = 1; i < width; i++)
		binary_img[i] = binary_img[i - 1] + height;
		
	for(i = 0, feature_point = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			binary_img[i][j] = result_r[i][j];
			if(binary_img[i][j] == 255) 
				feature_point++;
		}
	}
	
	//建立表格 
	List = new int* [feature_point];
	List[0] = new int [feature_point * angle_interval];
	for(i = 1; i < feature_point; i++)
		List[i] = List[i - 1] + angle_interval;
	
	//填寫表格 
	for(i = 0, m = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(binary_img[i][j] == 255)
			{
				for(n = 0; n < angle_interval; n++)
				{
					List[m][n] = int( (i * cos(n * PI / 180) + j * sin(n * PI / 180)) );
				}
				m++;
			}
		}
	}
	
	//找出哪一個s和theta的組合是出現最多次
		//資料排序
	for(n = 0; n < angle_interval; n++)
	{
		for(i = feature_point - 1; i > 0; i--)
		{
			for(j = 1; j <= i; j++)
			{
				if(List[j][n] < List[j - 1][n])
				{
					Reg = List[j][n];
					List[j][n] = List[j - 1][n];
					List[j - 1][n] = Reg;
				}
			}
		}
	}
		//整理並計算s和theta的組合出現次數,並只記錄超過指定次數的組合(s,theta)  
	S_angle *Rank;
	int combination_amount;
	Rank = new S_angle [feature_point * angle_interval];
	
	for(n = 0, combination_amount = 0; n < angle_interval; n++)
	{
		count = 1;
		Reg = List[0][n];
		for(i = 1; i < feature_point; i++)
		{
			if(List[i][n] == Reg) 
				count++;
			else
			{
				if(count > threshold_count)
				{
					Rank[combination_amount].count = count;
					Rank[combination_amount].S = Reg;
					Rank[combination_amount].angle = n;
					combination_amount++;
				}
				count = 1;
				Reg = List[i][n];
			}
		}
		if(count > threshold_count)
		{
			Rank[combination_amount].count = count;
			Rank[combination_amount].S = Reg;
			Rank[combination_amount].angle = n;
			combination_amount++;
		}
	}
	delete [] List[0];
	delete [] List;

	//找邊緣圖的像素點有沒有符合公式 
	int flag_reserved;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(binary_img[i][j] == 255)
			{
				flag_reserved = 0;
				for(n = 0; n < combination_amount; n++)
				{
					if(fabs(Rank[n].S-int( (i * cos(Rank[n].angle * PI / 180) + j * sin(Rank[n].angle * PI / 180)))) < 1e-6)
						flag_reserved = 1;
				}
				if(flag_reserved == 0)
					binary_img[i][j] = 0;
			}
		}
	}
	
	//把原圖複製到結果圖上 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			result_r[i][j] = origin_r[i][j];
			result_g[i][j] = origin_g[i][j];
			result_b[i][j] = origin_b[i][j];
		}
	}
	//將標記的線標記在結果圖上 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			if(binary_img[i][j] == 255)
			{
				result_r[i][j] = 0;
				result_g[i][j] = 255;
				result_b[i][j] = 0;
			}
		}
	}
	delete [] binary_img[0];
	delete [] binary_img;
	delete [] Rank;
	
}
