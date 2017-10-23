#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[]) {

  double a = (time(NULL));
  srand(time(NULL));  /* We usually do this to create the seed. Otherwise it was default with 0 */

  /* You can pass in a pointer to a time_t object that time will fill up with the current time
  * (and the return value is the same one that you pointed to). If you pass in NULL, it just ignores it and
  * merely returns a new time_t object that represents the current time.
  * */

  printf("%f\n", a);
  return 0;
}
