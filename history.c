#include "types.h"

void add_history(struct history **history_head, const char *cmd) { //add a command to the front history list
    struct history *new_node = malloc(sizeof(struct history));
    if (new_node == NULL) {
        printf("Error: Out of memory\n");
        return;
    }
    strncpy(new_node->cmd, cmd, MAX_CMD_LEN); //copy the command to the cmd of the struct
    new_node->next = *history_head;//attach the new node
    *history_head = new_node; //head now points to new node
}

void print_history(struct history *history_head) {
    int i = 1;
    while (history_head != NULL && i <= 20) { //prints last 20 commands the user has given
        printf("%d. %s\n", i, history_head->cmd);
        history_head = history_head->next;
        i++;
    }
}

int run_com(struct history *history_head,char *token,char *ret)
{
    int x= atoi(token); //this function stores to ret the x command that is in the history list, x is the number of command the user asked for
    if(x>0 && x<21){
    for (int i=0;i<x;i++){
    history_head = history_head->next;
    }
    strcpy(ret,history_head->cmd);
    printf(">%s\n",ret);
    return 1;
    }
    else
    {
    return 0;
    }
}

int history(struct history **history_head,char* str,char *ret)
{
char **tokens;
tokens= lsh_split_line(str);



    if ((strcmp(tokens[0],"myHistory")==0) && tokens[1]==NULL) { //command myHistory 
        print_history(*history_head);//printf history list  
        free(tokens);
        return 1;
    } else if ((strcmp(tokens[0],"myHistory")==0) && tokens[1]!=NULL && tokens[2]==NULL){ //command myHistory $number
        if(run_com(*history_head,tokens[1],ret)){
            //free tokens;
            free(tokens);  
            return 0;
        }
    }
    printf("Bad command given\n");
return -1;
}

void free_history(struct history *history_head) { //function to free history list
    while (history_head != NULL) {
        struct history *temp = history_head;
        history_head = history_head->next;
        free(temp);
    }
}