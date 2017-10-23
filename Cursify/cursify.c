#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#define BUFF_LEN (10 *((sizeof(struct inotify_event)) + NAME_MAX +1)) // from manual of inotify
extern char *strdup(const char*); // part of POSIX, rather than ISO C
int compare(const void*, const void*);
void cursify(int, char*); //traverse the given path in system way and add watch to it
void listen(int); // start inotify
int mapsize = 1, mapindex = 0; // event->wd starts from 1.
char **map; // map of watch descriptor to path

int main(int argc, char *argv[]) {
  int i, fd;
  if( argc <2 ) {
    printf("too few arguemnets!\n");
    return 0;
  }
  map = malloc(sizeof(char*) * mapsize);
  if((fd = inotify_init()) < 0) { printf("init die\n"); return 0; }
  for( i=1; i<argc; i++ )
    cursify(fd, argv[i]);
  listen(fd);
  return 0;
}
void cursify(int fd, char *curDir) {

  if( inotify_add_watch(fd, curDir, IN_ALL_EVENTS) == -1 )  {
    printf("add watch error on: %s\n", curDir);
    return;
  }
  map[mapindex++] = strdup(curDir);
  if( mapindex == mapsize ) {
    mapsize<<=1;
    char **temp = realloc(map, sizeof(char*) * mapsize );
    if(temp == NULL)  {
      printf("(char**)map realloc error!\n");
      return;
    }
    map = temp;
  }
  DIR *dir, *ini;
  if( (dir = opendir(curDir)) == NULL )
    return; //use this instead of stat S_ISDIR() can avoid some unknown problems.
  ini = opendir(curDir);
  struct dirent *ent = readdir(ini);
  int size = 1, index = 0, i;
  char **ls = malloc(sizeof(char*) * size), path[1<<10];
  while( ent != NULL )  {
    ls[index++] = strdup(ent->d_name);
    if( index == size ) {
      size <<=1;
      char **temp = realloc(ls, sizeof(char*)*size);
      if(temp == NULL)  {
        printf("ls realloc error.\n");
        return;
      }
      ls = temp;
    }
    ent = readdir(ini);
  }
  qsort(ls, index, sizeof(char*), compare);
  printf("%s:\n", curDir);
  for(i=0; i<index; i++)
    printf("%s,  ", *(ls+i));
  printf("\n");
  free(ls);
  ent = readdir(dir);
  while( ent != NULL )  {
    if( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")){
      ent = readdir(dir);
      continue;
    }
    if( curDir[strlen(curDir)-1] == '/' ) { curDir[strlen(curDir)-1] = '\0'; }
    sprintf(path, "%s/%s", curDir, ent->d_name);
    cursify(fd, path);
    ent = readdir(dir);
  }
}
int compare(const void *a, const void *b) {
  return strcmp(*(char**) a, *(char**) b); // return strcmp( (char*)a, (char*)b) won't work.
}
void listen(int fd) {
  int num, option=-1;
  do {
    printf("\nStart listening? (1)yes, (0):exit): ");
    scanf("%d", &option);
  } while(option!=1 && option!=0);
  if(!option)
    return;
  system("clear");
  char buff[BUFF_LEN], *p;
  struct inotify_event *event;
  while(1)  {
    num = read(fd, buff, BUFF_LEN);
    if(num<0) {
      printf("inotify listen error!\n");
      return;
    }
    for(p=buff; p<buff+num; ) {
      event = (struct inotify_event*) p;
      printf("[%s]\t{ ", map[event->wd-1]);
      if( event->mask & IN_ACCESS ) printf("IN_ACCESS,");
      if( event->mask & IN_CREATE ) printf("IN_CREATE,");
      if( event->mask & IN_DELETE ) printf("IN_DELETE,");
      if( event->mask & IN_ATTRIB ) printf("IN_ATTRIB,");
      if( event->mask & IN_CLOSE_WRITE ) printf("IN_CLOSE_WRITE,");
      if( event->mask & IN_CLOSE_NOWRITE ) printf("IN_CLOSE_NOWRITE,");
      if( event->mask & IN_DELETE_SELF ) printf("IN_DELETE_SELF,");
      if( event->mask & IN_MODIFY ) printf("IN_MODIFY,");
      if( event->mask & IN_MOVE_SELF ) printf("IN_MOVE_SELF,");
      if( event->mask & IN_MOVED_FROM ) printf("IN_MOVED_FROM,");
      if( event->mask & IN_MOVED_TO ) printf("IN_MOVE_TO,");
      if( event->mask & IN_OPEN ) printf("IN_OPEN,");
      printf(" }\t");
      printf("cookie = %d\t", event->cookie);
      if( event->len )
        printf("name = %s\n", event->name);
      else
        printf("name = NULL\n");
      p+=sizeof(struct inotify_event)+event->len;
    }
  }
}
