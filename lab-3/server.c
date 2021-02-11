
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

#define NPROCESS 6

struct msgbuf{
    long type;
    char text[1024];
};

pthread_mutex_t lck;

int processids[6][2];
int sz = 0;

void *pids(void *arg){
    int *id = (int *)arg;
    int i = 0;
    struct msgbuf msgp;
    while(1){
        msgrcv(*id, &msgp, 1024, 1, 0);
        pthread_mutex_lock(&lck);
        processids[sz][0] = atoi(msgp.text);
        processids[sz][1] = *id;
        sz++;
        pthread_mutex_unlock(&lck);
    }
    pthread_exit(NULL);
}

int main(){
    printf("======Server=====\n");
    memset(processids, 0, sizeof(processids));
    pthread_mutex_init(&lck, NULL);
    pthread_t t1, t2;
    key_t key1 = ftok("key1.txt", 69);
    key_t key2 = ftok("group.txt", 70);
    key_t key3 = ftok("key2.txt", 89);
    int id1 = msgget(key1, IPC_CREAT|0666);
    int id2 = msgget(key2, IPC_CREAT|0666);
    int id3 = msgget(key3, IPC_CREAT|0666);
    pthread_create(&t1, NULL, pids, &id1);
    pthread_create(&t2, NULL, pids, &id3);
    
    while(1){
        struct msgbuf msgpx;
        msgrcv(id2, &msgpx, 1024, 1, 0);
        char buffer[1024];
        memset(buffer, 0, 1024);
        strcpy(buffer, msgpx.text);
        char process[1024];
        int size = 0;
        memset(process, 0, sizeof(process));  
        int i;
        for(i = 0; i < strlen(buffer); i++){
            if(buffer[i] == ';'){
                i++;
                break;
            }
            else {
                process[size] = buffer[i];
                size++;
            }
        }
        int pid = atoi(process);
        printf("msg from process: %d\n", pid);
        char msg[1024];
        memset(msg, 0, sizeof(msg));
        int size2 = 0;
        for(; i < strlen(buffer); i++){
            msg[size2] = buffer[i];
            size2++;
        }
        for(int j = 0; j < sz; j++){
            if(pid == processids[j][0])continue;
            else {
                msgpx.type = processids[j][0];
                strcpy(msgpx.text, msg);
                msgsnd(processids[j][1], &msgpx, 1024, 0);
                printf("Sent to process: %d thorugh msgq: %d\n", processids[j][0], processids[j][1]);
            }
        }
    }
}