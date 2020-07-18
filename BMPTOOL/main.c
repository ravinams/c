#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "bmplib.h"

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL *original, int rows, int cols, int scale,
            PIXEL **new, int *newrows, int *newcols)
{

  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */

    int row, col;
    if((rows<=0)|| (cols<=0))
    {
        return -1;
    }

    (*newrows)=(int)((float)rows * scale);
    (*newcols)=(int)((float)cols * scale);

    *new=(PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));

    for(row=0;row<(*newrows);row++)
    {
            for(col=0;col<(*newcols);col++)
            {
                     (*new)[row * (*newcols) + col] = original[(row * rows / (*newrows))  * cols + col * cols / (*newcols)];
            }
    }

  return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL *original, int rows, int cols, int rotation,
           PIXEL **new, int *newrows, int *newcols)
{

  int row, col;

  if ((rows <= 0) || (cols <= 0))
    return -1;

  *new = (PIXEL *)malloc(rows * cols * sizeof(PIXEL));

  //Checks if and correct if it is a negative number.
  while (rotation > 0)
  {
    rotation = rotation - 360;
  }

  int Num = rotation / 90;

  for (; 1;)
  {
    if (Num == -3) //Rotates 270 degrees
    {
      for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++)
        {
          PIXEL *o = original + rows * cols - (row * cols + col);
          PIXEL *n = (*new) + (col * rows) + (rows - row - 1);

          *n = *o;
        }
      break;
    }
    else if (Num == -2) //Rotates 180 degrees
    {
      for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++)
        {
          PIXEL *o = original + row * cols + col;
          //           PIXEL* n =  (*new) + cols*rows - (rows*col + col);
          PIXEL *n = (*new) + ((rows - row - 1) * cols) + (cols - col - 1);
          *n = *o;
        }
      break;
    }
    else if (Num == -1) // Rotates 90 degrees
    {
      for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++)
        {
          PIXEL *o = original + row * cols + col;
          PIXEL *n = (*new) + col * rows + (rows - 1 - row);
          *n = *o;
        }
      break;
    }
    else if (Num == 0) // Rotates 360 degrees basically dont touch the picture just pass it
    {
      for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++)
        {
          PIXEL *o = original + row * cols + col;
          PIXEL *n = (*new) + row * cols + col;
          *n = *o;
        }
      break;
    }

    Num = Num - 4;
  }

  return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip(PIXEL *original, PIXEL **new, int rows, int cols)
{

  int row, col;

  if ((rows <= 0) || (cols <= 0))
    return -1;

  *new = (PIXEL *)malloc(rows * cols * sizeof(PIXEL));

  for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
      PIXEL *o = original + row * cols + col;
      PIXEL *n = (*new) + (rows - 1 - row) * cols + col;
      *n = *o;
    }

  return 0;
}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip(PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0))
    return -1;

  *new = (PIXEL *)malloc(rows * cols * sizeof(PIXEL));

  for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
      PIXEL *o = original + row * cols + col;
      PIXEL *n = (*new) + row * cols + (cols - 1 - col);
      *n = *o;
    }

  return 0;
}

int main(int argc, char **argv)
{
  
  PIXEL *b=NULL, *nb=NULL;

  int r=0, c=0,nr = 0, nc = 0;
  extern char *optarg;
  extern int optind;
  int optchoice = 0;
  int sflag = 0, rflag = 0, fflag = 0, oflag = 0, vflag = 0;
  int scale;
  int degree;
  char *output_file = NULL, *input_file = NULL;

  // printf("\n Total Arguents : %d", argc - 1);

  while ((optchoice = getopt(argc, argv, "s:r:fvo:")) != -1)
  {
    switch (optchoice)
    {
      // printf("\n%c", optchoice);

    case 's':
      sflag = 1;
      scale = atoi(optarg);
      break;
    case 'r':
      rflag = 1;
      degree = atoi(optarg);
      break;
    case 'f':
      fflag = 1;
      break;
    case 'v':
      vflag = 1;
      break;
    case 'o':
      oflag = 1;
      output_file = optarg;
      break;
    case '?':

      if (optopt == 's' || optopt == 'r' || optopt == 'o')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unkown Option '-%c'\n", optopt);
      else
        fprintf(stderr, "Unkown Option character '\\x%x'\n", optopt);

      exit(1);

    default:
      abort();

    }
  }

  input_file = argv[optind];

  printf("\n Input File = %s , output File = %s", input_file, output_file);
  
  readFile(input_file, &r, &c, &b);

  if (sflag == 1)
  {
    enlarge(b, r, c, scale, &nb, &nr, &nc);
  }

  if (rflag == 1)
  {
    rotate( (nb !=NULL ? nb:b), (nr!=0 ? nr : r), (nc !=0 ? nc : c), degree, &nb, &nr, &nc);
  }

  if (vflag == 1)
  {
    verticalflip( (nb !=NULL ? nb:b) , &nb, (nr!=0 ? nr : r), (nc !=0 ? nc : c));
  }

  if (fflag == 1)
  {
    flip((nb !=NULL ? nb:b), &nb, (nr!=0 ? nr : r), (nc !=0 ? nc : c));
  }

  writeFile(output_file, ( nr != 0 ? nr : r ), ( nc != 0 ? nc : c ), ( nb != NULL ? nb : b ));
  
  free(b);
  free(nb);
  printf("\n\n");
  return 0;
}
