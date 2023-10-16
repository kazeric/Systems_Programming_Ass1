#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid;
    int status;

    // Fork to create a child process while checking for errors
    if ((child_pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (child_pid == 0) {
        // Child process execution since it has is a ssigned a pid of 0 
        printf("Child process: I am running (PID: %d)\n", getpid());
        sleep(2); // Simulate some work
        exit(42);  // Child exits with status 42
    } else {
        // Parent process sice it uses its original Pid
        printf("Parent process: I am waiting for child (PID: %d)\n", child_pid);

        // Using waitpid() to wait for a specific child
        pid_t terminated_pid = waitpid(child_pid, &status, 0);
        //check for errors in the wait function 
        if (terminated_pid == -1) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                printf("Parent Process: Child process (PID: %d) exited with status: %d\n", terminated_pid, WEXITSTATUS(status));
            }
        }

    }

    return 0;
}
