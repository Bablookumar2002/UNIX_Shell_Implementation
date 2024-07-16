#include <stdio.h>
#include <string.h>
#include <stdlib.h>			// exit()
#include <unistd.h>			// fork(), getpid(), exec()
#include <sys/wait.h>		// wait()
#include <signal.h>			// signal()
#include <fcntl.h>			// close(), open()

#define MAX_BUFFER_SIZE 100
#define MAX_COMMAND_SIZE 100
#define MAX_PATHSIZE_SIZE 100
#define myflag 1

void changeDirectory(char **Token) {

    if(chdir(Token[1])!=0){
	    printf("Shell: Incorrect command\n");
        
    }
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside change_Directory");
    }

}
char **parseInput(const char *input) {
    if(myflag==0)
    {
        printf("I am inside parse_Input at begin\n");
    }
    int location = 0;
    int buffer_size = 16; // Initial buffer size

    char **Token = (char **)malloc(buffer_size * sizeof(char *));
    if (!Token) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char *input_copys = strdup(input); // Create a copy to avoid modifying the original input
    if (!input_copys) {
        perror("Memory allocation error");
        free(Token);
        exit(EXIT_FAILURE);
    }

    char *token;
    char *token_copy = strdup(input_copys); // Create a copy of the input for strsep
    if(myflag==0)
        {
            printf("Ckeck");
        }
    while ((token = strsep(&token_copy, " ")) != NULL) {
        if (strlen(token) == 0) {
            continue;
        }

        if (location >= buffer_size) {
            buffer_size *= 2; // Double the buffer size if needed
            Token = (char **)realloc(Token, buffer_size * sizeof(char *));
            if (!Token) {
                perror("Memory reallocation error");
                free(input_copys);
                exit(EXIT_FAILURE);
            }
        }
        //ckecking
        if(myflag==0)
        {
            printf("value of location=%d\n",location);
        }
        Token[location++] = strdup(token);
    }

    Token[location] = NULL;

    free(input_copys);
    free(token_copy);
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside parse_Input at end");
    }
    return Token;
}
    

void executeCommand(char **Token) {
    if (strcmp(Token[0], "cd") == 0) {
        changeDirectory(Token);
    } else {
        pid_t child_pid = fork();
        // creating child process
        if (child_pid < 0) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Restore default behavior of signals for the child process
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            if (execvp(Token[0], Token) == -1) {
                printf("Shell: Incorrect command\n");
                exit(EXIT_FAILURE);
            }
            if(myflag==0)
            {
                printf("checking");
            }
        } else {
            int status;
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("Waiting for child process error");
            }
        }
    }
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside execute_Command");
    }
}

void executeSequentialCommands(char **Token)
{	
    int starts_of_cmds = 0;
    int i=0;
    while(Token[i]){
        if(myflag==0)
        {
            printf("value of i=%d\n",i);
        }
        while(Token[i] && strcmp(Token[i],"##")!=0) {
            i++;
        }
        Token[i] = NULL;
        executeCommand(&Token[starts_of_cmds]);
        i++;
        starts_of_cmds = i;
        if(myflag==0)
        {
            printf("value of i=%d\n",i);
        }
    }
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside execute_Sequential_Commands at end");
    }
}




void executeParallelCommands(char **Token) {
    int i = 0;
    int starts_of_cmds = 0;
    //ck and entry
    if(myflag==0)
    {
        printf("I am inside execute_Parallel_Commands at begning");
    }
    while (Token[i]) {
        while (Token[i] && strcmp(Token[i], "&&") != 0) {
            i++;
        }
        
        if (Token[i]) {
            Token[i] = NULL; // Terminate the command
            if (strcmp(Token[starts_of_cmds], "cd") == 0) {
                changeDirectory(&Token[starts_of_cmds]);
            } else {
                pid_t rc = fork();
                if (rc < 0) {
                    perror("Fork error");
                    exit(EXIT_FAILURE);
                } else if (rc == 0) {
                    // Restore default behavior of signals for the child process
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);

                    if (execvp(Token[starts_of_cmds], &Token[starts_of_cmds]) == -1) {
                        printf("Shell: Incorrect command\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            i++; // Move past the "&&" token
            starts_of_cmds = i; // Update the starting location for the next command
        }
    }
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside execute_Parallel_Commands at end");
    }
    // Wait for all child processes to complete
    while (wait(NULL) > 0) {}
}


void executeCommandRedirection(char **Token) {
    int total_no_of_token = 0;
     if(myflag==0)
    {
        printf("I am inside execute_Command_Redirection at start\n");
    }
    for (int i = 0; Token[i] != NULL; i++) {
        total_no_of_token++;
    }

    pid_t rc = fork();
    //ck and fork creating
    if (rc < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // Restore default behavior of signals for the child process
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        int fd = open(Token[total_no_of_token - 1], O_CREAT | O_WRONLY | O_APPEND, 0666);
        if (fd == -1) {
            perror("File open error");
            exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }

        close(fd);

        Token[total_no_of_token - 1] = NULL; // Terminate the command
        if (execvp(Token[0], Token) == -1) {
            printf("Shell: Incorrect command\n");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(rc, &status, 0) == -1) {
            perror("Waiting for child process error");
        }
    }
    //ck and exiting
    if(myflag==0)
    {
        printf("I am inside execute_Command_Redirection at end\n");
    }
}
char cwd[MAX_PATHSIZE_SIZE];     


void signalHandler(int sig) {
    //ck and entry
    if(myflag==0)
    {
        printf("I am inside signal_Handler");
    }
    printf("\n");
    //terminal initiate
	printf("%s$", cwd);
	//clear previous data
	fflush(stdout);
    return;

}


// void signalHandler(int sig) {
//     if (sig == SIGTSTP) {
//         // Handle SIGTSTP (ctrl + z)
//         // Your code here
//     } else if (sig == SIGINT) {
//         // Handle SIGINT (ctrl + c)
//         // Your code here
//     }
// }

void executeCommand(char **Token);
void executeParallelCommands(char **Token);
void executeSequentialCommands(char **Token);
void executeCommandRedirection(char **Token);

int main() {
    signal(SIGTSTP, &signalHandler); // for ctrl + z
    signal(SIGINT, &signalHandler);  // for ctrl + c
    if(myflag==0)
    {
        printf("I am inside main_function");
    }
    while (1) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s$", cwd);
        }

        char *input = NULL;
        size_t size = 0;

        int byte_read = getline(&input, &size, stdin);
        int len = strlen(input);
        input[len - 1] = '\0';
        //checking
        if(myflag==0)
        {
            printf("value of lenght=%d\n",len);
        }
        char **args = parseInput(input);

        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "EXIT") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        int types = 0;
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "&&") == 0) {
                types = 1;
                break;
            } else if (strcmp(args[i], "##") == 0) {
                types = 2;
                break;
            } else if (strcmp(args[i], ">") == 0) {
                types = 3;
                break;
            }
        }
        //checking
        if(myflag==0)
        {
            printf("Value of types=%d\n",types);
        }
        if (types == 1) {
            executeParallelCommands(args);
        } else if (types == 2) {
            executeSequentialCommands(args);
        } else if (types == 3) {
            executeCommandRedirection(args);
        } else {
            executeCommand(args);
        }
        if(myflag==0)
        {
            printf("Value of types=%d\n",types);
        }
        // Free allocated memory for args
        for (int i = 0; args[i] != NULL; i++) {
            free(args[i]);
        }
        free(args);
    }

    return 0;
}
