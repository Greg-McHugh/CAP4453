For the Canny edge detector, compile it with the -lm flag, (because it imports the math.h library),
$ gcc canny.c -lm

and then run the resulting executable with the input file name, the sigma value, and the percentage value as arguments.
$ ./a.out garb34.pgm 1 35

The program will create 3 output files:
 - output1.pgm
 - output2.pgm
 - output3.pgm

Supplied with canny.c:
 - garb34.pgm
 - 3 resulting images for sigma=1 (and percent=35)
 - 3 resulting images for sigma=2 (and percent=35)
