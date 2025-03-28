#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <sys/time.h>

int main()
{
	struct timeval	now;
	struct	timeval before;
	long			elapse;
	int i = 0;
	gettimeofday(&before, NULL);
	usleep(100 * 1000);
	gettimeofday(&now, NULL);
	while (i < 100)
		i ++;
	printf("elapse %ld\n", ((now.tv_sec * 1000) + (now.tv_usec / 1000)) - ((before.tv_sec * 1000) + (before.tv_usec / 1000)));
	return(0);
}