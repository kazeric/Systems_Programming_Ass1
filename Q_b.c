#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main() {

    //we can use a terminal command to find the process from its name ie Nyonga.sh 
    char command[256] =  "ps aux | grep -w Nyonga.sh | grep -v grep";
    //to store the result found
    char result[1024];


    // we can then use a pipe to run the command and catch its output
    FILE *pipe = popen(command, "r");


    //check for any failures with opening pipe
    if (pipe == NULL) {
        perror("pipe failed to open");
        return 1;
    }

    // Read the output of the command while checking  if the reading was successful
    if(fgets(result, sizeof(result), pipe) != NULL) {   //confirm that the process is found
        int pid;
        printf("\nFound process: %s \n", result);
        //retreive the pid
        sscanf(result, "%*s %d", &pid);
        //send a signal to terminate the pid while cheking if the signal was sent successfully 
        if(kill(pid,SIGTERM)==0){
            printf("Nyonga was successfully killed\n");
        }else{
            printf("Nyonga is a real problem, it couldnt be killed\n");
        }
        
    }
    else {
        //in the case Nyonga isnt active 
        printf("Process not found\n");
    }

    // Close the pipe once done 
    pclose(pipe);

    return 0;
}
