#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_args
{
  int sockfd;
};

long getFileSize(FILE *input)
{
long fileSizeBytes;
fseek(input, 0, SEEK_END);
fileSizeBytes = ftell(input);
fseek(input, 0, SEEK_SET);
return fileSizeBytes;
}

void *client_thread(void *t_args)
{
  struct thread_args *args = (struct thread_args *)t_args;
  int newsockfd = args->sockfd;
  while (1)
      {
        char name_file[256];
        long filesize;
        int n = read(newsockfd, name_file, 256);
        if (n == 0)
        {
          close(newsockfd);
          exit(0);
        }
        FILE *fin = fopen(name_file, "r");
        if (fin == NULL)
        {
          filesize = -1;
          write(newsockfd, &filesize, sizeof(filesize));
        }
        else
        {
          filesize = getFileSize(fin);
          write(newsockfd, &filesize, sizeof(filesize));

          char buffer[filesize];
          fread(buffer, sizeof(char), filesize, fin);
          fclose(fin);
          write(newsockfd, buffer, sizeof(buffer));
        }
      }
}

int main(int argc, char *argv[])
{

  int listener, sock;
  int bytes_read;

  listener = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr, cliaddr;

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    addr.sin_port = 0;
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      perror("Bind");
      close(listener);
      exit(2);
    }
  }
  
  socklen_t servlen = sizeof(addr);
  getsockname(listener, (struct sockaddr *)&addr, &servlen);
  printf("Listening on port: %d\n", ntohs(addr.sin_port));
  listen(listener, 10);
  
  if (fork() == 0)
  {
    while (1)
    {
      socklen_t clilen = sizeof(cliaddr);
      sock = accept(listener, (struct sockaddr *)&cliaddr, &clilen);
      struct thread_args *new_thread_args;
      new_thread_args->sockfd = sock;
      pthread_t thread;
      pthread_create(&thread, NULL, client_thread, new_thread_args);
    }
  }
  else
  {
    printf("Ведите exit чтобы выйти\n");
    char command[256];
    while (1)
    {
      scanf("%s", command);

      if (strcmp(command, "exit") == 0)
      {
        exit(0);
      }
      
    }
  }
}
  


