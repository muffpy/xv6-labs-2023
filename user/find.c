#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char* path, char* name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != 1){
    fprintf(2, "Usage: find [dir path] [file name] %s\n", path);
    close(fd);
    return;
  }

  // Create formatted string for printing
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
        continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // If file name matches user input, print [path/name]
    if (strcmp(de.name, name) == 0) {
      printf("%s\n", buf);
    }

    // If this is a directory, recurse through it
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    if (st.type == 1 && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
      find(buf, name);
    }
    // printf("I'm seeing: %s of type: %d\n", de.name, st.type);
  }
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(1, "Usage: find [dir path] [file name]");
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}