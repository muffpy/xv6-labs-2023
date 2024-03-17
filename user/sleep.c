#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) 
{
  int s;

  if (argc != 2){
    fprintf(2, "Usage: `sleep n` where n > 0\n");
  }
  s = atoi(argv[1]);

  if (sleep(s) != 0) {
    fprintf(2, "sys_sleep failed!\n");
  }
  exit(0);
}