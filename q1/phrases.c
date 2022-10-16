#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_LENGTH 256

int main(int argc, char* argv[]) {
	FILE *text_file;
	char line[BUFFER_LENGTH];

	switch(argc){
		case 1:{
			printf("usage: phrases [-l] file\n");
			return 1;
			break;
		}

		case 2:{
			int number_lines = 0;

			if((text_file= fopen(argv[1], "r")) == NULL){
				perror(argv[1]);
				return 1;
			}

			while(fgets(line, BUFFER_LENGTH, text_file)){
				for(int i = 0; i < strlen(line); i++){
					char character = line[i];
					if(character == '!' || character == '.' || character == '?'){
						number_lines++;
					}
				}
			}
			if(line[strlen(line)-1] != '!' || line[strlen(line)-1] != '.' || line[strlen(line)-1] != '?'){
				number_lines++;
			}

			printf("%d\n", number_lines);
			break;
		}

		case 3:{
			if(strcmp(argv[1], "-l")){
				printf("usage: phrases [-l] file\n");
				return 1;
			}

			if((text_file= fopen(argv[2], "r")) == NULL){
				perror(argv[2]);
				return 1;
			}
			
			bool exists_lines = false;
			char full_text[4096];
			int index = 0;
			char pontuation[128];
			int pontuation_index = 0;
			while(fgets(line, BUFFER_LENGTH, text_file)){
				exists_lines = true;
				for(int i = 0; i < strlen(line); i++){
					char character = line[i];
					if(index == 0){
						full_text[index] = ' ';
						index++;
					}
					if(character == '!' || character == '.' || character == '?'){
						full_text[index] = character;
						pontuation[pontuation_index] = character;
						pontuation_index++;
						index++;
					}
					else if(character == '\n'){
						continue;
					}
					else{
						full_text[index] = character;
						index++;
					}
					
				}
			}
			full_text[index] = '\0';
			char *tokens = strtok(full_text, "!?.");
			int pontuation_number = 0;
			int text_line = 1;
			if(exists_lines){
				while(tokens != NULL){
					printf("[%d]%s%c\n", text_line, tokens, pontuation[pontuation_number]);
					tokens = strtok(NULL, "!?.");
					pontuation_number++;
					text_line++;
				}
			}
			break;
		}
		default:
			break;
	}
	return 0;
}
