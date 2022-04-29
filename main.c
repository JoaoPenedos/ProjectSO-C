#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

void mostrar() {
	char fileName[256], *c = (char *) calloc(100, sizeof(char));
	int sz;

	printf("Qual e o nome do ficheiro que pertende visualizar?\n");
	fgets(fileName,sizeof(fileName),stdin);
	fileName[strcspn(fileName, "\n")] = '\0';

	int file = open(fileName,O_RDONLY);
	  
    if (file < 0) { 
        // print which type of error have in a code 
        printf("Error Number %d\n", errno);
        // print program detail "Success or failure" 
        perror("Program");                 
    }

	sz = read(file, c, 100);
	c[sz] = '\0';
	printf("%s",c);

	getchar();
	
	close(file);
}


void menu(long *opcao) {
    int success = 0;
	char *endptr, buf[1024];
	
	do {
		printf("\e[1;1H\e[2J");
        printf("----------------------------------------------------------\n"
               "   HELP menu:                                             \n"
               "                                                          \n"
               "   --0                Leave                               \n"
               "   --s                Show                                \n"
               "   --cp               Copy                                \n"
               "   --ap               Append                              \n"
               "   --ct               Count                               \n"
               "   --d                Delete                              \n"
               "   --i                Info                                \n"
               "   --l                List                                \n"
               "----------------------------------------------------------\n");
        printf("Opcao: ");
		
		if (!fgets(buf, sizeof(buf), stdin)) {
			success = 0;
		}

		errno = 0; // reset error number
		if (errno == ERANGE) {
			success = 0;
			printf("The number entered is either too large or too small.\n\n");
			setvbuf(stdout, NULL, _IONBF, 0);
			sleep(2);
		}
		else if (!(buf[0] == '-' && buf[1] == '-' )) {
			printf("\e[1;1H\e[2J");
			success = 0;
			buf[strcspn(buf, "\n")] = 0;
			printf("'%s' is not recognised as a permited option\n",buf);
			printf("Please enter a correct one!\n\n");
			setvbuf(stdout, NULL, _IONBF, 0);
			sleep(2);
		}
		else if (buf[2] == '0' && buf[3] == '\n') {
			success = 1;
			*opcao = 0;
		}
		else if (buf[2] == 's' && buf[3] == '\n') {
			success = 1;
			*opcao = 1;
		}
		else if (buf[2] == 'c' && buf[3] == 'p' && buf[4] == '\n') {
			success = 1;
			*opcao = 2;
		}
		else if (buf[2] == 'a' && buf[3] == 'p' && buf[4] == '\n') {
			success = 1;
			*opcao = 3;
		}
		else if (buf[2] == 'c' && buf[3] == 't' && buf[4] == '\n') {
			success = 1;
			*opcao = 4;
		}
		else if (buf[2] == 'd' && buf[3] == '\n') {
			success = 1;
			*opcao = 5;
		}
		else if (buf[2] == 'i' && buf[3] == '\n') {
			success = 1;
			*opcao = 6;
		}
		else if (buf[2] == 'l' && buf[3] == '\n') {
			success = 1;
			*opcao = 7;
		}
		else {
			printf("\e[1;1H\e[2J");
			success = 0;
			buf[strcspn(buf, "\n")] = 0;
			printf("'%s' is not recognised as a permited option\n",buf);
			printf("Please enter a correct one!\n\n");
			setvbuf(stdout, NULL, _IONBF, 0);
			sleep(2);
		}
	}while(!success);
}
//#####################################################################################################
//#####################################################################################################
int main(int argc, char const *argv[]) {
    long opcao;

    do {
        menu(&opcao);

        switch(opcao) {
            case 0: printf("Goodbye!!\n\n");
					break;
            case 1: mostrar();
					break;
            // case 2: copiar();
			// 		break;
            // case 3: apresentar();
			// 		break;
            // case 4: contar();
			// 		break;
            // case 5: apagar();
			// 		break;
            // case 6: informar();
			// 		break;
            // case 7: listar();
            // 		break;
        }

		/**
		 * para mudar este menu não lhe mexam muito
		 * talvez o acrescente em cima pra diminuir tamanho
		 *  
		 * 
		 */
    }while(opcao != 0);

    return 0;
}