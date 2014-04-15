#include <unistd.h>
#include <sys/types.h>

int entry(int argc, char *argv[]);

int Open(const char *pathname, int flags, mode_t mode);
int Close(int fd);
ssize_t Read(int fd, void *buf, int nbyte);
ssize_t Write(int fd, const void *buf, int nbyte);
off_t Seek(int fildes, off_t offset, int whence);
