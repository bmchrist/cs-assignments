#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <netdb.h>

int sockfd; 

int readResponse(char *buf){
  /*Response: <result int> <errno int> <l-data 2 bytes > <string of l-data bytes>*/
  int result, err;
  read(sockfd, &result, sizeof(int));
  read(sockfd, &err, sizeof(int));
  char temp[2];
  read(sockfd, temp, 2);
  int data_len = (temp[0] << 8) + temp[1];
  char *data = malloc(data_len);
  read(sockfd, buf, data_len); 

  errno = err;
  return result; 
}

int main(int argc, char *argv[]){
  if( argc < 3 ){
    write(1, "Not enough arguments\n",21);
    return 1;
  }
  int port = atoi(argv[2]);
  struct hostent *server;
  struct sockaddr_in serv_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    write(1,"bad sock\n",9);
    return 1;
  }
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    write(1,"bad server\n",11);
    return 1;
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port);
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    write(1,"error connecting\n",17);
    return 1;
  }

  /* Call the linked user program */
  return entry(argc-2, argv+2);
}
int Open(const char *pathname, int flags, mode_t mode){
  /*Request: <opcode=1> <length-2 bytes> <path string> <flags int> <mode int> */
  /*Response: <result int> <errno int> <l-data 2 bytes=0>   // no data.*/
  char *msg;
  int a_l = strlen(pathname);
  int len = 1 + 2 + a_l + sizeof(int) + sizeof(mode_t); /* Calculate message length */

  msg = malloc(len);
  len = 0;
  msg[len++] = 1; /* opcode */
  msg[len++] = a_l >> 8;
  msg[len++] = a_l & 0xff;
  strcpy(&msg[len],pathname); len += a_l; /* pathname */
  a_l = sizeof(int);
  bcopy(&flags,&msg[len],a_l); len+=a_l;
  bcopy(&mode,&msg[len],sizeof(mode_t)); len+=sizeof(mode_t);
  
  write(sockfd, msg, len);
  free(msg);
  return readResponse(NULL);
}
int Close(int fd){
  /*Request: <opcode=2> <descriptor # int>*/
  /*Response: <result int> <errno int> <l-data 2 bytes=0>  //no data*/
  int len = 1+sizeof(int);
  char *msg = malloc(len);
  len = 0;
  msg[len++] = 2; /* opcode */
  bcopy(&fd, &msg[len], sizeof(int)); len+=sizeof(int);

  write(sockfd, msg, len);
  free(msg);
  return readResponse(NULL);
}
ssize_t Read(int fd, void *buf, int nbyte){
  /*Request: <opcode=3> <descriptor int> <length=int>   // We can't pass pointers.*/
  /*Response: <result int> <errno int> <l-data 2 bytes > <string of l-data bytes>*/
  int len = 1+sizeof(int)*2;
  char *msg = malloc(len);
  len = 0;
  msg[len++] = 3;
  bcopy(&fd, &msg[len], sizeof(int)); len+=sizeof(int);
  bcopy(&nbyte, &msg[len], sizeof(int)); len+=sizeof(int);

  write(sockfd, msg, len);
  free(msg);
  return readResponse(buf);
}
ssize_t Write(int fd, const void *buf, int nbyte){
  /*Request: <opcode=4> <descriptor int> <length 2 bytes><string to be written for length>*/
  /*Response: <result int> <errno int> <l-data 2-bytes=0> // no data*/
  int len = 1+sizeof(int) + 2 + nbyte;
  char *msg = malloc(len);
  len = 0;
  msg[len++] = 4;
  bcopy(&fd, &msg[len], sizeof(int)); len+=sizeof(int);
  msg[len++] = nbyte >> 8;
  msg[len++] = nbyte & 0xff;
  bcopy(buf, &msg[len], nbyte); len+=nbyte;
  
  write(sockfd, msg, len);
  free(msg);
  return readResponse(NULL);
}
off_t Seek(int fd, off_t offset, int whence){
  /*Request: <opcode=5> <descriptor int> <offset int> <whence int>*/
  /*Response: <result int> <errno int> <l-data 2-bytes=0> // no data*/
  /*printf("seeking\n");*/
  /*printf("fd:%d offset:%d whence:%d\n", fd, offset, whence);*/
  int len = 1+sizeof(int)*3;
  char *msg = malloc(len); 
  len = 0;
  msg[len++] = 5;
  bcopy(&fd, &msg[len], sizeof(int)); len+=sizeof(int);
  bcopy(&offset, &msg[len], sizeof(off_t)); len+=sizeof(off_t);
  bcopy(&whence, &msg[len], sizeof(int)); len+=sizeof(int);

  write(sockfd, msg, len);
  free(msg);
  return readResponse(NULL);
}
