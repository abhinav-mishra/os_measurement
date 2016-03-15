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
#define ITERATIONS 40
#define KB(x)   ((x) << 10)
#define MB(x)   ((x) << 20)


using namespace std;

double avg ( vector<double>& v );
double variance ( vector<double>& v , double mean );
void inline GetElapsedTime();
vector<vector<double> > times;
double inline GetOverhead();
int numreads=500;
void Loop(void);
char* mode;

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
    for (int i = 0; i < ITERATIONS; i++) {
        double average = avg(times[i]);
        avg_vec.push_back(average);
    	double var = variance(times[i],average);
        int numMBs=i+1;
    	var_vec.push_back(var);
        cout<<numMBs<<"\t"<<avg_vec[i]<<"\t"<<var_vec[i]<<endl;

    }
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
    for (int numMB = 1; numMB <= ITERATIONS; numMB++) {
        	  char str[80];
              strcpy (str,"python maketestfile.py ");
              std::string s = std::to_string(numlines);
              char const *pchar = s.c_str();
              strcat (str,pchar);
             // strcat (str,"are ");
             // strcat (str,"concatenated.");


              numlines = numlines+500000;
              system(str);
              //system("python maketestfile.py "+to_string(numlines));





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
    

          
      for (int rep=0; rep<3;rep++){
          asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     : "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          if (strcmp(mode,"r")==0 | strcmp(mode,"xr")==0){
             char str2[80];
             strcpy (str2,"python client.py ");
             strcat (str2,pchar);
             std::string lines = std::to_string(numreads);
             char const *pchar2 = lines.c_str();
             if (strcmp(mode,"r")==0) {strcat(str2," r ");}
             else {strcat(str2, " xr ");}
             strcat(str2, pchar2);
             //string command= "python client.py "+to_string(numlines)+" r 5";
             //char* command1 = "python client.py 1000 r 5";
             system(str2);
          }
          if (strcmp(mode,"s")==0 | strcmp(mode, "xs")==0){
        	  char str2[80];
        	  strcpy (str2,"python client.py ");
        	  strcat (str2,pchar);
        	  std::string lines = std::to_string(numreads);
        	  char const *pchar2 = lines.c_str();
        	  if (strcmp(mode,"s")==0){
        	     strcat(str2," s ");
        	  }
        	  else {strcat(str2," xs ");}
        	  strcat(str2, pchar2);
        	  system(str2);

               // system("python client.py "+to_string(numlines)+" s 5");
          }
 
          asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t"
                     : "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

          start = (((uint64_t) cycles_high << 32) | cycles_low);
          end = (((uint64_t) cycles_high1 << 32) | cycles_low1);
          if (rep==0) {
        	  vector<double> timevec;
        	  timevec.push_back(((float)(end - start)/(cycles_per_msecond*numreads))-overhead);
        	  times.push_back(timevec);
          }
          else {
            times[numMB-1].push_back(((float)(end - start)/(cycles_per_msecond*numreads))-overhead);
          }

      }    
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
          return ((float)(end - start)/(cycles_per_msecond*numreads));
}

