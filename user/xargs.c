#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc <2 || argc > MAXARG){
    exit(0);
  }

  char* xargs[MAXARG];
  for(int i = 1; i < argc; i++) {
    xargs[i-1] = argv[i];
  }
  xargs[argc] = 0;
  char buf[256];
  int i=0;
  while(read(0, &buf[i], sizeof(char))){
    if(buf[i] == '\n'){
      buf[i] = 0;
      xargs[argc-1] = buf;
      i=0;
      if(fork() == 0){
        exec(xargs[0], xargs);
        exit(0);
      }
      else{
        wait(0);
      }
    }
    else{
        i++;
    }
  }

  exit(0);
}