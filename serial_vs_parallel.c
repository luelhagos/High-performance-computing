#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h> // for timing
#include <math.h>     /* cos */
#include <unistd.h>
#define MAXTHREADS 100
int N = 1000000;
float S = 0;
int NTHREADS = 8;
pthread_t ID_THREADS[MAXTHREADS]; // for the identifiers of the threads
float RES[MAXTHREADS];            // for the results (partial sums) of the threads

// a function that each thread will run
void *thread_cos(void *thread_id)
{
    int id, i;
    float sum = 0;
    id = (int)thread_id; //(int)(*thread_id); * get the content in that location
    for (i = id * (N / NTHREADS); i < (id + 1) * (N / NTHREADS); i++)
        sum = sum + cos(i);
    RES[id] = sum;
    pthread_exit(NULL);
}

float parallel_sumcos(int n)
{
    float sum = 0;
    int i;
    for (i = 0; i < NTHREADS; i++)
        pthread_create(&ID_THREADS[i], NULL, thread_cos, (void *)(i));

    for (i = 0; i < NTHREADS; i++)
        pthread_join(ID_THREADS[i], NULL);

    for (i = 0; i < NTHREADS; i++)
        sum = sum + RES[i];
    return sum;
}

float sumcost(int n)
{
    float sm = 0;
    for (int i = 0; i < n; i++)
        sm = sm + cos(i);
    return sm;
}
/* Time function */
double now()
{
    struct timeval t;
    double f_t;
    gettimeofday(&t, NULL);
    f_t = t.tv_usec;
    f_t = f_t / 1000000.0;
    f_t += t.tv_sec;
    return f_t;
}

int main()
{
    double to, t1, t_s, t_p;
    to = now();
    S = sumcost(N);
    t1 = now();
    t_s = t1 - to;
    printf(" The serial sum of the %d cosi is : %f \n And the time is: %f \n", N, S, t_s);
    to = now();
    float p = parallel_sumcos(N);
    t1 = now();
    t_p = t1 - to;
    printf(" The parallel sum of the %d cosi is : %f\n And the time is %f\n", N, p, t_p);
    return 0;
}
