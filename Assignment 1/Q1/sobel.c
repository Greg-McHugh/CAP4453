/* Gregory McHugh
 * GR795710
 * CAP4453-17Fall 0001
 * Assignment 1 – Q1
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

        int pic[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[256][256],maxival;
        double maxoutpicx, maxoutpicy;

main(argc,argv)
int argc;
char **argv;
{
  int i,j,p,q,mr,sum1,sum2;
  double lowThreshold, highThreshold;
  FILE *fGradient, *fLowThreshold, *fHighThreshold, *fxGradient, *fyGradient, *fInput, *fopen();
  char *foobar;

  // face05.pgm
  argc--; argv++;
  foobar = *argv;
  fInput=fopen(foobar,"rb");

  // high threshold
  argc--; argv++;
	foobar = *argv;
	highThreshold = atof(foobar);

  // low threshold
  argc--; argv++;
	foobar = *argv;
	lowThreshold = atof(foobar);

  // five output files
  fGradient=fopen("gradient.pgm", "wb");
  fLowThreshold=fopen("lowThreshold.pgm", "wb");
  fHighThreshold=fopen("highThreshold.pgm", "wb");
  fxGradient=fopen("xGradient.pgm", "wb");
  fyGradient=fopen("yGradient.pgm", "wb");

  // bypasses file header
  for(i = 0; i < 15; i++) getc(fInput);

  // stores image pixels in array
  for (i=0;i<256;i++)
  {
    for (j=0;j<256;j++)
    {
      pic[i][j]  =  getc (fInput);
      pic[i][j]  &= 0377;
    }
  }

        mr = 1;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
             outpicx[i][j] = sum1;
             outpicy[i][j] = sum2;
          }
        }

        maxival = 0; maxoutpicx = 0; maxoutpicy = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
            // calculates gradient
             ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));

             if (ival[i][j] > maxival) maxival = ival[i][j];

             if (outpicx[i][j] > maxoutpicx) maxoutpicx = outpicx[i][j];
             if (outpicy[i][j] > maxoutpicy) maxoutpicy = outpicy[i][j];


           }
        }


        // output file header
        fprintf(fGradient, "P5\n256 256\n255\n");
        fprintf(fxGradient, "P5\n256 256\n255\n");
        fprintf(fyGradient, "P5\n256 256\n255\n");


        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
              ival[i][j] = (ival[i][j] / maxival) * 255;
              fprintf(fGradient,"%c",(char)((int)(ival[i][j])));

              outpicx[i][j] = (outpicx[i][j] / maxoutpicx) * 255;
              fprintf(fxGradient,"%c",(char)((int)(outpicx[i][j])));

              outpicy[i][j] = (outpicy[i][j] / maxoutpicy) * 255;
              fprintf(fyGradient,"%c",(char)((int)(outpicy[i][j])));

            }
          }

        // file headers
        fprintf(fLowThreshold, "P5\n256 256\n255\n");
        fprintf(fHighThreshold, "P5\n256 256\n255\n");

        for(i=0; i<256; i++){
          for(j=0; j<256; j++){
            fprintf(fLowThreshold, "%c", (ival[i][j] > lowThreshold) ? 255 : 0);
            fprintf(fHighThreshold, "%c", (ival[i][j] > highThreshold) ? 255 : 0);
          }
        }
}
