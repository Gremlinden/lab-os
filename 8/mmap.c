#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const char *input = argv[1];
  const char *output = argv[2];

  int in_file = open(input, O_RDONLY, 0600);
  int out_file = open(output, O_RDWR | O_CREAT, 0600);

  struct stat* buff;
  stat(input, buff);
  int length = buff->st_size;

  ftruncate(out_file, length);

  char *data  = (char *)mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, out_file, 0);

  read(in_file, data , length);
  munmap(data , length);

  close(in_file);
  close(out_file);
}
