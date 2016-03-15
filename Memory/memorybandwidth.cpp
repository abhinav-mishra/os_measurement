#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <cstring>
#include <vector>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#define ITERATIONS 10
#define KB(x)   ((x) << 10)
#define MB(x)   ((x) << 20)
#define GB(x)   ((x) << 30)

using namespace std;

int compare(const void *, const void *);
void GetElapsedTime(uint64_t *, double size_increment);
void Loop(void);

const int numMB=1000;
unsigned char first_array[MB(numMB)];
unsigned char second_array[MB(numMB)];
float cycles_per_second = 2.4*1000*1000*1000;  //2.4*10^9 = 2.5GHZ

int main(void)
{
    int i;
    uint64_t *times;
    vector<double> results;

    times = (uint64_t *)malloc(ITERATIONS*sizeof(uint64_t*));

    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }
    vector<float> divisions;
    for (int i=0; i<10; i++){
       divisions.push_back(.1*i);
    }
 
    
    for (int a=0; a<10; ++a){
       double mem_amount = divisions[a]*sizeof(first_array);
       GetElapsedTime(times,mem_amount);
       for (i = 0; i < ITERATIONS; i++) {
           results.push_back(times[i]);
           //printf("Iteration %d: timing overhead = %" PRIu64 " cycles\n", i, times[i]);
       }
       sort(results.begin(), results.end());
       double fastest = results[0];
       float num_MBs_copied = numMB*divisions[a];
       printf("Size of array slice: %f --- MB     Bandwidth:  %6.10lf GB/s",num_MBs_copied, pow(10,-9)*(num_MBs_copied)/((float) fastest/cycles_per_second));
       cout<<" "<<endl;
 //      printf("The median time to copy across all iterations is: %f nanoseconds\n", results[ITERATIONS/2]*cycle_time);
 //      printf("The minimum time to copy accross all iterations is: %1.13lf nanoseconds\n", ((float) fastest)*cycle_time); 
       
    }
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void inline GetElapsedTime(uint64_t *times, double size) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;

    asm volatile (
                 "CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                 );


    asm volatile (
                 "RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t"
                 : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                 );
    
    for (int i = 0; i < ITERATIONS; i++) {

    
          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          /*int position = (rand() % (int)(sizeof(second_array) - size - 1));
          for (int ind=position; ind<(position+size); ind++){
             unsigned char a = second_array[ind];
             if (a==11){ //just to make the compiler happy
            	 cout<<a<<endl;
             }
          }*/

          std::memcpy(&first_array, &second_array, size);

          // No code because we are measuring only the 
          // timing overhead

          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
          times[i] = end - start;
    }
    
    return;

   

}
