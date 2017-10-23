#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  struct stat sb;
  if( (stat(argv[1], &sb) == 0) && S_ISDIR(sb.st_mode))
    printf("is dir.\n");
  else
    printf("not dir.\n");

  return 0;
}
