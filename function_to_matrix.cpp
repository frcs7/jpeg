#include <stdio.h>

#include <math.h>

#include <iostream>

#include <vector>

#include <string.h>

#include <map>

#include "DCT.h"

using namespace std;

#pragma warning(disable : 4996)；

double tomatrix(double X[32 * 32][256][256 * 256]) //这是把Y通道数据变成矩阵
{

    // double matrixX[256][256];

    double OUTPUT[32 * 32][256][256];

    for (int k = 0; k < 32 * 32; k++)

        for (int i = 0; i < 256; i++)

        {

            for (int j = 0; j < 256; j++)

            {

                OUTPUT[k][i][j] = X[k][i][256 * i + j] - 128; // Y数据的矩阵
            }
        }

    memset(X, 0, 32 * 32 * 256 * 256 * 256 * sizeof(double));

    ::memcpy(X, OUTPUT, 32 * 32 * 256 * 256 * 256 * sizeof(double));
    
}

short codelength(int a) // a=ZY[k][i]  这是把游程编码的第二位变成编码长度

{

    short k = 0;

    do

    {
        k++;
    } while (a >= pow(2, k));

    return k;
}

string binary(int n) //这是转换成游程编码所需的伪二进制

{

    short m = codelength(n);

    string r;

    while (n != 0)

    {

        r = (n % 2 == 0 ? "0" : "1") + r;

        n = n / 2;
    }

    while (r.length() < m)
    {
        r = "0" + r;
    }

    return r;
}

int main()

{

    int b = 0;
    int g = 0;
    int r = 0;

    unsigned char buffer[3 * 256 * 256];

    unsigned char red[256 * 256] = {0};

    unsigned char green[256 * 256] = {0};

    unsigned char blue[256 * 256] = {0};

    FILE *fp;

    fp = fopen("demo.bin", "rb");

    if (fp == NULL)

        printf("臭猪你打开了个啥\n");

    else

        printf("File opened!\n"); //打开文件

    fread(buffer, sizeof(unsigned char), 3 * 256 * 256, fp);

    for (int i = 0; i < 3 * 256 * 256; i++)

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

    double Y[32 * 32][256][256 * 256] = {0};

    double Cr[32 * 32][256][256 * 256] = {0};

    double Cb[32 * 32][256][256 * 256] = {0};

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int j = 0; j < 256; j++)

        {

            for (int i = 0; i < 256 * 256; i++)

            {

                Y[k][j][i] = 0.299 * red[i] + 0.587 * green[i] + 0.114 * blue[i];

                Cr[k][j][i] = 0.5 * red[i] - 0.418 * green[i] - 0.0813 * blue[i] + 128;

                Cb[k][j][i] = -0.1687 * red[i] - 0.3313 * green[i] + 0.5 * blue[i] + 128;

            } //转换YCrCb--下面先写Y通道的变换
        }
    }

tomatrix(Y);

    double dmatrixY[32 * 32][8][8];

    for (int i = 0; i < 256; i++)

    {

        for (int j = 0; j < 256; j++)

        {

            dmatrixY[32 * (i / 8) + j / 8][i % 8][j % 8] = Y[1][i][j]; //把Y数据分成32*32个小块
        }
    }

    double temp[8][8];

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                temp[i][j] = dmatrixY[k][i][j];
            }
        }

        DCT(temp);

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                dmatrixY[k][i][j] = temp[i][j]; // Y通道的离散余弦变换完成啦！
            }
        }
    }











tomatrix(Cb);

    double dmatrixCb[32 * 32][8][8];

    for (int i = 0; i < 256; i++)

    {

        for (int j = 0; j < 256; j++)

        {

            dmatrixCb[32 * (i / 8) + j / 8][i % 8][j % 8] = Cb[1][i][j]; //把Cb数据分成32*32个小块
        }
    }

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                temp[i][j] = dmatrixCb[k][i][j];
            }
        }

        DCT(temp);

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                dmatrixCb[k][i][j] = temp[i][j]; // Cb通道的离散余弦变换完成啦！
            }
        }
    }

