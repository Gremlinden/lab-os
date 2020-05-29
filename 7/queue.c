#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


struct Strmsg
{
  long mtype;
  int mtext[4];
  int islast;
};

void print_nums(int *nums, int n)
{
  for (int i = 0; i < n; i++)
  {
    printf("%d ", nums[i]);
  }
  printf("\n");
}
int comparator(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void swap(int *nums, int i, int j)
{
    int a = nums[i];
    nums[i] = nums[j];
    nums[j] = a;
}

int next(int *nums, int n)
{
    int i = n - 2;
    while (i != -1 && nums[i] >= nums[i + 1]) i--;
    
    if (i == -1) return 0;
    
    int j = n - 1;
    while (nums[i] >= nums[j]) j--;
    
    swap(nums, i, j);
    int m = i + 1, k = n - 1;
    
    while (m < k) swap(nums, m++, k--);
    
    return 1;
}

void *parentMainCode(int msgId)
{
  struct Strmsg localmsg;
  
  int nums[4], count = 0;
  
  srand(time(0));
  for (int i = 0; i < 4; i++)
  {
    localmsg.mtext[i] = rand() % 10;
  }
  print_nums(localmsg.mtext, 4);
  printf("\n");

  localmsg.mtype = 1;
  localmsg.islast = 1;
  msgsnd(msgId, &localmsg, sizeof(localmsg), 0);
  while (localmsg.islast)
  {
	msgrcv(msgId, &localmsg, sizeof(localmsg), 2, 0);
    if (!localmsg.islast) break;
    print_nums(localmsg.mtext, 4);
    count++;
  }
  
  printf("Combinations count:%d\n", count);
  waitpid(0, 0, 0);
  msgctl(msgId, IPC_RMID, NULL);
}

void *childMainCode(int msgId)
{
  struct Strmsg localmsg;

  msgrcv(msgId, &localmsg, sizeof(localmsg), 1, 0);
  qsort(localmsg.mtext, 4, sizeof(int), comparator);
  localmsg.mtype = 2;
  msgsnd(msgId, &localmsg, sizeof(localmsg), 0);
  
  while(localmsg.islast)
  {
    localmsg.islast = next(localmsg.mtext, 4);
    localmsg.mtype = 2;
    msgsnd(msgId, &localmsg, sizeof(localmsg), 0);
  }
}

int main()
{
   const size_t semCount = 10;
   int msgId = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);

 
  pid_t childId = fork();
  
  if (childId > 0)
    {
        parentMainCode(msgId);
    }
    else
    {
        childMainCode(msgId);
    }
}
