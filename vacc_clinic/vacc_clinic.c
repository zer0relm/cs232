/*
 * Name: AJ Vrieland (ajv234)
 * Date: 3-22-21
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DEBUG 1

// #define NUM_VIALS 30 // Given number
#define NUM_VIALS 10 //so it doesn't run forever when writing code
#define SHOTS_PER_VIAL 6
// #define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL) 
#define NUM_CLIENTS 10 //so it doesn't run forever when writing code
#define NUM_NURSES 10
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4


/* global variables */
pthread_mutex_t client_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nurse_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t nurse_reg;
sem_t client_reg;
int status;
long int client_array[5];
int count = 0; 
int in = 0;
int out = 0;
int usable_vials = NUM_VIALS;

int get_rand_in_range(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

char *time_str;
char *curr_time_s() {
    time_t t;
    time(&t);
    time_str = ctime(&t);
    // replace ending newline with end-of-string.
    time_str[strlen(time_str) - 1] = '\0';
    return time_str;
}

// lower and upper are in seconds.
void walk(int lower, int upper) {
    // TODO: fill in code here.  Use usleep() and get_rand_in_range() from
    // above.
    sleep(get_rand_in_range(lower, upper));

}

// arg is the nurses station number.
void *nurse(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: nurse %ld started\n", curr_time_s(), id);
    while(usable_vials != 0){
        walk(1, 3);
        pthread_mutex_lock(&nurse_mutex);
        usable_vials--;
        pthread_mutex_unlock(&nurse_mutex);
        //for(int i = 0; i < 6; i++){
            while( 0 != pthread_mutex_lock(&client_buffer_mutex)){

            }
            if (out != in){
                
                    int working_client = client_array[out];
                    fprintf(stderr, "%s: nurse %ld, is vaccinating client %i\n", curr_time_s(), id, working_client);
                    out = (out + 1) % 5;
                    count--;
                pthread_mutex_unlock(&client_buffer_mutex);

            }
        //}
    }

    fprintf(stderr, "%s: nurse %ld is done\n", curr_time_s(), id);
    pthread_exit(NULL);
}

void *client(void *arg) {
    long int id = (long int)arg;
    sem_wait(&client_reg);
    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);
    walk(3, 10);
    fprintf(stderr, "%s: client %ld is regestering\n", curr_time_s(), id);
    walk(3, 10);
    
    while(0 != pthread_mutex_lock(&client_buffer_mutex)){
        
    }
    if (out != ((in + 1) % 5) || out == in){
#if DEBUG
    fprintf(stderr, "in is: %i\ncount is: %i\n", in, count);
#endif
        client_array[in] = id;
        in = (in + 1) % 5;
        count++;
        fprintf(stderr, "%s: client %ld has registered, and is waiting to get a vaccine\n", curr_time_s(), id);
        pthread_mutex_unlock(&client_buffer_mutex);
    }else{
        
#if DEBUG
    fprintf(stderr, "BUFFER is full\nin is: %i\ncount should equal 4 = %i\n", in, count);
#endif
    }
    
     
    

    fprintf(stderr, "%s: client %ld leaves the clinic!\n", curr_time_s(), id);
    sem_post(&client_reg);

    pthread_exit(NULL);
}

int main() {
    
    status = sem_init(&nurse_reg, 0, 4);
    status = sem_init(&client_reg, 0, 4);
    srand(time(0));
    pthread_t nurse_threads[NUM_NURSES];
    pthread_t client_threads[NUM_CLIENTS];

    pthread_create(&client_threads[0], NULL, client, NULL);
    pthread_create(&nurse_threads[0], NULL, nurse, NULL);
    // for(int i = 0; i < NUM_CLIENTS; i++){
    //     walk(0, 1);
    //     void *number = (void *)(intptr_t)i;
    //     pthread_create(&client_threads[i], NULL, client, number);
    // }
    // for(int i = 0; i < NUM_NURSES; i++){
    //     void *number = (void *)(intptr_t)i;
    //     pthread_create(&nurse_threads[i], NULL, nurse, number);
    // }
    

    pthread_exit(0);
}
