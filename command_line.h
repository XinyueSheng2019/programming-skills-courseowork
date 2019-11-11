#ifndef _A_H_
#define _A_H_
struct command_line
{
  int L, seed, MAX, opt;
  float rho;
  char* datafile_name;
  char* imagefile_name;
};

#endif


int valid_result;


int judge_validation_of_command_lines(struct command_line* p);
void percolate_processing(struct command_line* p);