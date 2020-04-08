#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

const char *NAME = "test";

int compare(const void *a, const void *b)
{
    return(*(int *)b) - *((int *)a);
}

int *random_nums(int n)
{
    srand((unsigned) (time(0)));
    int *nums = malloc( sizeof( int) * n);
    for( int i = 0; i < n; i++)
    {
 nums[i] = rand() % 1000;
    }
    return nums;
}

void out_nums(int *nums, int c)
{
    for( int i = 0; i < c; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");
}

int main(int argv, char *argc[])
{
    int n = atoi(argc[1]);
    int *nums = random_nums(n);
    out_nums(nums, n);

    mknod(NAME, S_IFIFO | 0666, 0);
    int p[2];
    pipe(p);

    int child_id = fork();

    if (child_id == 0)
    {
        close(p[1]);
        int fifo = open(NAME, O_WRONLY);
        int *received_nums = malloc(sizeof(int) * n);
        read(p[0], received_nums, sizeof(int) * n);
        close(p[0]);
        qsort(received_nums, n, sizeof(int), compare);
        write(fifo, received_nums, sizeof(int) * n);
    }
    else
    {
        close(p[0]);
        int fifo = open(NAME, O_RDONLY);
        write(p[1], nums, sizeof(int) * n);
        close(p[1]);
        int *sorted_nums = malloc(sizeof(int) * n);
        read(fifo, sorted_nums, sizeof(int) * n);
        out_nums(sorted_nums, n);
        unlink(NAME);
    }
}