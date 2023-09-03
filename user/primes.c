#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
mapping(int n, int pd[])
{
  close(n);
  dup(pd[n]);
  close(pd[0]);
  close(pd[1]);
}

void
primes()
{
  int previous, next;
  int fd[2];

  // read one number per recursion. Recursion will end if nothing is read.
  if (read(0, &previous, sizeof(int)))
  {
    printf("prime %d\n", previous);
    pipe(fd);
    if (fork() == 0)
    {
      mapping(1, fd);
      //read right neighbour
      while (read(0, &next, sizeof(int)))
      {
        if (next % previous != 0)
        {
          // write to output descriptor
          write(1, &next, sizeof(int));
        }
      }
    }
    else
    {
      wait(0);
      mapping(0, fd);
      primes();
    }  
  }  
}

int 
main(int argc, char *argv[])
{

  int fd[2];
  pipe(fd);
  int max = 35;

  if (fork() == 0)
  {
    mapping(1, fd);
    for (int i = 2; i <= max; i++)
    {
      write(1, &i, sizeof(int));
    }
  }
  else
  {
    wait(0);
    mapping(0, fd);
    primes();
  }
  exit(0);
}