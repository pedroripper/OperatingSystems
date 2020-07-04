#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h> 
#include <dirent.h>

#define MAX_TEXT 50

/*
Questao 1
Criar o diretório “so” em seu diretório corrente;
Criar os subdiretórios “a”, “b” e “c”; 
Criar arquivos “.txt” nesses subdiretórios (“arqa.txt”, “arqb.txt” e “arqc.txt”)
Escrever textos nestes arquivos.
*/
void createDirectory(char *dirName){
	int checkDir;
	checkDir = mkdir(dirName,0777);
	if(!checkDir){
		printf("Diretorio %s criado com sucesso\n", dirName);
	} else {
		printf("Erro criando diretorio\n");
	}
}

int changeDirectory(char *path){
	char directory[20] = "./";
	strcat(directory, path);
	return chdir(directory); 
}

void createFile(char *fileName){
	char text[MAX_TEXT];
	FILE *createFile = fopen(fileName, "w");
	printf("Escreva o que voce quer guardar no arquivo %s\n", fileName);
	fgets(text, sizeof(text), stdin);
	fprintf(createFile, "%s", text);
	fclose(createFile);
}

void question_one(){
	createDirectory("so");
	changeDirectory("so");
	createDirectory("a");
	createDirectory("b");
	createDirectory("c");
	changeDirectory("a");
	createFile("arqa.txt");
	changeDirectory("..");
	changeDirectory("b");
	createFile("arqb.txt");
	changeDirectory("..");
	changeDirectory("c");
	createFile("arqc.txt");
	changeDirectory("../..");
}


/*
Questao 2
Exibir os atributos de um dos arquivos criados.
*/
int readFile(char *fileName){
	char text[MAX_TEXT];
	FILE *readFile = fopen(fileName, "r");
	if(readFile == NULL){
		return 1;
	}
	fscanf(readFile, "%[^\n]", text);
   	printf("%s\n", text );
   	return 0;
}

void question_two(){
	changeDirectory("so");
	changeDirectory("a");
	readFile("arqa.txt");
	changeDirectory("../..");
}

/*
Questao 3
 Buscar um dado arquivo a partir do diretório “so”
 ler e imprimir o conteúdo do arquivo encontrado
*/
int searchFile(char *fileName){
	struct dirent *de;
	DIR *directory = opendir("."); 

  
    if (directory == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        return 0; 
    } 
    while ((de = readdir(directory)) != NULL) {
    	if(strcmp(de->d_name,".") == 0|| strcmp(de->d_name,"..") == 0 || strcmp(de->d_name,"..") == 0) {
				continue;
		}
		if(strcmp(de->d_name,fileName) == 0){
			readFile(fileName);
		}
            if(de->d_type == DT_DIR) {
            	changeDirectory(de->d_name);
            	searchFile(fileName);
            }
    }
    changeDirectory("..");
  
    
    return 0; 
      
    return 0;

}


void question_three(){
	changeDirectory("so");
	searchFile("arqc.txt");
}


int main(){
	// question_one();
	// question_two();
	question_three();

	return 0;

}
