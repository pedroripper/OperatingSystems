#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h> 

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

void changeDirectory(char *path){
	char directory[20] = "./";
	strcat(directory, path);
	printf("%s\n", directory);
	chdir(directory); 
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
}



int main(){
	question_one();

	return 0;

}
