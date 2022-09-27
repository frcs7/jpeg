#include <stdio.h>

#include "DCT.h"

void DCT_new(double X[256 * 256])

{


   double matrixX[256][256];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         matrixX[i][j] = X[256 * i + j] - 128; // Y数据的矩阵
      }
   }

   double dmatrixX[32 * 32][8][8];

   for (int i = 0; i < 256; i++)

   {

      for (int j = 0; j < 256; j++)

      {

         dmatrixX[32 * (i / 8) + j / 8][i % 8][j % 8] = matrixX[i][j]; //把Y数据分成32*32个小块
      }
   }
   
   double temp[8][8];

   for (int k = 0; k < 32 * 32; k++)

   {

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            temp[i][j] = dmatrixX[k][i][j];              
            

         }
      }

      DCT(temp);

      for (int i = 0; i < 8; i++)

      {

         for (int j = 0; j < 8; j++)

         {                                                     //mark
         
            dmatrixX[k][i][j] = temp[i][j];                // Y通道的离散余弦变换完成啦！
            

         }
      }

   }
}