#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmp.cpp" 

void open_bmp(char filename[], int bmp_r[MaxBMPSizeX][MaxBMPSizeY], int bmp_g[MaxBMPSizeX][MaxBMPSizeY], int bmp_b[MaxBMPSizeX][MaxBMPSizeY], int& width, int& height);
void save_bmp(char filename[], int bmp_r[MaxBMPSizeX][MaxBMPSizeY], int bmp_g[MaxBMPSizeX][MaxBMPSizeY], int bmp_b[MaxBMPSizeX][MaxBMPSizeY], int& width, int& height);
void close_bmp();

