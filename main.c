#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void menu(long *opcao) {
    int success = 0;
	char *endptr, buf[1024];
	
	do {
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
			sleep(3);
		}
		else if (!(buf[0] == '-' && buf[1] == '-' ))
		{
			success = 0;
			buf[strcspn(buf, "\n")] = 0;
			printf("'%s' is not recognised as a permited option\n",buf);
			printf("Please enter a correct one!\n\n");
			setvbuf(stdout, NULL, _IONBF, 0);
			sleep(3);
		}
		else if (buf[2] == '0' && buf[3] == '\n'){
			success = 1;
			*opcao = 0;
		}
		else if (buf[2] == 's' && buf[3] == '\n'){
			success = 1;
			*opcao = 1;
		}
		else if (buf[2] == 'c' && buf[3] == 'p' && buf[4] == '\n'){
			success = 1;
			*opcao = 2;
		}
		else if (buf[2] == 'a' && buf[3] == 'p' && buf[4] == '\n'){
			success = 1;
			*opcao = 3;
		}
		else if (buf[2] == 'c' && buf[3] == 't' && buf[4] == '\n'){
			success = 1;
			*opcao = 4;
		}
		else if (buf[2] == 'd' && buf[3] == '\n'){
			success = 1;
			*opcao = 5;
		}
		else if (buf[2] == 'i' && buf[3] == '\n'){
			success = 1;
			*opcao = 6;
		}
		else if (buf[2] == 'l' && buf[3] == '\n'){
			success = 1;
			*opcao = 7;
		}
		else {
			success = 0;
			buf[strcspn(buf, "\n")] = 0;
			printf("'%s' is not recognised as a permited option\n",buf);
			printf("Please enter a correct one!\n\n");
			setvbuf(stdout, NULL, _IONBF, 0);
			sleep(3);
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
            // case 1: Mostrar();
			// 		break;
            // case 2: Copiar();
			// 		break;
            // case 3: Apresentar();
			// 		break;
            // case 4: Contar();
			// 		break;
            // case 5: Apagar();
			// 		break;
            // case 6: Informar();
			// 		break;
            // case 7: Listar();
            // 		break;
        }
    }while(opcao != 0);

    return 0;
}