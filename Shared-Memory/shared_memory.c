#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


// 把 mode 轉成 rwx 模式
char* decode(int x)
{
  int i, j, bit = 1, base, temp;
  char *ret = (char *) malloc(sizeof(char) * 4);
  ret[3] = '\0';

  for(i = 0; i < 3; i++)
  {
    temp = 0;
    base = 1;
    for(j = 0; j < 3; j++)
    {
      if(x & bit)
      {
        temp += base;
      }
      bit <<= 1;
      base <<= 1;
    }
    ret[2 - i] = temp + '0';
  }
  return ret;
}


int main(int argc, char **argv)
{
  int shmid, ret;
  struct shmid_ds buf;

  // IPC_PRIVARE : 創造一個新的共享記憶體空間
  shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0600);

  if(shmid == -1)
  {
    // shmget returns -1 on error
    fprintf(stderr, "shmget error.\n");
    return 0;
  }

  // IPC_STAT: get the status of shm and cpy the shmid_ds to buf
  ret = shmctl(shmid, IPC_STAT, &buf);
  if(ret == -1)
  {
    // shmctl returns -1 on error
    fprintf(stderr, "shmctl error.\n");
    return 0;
  }

  printf("Segment ID: %d\n", shmid);
  printf("Key: %d\n", (int)buf.shm_perm.__key);
  printf("Mode: %d\n", (int)buf.shm_perm.mode);

  char *rwxcode = decode((int) buf.shm_perm.mode);
  printf("Mode(rwx): %s\n", rwxcode);
  free(rwxcode);

  printf("OWner UID: %d\n", (int)buf.shm_perm.uid);
  printf("Size: %d\n", (int)buf.shm_segsz);
  printf("Number of attaches: %d\n", (int)buf.shm_nattch);

  // 把共享記憶體空間刪除
  ret = shmctl(shmid, IPC_RMID, NULL);
  if(ret == -1)
  {
    // shmctl returns -1 on error
    fprintf(stderr, "shmctl error.\n");
    return 0;
  }

  return 0;
}
