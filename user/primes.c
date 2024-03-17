#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
spawn(int left)
{
  int prime, n;
  
  // 1. child reads prime from parent (left/read-side of pipe)
  read(left, &prime, sizeof(prime));

  // 2. print principal prime from left neighbor
  fprintf(1, "prime %d\n", prime);

  // 3. Pipe+Fork on this prime if more numbers to be read from left
  if (read(left, &n, 4) == 4){

    int pd[2]; // new pipe
    pipe(pd);
    int right = pd[1];
    if (fork()){
      close(pd[0]);

      // 4. keep reading numbers from left in a loop
      while(1){

        // 5. Send indivisible integers to 
        // right/write-side of new pipe.
        if (n % prime) write(right, &n, 4);

        if (read(left, &n, 4) != 4) break; // stop when left neighbor stops sending feed 
      }
      close(left);
      close(right);
      wait(0);
    }
    else {
      // 6. Repeat all steps for new primes found
      close(pd[1]);
      spawn(pd[0]);
    }
  }
}

int 
main(int argc, char *argv[]) 
{  
  int fds[2];
  pipe(fds);

  if (fork()){ // parent spawns new prime filters for every prime discovered

    close(fds[1]); 

    // spawn filter
    spawn(fds[0]);

    wait(0);
    exit(0);

  } else { // child provides source feed of integers from 2..35
    close(fds[0]);

    for (int i = 2; i < 36; ++i) {
      write(fds[1], &i, sizeof(i));
    }
    
    close(fds[1]);
  }
  
  return 0;
}