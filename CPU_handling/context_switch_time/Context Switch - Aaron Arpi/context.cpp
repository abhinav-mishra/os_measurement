#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdio.h>
#include <python2.7/Python.h>
#define ITERATIONS 1
#define KB(x)   ((x) << 10)
#define MB(x)   ((x) << 20)


using namespace std;

double avg ( vector<double>& v );
double variance ( vector<double>& v , double mean );
void inline GetElapsedTime();
vector<double> times;
double inline GetOverhead();
void Loop(void);
char* mode;
int numswitches=300;

float cycles_per_second = 2.5*1000*1000*1000;  //2.5*10^9 = 2.5GHZ
float cycles_per_msecond = 2.5*1000*1000;
int numlines = 500000;

int main(int argc,char *argv[])
{
    mode = argv[1]; // r or s
    int i;
    vector<double> avg_vec;
    vector<double> var_vec;
    
    GetElapsedTime();
    cout<<"\t"<<"File Size(MB)"<<"\t"<<"avg read time ms"<<"\t"<<"variance ms"<<endl;
    cout<<"Trial: "<<i<<"   Time Per Switch"<<endl;
    for (int i = 0; i < times.size(); i++) {
    	cout<<"    "<<i<<"      "<<times[i]<<endl;
    }
    double ave = avg(times);
    double va = variance(times,ave);


    cout<<"Average Time Per Context Switch: "<<ave<<"  Variance: "<<va<<endl;


    return 0;
}

double avg ( vector<double>& v )
{
        double return_value = 0.0;
        int n = v.size();
       
        for ( int i=0; i < n; i++)
        {
            return_value += v[i];
        }
       
        return ( return_value / n);
}
//****************End of average funtion****************


//Function for variance
double variance ( vector<double>& v , double mean )
{
        double sum = 0.0;
        double temp =0.0;
        double var =0.0;
       
        for ( int j =0; j < v.size(); j++)
        {
            temp = pow((v[j] - mean),2);
            sum += temp;
        }
       
        return var = sum/(v.size());
}

void inline GetElapsedTime() {
    unsigned long flags;uint64_t start, end;
    int i; unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;
    double overhead = GetOverhead();

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
    

          
      for (int rep=0; rep<30;rep++){
          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          system("python contextswitch.py");


 
          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);


          times.push_back(((float)(end - start)/(cycles_per_msecond*numswitches)));


      }    

    return;
}

double inline GetOverhead() {
    unsigned long flags;uint64_t start, end;
    int i; unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
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

          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

    
          system("python client.py 10000000 r 0");

          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
          return ((float)(end - start)/(cycles_per_msecond));
}

