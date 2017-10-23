#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[]) {

  srand(time(NULL)); /* Create seed of rand with time() */
  /* To randomly create float nuber within 0~1 */
  int i, dotime;
  fprintf(stdout, "How many float numbers (0,1) you want to generate?\n");
  scanf("%d", &dotime);
  for(i = 0; i < dotime; i++)
    fprintf(stdout, "[%d]:\t%.6f\n", i, rand() / (double)RAND_MAX);
  return 0;
}
