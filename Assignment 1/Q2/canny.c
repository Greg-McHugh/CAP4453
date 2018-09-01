/* Gregory McHugh
 * GR795710
 * CAP4453-17Fall 0001
 * Assignment 1 – Q2
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         int    edge[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double convx[PICSIZE][PICSIZE];
         double convy[PICSIZE][PICSIZE];
         double final[PICSIZE][PICSIZE];
         double ival[PICSIZE][PICSIZE];
         double histogram[PICSIZE];
         int    slope;

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr, low, high, centx,centy, sum;
        int uno = 1;
        double  maskvalx, maskvaly,sumx,sumy,sig,maxival, percent;
        FILE    *fo1, *fo2, *fo3, *fo4, *fp1, *fopen();
        char    *foobar;

        // garb34.pgm
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        // sig
        argc--; argv++;
      	foobar = *argv;
      	sig = atof(foobar);

        // percent
        argc--; argv++;
      	foobar = *argv;
      	percent = atof(foobar) / 100;


        // output 1
        fo2=fopen("output1.pgm","wb");

        // output 2
        fo3=fopen("output2.pgm","wb");

        // output 3
        fo4=fopen("output3.pgm","wb");



        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        // bypasses file header
        for(i = 0; i < 15; i++) getc(fp1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                }
        }

        // derivatives
        for (p=-mr;p<=mr;p++)
        {
          for (q=-mr;q<=mr;q++)
          {
            maskvalx = -(p*(exp(-(1/2)*(p/sig)*(p/sig))));
            maskvaly = -(q*(exp(-(1/2)*(q/sig)*(q/sig))));

            maskx[p+centy][q+centx] = maskvalx;
            masky[p+centy][q+centx] = maskvaly;

          }
        }

        // convolutes image
        for (i=0;i<=255;i++)
        { for (j=0;j<=255;j++)
          {
             sumx = 0;
             sumy = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sumx += pic[i+p][j+q] * maskx[p+centy][q+centx];
                   sumy += pic[i+p][j+q] * masky[p+centy][q+centx];
                }
             }
             convx[i][j] = sumx;
             convy[i][j] = sumy;
          }
        }

        // finds gradient
        maxival = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((convx[i][j]*convx[i][j]) + (convy[i][j]*convy[i][j])));

             if (ival[i][j] > maxival)
                maxival = ival[i][j];
           }
        }

        for (i=mr;i<256-mr;i++)
        {
          for (j=mr;j<256-mr;j++) histogram[(int)ival[i][j]]++;
        }

        for (i=mr;i<256-mr;i++)
        {
          for (j=mr;j<256-mr;j++) ival[i][j] = (ival[i][j] / maxival) * 260;
        }

        sum=0;
        for (i=255;i>=0;i--)
        {
            sum+=histogram[i];
            if (sum >= percent*PICSIZE*PICSIZE) break;
        }

        high = i;
        low = 0.35*high;

        fprintf(fo3,"P5\n");
        fprintf(fo3,"%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo3,"255\n");

        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++) fprintf(fo3,"%c",(char)((int)(ival[i][j])));
        }

        for(i = mr; i < 256-mr; i++)
        {
          for(j = mr; j < 256-mr; j++)
          {
            if((convx[i][j]) == 0.0) convx[i][j] = 0.00001;
            slope = convy[i][j] / convx[i][j];

            if((slope <= .4142)
               && (slope > -.4142)
               && (ival[i][j] > ival[i][j-1])
               && (ival[i][j] > ival[i][j+1]))
            {
                edge[i][j] = 255;
            }
            else if((slope <= 2.4142)
                    && (slope > .4142)
                    && (ival[i][j] > ival[i-1][j-1])
                    && (ival[i][j] >ival[i+1][j+1]))
            {
                edge[i][j] = 255;
            }
            else if((slope <= -.4142)
                    && (slope > -2.4142)
                    && (ival[i][j] > ival[i+1][j-1])
                    && (ival[i][j] > ival[i-1][j+1]))
            {
                edge[i][j] = 255;
            }
            else if((ival[i][j] > ival[i-1][j])
                    && (ival[i][j] > ival[i+1][j]))
            {
                edge[i][j] = 255;
            }
          }
        }


        fprintf(fo4,"P5\n");
        fprintf(fo4,"%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo4,"255\n");

        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++) fprintf(fo4,"%c",(char)((int)(edge[i][j])));
        }
                for(i=mr;i<256-mr; i++)
                { for(j=mr;j< 256-mr; j++)
                  {
                        if (edge[i][j] == 255)
                        {
                          if (ival[i][j] < low)
                          {
                              edge[i][j] = 0;
                              final[i][j] = 0;
                          } else if (ival[i][j] > high)
                          {
                            edge[i][j] = 0;
                            final[i][j] = 255;
                          }
                        }
                    }
                }

                int middle;
                do {
                    middle = 0;
                    for(i=mr; i<256-mr; i++)
                    { for(j=mr ;j<256-mr; j++)
                      {
                            if (edge[i][j] == 255){
                                  for (p=-1;p<=1;p++)
                                  {  for (q=-1;q<=1;q++)
                                    {
                                      if (final[i+p][j+q] == 255)
                                      {
                                        edge[i][j] = 0;
                                        final[i][j] = 255;
                                        middle = 1;
                                      }
                                    }
                                  }
                            }
                        }
                    }
                } while (middle);


        fprintf(fo2,"P5\n");
        fprintf(fo2,"%d %d\n", PICSIZE, PICSIZE);
        fprintf(fo2,"255\n");

        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++) fprintf(fo2,"%c",(char)((int)(final[i][j])));
        }
}
