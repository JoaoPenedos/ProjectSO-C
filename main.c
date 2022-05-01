#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

extern int errno;

void show() {
	char ch, fileName[256];
	int i;

	write(STDOUT_FILENO,"Qual e o nome do ficheiro que pertende visualizar?\n",51);
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
	int fileOrigin, fileCopy, saved_errno;
	ssize_t nread;

	// printf("Qual e o nome do ficheiro que pertende criar uma copia?\n");
	write(STDOUT_FILENO,"Qual e o nome do ficheiro que pertende criar uma copia?\n",56);
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';
	strcpy(fileNameCP,fileName);
	strip_ext(fileNameCP);

	fileOrigin = open(fileName, O_RDONLY);
	if (fileOrigin < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
		getchar(); 
		return -1;
	}

	fileCopy = open(strcat(fileNameCP,"_copia.txt"), O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fileCopy < 0)
		goto out_error;

	while (nread = read(fileOrigin, buf, sizeof buf), nread > 0) {
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fileCopy, out_ptr, nread);

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
		if (close(fileCopy) < 0) {
			fileCopy = -1;
			goto out_error;
		}
		close(fileOrigin);

		return 0;
	}

	out_error:
	saved_errno = errno;
	printf("Error Number %d\n", saved_errno);
	perror("Program");
	getchar(); 

	close(fileOrigin);
	if (fileCopy >= 0)
		close(fileCopy);

	errno = saved_errno;
	return -1;
}

int append() {
	char fileNameO[256], fileNameD[256], buf[4096];
	int fileOrigin, fileDestiny, saved_errno;
	ssize_t nread;

	write(STDOUT_FILENO,"Qual e o nome do ficheiro, origem?\n",35);
	read(STDIN_FILENO, fileNameO, sizeof(fileNameO));
	fileNameO[strcspn(fileNameO, "\n")] = '\0';
	write(STDOUT_FILENO,"Qual e o nome do ficheiro, destino?\n",36);
	read(STDIN_FILENO, fileNameD, sizeof(fileNameD));
	fileNameD[strcspn(fileNameD, "\n")] = '\0';

	fileOrigin = open(fileNameO, O_RDONLY);
	if (fileOrigin < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
		getchar(); 
		return -1;
	}

	fileDestiny = open(fileNameD, O_RDWR | O_APPEND);
	write(fileDestiny, "\n", 1);
	if (fileDestiny < 0)
		goto out_error;

	while (nread = read(fileOrigin, buf, sizeof buf), nread > 0) {
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fileDestiny, out_ptr, nread);

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
		if (close(fileDestiny) < 0) {
			fileDestiny = -1;
			goto out_error;
		}
		close(fileOrigin);
		return 0;
	}

	out_error:
	saved_errno = errno;
	printf("Error Number %d\n", saved_errno);
	perror("Program");
	getchar(); 

	close(fileOrigin);
	if (fileDestiny >= 0)
		close(fileDestiny);

	errno = saved_errno;
	return -1;
}

void count() {
	size_t lines = 1;
	char c, fileName[256];
	int i;

	write(STDOUT_FILENO,"Qual e o nome do ficheiro que pertende visualizar?\n",51);
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';

	int file = open(fileName,O_RDONLY,0);
	if (file < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
		getchar(); 
	}
	else {
		while (read(file, &c, 1) == 1) {
			if (c == '\n') {
				lines++;
			}
		}

		printf("Linecount: %zu\n", lines);
		getchar();
	}
}

void delete() {
	struct stat sb;
	char fileName[256];
    int fd;

	write(STDOUT_FILENO,"Qual e o nome do ficheiro que pertende visualizar?\n",51);
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';

	if (!stat(fileName, &sb))
	{
		if (S_ISDIR(sb.st_mode)){
			printf("Pasta %s foi removido com sucesso", fileName);
			rmdir(fileName);
			getchar(); 
		}
		else {
			printf("Ficheiro %s foi removido com sucesso", fileName);
			unlink(fileName);
			getchar(); 
		}
	}
	else {
		printf("Error Number %d\n", errno);
        perror("Program");
		getchar(); 
	}
}

void info() {
    struct stat sb;
	char fileName[256];

	write(STDOUT_FILENO,"Qual e o nome do ficheiro que pertende visualizar?\n",51);
	read(STDIN_FILENO, fileName, sizeof(fileName));
	fileName[strcspn(fileName, "\n")] = '\0';

	// if (argc != 2) {
	// 	fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	// 	exit(EXIT_FAILURE);
	// }

	if (stat(fileName, &sb) < 0) {
		printf("Error Number %d\n", errno);
		perror("stat");
		getchar();
	}
	else {
		printf("File type:                ");

		switch (sb.st_mode & S_IFMT) {
			case S_IFBLK:  printf("block device\n");            break;
			case S_IFCHR:  printf("character device\n");        break;
			case S_IFDIR:  printf("directory\n");               break;
			case S_IFIFO:  printf("FIFO/pipe\n");               break;
			case S_IFLNK:  printf("symlink\n");                 break;
			case S_IFREG:  printf("regular file\n");            break;
			case S_IFSOCK: printf("socket\n");                  break;
			default:       printf("unknown?\n");                break;
		}
		struct passwd *user = getpwuid(sb.st_uid);
		struct group  *group = getgrgid(sb.st_gid);

		printf("I-node number:            %ld\n", (long) sb.st_ino);
		printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
		printf("Link count:               %ld\n", (long) sb.st_nlink);
		printf("Ownership:                UID=%ld (%s)  GID=%ld (%s)\n", (long) sb.st_uid, user->pw_name, (long) sb.st_gid, group->gr_name);
		printf("Preferred I/O block size: %ld bytes\n",	(long) sb.st_blksize);
		printf("File size:                %lld bytes\n", (long long) sb.st_size);
		printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);
		printf("Last status change:       %s", ctime(&sb.st_ctime));
		printf("Last file access:         %s", ctime(&sb.st_atime));
		printf("Last file modification:   %s", ctime(&sb.st_mtime));

		getchar();
	}
	// exit(EXIT_SUCCESS);
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
		if(err == 0){
			write(STDOUT_FILENO,"Ficheiro copia criado e preenchido com sucesso\n",47);
			getchar();
			return 1;
		}
		else {
			write(STDOUT_FILENO,"Nao foi possivel criar e/ou copiar o conteudo para ficheiro\n",60);
		getchar();
			return 1;
		}
	}
	else if (buf[2] == 'a' && buf[3] == 'p' && buf[4] == '\n') {
		err = append();
		if(err == 0){
			write(STDOUT_FILENO,"Conteudo do ficheiro acrescentado com sucesso\n",46);
			getchar();
			return 1;
		}
		else {
			write(STDOUT_FILENO,"Nao foi possivel acrescentar o conteudo do ficheiro\n",52);
			getchar();
			return 1;
		}
	}
	else if (buf[2] == 'c' && buf[3] == 't' && buf[4] == '\n') {
		count();
		return 1;
	}
	else if (buf[2] == 'd' && buf[3] == '\n') {
		delete();
		return 1;
	}
	else if (buf[2] == 'i' && buf[3] == '\n') {
		info();
		return 1;
	}
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