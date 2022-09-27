#include <stdio.h>

#include <math.h>

#include <iostream>

#include "DCT.h"

#pragma warning(disable : 4996)；


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

   double Y[width * height] = {0};

   double Cr[width * height] = {0};

   double Cb[width * height] = {0};

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

   for (int i = 0; i < width * height; i++)

   {

      Y[i] = 0.299 * red[i] + 0.587 * green[i] + 0.114 * blue[i];

      Cr[i] = 0.5 * red[i] - 0.418 * green[i] - 0.0813 * blue[i] + 128;

      Cb[i] = -0.1687 * red[i] - 0.3313 * green[i] + 0.5 * blue[i] + 128;

   } //转换YCrCb--下面先写Y通道的变换
   


   double matrixY[256][256];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         matrixY[i][j] = Y[256 * i + j] - 128; // Y数据的矩阵
      }
   }

   double dmatrixY[32 * 32][8][8];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         dmatrixY[32 * (i / 8) + j / 8][i % 8][j % 8] = matrixY[i][j]; //把Y数据分成32*32个小块
      }
   }
   
   double temp[8][8];

   for (int k = 0; k < 32 * 32; k++)

   {

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            temp[i][j] = dmatrixY[k][i][j];              
            

         }
      }

      DCT(temp);

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            dmatrixY[k][i][j] = temp[i][j];                // Y通道的离散余弦变换完成啦！
            

         }
      }

   }


   double matrixCb[256][256];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         matrixCb[i][j] = Cb[256 * i + j] - 128; // Cb数据的矩阵
      }
   }

   double dmatrixCb[32 * 32][8][8];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         dmatrixCb[32 * (i / 8) + j / 8][i % 8][j % 8] = matrixCb[i][j]; //把Cb数据分成32*32个小块
      }
   }
   

   for (int k = 0; k < 32 * 32; k++)

   {

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            temp[i][j] = dmatrixCb[k][i][j];              
            

         }
      }

      DCT(temp);

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            dmatrixCb[k][i][j] = temp[i][j];                // Cb通道的离散余弦变换完成啦！
            

         }
      }

   }


   double matrixCr[256][256];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         matrixCr[i][j] = Cr[256 * i + j] - 128; // Cr数据的矩阵
      }
   }

   double dmatrixCr[32 * 32][8][8];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         dmatrixCr[32 * (i / 8) + j / 8][i % 8][j % 8] = matrixCr[i][j]; //把Cr数据分成32*32个小块
      }
   }
   

   for (int k = 0; k < 32 * 32; k++)

   {

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            temp[i][j] = dmatrixCr[k][i][j];              
            

         }
      }

      DCT(temp);

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            dmatrixCr[k][i][j] = temp[i][j];                // Cr通道的离散余弦变换完成啦！
            

         }
      }

   }
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
//导入量化矩阵

      short QdmatrixY[32*32][8][8];

      for (int k = 0; k < 32*32; k++)

      {

         for (int i = 0; i < 8; i++)

         {

            for (int j = 0; j < 8; j++)

            {

               QdmatrixY[k][i][j] = dmatrixY[k][i][j] / QY[i][j];

            }

         }

      }
//dmatrixY矩阵量化处理完成！

      short QdmatrixCr[32*32][8][8];

      for (int k = 0; k < 32*32; k++)

      {

         for (int i = 0; i < 8; i++)

         {

            for (int j = 0; j < 8; j++)

            {

               QdmatrixCr[k][i][j] = dmatrixCr[k][i][j] / QC[i][j];

            }

         }

      }
//dmatrixCr矩阵量化处理完成！

      short QdmatrixCb[32*32][8][8];

      for (int k = 0; k < 32*32; k++)

      {

         for (int i = 0; i < 8; i++)

         {

            for (int j = 0; j < 8; j++)

            {

               QdmatrixCb[k][i][j] = dmatrixCb[k][i][j] / QC[i][j];

            }

         }

      }
