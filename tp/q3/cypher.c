#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define MESSAGE_BUFFER_SIZE 1025

int main() {
    //----read from file cypher.txt to array cypher----
    FILE*  stream;
    char cypher[100][100];
    int word_count = 0;
    int char_count = 0;
    char c;

    stream = fopen("cypher.txt", "r");
    if(stream == NULL) {
      perror("Error");
      return EXIT_FAILURE;
    }
    
    while ((c = fgetc(stream)) != EOF) {
        if (c == ' ' || c == '\n') {

            cypher[word_count][char_count] = '\0'; //Terminate the string
            char_count = 0; //Reset the counter.
            word_count++;
        }
        else {
            cypher[word_count][char_count] = c;

            if (char_count < 99)
                char_count++;
            else
                char_count = 0;
        }
    }

    int pipefds1[2], pipefds2[2];
    int returnstatus1, returnstatus2;

    returnstatus1 = pipe(pipefds1);

    if (returnstatus1 == -1) {
        printf("Unable to create pipe 1 \n");
        return 1;
    }
    returnstatus2 = pipe(pipefds2);

    if (returnstatus2 == -1) {
        printf("Unable to create pipe 2 \n");
        return 1;
    }

    char normal_message[MESSAGE_BUFFER_SIZE];
    char coded_message[MESSAGE_BUFFER_SIZE];
    int isWriting = 1;
    int isReading = 1;
    int pid = fork();

    if (pid == -1) { // error in fork
        perror("Error");
        return EXIT_FAILURE;
    }
    else if (pid == 0) { // child process
        close(pipefds1[1]); // Close the unwanted pipe1 write side
        close(pipefds2[0]); // Close the unwanted pipe2 read side

        while (isReading) {
            memset(normal_message, 0, MESSAGE_BUFFER_SIZE);
            isReading = read(pipefds1[0], normal_message, MESSAGE_BUFFER_SIZE);
            if(isReading == 0) { close(pipefds1[0]); } // reached EOF
            else if (isReading == -1) { 
                perror("Read child");
                return EXIT_FAILURE; 
            }
            // deal with input 
            memset(coded_message, 0, MESSAGE_BUFFER_SIZE);
            for (int i = 0; normal_message[i] != 0; i++) {
                if(isalpha(normal_message[i])) {
                    char temp[100];
                    memset(temp, 0, 100);
                    int j = 0;
                    while(isalpha(normal_message[i+j])){
                        temp[j] = normal_message[i+j];
                        j++;    
                    }

                    char oldsub[100];
                    char newsub[100];
                    bool found = false;

                    for(int k = 0; k < word_count; k++) {
                        if(k%2==0){
                            strcpy(oldsub, cypher[k]);
                            strcpy(newsub, cypher[k+1]);
                        }
                        else{
                            strcpy(oldsub, cypher[k]);
                            strcpy(newsub, cypher[k-1]);
                        }
                        
                        if(strcmp(temp, oldsub)==0){
                            i += strlen(oldsub) - 1;
                            strcat(coded_message, newsub);
                            if(k%2==0){
                                k++;
                            }
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        strcat(coded_message, temp);
                        i += strlen(temp) - 1;
                    }
                }
                else {
                    strncat(coded_message, (normal_message + i), 1);
                }
            }

            
            if(write(pipefds2[1], coded_message, MESSAGE_BUFFER_SIZE) == -1) {
                perror("Write child");
                return EXIT_FAILURE; 
            }
            else { close(pipefds1[1]); } // Reached EOF
        }

        exit(0);

    }
    else { // parent process
        close(pipefds1[0]); // Close the unwanted pipe1 read side
        close(pipefds2[1]); // Close the unwanted pipe2 write side

        while (isWriting || isReading) {
            if(isWriting) {
                memset(normal_message, 0, MESSAGE_BUFFER_SIZE);
                isWriting = (fgets(normal_message, MESSAGE_BUFFER_SIZE, stdin) != NULL);
                if(isWriting) {
                    if(write(pipefds1[1], normal_message, MESSAGE_BUFFER_SIZE) == -1) {
                        perror("Write parent");
                        return EXIT_FAILURE; 
                    }
                }
                else { close(pipefds1[1]); } // Reached EOF
            }

            isReading = read(pipefds2[0], coded_message, MESSAGE_BUFFER_SIZE);
            if(isReading == 0) { close(pipefds2[0]); } // reached EOF
            else if (isReading == -1) { 
                perror("Read parent");
                return EXIT_FAILURE; 
            }
            else { printf("%s", coded_message); }
        }
    }

    int status;
    wait(&status);
    
    return EXIT_SUCCESS;
}
