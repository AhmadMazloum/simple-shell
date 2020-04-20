#include <iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

// A function that Write in a file "Child Terminated" whenever a child is terminated.
void childTerm(int dummy){
    FILE * fptr = fopen("/home/ahmad/Desktop/shell/children.txt","a");
    fputs("Child Terminated \n", fptr);
}

int main()
{
    // openning the file with "w" each time in the beginning of the program to delete any contents
    FILE * fptr = fopen("/home/ahmad/Desktop/shell/children.txt","w");
    string in;
    char * words [32];
    bool flag = 0;
    int i = 0;
    int waitInt;

    while(1){
        // Signal funtion that call the chilTerm function whenever a child is terminated
        signal(SIGCHLD,childTerm);
        i = 0;
        flag = 0;
        // Setting all the array locations to 0 not to have any garbage from the previous line
        memset(words,NULL,sizeof(words));
        getline(cin, in);
        char * token;
        token = strtok((char*) in.c_str(), " ");
        // putting the words of the line in an array each word in a place to process it later
        while(token != NULL){
            words[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        // checking if the command is exit to exit.
        if(strcmp(words[0],"exit") == 0){
            exit(0);
        }
        // checking if its a background process or not by checking the last word in the command with "&" and setting the flag accordingly
        if(strcmp(words[i-1],"&") == 0){
            flag = 1;
            words[i-1] = NULL;
        }
        // forking to let the parent keep taking the command lines and the children process to make the commands
        pid_t pid = fork();
        // checking if its the child process
        if(pid == 0){
            // execvp to execute the command and checking if its value == '-1' to display an error
            if(execvp(words[0],words) == -1)
                perror("exec");

        }
        // checking if we're in the parent process and its not a background process to wait until the child is terminated
        else if (pid > 0 && flag == 0)
        {
                //wait(NULL);
                waitpid(pid, NULL,0);
        }
        // reseting the flag to the default that's is not a background process
        flag = 0;
        cout <<"\n";
    }
    return 0;
}
