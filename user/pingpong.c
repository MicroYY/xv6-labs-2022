#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipe_ping[2];
  int pipe_pong[2];

  char *pingpong[2];
  pingpong[0] = "received ping";
  pingpong[1] = "received pong";

  pipe(pipe_ping);
  pipe(pipe_pong);
  
  if(fork() == 0) {
    char buffer[256];
    read(pipe_ping[0], buffer, 14);
    printf("%d: %s\n", getpid(), buffer);
    write(pipe_pong[1], pingpong[1], 14);

  }
  else {
    write(pipe_ping[1], pingpong[0], 14);
    wait(0);
    char buffer[256];
    read(pipe_pong[0], buffer, 14);
    printf("%d: %s\n", getpid(), buffer);
  }
  exit(0);
}
