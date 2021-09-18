#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i;


	while(1) {
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("minersh$ ");
		scanf("%[^\n]", line);
		getchar();

		//printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

   		if(*tokens==NULL){
   			printf("Please enter a command or type exit.\n");
   			}

   		else if(strcmp(*tokens, "exit")==0){
   			printf("%s", "Exitting\n");
   			exit(0);
   		}

      else if(strcmp(*tokens, "cd")==0){
        if(chdir(tokens[1])==-1){
          printf("Incorrect command to the display.\n");
        }
      }
        else{
   		int rc = fork();
   		pid_t cpid;

   		if (rc<0)
   			{
   				fprintf(stderr, "fork failed\n");
   				exit(1);
   			} else if (rc == 0)
   			{
   			if(execlp(*tokens, *tokens, tokens [1],  NULL) == -1){
          printf("%s", "Invalid command.\n");
          }
   				exit(0);
   			}
   				wait(NULL);
        }
       //do whatever you want with the commands, here we just print them
		/*for(i=0;tokens[i]!=NULL;i++){
			printf("found token %s (remove this debug output later)\n", tokens[i]);
		}*/
		// Freeing the allocated memory
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
