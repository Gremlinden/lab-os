#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const char *name = argv[1];
  int byteCount = atoi(argv[2]);

  FILE *file = fopen(name, "r");
  char data[byteCount];
  int res = fread(data, sizeof(char), byteCount, file);

  printf("%d\n", res);
  for (size_t i = 0; i < res; i++)
  {
    printf("%c[%d]\n", data[i], data[i]);
  }
  
  fclose(file);
}
