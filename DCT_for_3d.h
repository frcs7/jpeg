#ifndef DCT_H

#define DCT_H

#include <iostream>

#include <math.h>

using namespace std;

#define NUM 8

#define PI 3.1415926

/*

double round(double a)

{

         if (a >= 0)

         {

                   return (short)(a + 0.5);

         }

         else

         {

                   return (short)(a - 0.5);

         }

}

*/

// DCT

void DCT(double data[32 * 32][8][8])

{

    double output[32 * 32][NUM][NUM];

    double Cu, Cv;

    int u = 0;

    int v = 0;

    int i = 0;

    int j = 0;

    int k = 0;

    for ( u = 0; u < NUM; u++)

    {

        for ( v = 0; v < NUM; v++)

        {

            if (u == 0)

            {

                Cu = sqrt(1.0 / NUM);
            }

            else

            {

                Cu = sqrt(2.0 / NUM);
            }

            if (v == 0)

            {

                Cv = sqrt(1.0 / NUM);
            }

            else

            {

                Cv = sqrt(2.0 / NUM);
            }

            double tmp = 0.0;

            

                for ( i = 0; i < NUM; i++)

                {

                    for ( j = 0; j < NUM; j++)

                    {

                        tmp += data[k][i][j] * cos((2 * i + 1) * u * PI / (2.0 * NUM)) * cos((2 * j + 1) * v * PI / (2.0 * NUM));
                    }
                }
            
            output[k][u][v] = Cu * Cv * tmp;
            
        }
    }

     memset(data, 0, 32*32 * NUM * NUM * sizeof(double));

    ::memcpy(data, output, 32*32 * NUM * NUM * sizeof(double));
}

#endif