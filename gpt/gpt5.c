#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

	sleep(1);
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    long total_microseconds = seconds * 1000000 + microseconds;
    printf("Elapsed time: %ld microseconds\n", total_microseconds);
    
	printf("\n\nDiferent approach:\n");
	long start_t = start.tv_sec * 1000 + start.tv_usec;
    long end_t = end.tv_sec * 1000 + end.tv_usec;
   	long other_microseconds = end_t - start_t;
    printf("Elapsed time: %ld microseconds\n", other_microseconds);
	return 0;
}
