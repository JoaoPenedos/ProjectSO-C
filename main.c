#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

extern int errno;

void help() {
	printf("\e[1;1H\e[2J");
	printf("------------------------------------------------------------------------------------\n"
			"   HELP menu:\n\n"
			"   --h          Help - shows a help menu\n"
			"   --s          Show - takes 1 file as argument and writes the content of the file in the console\n"
			"   --cp         Copy - takes 1 file as argument and creat a copy of that file with the name fileName_copia.txt\n"
			"   --ap         Append - takes 2 files as argument and appends the content of the first to the second\n"
			"   --ct         Count - takes 1 file as argument and count it's lines\n"
			"   --d          Delete - takes 1 file/dir as argument and deletes\n"
			"   --i          Info - takes 1 file/dir as argument and show the system info about them\n"
			"   --l          List - takes 1 dir as argument or none and list the content of that dir, if none where taken as argument list the current directory\n"
			"------------------------------------------------------------------------------------\n");
}

void show(const char *fileName) {
	char ch;
	int file = open(fileName,O_RDONLY,0);
	  
    if (file < 0) { 
        // print which type of error have in a code 
        printf("Error Number %d\n", errno);
        // print program detail "Success or failure" 
        perror("Program"); 
    }
	else {
		printf("\nFile content %s: \n\n",fileName);
		while (read(file, &ch, sizeof(char)) != 0) {
			printf("%c", ch);
		}
		close(file);
		printf("\n\n");
	}
}

void strip_ext(char *fname) {
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.') {
        --end;
    }

    if (end > fname) {
        *end = '\0';
    }
}

int copy(const char *fileName) {
	char fileNameCP[256], buf[4096];
	int fileOrigin, fileCopy, saved_errno;
	ssize_t nread;

	strcpy(fileNameCP,fileName);
	strip_ext(fileNameCP);

	fileOrigin = open(fileName, O_RDONLY);
	if (fileOrigin < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
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

	close(fileOrigin);
	if (fileCopy >= 0)
		close(fileCopy);

	errno = saved_errno;
	return -1;
}

int append(const char *fileNameO,const char *fileNameD) {
	char buf[4096];
	int fileOrigin, fileDestiny, saved_errno;
	ssize_t nread;

	fileOrigin = open(fileNameO, O_RDONLY);
	if (fileOrigin < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
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

	close(fileOrigin);
	if (fileDestiny >= 0)
		close(fileDestiny);

	errno = saved_errno;
	return -1;
}

void count(const char *fileName) {
	size_t lines = 1;
	char c;
	int i;

	int file = open(fileName,O_RDONLY,0);
	if (file < 0) {
        printf("Error Number %d\n", errno);
        perror("Program");
	}
	else {
		while (read(file, &c, 1) == 1) {
			if (c == '\n') {
				lines++;
			}
		}

		printf("Linecount: %zu\n", lines);
	}
}

void delete(const char *fileName) {
	struct stat sb;
    int fd;

	if (!stat(fileName, &sb))
	{
		if (S_ISDIR(sb.st_mode)){
			printf("Directory %s was removed with success\n", fileName);
			rmdir(fileName);
		}
		else {
			printf("File %s was removed with success\n", fileName);
			unlink(fileName);
		}
	}
	else {
		printf("Error Number %d\n", errno);
        perror("Program");
	}
}

void info(const char *fileName) {
    struct stat sb;

	if (stat(fileName, &sb) < 0) {
		printf("Error Number %d\n", errno);
		perror("stat");
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
	}
}

void list(const char *dirName) {
	DIR *d;
	struct dirent *dir;

	d = opendir(dirName);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
				printf("File:			 %s\n", dir->d_name);
			}
			else if (dir->d_type == DT_DIR) {
				printf("Directory:		 %s\n", dir->d_name);
			}
		}
	}
	else {
		printf("Error Number %d\n", errno);
		perror("stat");	
	}
	closedir(d);
}

//#####################################################################################################
//#####################################################################################################
int main(int argc, char const **argv) {
	int err;

	if(argc > 1) {
		if (strcmp(argv[1],"--h")==0) {
			if(argc == 2)
				help();
			else {
				printf("'%s' is only needed alone\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");			
			}
		}
		else if (strcmp(argv[1],"--s")==0) {
			if(argc == 3)
				show(argv[2]);
			else {
				printf("'%s' needs a file name specified in parameter (ex: --s file.txt)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--cp")==0) {
			if(argc == 3) {
				err = copy(argv[2]);
				if(err == 0)
					write(STDOUT_FILENO,"Ficheiro copia criado e preenchido com sucesso\n",47);
				else 
					write(STDOUT_FILENO,"Nao foi possivel criar e/ou copiar o conteudo para ficheiro\n",60);
			}
			else {
				printf("'%s' needs a file name specified in parameter (ex: --cp file.txt)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--ap")==0) {
			if(argc == 4) {
				err = append(argv[2],argv[3]);
				if(err == 0)
					write(STDOUT_FILENO,"Conteudo do ficheiro acrescentado com sucesso\n",46);
				else 
					write(STDOUT_FILENO,"Nao foi possivel acrescentar o conteudo do ficheiro\n",52);
			}
			else {
				printf("'%s' needs 2 files name specified in parameters (ex: --ap origin.txt destiny.txt)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--ct")==0) {
			if(argc == 3)
				count(argv[2]);
			else {
				printf("'%s' needs a file name specified in parameter (ex: --ct file.txt)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--d")==0) {
			if(argc == 3)
				delete(argv[2]);
			else {
				printf("'%s' needs a file name specified in parameter (ex: --d file.txt)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--i")==0) {
			if(argc == 3)
				info(argv[2]);
			else {
				printf("'%s' needs a file/directory name specified in parameter (ex: --i file.txt/dir)\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else if (strcmp(argv[1],"--l")==0) {
			if(argc == 2)
				list(".");
			else if(argc == 3)
				list(argv[2]);
			else {
				printf("'%s' alone for current directory or it needs a directory name specified in parameter (ex: --l [dir])\n",argv[1]);
				printf("Please enter a correct one! (--h for help menu)\n\n");
			}
		}
		else {
			printf("'%s' is not recognised as a permited option\n",argv[1]);
			printf("Please enter a correct one! (--h for help menu)\n\n");
		}

		exit(EXIT_SUCCESS);
	}
	else{
		printf("For the program to work it needs a option (ex: programName --s [file.txt])\n");
		printf("Please enter one! (--h for help menu)\n\n");
		exit(EXIT_FAILURE);
	}

}