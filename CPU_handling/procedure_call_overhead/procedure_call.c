#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define ITERATIONS 50000
#define STACK_SIZE (1024 * 1024)

struct timespec time_start,time_end,time_taken;
struct timespec diff(struct timespec start, struct timespec end);
unsigned long  *times;

void procedure_call_arg_0 () {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_1 (int a1) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_2 (int a1,int a2) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_3 (int a1,int a2,int a3) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_4 (int a1,int a2,int a3,int a4) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_5 (int a1,int a2,int a3,int a4,int a5) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_6 (int a1,int a2,int a3,int a4,int a5,int a6) {
	clock_gettime(CLOCK_REALTIME,&time_end);
	time_taken = diff(time_start,time_end);
}

void procedure_call_arg_7 (int a1,int a2,int a3,int a4,int a5,int a6,int a7) {
	clock_gettime(CLOCK_REALTIME,&time_end);	
	time_taken = diff(time_start,time_end);
}

void inline GetElapsedTime(unsigned long iterations, int j) {
    	int i;
    	pid_t pid = getpid();
    	for (i = 0; i < iterations; i++) {
		switch (j) {
			case 0:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_0();
				break;
			case 1:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_1(1);
				break;
			case 2:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_2(1,2);
				break;
			case 3:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_3(1,2,3);
				break;
			case 4:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_4(1,2,3,4);
				break;
			case 5:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_5(1,2,3,4,5);
				break;
			case 6:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_6(1,2,3,4,5,6);
				break;
			case 7:
		        clock_gettime(CLOCK_REALTIME,&time_start);
		        procedure_call_arg_7(1,2,3,4,5,6,7);
				break;
			default:
				break;
		}
    		times[i] = time_taken.tv_nsec;
    	}
    	return;
}

int main( int argc, const char* argv[] ){

    	int i,j;
	unsigned long average_time = 0;
	unsigned long iter = ITERATIONS;
	if (argc > 1)
		iter = (unsigned long) atoi(argv[1]);
	
    	times = malloc(iter*sizeof(unsigned long*));

    	if (!times) {
        	printf("Failed to allocate memory\n");
        	return 0;
    	}

	//Launch the timing
	for (j = 0 ; j < 8 ; j++) {
        	GetElapsedTime(iter, j);
        	
		for (i = 0; i < iter; i++) {
            		if (i != 0)
				average_time += times[i];
        	}
        	average_time /= iter - 1;
        	printf("%i %lu\n", j , average_time);
	}
    return 0;
}

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec t;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		t.tv_sec = end.tv_sec-start.tv_sec-1;
		t.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		t.tv_sec = end.tv_sec-start.tv_sec;
		t.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return t;
}
