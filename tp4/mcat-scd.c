#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define USAGE "mcat-scd: illegal arguments\n./mcat-scd [file] [buffer size (in bytes)]\n"

void mcat(const char* file, int buffersize) {
  char* buffer = (char *) malloc(buffersize);
  int flux = 0;
  int lus = 0;
 
  assert((flux = open(file,O_RDONLY)) != -1);

  while((lus = read(flux, buffer, buffersize)) != 0) {
    write(STDOUT_FILENO, buffer,lus);
  }

  close(flux);

}  

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf(USAGE);
    return 255;
  }
  mcat(argv[1],atoi(argv[2]));
  return 0;
}
