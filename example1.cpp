#include <stdio.h>

#include <math.h>

#include <iostream>

#include <vector>

#include <string.h>

#include <map>

#include "DCT.h"

using namespace std;

#pragma warning(disable : 4996)；


short codelength(int a) //a=ZY[k][i]  这是把游程编码的第二位变成编码长度

{

    short k=0;

    do

    {
        k++;
    } while (fabs(a) >= pow(2, k));

    return k;

}

string binary(int a)//这是转换成游程编码所需的伪二进制

{

    int n;

    short m = codelength(n);
    
    if(a < 0)

    {

      n = a + pow(2, m);

    }

    if(a > 0)

    {

      n = pow(2, m) - a;

    }

    string r;

    while(n != 0)

    {

        r = (n%2 == 0 ? "0" : "1") + r;

        n = n/2;

    }

    while (r.length() < m)

    {

      r = "0" + r;

    }
    

    return r;

}

void tomatrix(double *X, double (*matrixX)[256])//把Y通道数据写成矩阵

{

       for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         matrixX[i][j] = X[256 * i + j] - 128; // Y数据的矩阵

      }

   }

}

void dividematrix(double (*matrixX)[256], double (*dmatrixX)[8][8])

{

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         dmatrixX[32 * (i / 8) + j / 8][i % 8][j % 8] = matrixX[i][j]; //把Y数据分成32*32个小块

      }

   }

}

void DCTgeneral(double (*dmatrixX)[8][8])//对8*8矩阵做DCT变换

{

    double temp[8][8];

   for (int k = 0; k < 32 * 32; k++)

   {

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     
         
            temp[i][j] = dmatrixX[k][i][j];              
            

         }

      }

      DCT(temp);

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     
         
            dmatrixX[k][i][j] = temp[i][j];    
            
         }

      }

   }

}

void quantize_matrix(double (*dmatrixX)[8][8], short (*QdmatrixX)[8][8], short (*QX)[8])

{

          for (int k = 0; k < 32*32; k++)

      {

         for (int i = 0; i < 8; i++)

         {

            for (int j = 0; j < 8; j++)

            {

               QdmatrixX[k][i][j] = dmatrixX[k][i][j] / QX[i][j];

            }

         }

      }

}

void zigzagX(double (*ZX)[64], short (*QdmatrixX)[8][8])

{

    for (int k = 0; k < 32*32; k++)

   {

      short rowY = 0, columnY = 1;

      ZX[k][0] = QdmatrixX[k][0][0];

      for (int i = 1; i <= 63; i++)

      {

         ZX[k][i] = QdmatrixX[k][rowY][columnY];

         if ((rowY + columnY)%2 == 1)

         {

            rowY ++;

            if (columnY != 0)

            {

               columnY --;

            }

         }

         if ((rowY + columnY)%2 == 0)

         {

            columnY ++;

            if (rowY != 0)

            {

               rowY --;

            }

         }

      }

   }

}


int main()

