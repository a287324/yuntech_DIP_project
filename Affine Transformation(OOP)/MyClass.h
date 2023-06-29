#include <iostream>
#include <iomanip>
#include <new>
#include <cmath>
#include "MyClass.cpp"

enum Coordinate_symbol {Px = 0, Py};

void LackMemory(void);
float Determinant(float**, float);
void cofactor(float**, float);

template <class T>
class Coordinate
{
	private:
		T* coordinate;
	public:
		Coordinate();
		Coordinate(T, T);
		Coordinate(const Coordinate<T>&);
		~Coordinate();
		T& at(int);		//座標值 
		Coordinate<T>& operator = (const Coordinate<T>&); 
};

template<class T>
class Matrix
{
	private:
		int Row;
		int Col;
		T** matrix; 
	public:
		Matrix();
		Matrix(int, int);
		Matrix(const Matrix<T>&);
		~Matrix();
		T& at(int, int) const;	//矩陣元素值 
		void print(void) const;	//印出矩陣 
		double determinant(void) const; //矩陣的行列式計算 
		Matrix<T>& operator = (const Matrix<T>&);	
		Matrix<float> operator ! (void) const;	//矩陣的反矩陣
		Matrix<T> operator ~ (void) const;	//矩陣的轉置
		Matrix<T> operator * (const Matrix<T>&) const;	//矩陣的乘法 
};

//Coordinate
template <class T>
Coordinate<T>::Coordinate()
{
	this->coordinate = new T[2];
}

template <class T>
Coordinate<T>::Coordinate(T M, T N)
{
	this->coordinate = new T[2];
	this->coordinate[0] = M;
	this->coordinate[1] = N;
}

template <class T>
Coordinate<T>::Coordinate(const Coordinate<T>& Old)
{
	this->coordinate = new T[2];
	this->coordinate[0] = Old.coordinate[0];
	this->coordinate[1] = Old.coordinate[1];
}

template <class T>
Coordinate<T>::~Coordinate()
{
	delete [] this->coordinate;
}

template <class T>
T& Coordinate<T>::at(int M)
{
	return (this->coordinate[M]);
}

template <class T>
Coordinate<T>& Coordinate<T>::operator = (const Coordinate<T>& Old)
{
	coordinate[0] = Old.coordinate[0];
	coordinate[1] = Old.coordinate[1];
	return *this;
}

//Matrix
template <class T>
Matrix<T>::Matrix()
{
	this->Row = 1;
	this->Col = 1;
	this->matrix = new T* [2];
	this->matrix[0] = new T [2];
	this->matrix[1] = this->matrix[0];
}

template <class T>
Matrix<T>::Matrix(int M, int N)
{
	if(M <= 0 || N <= 0)
	{
		std::cerr << "ERROR: Matrix的行數或列數不可是零或負數" << std::endl;
		exit(1);
	}
	this->Row = M;
	this->Col = N;
	this->matrix = new T* [M + 1];
	this->matrix[0] = new T [M * N + 1];
	this->matrix[1] = this->matrix[0];
	if(this->Row > 1)
		for(int i = 2; i <= M; i++)
			this->matrix[i] = this->matrix[i - 1] + N;
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& M)
{
	this->Row = M.Row;
	this->Col = M.Col;
	this->matrix = new T* [M.Row + 1];
	this->matrix[0] = new T [M.Row * M.Col + 1];
	this->matrix[1] = this->matrix[0];
	if(this->Row > 1)
		for(int i = 2; i <= M.Row; i++)
			this->matrix[i] = this->matrix[i - 1] + M.Col;

	for(int i = 1; i <= M.Row; i++)
	{
		for(int j = 1; j <= M.Col; j++)
		{
			this->matrix[i][j] = M.matrix[i][j];
		}
	}
}

template <class T>
Matrix<T>::~Matrix()
{
	delete [] this->matrix[0];
	delete [] this->matrix;
}

template <class T>
T& Matrix<T>::at(int M, int N) const
{
	if(M < 1 || N < 1)
	{
		std::cerr << "ERROR: 矩陣無法取出有效值" << std::endl;
		exit(2);
	}
	return this->matrix[M][N];
}

