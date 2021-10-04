#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line){
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++){

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';

			if (tokenIndex != 0){
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else{
			token[tokenIndex++] = readChar;
		}
	}
	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}


int main(int argc, char *argv[]){
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;

	while (1){
		/* BEGIN: TAKING INPUT */
		//dup2(STDOUT_FILENO, 1);
		bzero(line, sizeof(line));
		printf("minersh$ ");
		scanf("%[^\n]", line);
		getchar();

		line[strlen(line)] = '\n';
		tokens = tokenize(line);

		if(*tokens == NULL){

		}



		else if(strcmp(*tokens, "cd")==0){
			if(chdir(tokens[1]) == -1){
				printf("%s", "--Incorrect command to the display.\n");
			}
		}


		else if (strcmp(*tokens, "exit") == 0 ){
			printf("%s", "--Exitting\n");
			exit(0);
		}


		else if(*tokens){

			char **index = tokens;
			int piping = 0;
			while( *index != 0 ){
				if( strcmp(*index, "|") ==0 ){
					printf("%s", "--Piping\n");
					piping = 1;
					break;
				}
				index++;
			}

			int fd[2];
			if(piping && pipe(fd) == -1){
				char error_message[30] = "An error has occurred.\n";
				write(STDERR_FILENO,error_message,strlen(error_message));
			}


			int rc = fork();
			if (rc < 0){
				// fork failed; exit
				fprintf(stderr, "fork failed\n");
				exit(1);
			}

			// child
			else if (rc == 0){

				int count = 0;
				char **clone = tokens;
				while(*clone){
					clone++;
					count += 1;
				}


				//---------------------checking if piping-------------
				if(piping){
					dup2(fd[1], STDOUT_FILENO);
					close(fd[0]);
					close(fd[1]);
					execlp(tokens[0], tokens[0], (char *)NULL);
				}
				//------------------------checking if redirecting------
				if(!piping && count >= 2 && tokens[count-2] && strcmp(tokens[count-2], ">") == 0){
					printf("%s", "--Duplicating\n");
					int fw = open(tokens[count-1],  O_WRONLY | O_CREAT | O_TRUNC);
					dup2(fw, STDOUT_FILENO);
					dup2(fw, STDERR_FILENO);

					//special echo case
					if(strcmp(*tokens, "echo")==0){
						char *pointer = line+5;
						execlp(*tokens, *tokens, tokens[1], (char *)NULL);
					}
					// 2 flags case
					else if(count == 5 && execlp(*tokens, *tokens, tokens[1], tokens[2], (char *)NULL) == -1){
						printf("%s", "--Invalid command.\n");
					}
					// single flag case
					else if(count == 4 && execlp(*tokens, *tokens, tokens[1], (char *)NULL) == -1){
						printf("%s", "--Invalid command.\n");
					}

					else if(count == 3 && execlp(*tokens, *tokens, (char *)NULL) == -1){
						printf("%s", "--Invalid command.\n");
					}
					close(fw);
				}



				else if(!piping && strcmp(*tokens, "echo")==0){
					char *pointer = line+5;
					execlp(*tokens, *tokens, pointer, (char *)NULL);
				}


				else if(!piping && execlp(*tokens, *tokens, tokens[1], (char *)NULL) == -1){
					printf("%s", "--Invalid command.\n");
				}
				exit(0);
			}

			else{
				if(piping){
					int child2 = fork();
					if(child2 == 0){
						dup2(fd[0], STDIN_FILENO);
						close(fd[0]);
						close(fd[1]);
						if(execlp(tokens[2], tokens[2], (char *)NULL)==-1){
							printf("%s", "--Invalid command.");
						}
						exit(0);
					}
					else{
						close(fd[0]);
						close(fd[1]);
						wait(NULL);
					}
				}
				wait(NULL);
			}
		}

		// Freeing allocated memory
		for (i = 0; tokens[i] != NULL; i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
