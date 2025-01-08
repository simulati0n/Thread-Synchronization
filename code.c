#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_PHILOS 5
pthread_mutex_t turn;
pthread_cond_t condVar;
int nextIndex = 0;

void thinking(int index) {
    printf("Philosopher #%d: starts thinking\n", index + 1); //I add 1 to the index when printing philosopher related information to refer to their position
    //rather than their array index to make it more realistic.

    int random_ms = (rand() % 500) + 1;  
    usleep(random_ms * 1000);  // Convert milliseconds to microseconds
    printf("Philosopher #%d: ends thinking\n", index + 1);
}

void pickUpChopsticks(int index) {
    printf("Philosopher #%d: picked up chopsticks\n", index+1);
}

void eating(int index) {
    printf("Philosopher #%d: starts eating\n", index + 1);
    int random_ms = (rand() % 500) + 1;  
    usleep(random_ms * 1000); 
    printf("Philosopher #%d: ends eating\n", index + 1);
}

void putDownChopsticks(int index) {
    printf("Philosopher #%d: releases chopsticks\n", index+1);
}

void* philosopherThread(void* arg) {
    int index = *((int*)arg);
    int eatCount = 0;

    while (eatCount < 1) {
        thinking(index);

        pthread_mutex_lock(&turn);
        while (nextIndex != index) {
            pthread_cond_wait(&condVar, &turn);
        }

        pickUpChopsticks(index);
        eating(index);
        putDownChopsticks(index);
        
        nextIndex = (nextIndex + 1) % NUM_PHILOS;
        pthread_cond_broadcast(&condVar);
        pthread_mutex_unlock(&turn);

        eatCount++;
    }
    return NULL;
}

int main() {
    srand(time(NULL)); 
    int n = 1;
    for (int loop = 0; loop < n; loop++) {  // Execute the dining process n times
    
    pthread_mutex_init(&turn, NULL);
    pthread_cond_init(&condVar, NULL);

    pthread_t philosophers[NUM_PHILOS];
    int indices[NUM_PHILOS];

    for (int i = 0; i < NUM_PHILOS; i++) {
        indices[i] = i;
        pthread_create(&philosophers[i], NULL, philosopherThread, &indices[i]);
    }

    for (int i = 0; i < NUM_PHILOS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    pthread_mutex_destroy(&turn);
    pthread_cond_destroy(&condVar);
        printf("------------------------------------Completed dining round #%d------------------------------------------------\n", loop + 1); 
    }

    return 0;
}