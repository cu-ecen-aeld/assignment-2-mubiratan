#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Configura o syslog para usar a facility LOG_USER
    openlog("writer", 0, LOG_USER);

    // Validação de argumentos
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments. Usage: %s <file> <string>", argv[0]);
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    // Abre o arquivo (O_CREAT cria se não existir, O_TRUNC zera o arquivo, O_WRONLY apenas escrita)
    // 0644 são as permissões padrão do Linux (rw-r--r--)
    int fd = open(writefile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        syslog(LOG_ERR, "Failed to open file: %s", writefile);
        return 1;
    }

    // Registra a operação no syslog com nível DEBUG
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    // Escreve o conteúdo no arquivo
    ssize_t bytes_written = write(fd, writestr, strlen(writestr));
    if (bytes_written == -1 || bytes_written != strlen(writestr)) {
        syslog(LOG_ERR, "Failed to write completely to file: %s", writefile);
        close(fd);
        return 1;
    }

    close(fd);
    closelog();
    return 0;
}

