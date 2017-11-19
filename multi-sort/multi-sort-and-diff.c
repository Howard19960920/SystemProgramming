#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>


// 傳給 pthread 函式的物件結構宣告
typedef struct ds_pthread
{

  int *arr, *ret, l, r;
  pthread_mutex_t *mutex;

} ds_pthread;



// pthread_diff 的呼叫函式
void pthread_diff(void *ps)
{
  int idx_i, local = 0;
  ds_pthread *p = (ds_pthread *) ps; // 先強轉一波

  for(idx_i = p->l + 1; idx_i <= p->r; idx_i++)
    local += (p->arr)[idx_i] - (p->arr)[idx_i - 1];

  pthread_mutex_lock(p->mutex);
  *(p -> ret) += local; // critical section
  pthread_mutex_unlock(p->mutex);
}


// 合併兩個 sorted array
void merge(int *arr, int size, int l, int m, int r)
{
  // merge 需要一塊跟 arr 一樣大的空間
  int *temp = (int *) malloc(sizeof(int) * size);
  if(temp == NULL)
  {
    fprintf(stderr, "Malloc error.\n"); // malloc 失敗
    exit(1);
  }
  int i, j, k;
  for(i = l, j = m + 1, k = l; i <= m && j <= r;)
  {
    if(arr[i] < arr[j])
      temp[k++] = arr[i++];
    else
      temp[k++] = arr[j++];
  }
  while(i <= m)
    temp[k++] = arr[i++];
  while(j <= r)
    temp[k++] = arr[j++];
  for(i = l; i <= r; i++)
    arr[i] = temp[i];

  free(temp);
}


// inline 的 swap 函式
__attribute__((always_inline)) inline void SWAP(int *x, int *y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}


// 自訂義的 quick sort, 每次選最右邊的當作 pivot
void quickSort(int *arr, int lbnd, int rbnd)
{
  if(lbnd >= rbnd)
    return;

  int pivot, pidx, i;
  pivot = arr[rbnd];
  for(pidx = lbnd, i = lbnd; pidx < rbnd && i < rbnd; i++)
  {
    if(arr[i] <= pivot)
    {
      SWAP(&arr[pidx], &arr[i]);
      pidx++;
    }
  }
  SWAP(&arr[pidx], &arr[rbnd]);
  quickSort(arr, lbnd, pidx - 1);
  quickSort(arr, pidx + 1, rbnd);
}


// pthread_sort 的呼叫函式
void pthread_sort(void *ps)
{
  ds_pthread *p = (ds_pthread *) ps;
  quickSort(p -> arr, p -> l, p -> r);
}


// 初始化資料
int *initialize(int seed, int size)
{
  int idx_i;
  srand(seed);
  int *ret = (int *) malloc(sizeof(int) * size);
  // 檢查 malloc 是否成功
  if(ret == NULL)
  {
    fprintf(stderr, "Malloc failed.\n");
    exit(1);
  }
  // 產生 n 筆資料
  for(idx_i = 0; idx_i < size; idx_i++)
    ret[idx_i] = rand();
  return ret;
}


int main(int argc, char **argv)
{
  // 檢查argv數量
  if(argc != 3)
  {
    fprintf(stderr, "Too few or too many arguments.\n");
    return 0;
  }

  int seed = atoi(argv[1]);
  int size = atoi(argv[2]);

  // 檢查 seed 與 size 範圍是否合理
  if(seed < 0 || size <= 0)
  {
    fprintf(stderr, "Invalid arguments.\n");
    return 0;
  }

  int *arr = initialize(seed, size); // 初始化資料

  // 雙線程的呼叫quicksort, 把 array 切成一半, 各自排序
  int idx_i, ret = 0;
  pthread_t pid[2];
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL); // mutex lock 初始化
  ds_pthread ps[2] = {{arr, &ret, 0, size / 2, &mutex}, {arr, &ret, size / 2 + 1, size - 1, &mutex}};

  for(idx_i = 0; idx_i < 2; idx_i++)
    pthread_create(&pid[idx_i], NULL, (void *)pthread_sort, &ps[idx_i]);

  for(idx_i = 0; idx_i < 2; idx_i++)
    pthread_join(pid[idx_i], NULL);

  // 把排序好的兩個 sorted array 合併
  merge(arr, size, 0, size / 2, size - 1);

  // 雙線程去計算 difference
  ps[2].l --; // 這樣才不會漏掉切點的 difference

  for(idx_i = 0; idx_i < 2; idx_i++)
    pthread_create(&pid[idx_i], NULL, (void *)pthread_diff, &ps[idx_i]);

  for(idx_i = 0; idx_i < 2; idx_i++)
    pthread_join(pid[idx_i], NULL);

  printf("Total difference: %d\n", ret);
  free(arr);
  return 0;
}
