#define _GNU_SOURCE

#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Utilizando o comando strace, ferifique como o programa ls faz para retornar
 * as entradas de um diretório. Após isso, fazer um programa que liste os
 * arquivos do diretório atual e suas propriedades.
 *****************************************************************************/

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 1024

/* linux_dirent structure */
struct linux_dirent {
	long d_ino;                    /* Inode number */
	off_t d_off;                /* Offset to next linux_dirent */
	unsigned short d_reclen;    /* Length of this linux_dirent */
	char d_name[];                /* Filename (null-terminated) */
};

int main(int argc, const char *argv[]) {
	int fd, n_read, b_pos;
	char buf[BUF_SIZE], d_type;
	struct linux_dirent *d;

	fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
	if (fd == -1) {
		perror("OPEN");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		n_read = syscall(SYS_getdents, fd, buf, BUF_SIZE);
		if (n_read == -1) {
			perror("getdents");
			exit(EXIT_FAILURE);
		}

		if (n_read == 0) break;

		printf(" inode#\t\tfile_type\td_reclen\td_off\t\t\td_name\n");
		printf(" ------\t\t---------\t--------\t-----\t\t\t------\n");
		for (b_pos = 0; b_pos < n_read;) {
			d = (struct linux_dirent *) (buf + b_pos);
			printf("%8ld\t", d->d_ino);
			d_type = *(buf + b_pos + d->d_reclen - 1);
			printf("%-10s\t", (d_type == DT_REG) ? "regular" :
							  (d_type == DT_DIR) ? "directory" :
							  (d_type == DT_FIFO) ? "FIFO" :
							  (d_type == DT_SOCK) ? "socket" :
							  (d_type == DT_LNK) ? "symlink" :
							  (d_type == DT_BLK) ? "block dev" :
							  (d_type == DT_CHR) ? "char dev" : "???");
			printf("%4d\t\t%10lld\t%s\n", d->d_reclen, (long long) d->d_off, d->d_name);
			b_pos += d->d_reclen;
		}
	}

	return EXIT_SUCCESS;
}