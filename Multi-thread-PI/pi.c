/*
 * System Software Programming Homework#12
 * A practice of pthread, semaphore, and Monte Carlo method
 * Friday, June, 16th, 2017
 * 4044-100-16
 * CSIE 2nd Grader Howard, Sung
 */
#define _XOPEN_SOURCE
#define _BSD_SOURCE
#define _SVID_SOURCE
/* Some of the function below(i.e srand48_r(), drand48_r()) were defined in one of the above
 * Since I'm not sure whether the OS of your computer will influence the accuracy or the executability of the program
 * So I included them all above. With more details, please check the url as follow: https://goo.gl/37cD3D
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
/* Macro function used to check if the given arguments is sufficient */
#define argCheck(void) \
  if(argc != 3) {fprintf(stderr, "Too few or too many arguments\n"); return 0;}

volatile int valid_count = 0;
pthread_mutex_t mutex;

void thread(void *pCount) {

  int local_valid = 0, i;
  double xAxis, yAxis;
  struct drand48_data rand_buff;

  srand48_r(time(NULL), &rand_buff);
  for(i = 0; i < *(int*)pCount; i++)  {
    drand48_r(&rand_buff, &xAxis); /* This gaurantee the thread-safe access to rand() */
    drand48_r(&rand_buff, &yAxis); /* This gaurantee the thread-safe access to rand() */
    if((xAxis * xAxis + yAxis * yAxis) < 1)
      local_valid++; /* temp how many valid points in each thread */
  }
  pthread_mutex_lock(&mutex);
  valid_count += local_valid;
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv) {
  argCheck(); /* Check if input arguments is as required, (i) loop times (ii) thread count */
  int loop_count = atoi(argv[1]), thread_count = atoi(argv[2]), each_loop_count = loop_count / thread_count, i;
  /* each_loop_count is the result of divide the total loop count with N threads */
  pthread_t id[thread_count]; /* Create pthread_t structure array with size of (int)thread_count */
  pthread_mutex_init(&mutex, NULL); /* Initialize the mutex */
  for(i = 0; i < thread_count; i++)
    pthread_create(&id[i], NULL, (void*)thread, &each_loop_count); /* create pthread (int)pthread_count times */
  for(i = 0; i < thread_count; i++)
    pthread_join(id[i], NULL); /* join pthread (int)pthread_count times */
  /* According to def of pi, print the result  on screen*/
  fprintf(stdout, "pi:  %.10f\n", 4.0 * valid_count / loop_count);
  return 0;
}
