For the Sobel program, compile it with the -lm flag, (because it imports the math.h library),
$ gcc sobel.c -lm

and then run the resulting executable with the input file name, the high threshold value, and the low threshold value as arguments.
$ ./a.out face05.pgm 50 12

The program will create five new files:
 - gradient.pgm
 - xGradient.pgm
 - yGradient.pgm
 - lowThreshold.pgm
 - highThreshold.pgm

Supplied with sobel.c:
 - face05.pgm
 - 5 resulting images.
