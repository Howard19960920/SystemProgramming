#include <sys/signalfd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define herr(msg) \
  do{ perror(msg); exit(EXIT_FAILURE); }while(0)

int main(int argc, char *argv[]) {

  sigset_t mask;
  int sigfd;
  struct signalfd_siginfo sigblk;
  ssize_t rsize;

  sigemptyset(&mask);
  sigfillset(&mask); // include all signals.
  //sigaddset(&mask, SIGINT);
  //sigaddset(&mask, SIGQUIT);
  //sigaddset(&mask, SIGTSTP);

  if(sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    herr("err: sigprocmask");

  sigfd = signalfd(-1, &mask, 0);

  if(sigfd == -1)
    herr("err: signalfd");

  printf("My pid is:  %d\n\nThis program can detect signals:\n\n1.  SIGINT(ctl+c)\n2.  SIGQUIT(ctl+\\)\n3.  SIGTSTP(ctl+z)\n\n", getpid());

  while(1)  {

    rsize = read(sigfd, &sigblk, sizeof(struct signalfd_siginfo));

    if(rsize != sizeof(struct signalfd_siginfo))
      herr("err: read");

    if(sigblk.ssi_signo == SIGINT)
      printf(" Got signal: SIGINT\n");

    else if(sigblk.ssi_signo == SIGQUIT)
      printf("Got signal: SIGQUIT\n");

    else if(sigblk.ssi_signo == SIGTSTP)
      printf("Got signal: SIGTSTP\n");

    else
      printf("Got unexpected siganl:  %s\n\nThis program is designed for detecting:\n\n1.  SIGINT(ctl+c)\n2.  SIGQUIT(ctl+\\)\n3.  SIGTSTP(ctl+z)\n\n",
          sys_siglist[sigblk.ssi_signo]);
  }
  close(sigfd);
  return 0;
}
