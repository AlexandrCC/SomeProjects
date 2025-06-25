#include <stdio.h>
#include <unistd.h>
#include "pthread.h"
#include "semaphore.h"

#define STOP 4
#define N 15                                  // встановлення розміру буферу
#define use_delays 1                            // налаштування затримок

#if use_delays
#define producer_delay 2000 - flag * 1900
#define consumer_delay 2000 + flag * 1900
#define untyped__delay 8000
#else
#define producer_delay 0
#define consumer_delay 0
#define untyped__delay 0
#endif

int stack[N];
int top = -1;
int break_count = 0, flag = 1;

int getElement() {
    return stack[top--];
}

void addElement() {
    top++;
    if (top == 0)
        stack[top] = 0;
    else
        stack[top] = stack[top - 1] + 1;
}

// атомарні змінні
int ivar1 = 0;
int ivar2 = 0;

unsigned uvar1 = 0; 
unsigned uvar2 = 0;

long lvar1 = 0;
long lvar2 = 0;

long unsigned luvar1 = 0;
long unsigned luvar2 = 0;

FILE* Fp;

// оголошення потоків
pthread_t p1;
pthread_t p2;
pthread_t p3;
pthread_t p4;
pthread_t p5;
pthread_t p6;

// оглошення і ініціалізація бар'єру, семафору, м'ютексу та сигнальних змінних
pthread_barrier_t bcr2;
sem_t scr1;
pthread_mutex_t mcr1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sig21 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut_sig21 = PTHREAD_MUTEX_INITIALIZER;
int sig_p2_flag = 0, sig_p5_flag = 0;

void AtomUse(int thread_number)
{
    fprintf(Fp, "\n THREAD %d:\n", thread_number);
    fprintf(Fp, "Using int atomic vars:%d %d\n", ivar1, ivar2);
    fprintf(Fp, "Using unsigned atomic vars:%u %u\n", uvar1, uvar2);
    fprintf(Fp, "Using long atomic vars: %ld, %ld\n", lvar1, lvar2);
    fprintf(Fp, "Using long unsigned atomic vars:: %lu, %lu\n", luvar1, luvar2);
}


