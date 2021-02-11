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

struct msgbuf{
    long type; 
    char text[1024];
}msgp, msgpx;

void *reader(void *arg){
    int *id = (int *)arg;
    while(1){
        msgrcv(*id, &msgpx, 1024, getpid(), 0);
        printf("Server sent: %s\n", msgpx.text);
    }
}

int main(){
    printf("======Client1=======\n");
    char *name1 = "sem1";
    char *name2 = "groupsem";
    sem_t *id3 = sem_open(name1, O_CREAT, 0666, 1);
    sem_t *id4 = sem_open(name2, O_CREAT, 0666, 1);
    
    key_t key1 = ftok("key1", 69);
    key_t key2 = ftok("group", 70);

    int id1 = msgget(key1, IPC_CREAT|0666);
    int id2 = msgget(key2, IPC_CREAT|0666);

    msgp.type = 1;
    char buffer[1024];
    sprintf(buffer, "%d", getpid());
    strcpy(msgp.text, buffer);
    printf("%s\n",msgp.text);
    sem_wait(id3);
    msgsnd(id1, &msgp, 1024, 0);
    sem_post(id3);
    printf("pid: %d, msgqid: %d\n", getpid(), id1);
    pthread_t rthread;
    pthread_create(&rthread, NULL, reader, &id1);

    while(1){
        printf("Enter a msg:\t\t\t");
        char temp[1024];
        memset(temp, 0, sizeof(temp));
        scanf("%s", temp);
        strcpy(msgp.text, buffer);
        strcat(msgp.text, ";");
        strcat(msgp.text, temp);
        sem_wait(id4);
        msgsnd(id2, &msgp, 1024, 0);
        sem_post(id4);
    }
}   