template <class T>
void Matrix<T>::print(void) const
{
	std::cout << std::setw(6) << std::right << std::fixed << std::setprecision(2);
	for(int i = 1; i <= this->Row; i++)
	{
		for(int j = 1; j <= this->Col; j++)
		{
			std::cout << this->matrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}

template <class T>
double Matrix<T>::determinant(void) const
{
	float **Matrix_reg;
	if(this->Row != this->Col)
	{
		std::cerr << "此矩陣不是方形矩陣,沒有辦法計算行列式值" << std::endl;
		exit(4);
	}
	Matrix_reg = new float* [this->Row];
	Matrix_reg[0] = new float [this->Row * this->Col];
	if(this->Row > 1)
		for(int i = 1; i < this->Row; i++)
			Matrix_reg[i] = Matrix_reg[i - 1] + this->Col;
	for(int i = 0; i < this->Row; i++)
	{
		for(int j = 0; j < this->Col; j++)
		{
			Matrix_reg[i][j] = this->matrix[i+1][j+1];
		}
	}
	
	return Determinant(Matrix_reg, this->Row);
}

template <class T>
Matrix<T>& Matrix<T>::operator = (const Matrix<T>& M)
{
	delete [] matrix[0];
	delete [] matrix;
	
	this->Row = M.Row;
	this->Col = M.Col;
	this->matrix = new T* [M.Row + 1];
	this->matrix[0] = new T [M.Row * M.Col + 1];
	this->matrix[1] = this->matrix[0];
	if(this->Row > 1)
		for(int i = 2; i <= M.Row; i++)
			this->matrix[i] = this->matrix[i - 1] + M.Col;

	for(int i = 1; i <= M.Row; i++)
	{
		for(int j = 1; j <= M.Col; j++)
		{
			this->matrix[i][j] = M.matrix[i][j];
		}
	}
	return *this;
}

template <class T>
Matrix<float> Matrix<T>::operator ! (void) const	//反矩陣 
{
	Matrix<float> matrix_inv(this->Row, this->Col);
	float **Matrix_reg;
	int i, j;
	
	if(this->Row != this->Col)
	{
		std::cerr << "此矩陣不是方形矩陣,反矩陣不存在" << std::endl;
		exit(4);
	}
	Matrix_reg = new float* [this->Row];
	Matrix_reg[0] = new float [this->Row * this->Col];
	if(this->Row > 1)
		for(i = 1; i < this->Row; i++)
			Matrix_reg[i] = Matrix_reg[i - 1] + this->Col;
	for(i = 0; i < this->Row; i++)
	{
		for(j = 0; j < this->Col; j++)
		{
			Matrix_reg[i][j] = this->matrix[i+1][j+1];
		}
	}
	
	if(Determinant(Matrix_reg, this->Row) == 0)
	{
		std::cerr << "ERROR: 此矩陣行列式值為0,所以反矩陣不存在" << std::endl;
		exit(6);
	}
	
	cofactor(Matrix_reg, this->Row);
	for(i = 1; i <= this->Row; i++)
	{
		for(j = 1; j <= this->Col; j++)
		{
			matrix_inv.at(i, j) = Matrix_reg[i - 1][j - 1];
		}
	}
	
	delete [] Matrix_reg[0];
	delete [] Matrix_reg;
	return matrix_inv;
}

template<class T>
Matrix<T> Matrix<T>::operator ~ (void) const	//轉置 
{
	Matrix<T> Transpose_matrix;
	int i, j, Reg;
	
	Transpose_matrix.Row = this->Col;
	Transpose_matrix.Col = this->Row;
	
	Transpose_matrix.matrix = new T* [Transpose_matrix.Row + 1];
	Transpose_matrix.matrix[0] = new T [Transpose_matrix.Row * Transpose_matrix.Col + 1];
	Transpose_matrix.matrix[1] = Transpose_matrix.matrix[0];
	if(Transpose_matrix.Row > 1)
		for(int i = 2; i <= Transpose_matrix.Row; i++)
			Transpose_matrix.matrix[i] = Transpose_matrix.matrix[i - 1] + Transpose_matrix.Col;
			
	for(i = 1; i <= Transpose_matrix.Row; i++)
	{
		for(j = 1; j <= Transpose_matrix.Col; j++)
		{
			Transpose_matrix.at(i, j) = this->at(j, i);
		}
	}
	return Transpose_matrix;
}

template <class T> 
Matrix<T> Matrix<T>::operator * (const Matrix<T>& M) const
{
	if(this->Col != M.Row)
	{
		std::cerr << "ERROR: 矩陣乘法出錯!!" << std::endl;
		exit(3);
	}
	
	Matrix<T> Temp(this->Row, M.Col);
	int i, j, k;
	
	for(i = 1; i<=Temp.Row; i++)
	{
		for(j = 1; j<=Temp.Col; j++)
		{
			Temp.matrix[i][j] = 0;
			for(k = 1; k<=this->Col; k++)
			{
				Temp.matrix[i][j] = Temp.matrix[i][j] + (this->matrix[i][k] * M.matrix[k][j]);
			}
		}
	}
	return Temp;
}
