
#include "types.h"

void take_args(char *args[],char **args_aux)
{
  // We look for the special characters and separate the command itself
	// in a new array for the arguments
  int j=0;
  while ( args[j] != NULL){
		if ( (strcmp(args[j],">") == 0) || (strcmp(args[j],"<") == 0) || (strcmp(args[j],">>") == 0) || (strcmp(args[j],"&") == 0)){
			break;
		}
		args_aux[j] = args[j];
		j++;

  }
  args_aux[j] = NULL; //null last token
}

void wild_cards(char** args_aux)
{
  int j;
  glob_t globbuf;
  int flags = GLOB_NOCHECK | GLOB_TILDE;

  for (int i = 0; args_aux[i] != NULL; i++) { //for each token
        if (strchr(args_aux[i], '*') != NULL || strchr(args_aux[i], '?') != NULL) { //if contains * or ?
            if (glob(args_aux[i], flags, NULL, &globbuf) == 0) { //pathnames were found
                for (j = 0; j < globbuf.gl_pathc; j++) {
                    args_aux[i + j] = strdup(globbuf.gl_pathv[j]);//copy pathname
                }
                args_aux[i + j] = NULL;
                i += j - 1;
                globfree(&globbuf);
            }
        }
    }
}

int redirection(char **tokens)
{
    int i = 0, fd, dsc;
    while (tokens[i] != NULL) //this functions opens/creates files for read write /append
    {
        if (!strcmp(tokens[i], "<") || !strcmp(tokens[i], ">") || !strcmp(tokens[i], ">>"))
        {
            char *file = tokens[i + 1];
            if (file == NULL)
            {
                printf("Missing file for redirection");
                return -1;
            }
            if (!strcmp(tokens[i], "<"))
            {
                dsc = STDIN_FILENO;
                fd = open(file, O_RDONLY);
            }
            else if (!strcmp(tokens[i], ">"))
            {
                dsc = STDOUT_FILENO;
                fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
             else if (!strcmp(tokens[i ], ">>"))
            {
                if (file == NULL)
                {
                    printf("Missing file for redirection");
                    return -1;
                }
                dsc = STDOUT_FILENO;
                fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0644);
            }
            if (fd == -1)
            {
                perror("open");
                exit(0);
            }
            close(dsc);
            if (dup2(fd, dsc) == -1)
                exit(0);
            close(fd);
        }
        i++;
    }
     return 0;
}



void fileIO(char * args_aux[],char * tokens[],int bg,pid_t* jobpg)
{
  pid_t pid;



  if((pid = fork()) < 0)
    { // Fork new process
      perror("Failed to create process");
      return ;
    }
    else if(pid==0) //child process
    {
      if(bg) //if its backround command
        setpgid(pid,*jobpg); //process group
      else{
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
      }

      wild_cards(args_aux); //check for wildcards

      if(redirection(tokens)==-1) //check for redirections
        perror("Redirection failed");
      
      free(tokens);


		  if (execvp(args_aux[0],args_aux)<0){ //exec the commands
			  perror("Execvp failed");
      }
      exit(0);
    }
    else
    {
      int status;
      if(bg){
        if(!(*jobpg))
          *jobpg=pid;
        setpgid(pid,*jobpg);

      }
      else{ //wait if not & command
        waitpid(pid, &status, WUNTRACED);
     }
    }
  return ;
}
 
void command_pipe(char **strings,int bg,pid_t* jobpg)
{ //this runs for commands including pipes
  int fds[2];
  int input=0, output;
  char **tokens;
  pid_t pid;

  int j=0;

  
  while(strings[j]!=NULL) //the number of commands seperated by |
  {
    j++;
  }

  pid_t pids[j-1];

  for(int i=0; i<j; i++) {

    tokens=lsh_split_line(strings[i]); //split the command

    char *args_aux[256];
    take_args(tokens,args_aux); 

  if(i<j-1) { //if we are not in the last command
     pipe(fds);
     output=fds[1]; //we have a pipe so need a writer
  } 
  else 
    output=1;

  pid=fork();

  if(pid==0) { //child

    if(bg) //if it is a backround command
      setpgid(pid,*jobpg);
    else{
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
    }

    wild_cards(args_aux);

    if(redirection(tokens)==-1) //check for redirections
        perror("Redirection failed");
      /* children */
     if(input!=0) { //if its not the first command
        dup2(input,0);
        close(input);
     }
     if(output!=1) { //if its not the last command
        dup2(output,1);
        close(output);
        close(fds[0]);
     } 
     if (execvp(args_aux[0],args_aux)<0){
			  perror("Execvp failed");
      }
      exit(0);
  }
  /* parent */

  if(bg){
    if(!(*jobpg))
      *jobpg=pid;
    setpgid(pid,*jobpg);
  }
  else
  {
    pids[i]=pid;
  }

  if(output!=1) close(output);
  if(input!=0) close(input);
  input=fds[0]; //reader

  free(tokens);
}


int status;

if(!bg){ 
  for(int i=0; i<j; i++) {
      waitpid(pids[i], &status, WUNTRACED);
  }
}
  return;
}





void command_handler(char *args[],int bg,pid_t* jobpg)
{//runs only for commands not including pipes

  char *args_aux[256];
  take_args(args,args_aux); //returns args_aux

  fileIO(args_aux,args,bg,jobpg);

  return ;
}
