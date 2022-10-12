#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define READ 0
#define WRITE 1

void sigInterrupt(int); //Ctrl + C signal handler
int apple = 1; 

int main()
{
    signal(SIGINT, sigInterrupt);
    //char message[100];
    int k = 0;
    int flag = 0;
    printf("Please enter the amount of nodes you would like: \n");
    scanf("%d", &k);
    printf("Spawning %d nodes...\n", k);
    int nodeArray[k]; // array to keep track of pids and where the apple is 
    nodeArray[0] = getpid(); //parent pid: the sender node
    apple = nodeArray[0]; // gives apple to sender to start
    int fd[2];
    int pipeCreationResult;
    printf("Node 0 (Parent): %d\n", nodeArray[0]);
    for(int i = 1; i < k; i++) //the parent will create the pipe and the child. 
    {
        pipeCreationResult = pipe(fd); //creates pipe inside parent
        if(pipeCreationResult < 0)
        {
        perror("Failed pipe creation\n");
        exit(1);
        }
        int pid = fork();
        
        if(pid != 0)
        {// parent process exits after creating pipe and child
            break;
        }
        else
        { //child process
            close(fd[WRITE]); //closes childs ability to write back to papa "OH NO PAPPPPPAAAAAAAAA"
            nodeArray[i] = getpid();
            printf("Node %d created, pid: %d\n", i, nodeArray[i]);
        }
    }
    if(getpid() ==  nodeArray[k-1]) //end of the for loop
    {
        printf("All %d Nodes have been created\n", k);
        flag = 1;
    }

    //TODO: Make Code into loop so that the nodes search 
    if(getpid() == nodeArray[0] ){ //parent 
    //TODO: Add Apple Variable and pass it on once message is created
        int node = 0;
        char message[50] = " ";
        printf("Please enter what node you would like to send a message to:\n");
        scanf("%d", &node);
        printf("Please enter the message you would like to send:\n");
        fflush(stdin);
        fgets(message, sizeof(message), stdin);
        printf("Node %d: ", node);
        puts(message);
    }

    //TODO: Add code for children
return 0;
}
void sigInterrupt (int sigNum)
{
    printf (" received. Terminating Program...\n");
    exit(0);
return;
}