
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <glob.h>
#include <signal.h>

#define MAX_CMD_LEN 200
#define MAX_HISTORY_SIZE 20
#define MAXLEN 200
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\n"


struct history{
    char cmd[MAX_CMD_LEN];
    struct history *next;
};

struct alias
{
    char alias[MAXLEN/2],cmd[MAXLEN/2];
    struct alias *next;
};



void command_handler(char *args[],int,pid_t*);
void command_pipe(char **strings,int,pid_t*);
char **lsh_split_line(char *line);
int input(char* str);
int history(struct history **history_head,char* str,char*);
int alias(char**,char*,struct alias**);
void add_history(struct history **history_head,const char* str);
char*** count_strs(char*);
void free_history(struct history *history_head);
void free_alias(struct alias *history_head);
char* get_alias(struct alias *alias_list, char *alias);
