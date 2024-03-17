#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int pid;
  char byte = 'b';
  char buf[1];
  
  int parent2child[2]; // read&write pipe FDs
  int child2parent[2]; // read&write pipe FDs
  pipe(parent2child);
  pipe(child2parent);

  pid = fork();
  if (pid == 0){
    // parent in pingpong: this process writes to parent2child and reads from child2parent

    // send ping
    write(parent2child[1], &byte, 1);
    // receive pong
    read(child2parent[0], buf, 1);
    fprintf(1, "%d: received pong\n", getpid());
    exit(0);
  }
  pid = fork();
  if (pid == 0){
    // child in pingpong: this process writes to child2parent and reads from parent2child

    // receive ping
    read(parent2child[0], buf, 1);
    fprintf(1, "%d: received ping\n", getpid());
    // send pong
    write(child2parent[1], &byte, 1);

    exit(0);
  }

  // close pipe fd's
  close(parent2child[0]); close(parent2child[1]);
  close(child2parent[0]); close(child2parent[1]);
  
  wait(0);
  wait(0);

  exit(0);
}