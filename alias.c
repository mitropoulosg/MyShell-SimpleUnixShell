#include "types.h"
void destroyalias(struct alias **head, char *alias_name) {
    struct alias *temp = *head;
    struct alias *prev = NULL;

    // Traverse the linked list to find the alias to delete
    while (temp != NULL && strcmp(temp->alias, alias_name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // If the alias was not found, return
    if (temp == NULL) {
        return;
    }

    // If the alias is the first node, update head to point to the next node
    if (prev == NULL) {
        *head = temp->next;
    } else {
        // Otherwise, update the next pointer of the previous node to point to the next node
        prev->next = temp->next;
    }

    // Free the memory used by the deleted node
    free(temp);
}
// Add a new alias to the list
void add_alias(struct alias **head, char *alias_name, char *cmd) {
    // Allocate memory for the new node
    struct alias *new_node = (struct alias*) malloc(sizeof(struct alias));

    // Copy the alias name and command into the new node
    strcpy(new_node->alias, alias_name);
    strcpy(new_node->cmd, cmd);

    // Set the next pointer of the new node to NULL
    new_node->next = NULL;

    // If the linked list is empty, set the new node as the head
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    // Traverse the linked list to find the end
    struct alias *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    // Add the new node to the end of the linked list
    temp->next = new_node;
}

// Find an alias in the list and return the corresponding command
char* get_alias(struct alias *alias_list, char *alias) {
    struct alias *current = alias_list;
    while (current != NULL) {
        if (strcmp(alias, current->alias) == 0) {
            return current->cmd;
        }
        current = current->next;
    }
    return NULL;
}

int alias(char **tokens,char *ret,struct alias **alias_list)
{
    // Check for aliases
    char *alias_command = get_alias(*alias_list, tokens[0]); 
    if(alias_command!=NULL) //if we found the command sore it in ret
    {
        strcpy(ret,alias_command);
        return 0;
    }
    if (strcmp(tokens[0],"createalias") == 0 && tokens[2]!=NULL){
        // Add a new alias
        char alias[MAXLEN/2], cmd[MAXLEN/2];
        strcpy(alias, tokens[1]);
        strcpy(cmd, tokens[2]);
        add_alias(alias_list, alias, cmd); //add alias and cmd to alias list
        return 1;
    }else if(strcmp(tokens[0],"destroyalias") == 0 && tokens[1]!=NULL)
    {
        destroyalias(alias_list, tokens[1]); //remove alias from the list
        return 1;
    }
    return 0;
}

void free_alias(struct alias *alias_head) { //free alias list
    while (alias_head != NULL) {
        struct alias *temp = alias_head;
        alias_head = alias_head->next;
        free(temp);
    }
}

