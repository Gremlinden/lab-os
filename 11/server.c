#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>



int comparator(const void *a, const void *b)
{
  return (*((int *)b) - *((int *)a));
}

int main(int argc, char *argv[])
{

  int listener, sock;
  int len = 1024;
  int buff[len];
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
  listen(listener, 1);
  
  while(1)
  {
  socklen_t clilen = sizeof(cliaddr);
  sock = accept(listener, (struct sockaddr *)&cliaddr, &clilen);

	 while (1)
	 {
	  bytes_read = recv(sock, buff, len, 0);
	  if(bytes_read <= 0) break;
	
    	qsort((int *)buff, bytes_read * sizeof(char) / sizeof(int), sizeof(int), comparator);
    	send(sock, (int *)buff, bytes_read, 0);
   }
   close(sock);
  }

}
