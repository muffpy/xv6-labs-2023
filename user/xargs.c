#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void
xarg_exec(char *args[])
{
  // for (int i = 0; args[i] != 0; ++i) {
  //   printf("    args[%d] = %s\n", i, args[i]);
  // }
  if (fork() == 0) {
    if (exec(args[0], args) == -1) {
      fprintf(2, "xargs: Error exec %s\n", args[0]);
    }
  }
  else wait(0);
}

int
main(int argc, char *argv[])
{
  char *execcmd[MAXARG];
  char buf[512]; char *p = buf;

  // 1. Fill first section of execcmd with argv
  for(int i=1; i<argc; i++)
    execcmd[i-1] = argv[i];
  
  // 2. Read from stdin and copy to execcmd
  while(read(0, p, 1)==1) {
    if(p - buf == 512) {
      fprintf(2, "xargs: arguments too long.\n");
      exit(1);
    }

    if(*p == '\n') {
      *p = '\0';
      // printf("Line buf: %s\n", buf);
      // printf("Buf size: %d\n", p-buf);
      
      // Copy args from buf to execcmd
      execcmd[argc-1] = malloc(p-buf);
      memmove(execcmd[argc-1], buf, p-buf);
      execcmd[argc] = 0;

      // 3. Fork and exec
      xarg_exec(execcmd);

      // Continue with next line from stdin
      free(execcmd[argc-1]);
      memset(buf, 0, p-buf);
      memset(execcmd[argc-1], 0, p-buf);
      p = buf;
    }

    else p++;
  }

  exit(0);
}