#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
    //this is a special data type used to represent a directory in c 
    DIR *snap;
    //in C this is a special structure that represents directory entries whether theyre files or directoris 
    struct dirent *entry;
    //to store file paths 
    char path[256];

    // Open the /proc directory. this is a special directory that keeps track of the current state of the kernel
    snap = opendir("/proc");

    //checking if any errors when opening the directory
    if (snap == NULL) {
        perror("opendir");
        return 1;
    }

    // Reading the /proc directory 
    while ((entry = readdir(snap)) != NULL) {
        //filter out processes directories which start with numeric values ie process ids 
        if (isdigit(entry->d_name[0])) {
            //prints out a formated path into the path variable
            snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

            // Open the stat file for this process
            FILE *stat_file = fopen(path, "r");

            if (stat_file) {    
                int pid, ppid, priority;
                char comm[128];

                // Parse the required information from the stat file 
                //the multiple %* are used to skip uneccesaru details 
                fscanf(stat_file, "%d %s %*s %d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %d", &pid, comm, &ppid, &priority);
                fclose(stat_file);
    

                // Print the information
                printf("Process ID: %d\n", pid);
                printf("Priority: %d\n", priority);
                printf("Parent ID: %d\n", ppid);
                printf("Command: %s\n", comm);
                printf("\n");
            }
        }
    }

    closedir(snap);
    return 0;
}
