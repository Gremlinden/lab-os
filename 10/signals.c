#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


int signals1 = 0, signals2 = 0, signals3 = 0;

void handler(int nsig)
{
    if (nsig == SIGINT)
    {
        signals1++;
    }
    else if (nsig == SIGUSR1)
    {
        signals2++;
    }
    else if(nsig == SIGUSR2)
    {
        signals3++;
    }
}

int main()
{
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    while(1)
    {
            time_t curTime = time(NULL);
            printf("Time %s\n", ctime(&curTime));
            printf("SIGINT:  %d\n", signals1);
            printf("SIGUSR1: %d\n", signals2);
            printf("SIGUSR2: %d\n", signals3);
       
      if (signals1 >= 5)
      break;

        sleep(1);
    }
    return 0;
}


