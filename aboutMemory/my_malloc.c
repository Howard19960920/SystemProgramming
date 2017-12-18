#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int main()
{
  // 要求配置一塊40MB的記憶體
  int *p = (int *) malloc(sizeof(int) * 10000000);
  assert(p != NULL);
  printf("Before assignments (in KBs)\n");
  system("free");
  // 真正使用該記憶體
  memset(p, 1, sizeof(int) * 10000000);
  printf("After assignments (in KBs)\n");
  system("free");
  free(p);
  return 0;
}
