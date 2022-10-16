#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096

struct Map {
    char word1[64];
    char word2[64];
};





int main(int argc, char* argv[]) {

	char full_text[BUFFER_SIZE];
	char line[4096];
	int readpipe[2];
	int writepipe[2];
	int index = 0;
    while((fgets(line, BUFFER_SIZE, stdin)) != NULL){
    	for(int i = 0; i< strlen(line); i++){
    		if(line[i] != '\0'){
    			full_text[index] = line[i];
    			index++;
    		}
    	}
    }
    full_text[index] = '\0';

    //readpipe: pai para filho
    //writepipe:filho para pai
    pipe(readpipe);
    pipe(writepipe);	

	switch(fork()){
		case 0:{
			close(readpipe[1]);
			close(writepipe[0]);
			char text[BUFFER_SIZE];
			char words[128];
			FILE *text_file;
			struct Map map[64];
			int map_index = 0;


			if(read(readpipe[0], text, BUFFER_SIZE) < 0){
				break;
			};
					

			if((text_file= fopen("cypher.txt", "r")) == NULL){
				perror("Cannot open cypher file");
				return 1;
			}

			while(fgets(words, 128, text_file) != NULL){
				char *tokens = strtok(words, " ");
				struct Map create_map;
				memset(create_map.word1,0,64);
				memset(create_map.word2, 0, 64);
				for(int i = 0; i < strlen(tokens); i++){
					create_map.word1[i] = tokens[i];
				}
				tokens = strtok(NULL, " ");
				for(int i = 0; i < strlen(tokens); i++){
					if(tokens[i] != '\0' && tokens[i] != '\n'){
						create_map.word2[i] = tokens[i];
					}
				}
				map[map_index] = create_map;
				map_index++;
			}	
			
			char cyphered_text[4096];
			int first_word = 1;
			int to_cypher = 0;
			char *token = strtok(text, " ");
			int cyphered_text_index = 0;
			int written = 0;
			char pontuation[1];
			
			while(token != NULL){
				if(first_word){
					if(token[strlen(token)-1] == '.' || token[strlen(token)-1]== '!' || token[strlen(token)-1] == '?' || token[strlen(token)-1] == ',' || token[strlen(token)-1] == '\n'){
						pontuation[0] = token[strlen(token)-1];
						token[strlen(token)-1] = '\0';
					}
					for(int i = 0; i < map_index; i++){
						if(strcmp(token, map[i].word1) == 0){
							for(int k = 0; k < strlen(map[i].word2); k++){
								cyphered_text[cyphered_text_index] = map[i].word2[k];
								cyphered_text_index++;
							}
							first_word = 0;
							to_cypher = 0;
						}
						else if(strcmp(token, map[i].word2) == 0){
							for(int k = 0; k < strlen(map[i].word1); k++){
								cyphered_text[cyphered_text_index] = map[i].word1[k];
								cyphered_text_index++;
							}
							first_word = 0;
							to_cypher = 1;
						}
					}
					if(first_word == 1){
						for(int k = 0; k < strlen(token); k++){
							cyphered_text[cyphered_text_index] = token[k];
							cyphered_text_index++;
						}
					}
					if(pontuation[0] == '.' || pontuation[0]== '!' || pontuation[0] == '?' || pontuation[0] == ',' || pontuation[0] == '\n'){
						cyphered_text[cyphered_text_index] = pontuation[0];
						cyphered_text_index++;
						memset(pontuation, 0, 1);
					}
					cyphered_text[cyphered_text_index] = ' ';
					cyphered_text_index++;
					token = strtok(NULL, " ");
				}
				else{
					if(token[strlen(token)-1] == '.' || token[strlen(token)-1]== '!' || token[strlen(token)-1] == '?' || token[strlen(token)-1] == ',' || token[strlen(token)-1] == '\n'){
						pontuation[0] = token[strlen(token)-1];
						token[strlen(token)-1] = '\0';
					}
					for(int i = 0; i < map_index; i++){
						if((strcmp(token, map[i].word1) == 0) && (to_cypher == 0)){
							for(int k = 0; k < strlen(map[i].word2); k++){
								cyphered_text[cyphered_text_index] = map[i].word2[k];
								cyphered_text_index++;
							}
							written = 1;
						}
						if((strcmp(token, map[i].word2) == 0) && (to_cypher == 1)){
							for(int k = 0; k < strlen(map[i].word1); k++){
								cyphered_text[cyphered_text_index] = map[i].word1[k];
								cyphered_text_index++;
							}
							written = 1;
						}
					}

					if(written == 0){
						for(int k = 0; k < strlen(token); k++){
							cyphered_text[cyphered_text_index] = token[k];
							cyphered_text_index++;
						}
					}
					if(pontuation[0] == '.' || pontuation[0]== '!' || pontuation[0] == '?' || pontuation[0] == ',' || pontuation[0] == '\n'){
						cyphered_text[cyphered_text_index] = pontuation[0];
						cyphered_text_index++;
						memset(pontuation, 0, 1);
					}
					written = 0;
					cyphered_text[cyphered_text_index] = ' ';
					cyphered_text_index++;
					token = strtok(NULL, " ");
				}
			}
			cyphered_text[cyphered_text_index - 1] = '\0';

			if(write(writepipe[1], cyphered_text, BUFFER_SIZE) < 0){
				break;
			};
			break;
		}

		case -1:{
			perror("fork");
			break;
		}

		default:{
			close(readpipe[0]);
			close(writepipe[1]);
			char textfinish[BUFFER_SIZE];
			if(write(readpipe[1], full_text, strlen(full_text) + 1) < 0){
				break;
			};
			if(read(writepipe[0], textfinish, BUFFER_SIZE) < 0){
				break;
			};
			printf("%s", textfinish);
			break;
		}


	}

	close(readpipe[0]);
	close(writepipe[1]);
	close(writepipe[0]);
	close(readpipe[1]);

    return 0;
}

