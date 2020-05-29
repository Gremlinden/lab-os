#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>


int main(int argc, char *argv[])
{

  long file_size, n;
  char name_file[256];
  
  int port = atoi(strchr(argv[1], ':') + 1);
  int pointer = strchr(argv[1], ':') - argv[1];
  char ip[pointer + 1];
  strncpy(ip, argv[1], pointer);
  ip[pointer] = '\0';

  int sock;
  sock = socket(PF_INET, SOCK_STREAM, 0); 
  struct sockaddr_in addr;

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_aton(ip, &addr.sin_addr);
  
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("Connect");
    exit(2);
  }
  
  printf("Ведите название файла для скачивания или ведите exit чтобы выйти\n");
  while (1)
  {
    scanf("%s", name_file);

    if (strcmp(name_file, "exit") == 0)
    {
      exit(0);
    }
    else
    {
      write(sock, name_file, strlen(name_file) + 1);
      long n = read(sock, &file_size, sizeof(file_size));
      if (n == 0)
      {
        perror("Connect");
        exit(1);
      }
      else if (file_size == -1)
      {
        printf("Такого файла на сервере нет\n");
      }
      else
      {
        printf("Процесс скачивания...\n");
        
        int downloaded_file = open(name_file, O_RDWR | O_CREAT, 0600);
        ftruncate(downloaded_file, file_size);
        char *downloaded_file_data = (char *)mmap(NULL, n, PROT_WRITE | PROT_READ, MAP_SHARED, downloaded_file, 0);
        read(sock, downloaded_file_data, file_size);
        munmap(downloaded_file_data, file_size);
        close(downloaded_file);
        
        printf("Файл скачен\n");
      }
    }
  }
  close(sock);
}
