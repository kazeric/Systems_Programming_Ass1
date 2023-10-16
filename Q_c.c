#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // Fork the current process
    pid_t child_pr = fork();

    if (child_pr == -1) {
        // check if fork has failed 
        perror("fork");
        return 1;
        //child processes are assigned a value of 0
    } else if (child_pr == 0) {
        printf("Child process: My PID is %d\n", getpid());
    } else {
        // the parent processes are known by their original pid 
        printf("Parent process: My PID is %d, Child's PID is %d\n", getpid(), child_pr);
    }

    return 0;
}
