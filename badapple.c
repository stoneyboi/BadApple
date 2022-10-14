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
    int nodeNum = -1;
    char num[65];
    printf("Please enter the amount of nodes you would like: \n");
    scanf("%d", &k);
    printf("Spawning %d nodes...\n", k);
    int nodeArray[k]; // array to keep track of pids and where the apple is 
    nodeArray[0] = getpid(); //parent pid: the sender node
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
        apple = nodeArray[0]; // give to parent after all children have been created
    }
    while(apple >= 0){
    if(apple == nodeArray[0] ){ //parent 
        int node = 0;
        char message[50] = " ";
        
        printf("Please enter what node you would like to send a message to:\n");
        scanf("%d", &node);
        printf("Please enter the message you would like to send:\n");
        fflush(stdin);
        fgets(message, sizeof(message), stdin);
        printf("Node %d: ", node);
        puts(message);
        sprintf(num, "%d: ", node);
        strcat(num, message);
        nodeNum = 1;
        apple = nodeArray[nodeNum];
    }
    if(apple == nodeArray[nodeNum]){
        printf("Apple is with node %d\n", nodeNum);
        printf("Reading Message...\n");
        char head = num[0];
        if (atoi(&head) == nodeNum){ // if found correct node
            printf("Correct Node found!\n");
            printf("message: %s\n", num);
            nodeNum = -1;
            apple = nodeArray[0]; //send back to parent
        }
        else{ // if not for this node 
            printf("Passing Message Along...\n");
            nodeNum++;
            apple = nodeArray[nodeNum];
        }
    }
    }
   
return 0;
}
void sigInterrupt (int sigNum)
{
    printf (" received. Terminating Program...\n");
    exit(0);
return;
}