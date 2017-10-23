#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("TOO FEW OR TOO MANY ARGUMENTS\n");
    exit(1);
  }

  int pipefd[2], ret, status;
  pipe(pipefd);
  ret = fork();

  if(ret == 0)  {
    /*first child*/
    char *paralist[3] = {"cat", argv[1], NULL};
    close(1); dup(pipefd[1]);
    close(pipefd[1]); close(pipefd[0]);
    execv("/bin/cat", paralist);
  }

  if(ret > 0) {
    ret = fork();
    if(ret == 0)  {
      /*second child*/
      char *paralist[2] = {"wc", NULL};
      close(0); dup(pipefd[0]);
      close(pipefd[0]); close(pipefd[1]);
      execv("/usr/bin/wc", paralist);
    }
    printf("RESULT(LINE, WORD, BYTES):\n");
    wait(&status);
  }
  return 0;
}