{

   const int width = 256;
   const int height = 256;
   int b = 0;
   int g = 0;
   int r = 0;

   unsigned char buffer[3 * width * height];

   unsigned char red[width * height] = {0};

   unsigned char green[width * height] = {0};

   unsigned char blue[width * height] = {0};

   FILE *fp;

   fp = fopen("demo.bin", "rb");

   if (fp == NULL)

      printf("臭猪你打开了个啥\n");

   else

      printf("File opened!\n"); //打开文件

   fread(buffer, sizeof(unsigned char), 3 * width * height, fp);

   for (int i = 0; i < 3 * width * height; i++)

   {

      if (i % 3 == 0)

      {

         red[r] = buffer[i];

         r++;
      }

      else if (i % 3 == 1)

      {

         green[g] = buffer[i];

         g++;
      }

      else if (i % 3 == 2)

      {

         blue[b] = buffer[i];

         b++;
      }

   } //将RGB的值分别存放在单独的数组中

   double Y[width * height] = {0};

   double Cr[width * height] = {0};

   double Cb[width * height] = {0};

   for (int i = 0; i < width * height; i++)

   {

      Y[i] = 0.299 * red[i] + 0.587 * green[i] + 0.114 * blue[i];

      Cr[i] = 0.5 * red[i] - 0.418 * green[i] - 0.0813 * blue[i] + 128;

      Cb[i] = -0.1687 * red[i] - 0.3313 * green[i] + 0.5 * blue[i] + 128;

   } //转换YCrCb--下面先写Y通道的变换
   


   double matrixY[256][256];

   tomatrix(Y,matrixY);//把Y通道数据写成矩阵

   double dmatrixY[32 * 32][8][8];

   dividematrix(matrixY,dmatrixY);//把Y矩阵分成32*32个小块
   
   DCTgeneral(dmatrixY);//Y的离散余弦变换完成


   double matrixCb[256][256];

   tomatrix(Cb,matrixCb);

   double dmatrixCb[32 * 32][8][8];

   dividematrix(matrixCb,dmatrixCb);
   
   DCTgeneral(dmatrixCb);//Cb的离散余弦变换完成

   double matrixCr[256][256];

   tomatrix(Cr,matrixCr);

   double dmatrixCr[32 * 32][8][8];

   dividematrix(matrixCr,dmatrixCr);
   
   DCTgeneral(dmatrixCr);//Cr的离散余弦变换完成

//三个通道的离散余弦变换全部完成！

      short QY[8][8] =

      {

         {16,11,10,16,24,40,51,61},
         {12,12,14,19,26,58,60,55},
         {14,13,16,24,40,57,69,56},
         {14,17,22,29,51,87,80,62},
         {18,22,37,56,68,109,103,77},
         {24,35,55,64,81,104,113,92},
         {49,64,78,87,103,121,120,101},
         {72,92,95,98,112,100,103,99}

      };

      short QC[8][8] =

      {

         {17,18,24,47,99,99,99,99},
         {18,21,26,66,99,99,99,99},
         {24,26,56,99,99,99,99,99},
         {47,66,99,99,99,99,99,99},
         {99,99,99,99,99,99,99,99},
         {99,99,99,99,99,99,99,99},
         {99,99,99,99,99,99,99,99},
         {99,99,99,99,99,99,99,99}

      };

//导入量化矩阵void quantize_divided_matrix(double (*dmatrixX)[8][8], double (*QmatrixX)[8][8])

      short QdmatrixY[32*32][8][8];

      quantize_matrix(dmatrixY, QdmatrixY, QY);//dmatrixY矩阵量化处理完成！

      short QdmatrixCr[32*32][8][8];

      quantize_matrix(dmatrixCr, QdmatrixCr, QC);//dmatrixCr矩阵量化处理完成！

      short QdmatrixCb[32*32][8][8];

      quantize_matrix(dmatrixCb, QdmatrixCb, QC);//dmatrixCb矩阵量化处理完成！

//量化处理完成！void zigzagX(double (*ZY)[64], short (*Qdmatrix)[8][8])

   double ZY[32 * 32][64];

   zigzagX(ZY, QdmatrixY);//量化处理后的Y数据矩阵ZigZag转换完成！

   double ZCr[32 * 32][64];

   zigzagX(ZCr, QdmatrixCr);//量化处理后的Cr数据矩阵ZigZag转换完成！

   double ZCb[32 * 32][64];

   zigzagX(ZCb, QdmatrixCb);//量化处理后的Cb数据矩阵ZigZag转换完成！

//所有数据ZigZag转换完成！

   for (short k = 0; k < 32*32-1; k++)

   {

      ZY[32*32-1 - k][0] -=ZY[32*32-2 - k][0];//对一维矩阵ZY的直流系数之间做差分

   }

   for (short k = 0; k < 32*32-1; k++)

   {

      ZCr[32*32-1 - k][0] -=ZCr[32*32-2 - k][0];//对一维矩阵ZCr做差分

   }

   for (short k = 0; k < 32*32-1; k++)

   {

      ZCb[32*32-1 - k][0] -=ZCb[32*32-2 - k][0];//对一维矩阵ZCb做差分

   }
   
//差分处理完成

   vector<vector<short> > YRLE;

   vector<short> YRLEtemp;

   map<short, vector<vector<short> > > YRLEmap;//第一个short只是index

   for(short k = 0; k < 32*32; k++)

   {

      short counter = 0;

      YRLEtemp.push_back(ZY[k][0]);

      YRLE.push_back(YRLEtemp);

      YRLEtemp.clear();

      for(short i =1 ; i < 64; i++) //i=1时做特殊处理，因为是直流系数

      {

         if (ZY[k][i] == 0)

         {

            counter++;

         }

         else

         {

            YRLEtemp.push_back(counter);

            YRLEtemp.push_back(codelength(ZY[k][i]));

            YRLE.push_back(YRLEtemp);

            YRLEtemp.clear();

            counter = 0;

         }

      }

      YRLEmap.insert(pair<short, vector<vector<short> > >(k, YRLE));

   }//Y通道游程编码完成

   vector<vector<short> > CrRLE;

   vector<short> CrRLEtemp;

   map<short, vector<vector<short> > > CrRLEmap;//第一个short只是index

   for(short k = 0; k < 32*32; k++)

   {

      short counter = 0;

      CrRLEtemp.push_back(ZCr[k][0]);

      CrRLE.push_back(CrRLEtemp);

      CrRLEtemp.clear();

      for(short i =1 ; i < 64; i++) //i=1时做特殊处理，因为是直流系数

      {

         if (ZCr[k][i] == 0)

         {

            counter++;

         }

         else

         {

            CrRLEtemp.push_back(counter);

            CrRLEtemp.push_back(codelength(ZCr[k][i]));

            CrRLE.push_back(CrRLEtemp);

            CrRLEtemp.clear();

            counter = 0;

         }

      }

      CrRLEmap.insert(pair<short, vector<vector<short> > >(k, CrRLE));

   }//Cr通道游程编码完成



   vector<vector<short> > CbRLE;

   vector<short> CbRLEtemp;

   map<short, vector<vector<short> > > CbRLEmap;//第一个short只是index

   for(short k = 0; k < 32*32; k++)

   {

      short counter = 0;

      CbRLEtemp.push_back(ZCb[k][0]);

      CbRLE.push_back(CbRLEtemp);

      CbRLEtemp.clear();

      for(short i =1 ; i < 64; i++) //i=1时做特殊处理，因为是直流系数

      {

         if (ZCb[k][i] == 0)

         {

            counter++;

         }

         else

         {

            CbRLEtemp.push_back(counter);

            CbRLEtemp.push_back(codelength(ZCb[k][i]));

            CbRLE.push_back(CbRLEtemp);

            CbRLEtemp.clear();

            counter = 0;

         }

      }

      CbRLEmap.insert(pair<short, vector<vector<short> > >(k, CbRLE));

   }   //Cb通道游程编码完成
//游程编码全部完成，事实上所需的转换也已经完成，只需要用binary和codelength函数把RLE中的需变换的数字变换之后就可以了

   return 0;
}