#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void menu(int *opcao) {
    int success;
	char *endptr, buf[1024], dest[4];
	
	do {
        printf("----------------------------------------------------------\n"
               "   HELP menu:                                             \n"
               "                                                          \n"
               "   --0                Leave                               \n"
               "   --s                Show                                \n"
               "   --cp               Copy                                \n"
               "   --ac               Append                              \n"
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
		// (*opcao) = strtol(buf, &endptr, 10);
		if (errno == ERANGE) {
			printf("The number entered is either too large or too small.\n\n");
			success = 0;
			sleep(3);
		}
		else if (!(buf[0] == '-' && buf[1] == '-' && 
		   (buf[2] == '0' || buf[2] == 's' || buf[2] == 'd' || buf[2] == 'i' || buf[2] == 'l' || 
		   (buf[2] == 'c' && buf[3] == 'p') || (buf[2] == 'a' && buf[3] == 'c') || (buf[2] == 'c' && buf[3] == 't')))) 
		{
			printf("'%c%c%c' is not recognised as a permited option",buf[0],buf[1],buf[2]);
			printf("Please enter one of the permited options");
			sleep(3);
		}
		else if (endptr == buf)	{
			// no character was read.
			success = 0;
		}
		else if ((*endptr) && (*endptr != '\n')) {
			// *endptr is neither end of string nor newline, so we didn't convert the *whole* input.
			success = 0;
		}
		else {
			success = 1;
		}
	}while(!success);
}
//#####################################################################################################
//#####################################################################################################
int main(int argc, char const *argv[]) {
    int opcao;

    do {
        menu(&opcao);

        switch(opcao) {
            // case 0: printf("Goodbye!!\n\n");
			// 		break;
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

    printf("Hello");
    printf("\n");
    return 0;
}