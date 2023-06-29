

void LackMemory()
{
	std::cerr << "°O¾ÐÅé¤£¨¬" << std::endl;
	abort();
}

float Determinant(float** matrix, float size)
{
    float s=1,det=0,**m_minor;
    int i,j,m,n,c;
    
    m_minor = new float* [int(size)];
	m_minor[0] = new float [int(size*size)];
	if(int(size) > 1)
		for(int i = 1; i < int(size); i++)
			m_minor[i] = m_minor[i - 1] + int(size);
    
    if (size==1)
    {
        return (matrix[0][0]);
    }
    else
    {
        det=0;
        for (c=0;c<size;c++)
        {
            m=0;
            n=0;
            for (i=0;i<size;i++)
            {
                for (j=0;j<size;j++)
                {
                    m_minor[i][j]=0;
                    if (i != 0 && j != c)
                    {
                       m_minor[m][n]=matrix[i][j];
                       if (n<(size-2))
                          n++;
                       else
                       {
                           n=0;
                           m++;
                       }
                    }
                }
            }
            det=det + s * (matrix[0][c] * Determinant(m_minor,size-1));
            s=-1 * s;
        }
    }
    delete [] m_minor[0];
    delete [] m_minor;
    return (det);
}

void cofactor(float **matrix, float size)
{
     float **m_cofactor, **matrix_cofactor, **m_transpose;
     int p,q,m,n,i,j;
     float d;
     
     m_cofactor = new float* [int(size)];
	 m_cofactor[0] = new float [int(size*size)];
	 if(int(size) > 1)
	 	for(int i = 1; i < int(size); i++)
		 	m_cofactor[i] = m_cofactor[i - 1] + int(size);
		 	
	 matrix_cofactor = new float* [int(size)];
	 matrix_cofactor[0] = new float [int(size*size)];
	 if(int(size) > 1)
	 	for(int i = 1; i < int(size); i++)
		 	matrix_cofactor[i] = matrix_cofactor[i - 1] + int(size);
		 	
     m_transpose = new float* [int(size)];
	 m_transpose[0] = new float [int(size*size)];
	 if(int(size) > 1)
	 	for(int i = 1; i < int(size); i++)
		 	m_transpose[i] = m_transpose[i - 1] + int(size);
		 	
     for (q=0;q<size;q++)
     {
         for (p=0;p<size;p++)
         {
             m=0;
             n=0;
             for (i=0;i<size;i++)
             {
                 for (j=0;j<size;j++)
                 {
                     if (i != q && j != p)
                     {
                        m_cofactor[m][n]=matrix[i][j];
                        if (n<(size-2))
                           n++;
                        else
                        {
                            n=0;
                            m++;
                        }
                     }
                 }
             }
             matrix_cofactor[q][p]=pow(-1,q + p) * Determinant(m_cofactor,size-1);
         }
     }
     for (i=0;i<size;i++)
     {
         for (j=0;j<size;j++)
         {
             m_transpose[i][j]=matrix_cofactor[j][i];
         }
     }
     d=Determinant(matrix, size);
     for (i=0;i<size;i++)
     {
         for (j=0;j<size;j++)
         {
             matrix[i][j]=m_transpose[i][j] / d;
         }
     }
     delete [] m_transpose[0];
	 delete [] m_transpose;
	 delete [] matrix_cofactor[0];
	 delete [] matrix_cofactor;
	 delete [] m_cofactor[0];
	 delete [] m_cofactor;
}


