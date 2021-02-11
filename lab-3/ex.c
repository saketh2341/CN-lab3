#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/sem.h>
#include<semaphore.h>

void *reader(void *arg){
   
  
    printf("Inside the thread\n"); 
  
    // exit the current thread 
    
}

int main(){

     pthread_t rthread;
    pthread_create(&rthread, NULL, reader, 0);

    printf("%s","first");

}