//dmatrixCb矩阵量化处理完成！
//量化处理完成！

   double ZY[32 * 32][64];

   for (int k = 0; k < 32*32; k++)

   {

      short rowY = 0, columnY = 1;

      ZY[k][0] = QdmatrixY[k][0][0];

      for (int i = 1; i <= 63; i++)

      {

         ZY[k][i] = QdmatrixY[k][rowY][columnY];

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
//量化处理后的Y数据矩阵ZigZag转换完成！

   double ZCr[32 * 32][64];

   for (int k = 0; k < 32*32; k++)

   {

      short rowCr = 0, columnCr = 1;

      ZCr[k][0] = QdmatrixCr[k][0][0];

      for (int i = 1; i <= 63; i++)

      {

         ZCr[k][i] = QdmatrixCr[k][rowCr][columnCr];

         if ((rowCr + columnCr)%2 == 1)

         {

            rowCr ++;

            if (columnCr != 0)

            {

               columnCr --;

            }

         }

         if ((rowCr + columnCr)%2 == 0)

         {

            columnCr ++;

            if (rowCr != 0)

            {

               rowCr --;

            }

         }

      }

   }
//量化处理后的Cr数据矩阵ZigZag转换完成！

   double ZCb[32 * 32][64];

   for (int k = 0; k < 32*32; k++)

   {

      short rowCb = 0, columnCb = 1;

      ZCb[k][0] = QdmatrixCb[k][0][0];

      for (int i = 1; i <= 64; i++)

      {

         ZCb[k][i] = QdmatrixCb[k][rowCb][columnCb];

         if ((rowCb + columnCb)%2 == 1)

         {

            rowCb ++;

            if (columnCb != 0)

            {

               columnCb --;

            }

         }

         if ((rowCb + columnCb)%2 == 0)

         {

            columnCb ++;

            if (rowCb != 0)

            {

               rowCb --;

            }

         }

      }

   }
//量化处理后的Cb数据矩阵ZigZag转换完成！
//所有数据ZigZag转换完成！

   for (short k = 1; k <= 32*32-1; k++)

   {

      ZY[k][1] -=ZY[k-1][1];//对一维矩阵ZY的直流系数之间做差分

   }

   for (short k = 1; k <= 32*32-1; k++)

   {

      ZCr[k][1] -=ZCr[k-1][1];//对一维矩阵ZCr做差分

   }

   for (short k = 1; k <= 32*32-1; k++)

   {

      ZCb[k][1] -=ZCb[k-1][1];//对一维矩阵ZCb做差分

   }
//差分处理完成

   for (int i =0; i < 64; i++)

   {

      cout << ZCr[2][i] << '\t';

   }


/*
   for (int u = 0; u < NUM; u++)

   {

      for (int v = 0; v < NUM; v++)

      {

         cout << QdmatrixY[1][u][v] << '\t';                              //这里是测试程序！
      }

      cout << endl;
   }
*/
   /*
      int bt[256] = {0};
      int gt[256] = {0};
      int rt[256] = {0};

      for (int i = 0; i < 256; i++)

      {

         for (int j = 0; j < width * height; j++)

         {

            if (i == blue[j])

               bt[i]++;

            if (i == green[j])

               gt[i]++;

            if (i == red[j])

               rt[i]++;
         }

      } //统计RGB值出现的次数

      double freqr[256] = {0};

      double freqg[256] = {0};

      double freqb[256] = {0};

      for (int i = 0; i < 256; i++)

      {

         freqr[i] = double(rt[i]) / (width * height);

         freqg[i] = double(gt[i]) / (width * height);

         freqb[i] = double(bt[i]) / (width * height);

      } //计算出现的频率

      double hr = 0;
      double hg = 0;
      double hb = 0;

      for (int i = 0; i < 256; i++)

      {

         if (freqr[i] != 0)

            hr = hr - freqr[i] * log(freqr[i]) / log(2);

         if (freqg[i] != 0)

            hg = hg - freqg[i] * log(freqg[i]) / log(2);

         if (freqb[i] != 0)

            hb = hb - freqb[i] * log(freqb[i]) / log(2);

      } //计算RGB的熵

      FILE *RF;
      FILE *GF;
      FILE *BF;

      if ((RF = fopen("red.txt", "w")) == NULL)

         printf("fail\n");

      else

         printf("red success\n");

      if ((GF = fopen("green.txt", "w")) == NULL)

         printf("fail\n");

      else

         printf("green success\n");

      if ((BF = fopen("blue.txt", "w")) == NULL)

         printf("fail\n");

      else

         printf("blue success\n"); //以“w”方式创建TXT文件

      char s[] = "symbol\tfrequency\n";

      fprintf(RF, s);
      fprintf(GF, s);
      fprintf(BF, s);

      for (int i = 0; i < 256; i++)

      {

         fprintf(BF, "%d\t%f\n", i, freqb[i]);

         fprintf(GF, "%d\t%f\n", i, freqg[i]);

         fprintf(RF, "%d\t%f\n", i, freqr[i]);
      }

      fprintf(BF, "entropy = %f", hb);

      fprintf(GF, "entropy = %f", hg);

      fprintf(RF, "entropy = %f", hr); //将数据写入TXT文件

      fclose(fp);

      fclose(BF);

      fclose(GF);

      fclose(RF);
   */
   return 0;
}