#include "types.h"

char*** count_strs(char* str)//this functions split the commands as this example: ls | pwd ; cat
{                             //strings[0][0]= ls , strings[0][1]= pwd , strings[1][0]=cat
  int count;
  char *str1;
  char*** strs= malloc(LSH_TOK_BUFSIZE * sizeof(char**));
  // split the input string into separate strings
  count = 0;
  str1 =strtok(str, ";\n");
  while (str1 != NULL) {
    strs[count]=malloc(MAXLEN * sizeof(char*));
    strs[count][1]=NULL;
    strs[count][0]=str1;

    count++;
    str1 =strtok(NULL, ";\n");
  }
  strs[count]=NULL; //so far we have split the string to substrings seperated by ;

  count=0;
  while(strs[count]!=0)
  {
    int i=0;
    str1 =strtok(strs[count][0], "|\n");
    while (str1 != NULL) {
      strs[count][i]=str1;
      i++;
      str1 =strtok(NULL, "|\n");
    }
    strs[count][i]=NULL;
    count++;
  }
  strs[count]=NULL; //now we have split every substring seperated by |
  return strs;
}


char **lsh_split_line(char *line) //this functions take a command and splits it tokens
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token,token2[MAXLEN];

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM); //first splits the string seperated by space and if ti finds " it stores to one tokens the inside of " "
  int len;
  
  while(token != NULL){
    len=strlen(token);
    if(token[len-1]=='&' && len>1) //if the token has & removes it
    {
      token[len-1]='\0';
      len=strlen(token);
    }

    if(token[0]=='"') //this if case stores to a token all the characters that are inside " "
    {
      memmove(token,token+1,len);
      len=strlen(token);
      if(token[len-1]=='\"') //if first token starts with "
      {
      token[len-1]='\0';
      tokens[position] = token;
      }
      else{
      tokens[position] = token;
      token = strtok(NULL, LSH_TOK_DELIM);
      len=strlen(token);
      while(token[len-1]!='\"')  //while last character of token is "
      {
        strcat(tokens[position]," ");
        strcpy(token2,token);
        strcat(tokens[position],token2);
        token = strtok(NULL, LSH_TOK_DELIM);
        len=strlen(token);
      }
      strcat(tokens[position]," ");
      token[len-1]='\0';   
      strcpy(token2,token); 
      strcat(tokens[position],token2);
      } 
    }
    else{
      tokens[position] = token;
    }
    token = strtok(NULL, LSH_TOK_DELIM);
    position++;
  }
  tokens[position] = NULL;
  return tokens; //return final tokens
}

int input(char* str)
{
  int i;
  fgets(str, MAXLEN, stdin);
  /* remove newline, if present */
  if(strlen(str)!=1)
  {
    i = strlen(str)-1;
    if( str[ i ] == '\n') 
    str[i] = '\0';
    return 0;
  }
  else return 1;
}