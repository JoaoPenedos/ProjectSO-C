#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

void show() {
	char ch, fileName[256];
	int i;

	printf("Qual e o nome do ficheiro que pertende visualizar?\n");
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';

	int file = open(fileName,O_RDONLY,0);
	  
    if (file < 0) { 
        // print which type of error have in a code 
        printf("Error Number %d\n", errno);
        // print program detail "Success or failure" 
        perror("Program");
		getchar();              
    }
	else {
		printf("\e[1;1H\e[2J");
		printf("\nConteudo no ficheiro %s\n",fileName);
		while (read(file, &ch, sizeof(char)) != 0) {
			printf("%c", ch);
		}
		close(file);
		printf("\n\n");
		getchar();
	}
}

void strip_ext(char *fname)
{
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.') {
        --end;
    }

    if (end > fname) {
        *end = '\0';
    }
}

int copy() {
	char fileName[256], fileNameCP[256], buf[4096];
	int fd_to, fd_from, saved_errno;
	ssize_t nread;

	printf("Qual e o nome do ficheiro que pertende criar uma copia?\n");
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';
	strcpy(fileNameCP,fileName);
	strip_ext(fileNameCP);

	fd_from = open(fileName, O_RDONLY);
	if (fd_from < 0) {
		// print which type of error have in a code 
        printf("Error Number %d\n", errno);
        // print program detail "Success or failure" 
        perror("Program");
		getchar(); 
		return -1;
	}

	fd_to = open(strcat(fileNameCP,"_copia.txt"), O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		goto out_error;

	while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fd_to, out_ptr, nread);

			if (nwritten >= 0) {
				nread -= nwritten;
				out_ptr += nwritten;
			}
			else if (errno != EINTR) {
				goto out_error;
			}
		} while (nread > 0);
	}

	if (nread == 0) {
		if (close(fd_to) < 0) {
			fd_to = -1;
			goto out_error;
		}
		close(fd_from);

		/* Success! */
		return 0;
	}

	out_error:
	saved_errno = errno;
	printf("Error Number %d\n", saved_errno);
	perror("Program");
	getchar(); 

	close(fd_from);
	if (fd_to >= 0)
		close(fd_to);

	errno = saved_errno;
	return -1;
}

void append() {

}

int menu() {
	char *endptr, buf[1024];
	int err;
	
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
			"----------------------------------------------------------\nOption: \n");
	
	if (!read(STDIN_FILENO, buf, sizeof(buf))) 
		return 0;

	errno = 0; // reset error number
	if (errno == ERANGE) {
		printf("The number entered is either too large or too small.\n\n");
		getchar();

		return 1;
	}
	else if (!(buf[0] == '-' && buf[1] == '-' )) {
		printf("\e[1;1H\e[2J");
		buf[strcspn(buf, "\n")] = 0;
		printf("'%s' is not recognised as a permited option\n",buf);
		printf("Please enter a correct one!\n\n");
		// setvbuf(stdout, NULL, _IONBF, 0);
		// sleep(2);
		getchar();

		return 1;
	}
	else if (buf[2] == '0' && buf[3] == '\n') {
        printf("Goodbye!!\n\n");
		return 0;
	}
	else if (buf[2] == 's' && buf[3] == '\n') {
		show();
		return 1;
	}
	else if (buf[2] == 'c' && buf[3] == 'p' && buf[4] == '\n') {
		err = copy();
		if(err == 0)
			return 1;
	}
	else if (buf[2] == 'a' && buf[3] == 'p' && buf[4] == '\n') {
		apppend();
		return 1;
	}
	// else if (buf[2] == 'c' && buf[3] == 't' && buf[4] == '\n') {
	// 	count();
	// 	return 1;
	// }
	// else if (buf[2] == 'd' && buf[3] == '\n') {
	// 	delete();
	// 	return 1;
	// }
	// else if (buf[2] == 'i' && buf[3] == '\n') {
	// 	info();
	// 	return 1;
	// }
	// else if (buf[2] == 'l' && buf[3] == '\n') {
	// 	list();
	// 	return 1;
	// }
	else {
		printf("\e[1;1H\e[2J");
		buf[strcspn(buf, "\n")] = 0;
		printf("'%s' is not recognised as a permited option\n",buf);
		printf("Please enter a correct one!\n\n");
		getchar();

		return 1;
	}
}
//#####################################################################################################
//#####################################################################################################
int main(int argc, char const *argv[]) {
    int opcao;

    do {
        opcao = menu();
    }while(opcao != 0);

    return 0;
}