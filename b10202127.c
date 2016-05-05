#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
/*
 * & in the end, concurrent start the program
 * "history" print the most recently 10 inputs
 * "!!" execute the most recently command
 * "!4" execute the forth input
 */
int main()
{
    const unsigned int maxLength = 10;
    const unsigned int stackCapacity = 80;
    char* result[maxLength];         /*result is procced as command, this command can have at most 10 options*/
    char input[80];                  /*input command can store at most 80 words*/
    char stack[80][stackCapacity];   /*using stack to store history, which can store 80 commands, each commands can have at most 80 words*/
    char tokenBuffer[80];            /*a buffer for string processing*/
    char temp[80];                   /*a buffer for stack, using for string processing*/
    int  pos = 0, i = 0;             /*pos is a count for how many commands have already been used, i is a temp count for loops*/
    pid_t pid, wpid;                 /*pid, when pid=0, means child process, -1 is fail to fork, > 0 invoke wait*/

    while (1) {
        memset(result, 0, sizeof result);  /*clear temp whenever loop begin*/
        memset(temp, 0, sizeof temp);
        memset(tokenBuffer, 0, sizeof tokenBuffer);
        memset(input, 0, sizeof input);
        printf("osh>");
        fgets(input, sizeof input, stdin);
        input[strlen(input) - 1] = 0;

execc:
        strcpy(tokenBuffer, input);

        char* token = strtok(tokenBuffer, " ");

        for (i = 0; token != NULL && i < maxLength; i++)
        {
            result[i] = token;
            strcat(temp, token);
            strcat(temp, " ");
            token = strtok(NULL, " ");
        }

        strcpy(stack[pos], temp);

        /*exit*/
        if(result[0][0] == 'e' && result[0][1] == 'x' && result[0][2] == 'i' && result[0][3] == 't' && result[0][4] == NULL && result[1] == NULL){
            printf("Closed terminal.\n");
            break;
        }

        /*History*/
        if (result[0][0] == 'h' && result[0][1] == 'i' && result[0][2] == 's' && result[0][3] == 't' && result[0][4] == 'o' && result[0][5] == 'r' && result[0][6] == 'y') {
            if (pos < 10) {
                for (i = pos; i >= 0; i--) {
                    printf("%d %s\n", i + 1, stack[i]);
                }
                pos++;
                continue;
            } else {
                for (i = pos; i > pos - 10; i--) {
                    printf("%d %s\n", i + 1, stack[i]);
                }
                pos++;
                continue;
            }
        }

        /* '!+number' command*/
        if (result[0][0] == '!' && result[0][1] != '!' && result[0][3] == NULL && result[1] == NULL) { //0~9 48-57
            if (result[0][1] >= 48 && result[0][1] <= 57)
            {
                if (result[0][2] == NULL)
                {
                    // execute the specified single digit command
                    i = result[0][1] - 48 - 1;
                    if(i>pos){
                        printf("No such a command in the history!\n");
                        continue;
                    }else{
                        memset(input, 0, sizeof input);
                        strcpy(input, stack[i]);
                        printf("Execute %s\n", input);
                        goto execc;
                    }
                }
                else if (result[0][2] >= 48 && result[0][2] <= 57)
                {
                    // execute specified double digit command
                    i = (result[0][1] - 48) * 10 + result[0][1] - 48 - 1;
                    if(i>pos){
                        printf("No such a command in the history!\n");
                        continue;
                    }else{
                        memset(input, 0, sizeof input);
                        strcpy(input, stack[i]);
                        printf("Execute %s\n", input);
                        goto execc;
                    }
                }
                else
                {
                    printf("Wrong format, number is required after \'!\'\n");
                }
            }
            else
            {
                printf("Wrong format, number is required after \'!\'\n");
            }
        }

        /*'!!' command, execute preceding command*/
        if (result[0][0] == '!' && result[0][1] == '!' && result[0][2] == NULL && result[1] == NULL) {
            if(pos==0){
                printf("No such a command in the history\n");
                continue;
            }else{
                memset(input, 0, sizeof input);
                strcpy(input, stack[pos-1]);
                printf("Execute %s\n", stack[pos - 1]);
                goto execc;
            }

        }

        /* execute here */
        pid = fork();
        if(pid==0){
           //child process
           if(execvp(result[0],result)==-1) /*execute normal command here*/
                perror("Wrong command!");
           fflush(stdout);
           exit(EXIT_FAILURE);
        }else if(pid<0)
           perror("Fail while forking");

        pos++;
    }
    return 0;
}
