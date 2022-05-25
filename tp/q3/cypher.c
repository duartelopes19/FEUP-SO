#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main() {
    int pipefds1[2], pipefds2[2];
    int returnstatus1, returnstatus2;
    int pid;
    char pipe1writemessage[100];
    char pipe2writemessage[100];
    char readmessage[100];

    scanf("%[^'\n']s", pipe1writemessage);

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
    pid = fork();

    if (pid != 0) { //Parent process
        close(pipefds1[0]); // Close the unwanted pipe1 read side
        close(pipefds2[1]); // Close the unwanted pipe2 write side
        printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writemessage);
        write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
        read(pipefds2[0], readmessage, sizeof(readmessage));
        printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);
    }
    else { //child process
        close(pipefds1[1]); // Close the unwanted pipe1 write side
        close(pipefds2[0]); // Close the unwanted pipe2 read side
        read(pipefds1[0], readmessage, sizeof(readmessage));
        printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);
        //process the message recieved to pipe2writemessage
        //----read from file cypher.txt to array arr----
        FILE* inp;
        char arr[100][100];
        int i = 0;
        int word_count = 0;
        char c;
        int char_count = 0;

        inp = fopen("cypher.txt", "r");

        while ((c = fgetc(inp)) != EOF) {
            if (c == ' ' || c == '\n') {

                arr[word_count][char_count] = '\0'; //Terminate the string
                char_count = 0; //Reset the counter.
                word_count++;
            }
            else {
                arr[word_count][char_count] = c;

                if (char_count < 99)
                    char_count++;
                else
                    char_count = 0;
            }
        }
        //----------------------------
        printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
        write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
    }
    return 0;
}