tomatrix(Cr);

    double dmatrixCr[32 * 32][8][8];

    for (int i = 0; i < 256; i++)

    {

        for (int j = 0; j < 256; j++)

        {

            dmatrixCr[32 * (i / 8) + j / 8][i % 8][j % 8] = Cr[1][i][j]; //把Cr数据分成32*32个小块
        }
    }

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                temp[i][j] = dmatrixCr[k][i][j];
            }
        }

        DCT(temp);

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            { // mark

                dmatrixCr[k][i][j] = temp[i][j]; // Cr通道的离散余弦变换完成啦！
            }
        }
    }
    //三个通道的离散余弦变换全部完成！

    short QY[8][8] =

        {

            {16, 11, 10, 16, 24, 40, 51, 61},
            {12, 12, 14, 19, 26, 58, 60, 55},
            {14, 13, 16, 24, 40, 57, 69, 56},
            {14, 17, 22, 29, 51, 87, 80, 62},
            {18, 22, 37, 56, 68, 109, 103, 77},
            {24, 35, 55, 64, 81, 104, 113, 92},
            {49, 64, 78, 87, 103, 121, 120, 101},
            {72, 92, 95, 98, 112, 100, 103, 99}

        };

    short QC[8][8] =

        {

            {17, 18, 24, 47, 99, 99, 99, 99},
            {18, 21, 26, 66, 99, 99, 99, 99},
            {24, 26, 56, 99, 99, 99, 99, 99},
            {47, 66, 99, 99, 99, 99, 99, 99},
            {99, 99, 99, 99, 99, 99, 99, 99},
            {99, 99, 99, 99, 99, 99, 99, 99},
            {99, 99, 99, 99, 99, 99, 99, 99},
            {99, 99, 99, 99, 99, 99, 99, 99}

        };
    //导入量化矩阵

    short QdmatrixY[32 * 32][8][8];

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            {

                QdmatrixY[k][i][j] = dmatrixY[k][i][j] / QY[i][j];
            }
        }
    }
    // dmatrixY矩阵量化处理完成！

    short QdmatrixCr[32 * 32][8][8];

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            {

                QdmatrixCr[k][i][j] = dmatrixCr[k][i][j] / QC[i][j];
            }
        }
    }
    // dmatrixCr矩阵量化处理完成！

    short QdmatrixCb[32 * 32][8][8];

    for (int k = 0; k < 32 * 32; k++)

    {

        for (int i = 0; i < 8; i++)

        {

            for (int j = 0; j < 8; j++)

            {

                QdmatrixCb[k][i][j] = dmatrixCb[k][i][j] / QC[i][j];
            }
        }
    }
    // dmatrixCb矩阵量化处理完成！
    //量化处理完成！

    double ZY[32 * 32][64];

    for (int k = 0; k < 32 * 32; k++)

    {

        short rowY = 0, columnY = 1;

        ZY[k][0] = QdmatrixY[k][0][0];

        for (int i = 1; i <= 63; i++)

        {

            ZY[k][i] = QdmatrixY[k][rowY][columnY];

            if ((rowY + columnY) % 2 == 1)

            {

                rowY++;

                if (columnY != 0)

                {

                    columnY--;
                }
            }

            if ((rowY + columnY) % 2 == 0)

            {

                columnY++;

                if (rowY != 0)

                {

                    rowY--;
                }
            }
        }
    }
    //量化处理后的Y数据矩阵ZigZag转换完成！

    double ZCr[32 * 32][64];

    for (int k = 0; k < 32 * 32; k++)

    {

        short rowCr = 0, columnCr = 1;

        ZCr[k][0] = QdmatrixCr[k][0][0];

        for (int i = 1; i <= 63; i++)

        {

            ZCr[k][i] = QdmatrixCr[k][rowCr][columnCr];

            if ((rowCr + columnCr) % 2 == 1)

            {

                rowCr++;

                if (columnCr != 0)

                {

                    columnCr--;
                }
            }

            if ((rowCr + columnCr) % 2 == 0)

            {

                columnCr++;

                if (rowCr != 0)

                {

                    rowCr--;
                }
            }
        }
    }
    //量化处理后的Cr数据矩阵ZigZag转换完成！

    double ZCb[32 * 32][64];

    for (int k = 0; k < 32 * 32; k++)

    {

        short rowCb = 0, columnCb = 1;

        ZCb[k][0] = QdmatrixCb[k][0][0];

        for (int i = 1; i <= 64; i++)

        {

            ZCb[k][i] = QdmatrixCb[k][rowCb][columnCb];

            if ((rowCb + columnCb) % 2 == 1)

            {

                rowCb++;

                if (columnCb != 0)

                {

                    columnCb--;
                }
            }

            if ((rowCb + columnCb) % 2 == 0)

            {

                columnCb++;

                if (rowCb != 0)

                {

                    rowCb--;
                }
            }
        }
    }
    //量化处理后的Cb数据矩阵ZigZag转换完成！
    //所有数据ZigZag转换完成！

    for (short k = 0; k < 32 * 32 - 1; k++)

    {

        ZY[32 * 32 - 1 - k][0] -= ZY[32 * 32 - 2 - k][0]; //对一维矩阵ZY的直流系数之间做差分
    }

    for (short k = 0; k < 32 * 32 - 1; k++)

    {

        ZCr[32 * 32 - 1 - k][0] -= ZCr[32 * 32 - 2 - k][0]; //对一维矩阵ZCr做差分
    }

    for (short k = 0; k < 32 * 32 - 1; k++)

    {

        ZCb[32 * 32 - 1 - k][0] -= ZCb[32 * 32 - 2 - k][0]; //对一维矩阵ZCb做差分
    }
    //差分处理完成

    /*
    short counter = 0;

    vector<vector<short> > RLE;

    vector<short> RLEtemp;

    for(short k = 0; k < 32*32; k++)

    {

       RLEtemp.push_back(ZY[k][0]);

       RLE.push_back(RLEtemp);

       RLEtemp.clear();

       for(short i =1 ; i < 64; i++) //i=1时做特殊处理，因为是直流系数

       {

          if (ZY[k][i] == 0)

          {

             counter++;

          }

          else

          {

             RLEtemp.push_back(counter);

             RLEtemp.push_back(codelength(ZY[k][i]));

             RLE.push_back(RLEtemp);

             RLEtemp.clear();

             counter = 0;

          }

       }

       map<short, vector<vector<short> > > RLEmap;

       RLEmap.insert(pair<short, vector<vector<short> > >(k, RLE));//游程编码完成，事实上所需的转换也已经完成，只需要用binary和codelength把RLE中的需变换的数字变换之后就可以了

    }
 */

    //下面是测试程序！
    cout << ZY[1][0] << endl;

    for (int i = 0; i < 64; i++)

    {

        cout << ZY[0][i] << '\t';
    }
    cout << "\n"
         << endl;
    std::cerr << clock() / double(CLOCKS_PER_SEC) << std::endl;

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

    return 0;
}