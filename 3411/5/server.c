#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <netinet/in.h>

#define open_call 1 /* only three argument version of open is supported */
#define close_call 2
#define read_call 3
#define write_call 4
#define seek_call 5

int readRequest(int fd, void *buf, size_t count){
  int num_read=0;
  while(num_read < count){
    int temp = read(fd, buf, count);
    if (temp == -1){
      return -1;
    }
    else{
      num_read += temp;
    } 
  }
  return count;
}
void sendResponse(int sock, int result, int err, int d_length, char *data){
    /*Response: <result int> <errno int> <l-data 2 bytes > <string of l-data bytes>*/
    char *msg = malloc(sizeof(int)*2 + 2 + d_length);
    int idx=0;
    bcopy(&result,&msg[idx],sizeof(int)); idx+=sizeof(int);
    bcopy(&err,&msg[idx],sizeof(int)); idx+=sizeof(int);
    msg[idx++] = d_length >> 8 & 0xff;
    msg[idx++] = d_length & 0xff;

    bcopy(data,&msg[idx],d_length); idx+=d_length;
    write(sock, msg, idx);
    write(1,msg, idx);
    free(msg);
    return;
}

void do_child(int sock){
  char opcode;

  while(1){
    int size = readRequest(sock, &opcode, 1);
    errno = 0;
    if( size == 0){
      return;
    }
    if (opcode == open_call){
      /*Request: <opcode=1> <length-2 bytes> <path string> <flags int> <mode int> */
      /*Response: <result int> <errno int> <l-data 2 bytes=0>   // no data.*/
      
      char data[2];
      readRequest(sock, data, 2);
      int path_length = (data[0] << 8) + data[1];
      char *pathname = malloc(path_length);
      readRequest(sock, pathname, path_length);

      int flags;
      mode_t mode;
      readRequest(sock, &flags, sizeof(int));
      readRequest(sock, &mode, sizeof(mode_t));

      int fd = open(pathname, flags, mode); /* open 3 argument style TODO ? */
      sendResponse(sock, fd, errno, 0, NULL);
      free(pathname);
    }
    else if (opcode == close_call){
      /*Request: <opcode=2> <descriptor # int>*/
      /*Response: <result int> <errno int> <l-data 2 bytes=0>  //no data*/
      int fd;
      readRequest(sock, &fd, sizeof(int));
      int result = close(fd);
      sendResponse(sock, result, errno, 0, NULL);
    }
    else if (opcode == read_call){
      /*Request: <opcode=3> <descriptor int> <length=int>   // We can't pass pointers.*/
      /*Response: <result int> <errno int> <l-data 2 bytes > <string of l-data bytes>*/
      int fd, len;
      readRequest(sock, &fd, sizeof(int));
      readRequest(sock, &len, sizeof(int));
      char *data = malloc(len);
      int result = readRequest(fd, data, len);
      sendResponse(sock, result, errno, result, data);
      free(data);
    }
    else if (opcode == write_call){
      /*Request: <opcode=4> <descriptor int> <length 2 bytes><string to be written for length>*/
      /*Response: <result int> <errno int> <l-data 2-bytes=0> // no data*/
      int fd, len;
      readRequest(sock, &fd, sizeof(int));
      char temp[2];
      readRequest(sock, temp, 2);
      len = (temp[0] << 8) + temp[1];
      char *data = malloc(len);
      readRequest(sock, data, len);

      int result = write(fd, data, len);
      free(data);

      sendResponse(sock, result, errno, 0, NULL);
    }
    else if (opcode == seek_call){
      /*Request: <opcode=5> <descriptor int> <offset int> <whence int>*/
      /*Response: <result int> <errno int> <l-data 2-bytes=0> // no data*/
      int fd, whence;
      off_t offset;
      readRequest(sock, &fd, sizeof(int));
      readRequest(sock, &offset, sizeof(off_t));
      readRequest(sock, &whence, sizeof(int));

      int result = lseek(fd, offset, whence);
      sendResponse(sock, result, errno, 0, NULL);
    }
    else{
      write(2,"problems\n",9);
    }
  }
}

int main(int argc, char *argv[]){
  int sockfd, conn;
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    // TODO error
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(0);

  int temp = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (temp < 0){
    printf("something bad\n");
    return 1;
    // TODO error
  }
  socklen_t server_len = sizeof(serv_addr);
  getsockname(sockfd, (struct sockaddr *)&serv_addr, &server_len);
  printf("Listening on port: %d\n", ntohs(serv_addr.sin_port));

  listen(sockfd, 1);
  socklen_t cli_len = sizeof(cli_addr);
  while(1){
    conn = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len); 
    if( conn < 0){
      printf("darn\n");
      /* TODO error */
    }
    pid_t pid = fork();
    if(pid < 0){
      printf("eff\n");
      return 1;
      /* TODO error */
    }
    if( pid == 0){
      close(sockfd);
      do_child(conn);
      /* LOGIC */
      return 0;
    }
    else{
      close(conn);
    }
  }
  return 1;
}
