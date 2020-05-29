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


void random_nums(int *nums, int n, int min, int max)
{
  srand((time(0)));
  for (int i = 0; i < n; i++)
  {
    nums[i] = min + rand() % (max - min + 1);
  }
}

void print_nums(int *nums, int c)
{
  for (int i = 0; i < c; i++)
  {
    printf("%d ", nums[i]);
  }
  printf("\n");
}

long int time_server(struct timespec start, struct timespec end)
{
  return 1000000000*(end.tv_sec - start.tv_sec)+(end.tv_nsec - start.tv_nsec);;
}

int main(int argc, char *argv[])
{
  int n, min, max;

  int port = atoi(strchr(argv[1], ':') + 1);
  int pointer = strchr(argv[1], ':') - argv[1];
  char ip[pointer + 1];
  strncpy(ip, argv[1], pointer);
  ip[pointer] = '\0';

  printf("Enter array length, min, max: ");
  scanf("%d %d %d", &n, &min, &max);
  
  int arr[n];
  int sock;
  sock = socket(PF_INET, SOCK_STREAM, 0); 
  struct sockaddr_in addr;

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_aton(ip, &addr.sin_addr);

  random_nums(arr, n, min, max);
  print_nums(arr, n);
  
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("connect");
    exit(2);
  }

  struct timespec start, end;
  
  send(sock, arr, n * sizeof(int), 0);
  clock_gettime (CLOCK_REALTIME, &start);
  recv(sock, arr, n * sizeof(int), 0);
  clock_gettime (CLOCK_REALTIME, &end);
  print_nums(arr, n);
  
  printf("Server time: %ld нс\n", time_server(start, end));
  close(sock);
}
