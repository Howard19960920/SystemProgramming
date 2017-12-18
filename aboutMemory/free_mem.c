#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int main(int argc, char **argv)
{
  if(argc != 2)
  {
    fprintf(stderr, "Too few or too many arguments\n");
    exit(1);
  }
  int x = (int) atoi(*(argv + 1)), *p[8], idx = 0, i;
  if(x < 1 || x > 5)
  {
    fprintf(stderr, "I suggest that 1 <= argv[1] <= 5\n");
    exit(1);
  }
  printf("Before malloc:\n");
  system("free -h");
  printf("\n\n");


  for(i = 0; i < x; i++)
  {
    p[i] = (int *) malloc(sizeof(int) * 1024 * 1024 * 256);
    memset(p[i], 1, sizeof(int) * 1024 * 1024 * 256);
  }
  printf("After malloc:\n");
  system("free -h");
  printf("\n\n");


  for(i = 0; i < x; i++)
  {
    free(p[i]);
  }
  printf("After free:\n");
  system("free -h");
  printf("\n\n");


  return 0;
}
