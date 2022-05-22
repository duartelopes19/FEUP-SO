#include<stdio.h>
#include<unistd.h>

int pipefds1[2], pipefds2[2];
int returnstatus1, returnstatus2;
int pid;
char pipe1writemessage[100];
int i;
char pipe2writemessage[100];
char readmessage[100];
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

printf("Enter a text :");
scanf("%s %d", pipe1writemessage, &i);

if (pid != 0) // Parent process {
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
 printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
 write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
}
