#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
// #include <unistd.h>
#include <string.h>

// Signal handler for SIGALRM
void delete_file(int signum) {
    if (signum == SIGALRM) {
        // Delete the file
        if (remove("JUJU.txt") == 0) {
            printf("File 'JUJU' deleted successfully.\n");
            exit(2);
        } else {
            perror("File 'JUJU' deletion failed");
            exit(2);
        }
    }
}

int main() {
    // Register the signal handler for SIGALRM
    int newSig = signal(SIGALRM, delete_file);
    if ( newSig== SIG_ERR) {
        perror("Signal handler registration failed");
        return 1;
    }

    // Create the 'JUJU' file on drive C (/tmp in this case)
    FILE *file = fopen("JUJU.txt", "w");
    if (file == NULL) {
        perror("File creation failed");
        return 1;
    }
    fprintf(file, "This is the JUJU file.\n");
    fclose(file);

    printf("File 'JUJU' created.\n");

    // Schedule an alarm to delete the file after 5 seconds
    alarm(5);

    // Keep the program running to allow the alarm to trigger
    while (1) {
        sleep(1);
    }

    return 0;
}
