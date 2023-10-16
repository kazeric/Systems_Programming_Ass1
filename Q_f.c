#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int x = 0;
int choice;

void handle_sigusr1(int sig){
    printf("what would you like to listen to \n 1.HELLO\n 2.SAVED \n 3.HAPPY \n ");

}


int main(){
    //create 2 functions to communicate between each other 
    int pid = fork();
    //check if fork worked or failed
    if(pid == -1){
        return 1;
    }
    //in the child process 

    if(pid == 0){
        //ask what if they want to listen to music
        printf("do you want to play a song?\n 1.Yes\n 2.No\n");
        scanf("%d", &choice);
        //send a signal to print the list once they choose yes 
        if(choice==1){
        kill(getppid(), SIGUSR1);
        }else if (choice!=1){
            kill(getppid(),SIGTERM);
            exit(2);
        }
    }
    else {
        //create the signal struct to handle the singal sent from the child process 
        struct sigaction sa = { 0 };
        sa.sa_flags= SA_RESTART;
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1,&sa,NULL );

        //wait for the child process 
        wait(NULL);
        scanf("%d", &x);
        if(x == 1){
            printf("Playing Hello by Adel... \n");
            x=4;
        }
        else if(x==2){
            printf("Playing Saved by Khalid... \n");
            x=4;
        }
        else if(x==3){
            printf("Playing Happy by NF ... \n");
            x=4;

        }
        else{
            printf("Not playing anything\n");
            x=4;

        }

        }

    return 0;
}