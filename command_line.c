#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command_line.h"
#include "percolate.h"


int main(int argc, char* argv[])
{

 
  L = 20;
  rho = 0.400000;
  seed = 1564;
  MAX = L*L;
  datafile_name = "map.dat";
  imagefile_name = "map.pgm";

  while((opt = getopt(argc,argv,":l:r:s:m:d:g:"))!=-1)
  {
    switch(opt)
    {
      case 'l':
        L = atoi(optarg);
        break;
      case 'r':
        rho = atof(optarg); 
        break;
      case 's':
        seed = atoi(optarg);
        break;
      case 'm':
        MAX = atoi(optarg);
        break;
      case 'd':
        datafile_name = optarg;
        break;
      case 'g':
        imagefile_name = optarg;
        break;

    }
  }
  valid_result = judge_validation_of_command_lines(L, rho, seed, MAX);
  
  if (valid_result == -1) return 0;

  printf("**************************************\nHere are parameters:\n(Unset parameters have been given original values.)\nL   : %d\nrho : %f\nseed: %d\nMAX : %d\ndatafile_name : %s\nimagefile_name: %s\n**************************************\n", L, rho, seed, MAX, datafile_name, imagefile_name);

  percolate_processing(rho, L, MAX, seed, datafile_name, imagefile_name);

  return 0;

}

int judge_validation_of_command_lines(int L, int rho, int seed, int MAX)
{
/*judge the validation of parameters*/
  if (L < 1)
  {
    printf("map width and height should be no less than 1. \n");
    return -1;
  }
  if (rho < 0 || rho > 1)
  {
    printf("The value of ρ should be between 0 and 1.\n ");
    return -1;
  }
  if (seed < 0 || seed > 900000000)
  {
    printf("The value of seed should be between 0 and 900000000. \n");
    return -1;
  }
  if (MAX < 0 || MAX > L * L)
  {
    printf("The value of MAX should be between 0 and L*L. \n");
    return -1;
  }
  return 0;
}