#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
//extern char *strdup( char *s );
void RlsBfs( char *curDir ) {
  DIR *dir, *tmp;
  struct dirent *ent;
  int cnt = 0;
  char path[1<<10];
  if((dir = opendir( curDir ) ) == NULL)
    return;

  tmp = opendir( curDir );
  ent = readdir(dir);
  printf("%s:\n", curDir);
  while( ent != NULL )  {
    printf("%s   ", ent->d_name);
    ent = readdir(dir);
  }
  printf("\n");
  if( curDir[strlen(curDir)-1]=='/' )
    curDir[strlen(curDir)-1] = '\0';
  ent = readdir( tmp );
  while( ent != NULL )  {
    if( strcmp( ent->d_name, "." )==0 || strcmp( ent->d_name,".." )==0 ) {
      ent = readdir( tmp );
      continue;
    }
    sprintf( path, "%s/%s", curDir, ent->d_name );
//    printf("PATH:%s\n",path);
    RlsBfs( path );
    ent = readdir(tmp);
  }
}
int isDir( char *path ) {

  struct stat sb;
  return (stat( path, &sb ) == 0 && S_ISDIR( sb.st_mode ) )?1:0;
}
int main(int argc, char **argv) {

  if( argc == 1 || argc > 2 ) return 0;
  RlsBfs( argv[1] );
  return 0;
}

