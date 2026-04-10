#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>

// Export symbol for F#
__attribute__((visibility("default")))
void write_scatter_file(const char *filename) {
    struct iovec iov[3];
    char *buf[] = {
        "The term buccaneer comes from the word boucan.\n",
        "A boucan is a wooden frame used for cooking meat.\n",
        "Buccaneer is the West Indies name for a pirate.\n"
    };
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return; }
    for (int i = 0; i < 3; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len  = strlen(buf[i]);
    }
    ssize_t nr = writev(fd, iov, 3);
    if (nr == -1) { perror("writev"); close(fd); return; }
    printf("wrote %zd bytes\n", nr);
    close(fd);
}
//gcc -fPIC -shared scatter_lib.c -o libscatter.so

