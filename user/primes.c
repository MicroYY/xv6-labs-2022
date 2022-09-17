#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
int
main(int argc, char *argv[])
{
  int max = 1000;
  char isPrime[max];
  int i = 0;
  for(i = 0; i < max; i++) {
    isPrime[i] = 1;
  }
  isPrime[1] = 1;

  for(i = 2; i <= max; ) {
    if(!isPrime[i - 1]) {
      i++;
      continue;
    }
    printf("prime %d\n", i);
    int j = i;
    for(j = i + i; j <= max; j += i) {
        isPrime[j - 1] = 0;
    }
        i++;
  }

  exit(0);
}
*/


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

  // read one number per recursion. Recursion will end of nothing is read.
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
  int max = 100;

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
