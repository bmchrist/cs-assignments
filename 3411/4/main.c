#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
  if( argc < 4){
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  char *program = argv[1];
  char *dir = argv[argc-1];
  argv[argc-1] = NULL; /* mark the end of our arguments for exec */

  int checkdir = open(dir, O_RDONLY);
  if (checkdir >= 0){
    fprintf(stderr, "File named %s already exists\n",dir);
    close(checkdir);
    return 1;
  }

  int status = mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if(status < 0){
    fprintf(stderr, "Could not create directory\n");
    return 1;
  }
  chdir(dir);

  /* Need to make files in the dir TODO */
  int file_in = open("0", O_WRONLY | O_CREAT, 0600);
  int file_out = open("1", O_WRONLY | O_CREAT, 0600);
  int file_err = open("2", O_WRONLY | O_CREAT, 0600);

  chdir("..");

  if( file_in < 0 || file_out < 0 || file_err < 0){
    fprintf(stderr, "Could not create files\n");
    return 1;
  }

  int pipe_in[2], pipe_out[2], pipe_err[2];
  pipe(pipe_in);
  pipe(pipe_out);
  pipe(pipe_err);

  if (fork() == 0){/* Child */
    close(pipe_in[1]);
    close(pipe_out[0]);
    close(pipe_err[0]);

    /* Connect std: in,out,and err to the pipes */
    close(0);
    dup(pipe_in[0]);
    close(1);
    dup(pipe_out[1]);
    close(2);
    dup(pipe_err[1]);

    execvp(program,(argv+1));/* Pass all but the first arg */
    fprintf(stderr,"an error occured with script");
    return 1;
  }
  /* Parent */
  close(pipe_in[0]);
  close(pipe_out[1]);
  close(pipe_err[1]);

  int read_size = -1;
  while(read_size != 0){ /* Read until one of the pipes is closed */
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(pipe_out[0], &read_set);
    FD_SET(pipe_err[0], &read_set);
    FD_SET(0, &read_set);
    char buff[100];

    /* Wait until we have input on either stdin, or from the child */
    int ret = select(FD_SETSIZE, &read_set, NULL, NULL, NULL);

    /* Child has sent stderr */
    if (FD_ISSET(pipe_err[0], &read_set)){
      read_size = read(pipe_err[0], &buff, 100);
      write(2, &buff, read_size);/* Send to our stderr */
      write(file_err, &buff, read_size); /* and log it */
    }
    /* Child has sent stdout */
    if (FD_ISSET(pipe_out[0], &read_set)){
      read_size = read(pipe_out[0], &buff, 100);
      write(1, &buff, read_size);/* send to our stdout */
      write(file_out, &buff, read_size);/* and log it */
    }
    /* We have received input */
    if (FD_ISSET(0, &read_set)){
      read_size = read(0, &buff, 100);
      write(pipe_in[1], &buff, read_size);/* send it to the child */
      write(file_in, &buff, read_size);/* and log it*/
    }
  }

  wait(&status);
  return 0;
}