void AtomMod(int thread_number) 
{
    fprintf(Fp, "\n THREAD %d:\n", thread_number);
    fprintf(Fp, "Untyped  thread %d: Vars modification\n", thread_number);
    fprintf(Fp, "int sub fetch: %d\n", __atomic_sub_fetch(&ivar1, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "int fetch sub: %d\n", __atomic_fetch_sub(&ivar2, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "unsigned and fetch: %u\n", __atomic_and_fetch(&uvar1, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "unsigned xor fetch: %u\n", __atomic_xor_fetch(&uvar2, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "int fetch or: %ld\n", __atomic_fetch_or(&lvar1, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "int fetch nand: %ld\n", __atomic_fetch_nand(&lvar2, thread_number, __ATOMIC_RELAXED));
    fprintf(Fp, "int compare exchange n: %d\n", __atomic_compare_exchange_n(&luvar1, &luvar2, thread_number, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
    fprintf(Fp, "int exchange: before %lu, %lu;", luvar2, luvar1);
    __atomic_exchange(&luvar2, &luvar1, &luvar1, __ATOMIC_RELAXED);
    fprintf(Fp, " after %lu, %lu\n", luvar2, luvar1);
}
void* thread1(void* thread_number) {
    int num = *(int*)thread_number;
    int sem_value;

    while (1)
    {
        if (pthread_mutex_trylock(&mcr1) == 0)
        {

            sem_getvalue(&scr1, &sem_value);

            if (sem_value < N)
            {
                addElement();
                sem_post(&scr1);
                fprintf(Fp, "PRODUSER THREAD %d: SEM VALUE = %d; ELEMENT %d ADDED\n", num, sem_value, stack[top]);
            }
            else
            {

                if (flag >= 0)
                {
                    break_count++;
                    flag = -1;
                    fprintf(Fp, "\n\n\nPRODUSER THREAD %d: FAIL TO ADD ELEMENT STACK IS FULL , BREAK COUNT: %d\n\n\n", num,break_count);
                }
                if (break_count >= STOP) 
                {
                    fprintf(Fp, "\tPRODUSER THREAD %d: STOPING ALL THREADS\n", num);
                    pthread_cancel(p2);
                    pthread_cancel(p3);
                    pthread_cancel(p4);
                    pthread_cancel(p5);
                    pthread_cancel(p6);
                    fprintf(Fp, "PRODUSER THREAD %d: STOPED\n", num);
                    pthread_mutex_unlock(&mcr1);
                    break;
                }
            }
            pthread_mutex_unlock(&mcr1);
        }
        else {
            fprintf(Fp, "\nCR1 LOCKED THREAD %d DOING SOME OTHER JOB\n", num);
        }
        usleep(producer_delay);
    }
    fprintf(Fp, "Producer thread %d: stopped\n", num);
}

void* thread2(void* thread_number) {
    int num = *(int*)thread_number;
    int sem_value;

    while (1)
    {
        if (pthread_mutex_trylock(&mcr1) == 0)
        {

            sem_getvalue(&scr1, &sem_value);

            if (sem_value < N)
            {
                addElement();
                sem_post(&scr1);
                fprintf(Fp, "PRODUSER THREAD %d: SEM VALUE = %d; ELEMENT %d ADDED\n", num, sem_value, stack[top]);
            }
            else
            {

                if (flag >= 0)
                {
                    break_count++;
                    flag = -1;
                    fprintf(Fp, "\n\n\nPRODUSER THREAD %d: FAIL TO ADD ELEMENT STACK IS FULL \n\n\n", num);
                }
                if (break_count >= STOP)
                {
                    fprintf(Fp, "\tPRODUSER THREAD %d: STOPING ALL THREADS\n", num);
                    pthread_cancel(p1);
                    pthread_cancel(p3);
                    pthread_cancel(p4);
                    pthread_cancel(p5);
                    pthread_cancel(p6);
                    fprintf(Fp, "PRODUSER THREAD %d: STOPED\n", num);
                    pthread_mutex_unlock(&mcr1);
                    break;
                }
            }
            pthread_mutex_unlock(&mcr1);
        }
        pthread_mutex_lock(&mut_sig21);
        while (sig_p2_flag == 0) 
        {
            fprintf(Fp, "PRODUSER THREAD %d: WAITING FOR SIGNAL 21\n", num);
            pthread_cond_wait(&sig21, &mut_sig21);
        }
        sig_p2_flag--;
        pthread_mutex_unlock(&mut_sig21);
        AtomMod(num);
        AtomUse(num);
        usleep(producer_delay);
    }
    fprintf(Fp, "Producer thread %d: stopped\n", num);
}

void* thread3(void* thread_number) {
    int num = *(int*)thread_number;
    int sem_value;

    while (1)
    {
        AtomMod(num);
        fprintf(Fp, "\tTHREAD %d wait on barrier\n", num);
        pthread_barrier_wait(&bcr2);
        fprintf(Fp, "\tTHREAD %d PASSED the barrier\n", num);
        pthread_mutex_lock(&mut_sig21);
        sig_p2_flag++;
        fprintf(Fp, "THREAD %d SENT SIGNAL TO THREAD 2\n", num);
        pthread_cond_signal(&sig21);
        pthread_mutex_unlock(&mut_sig21);
        AtomUse(num);

        usleep(untyped__delay);
    }
    fprintf(Fp, "Producer thread %d: stopped\n", num);
}


void* thread4(void* thread_number) {

    int num = *(int*)thread_number;
    int sem_value;

    while (1)
    {
        if (pthread_mutex_trylock(&mcr1) == 0)
        {

            sem_getvalue(&scr1, &sem_value);
            if (sem_value > 0) {
                sem_wait(&scr1);
            }
            else
            {
                pthread_mutex_unlock(&mcr1);
                sem_wait(&scr1);
                pthread_mutex_lock(&mcr1);
            }
            int elem = getElement();
            sem_getvalue(&scr1, &sem_value);
            fprintf(Fp, "CONSUMER THREAD %d: SEM VALUE = %d; element %d taken\n", num, sem_value, elem);
            if (sem_value <= 0) {
                if (flag <= 0) {
                    break_count++;
                    flag = 1;
                    fprintf(Fp, "\n\n\nCONSUMER THREAD %d: FAIL TO TAKE ELEMENT STACK IS EMPTY, BREAK COUNT: %d \n\n\n", num, break_count);
                }
                if (break_count >= STOP) {
                    fprintf(Fp, "CONSUMER thread %d: trying stop all threads\n", num);
                    pthread_cancel(p1);
                    pthread_cancel(p2);
                    pthread_cancel(p3);
                    pthread_cancel(p5);
                    pthread_cancel(p6);
                    fprintf(Fp, "Consumer thread %d: stopped\n", num);

                    pthread_mutex_unlock(&mcr1);
                    break;
                }
            }
            pthread_mutex_unlock(&mcr1);
        }
        else {
            fprintf(Fp, "\nCR1 LOCKED THREAD %d DOING SOME OTHER JOB\n", num);
        }
        usleep(consumer_delay);
       
    }
    fprintf(Fp, "Consumer thread %d: stopped\n", num);
}

void* thread5(void* thread_number) {

    int num = *(int*)thread_number;
    int sem_value;

    while (1)
    {
        if (pthread_mutex_trylock(&mcr1) == 0)
        {

            sem_getvalue(&scr1, &sem_value);
            if (sem_value > 0) {
                sem_wait(&scr1);
            }
            else
            {
                pthread_mutex_unlock(&mcr1);
                sem_wait(&scr1);
                pthread_mutex_lock(&mcr1);
            }
            int elem = getElement();
            sem_getvalue(&scr1, &sem_value);
            fprintf(Fp, "CONSUMER THREAD %d: SEM VALUE = %d; element %d taken\n", num, sem_value, elem);
            if (sem_value <= 0) {
                if (flag <= 0) {
                    break_count++;
                    flag = 1;
                    fprintf(Fp, "\n\n\nCONSUMER THREAD %d: FAIL TO TAKE ELEMENT STACK IS FULL, BREAK COUNT: %d \n\n\n", num, break_count);
                }
                if (break_count >= STOP) {
                    fprintf(Fp, "CONSUMER thread %d: trying stop all threads\n", num);
                    pthread_cancel(p1);
                    pthread_cancel(p2);
                    pthread_cancel(p3);
                    pthread_cancel(p4);
                    pthread_cancel(p6);
                    fprintf(Fp, "Consumer thread %d: stopped\n", num);

                    pthread_mutex_unlock(&mcr1);
                    break;
                }
            }
            pthread_mutex_unlock(&mcr1);
        }
        else {
            fprintf(Fp, "\nCR1 LOCKED THREAD %d DOING SOME OTHER JOB\n", num);
        }

        pthread_mutex_lock(&mut_sig21);

        while (sig_p5_flag == 0)
        {
            fprintf(Fp, "PRODUSER THREAD %d: WAITING FOR SIGNAL 21\n", num);
            pthread_cond_wait(&sig21, &mut_sig21);
        }
        sig_p5_flag--;
        pthread_mutex_unlock(&mut_sig21);
        AtomUse(num);
        usleep(consumer_delay);

    }
    fprintf(Fp, "Consumer thread %d: stopped\n", num);
}

void* thread6(void* thread_number) {

    int num = *(int*)thread_number;
    int sem_value;

    AtomMod(num);

    while (1)
    {
        fprintf(Fp, "\tTHREAD %d wait on barrier\n", num);
        pthread_barrier_wait(&bcr2);
        fprintf(Fp, "\tTHREAD %d PASSED the barrier\n", num);
        pthread_mutex_lock(&mut_sig21);
        sig_p2_flag++;
        sig_p5_flag++;
        fprintf(Fp, "THREAD %d SENT SIGNAL TO THREAD 2\n", num);\
        fprintf(Fp, "THREAD %d SENT SIGNAL TO THREAD 5\n", num);
        pthread_cond_broadcast(&sig21);
        pthread_mutex_unlock(&mut_sig21);
        AtomUse(num);
        usleep(untyped__delay);

    }
    fprintf(Fp, "Consumer thread %d: stopped\n", num);
}

int main()
{
    if ((Fp = fopen("log.txt", "w")) == NULL)
    {
        perror("Error occurred while opening file");
        return 1;
    }

    pthread_barrier_init(&bcr2, NULL, 2);
    sem_init(&scr1, 0, 0);
    int sem_value;
    sem_getvalue(&scr1, &sem_value);
    fprintf(Fp, "semaphore = %d\n", sem_value);
    for (int i = 0; i < 10; i++) {
        addElement();
        sem_post(&scr1);
    }
    sem_getvalue(&scr1, &sem_value);
    fprintf(Fp, "semaphore = %d\n", sem_value);

    int thread1_number = 1;
    int thread2_number = 2;
    int thread3_number = 3;
    int thread4_number = 4;
    int thread5_number = 5;
    int thread6_number = 6;

    pthread_create(&p1, NULL, &thread1, &thread1_number);
    pthread_create(&p2, NULL, &thread2, &thread2_number);
    pthread_create(&p3, NULL, &thread3, &thread3_number);
    pthread_create(&p4, NULL, &thread4, &thread4_number);
    pthread_create(&p5, NULL, &thread5, &thread5_number);
    pthread_create(&p6, NULL, &thread6, &thread6_number);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);
    pthread_join(p6, NULL);

    fprintf(Fp, "All threads stopped\n");
    fclose(Fp);

    return 0;
}
