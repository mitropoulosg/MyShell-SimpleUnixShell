#include "types.h"
int main(){
  struct alias *alias_list= NULL;
  struct history *history_head = NULL;
  int result;
  char** tokens;
  char*** strings;
  char *prompt = "in-mysh-now:> ";
  char str[MAXLEN],ret[MAXLEN],ret2[MAXLEN];
  char str1[MAXLEN],str2[MAXLEN];
  int k=0,i;
  int bg;
  pid_t *jjobpg;
  char *home_dir = getenv("HOME");

    /* create an infinite loop */
  while (1)
  {

     signal(SIGINT, SIG_IGN);
     signal(SIGTSTP, SIG_IGN);
    // Register signal handlers
    
    i=0;
    printf("%s", prompt); //print in-my-mysh>



    if (input(str)) //reads string from terminal
      continue;

    

    if (strcmp(str,"exit")==0)
    {
      break;
    }

    strings=count_strs(str); //tokenize the string to substrings if ; found and the substring to sub-sub strings if | found

    while(strings[i]!=NULL) //for every command
    {

      jjobpg=&k;

      int j=0;
      while(strings[i][j]!=NULL){
        if(strchr(strings[i][j],'&')!=NULL) //bg 1 if it is a backround command
        {
          bg=1;  
        }
        else
          bg=0;

        j++;
      }

    int status;
    pid_t pidd;
    pidd=waitpid(-1,&status,WNOHANG); //checking if a child has terminated and its status is available,(for backround commands)
    if (pidd!=-1 && pidd!=0)
    {
      printf("A child executed a backround command has terminated\n");
    }
    
       
      result=0;

      strcpy(str1,strings[i][0]);
      strcpy(str2,strings[i][0]);
      strcpy(ret," ");
      strcpy(ret2," ");



      add_history(&history_head, str2); //add the command to history

      tokens= lsh_split_line(str2); //split the command to tokens

      if (alias(tokens,ret2,&alias_list)) //return 1 if the command is about createalias or destroyalias
      {
        free(tokens);
        free(strings[i]);
        i++;
        if(strings[i]==NULL)
        {
          free(strings);
          break;
        }
        continue;
      }
      if (strcmp(ret2," ")!=0) //if alias returned a string found in alias list
      {
        printf("%s\n",ret2);
        free(tokens); 
        strcpy(str1,ret2);
        tokens= lsh_split_line(ret2); //split the command that returned for alias list
      }

      while(!strcmp(tokens[0],"myHistory"))
      {
        free(tokens);
        result=history(&history_head,str1,ret);
        if(result==-1){  //for bad command given with myHistory
          break;
        }
        else if (result) //break for myHistory command
          break;
        if (strcmp(ret," ")!=0) //if we want to run a command from history list
        {
          char *alias_command = get_alias(alias_list, ret); //check if this command is in alias list
          if(alias_command!=NULL)
          {
            strcpy(ret,alias_command);
          }   
          if(strchr(ret,'&')!=NULL) //check if command is backround
          {
            bg=1;
          }
          else
            bg=0;
          tokens= lsh_split_line(ret); //split the command that returned for history list
        }
      }
    
      if(result )
      {
        if(strings[i+1]==NULL)
        {
          free(strings[i]);
          free(strings);
          break;
        }
        else
        {
          free(strings[i]);
          i++;
          continue;
        }
      }

      if (strcmp(tokens[0], "cd") == 0) { //for cd command
        if (tokens[1] == NULL) {
                // Change to the home directory if no argument is provided
          if (chdir(home_dir) != 0) {
            printf("Error changing directory\n");
          }
        } else if (chdir(tokens[1]) != 0) {
          printf("Error changing directory\n");
        }
        free(strings[i]);
        free(tokens);
        if(strings[i+1]==NULL)
        {
          free(strings);
          break;
        }
        i++;
        continue;
      }

      if(strings[i][1]==NULL) //if there isnt a pipe here
      {
        command_handler(tokens,bg,jjobpg); //to execute the command
        free(tokens);

      }
      else //if there is a pipe here
      {
        command_pipe(strings[i],bg,jjobpg); //to execute the command
      }

      free(strings[i]);
      i++;
      if(strings[i]==NULL)
      {
        free(strings);
        break;
      }
    }
  }



  if (strcmp(str,"exit")==0)
  {
    //free(tokens);
    free_history(history_head); //free allocations
    free_alias(alias_list);
 
    if(*jjobpg!=0) //kill process group
      killpg(*jjobpg, SIGKILL);
  }

  return (0);
}