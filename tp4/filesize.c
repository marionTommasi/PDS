#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void create (int size) {
  int i;
  char spam='a';
  
  for(i = 0; i < size; i++) {
	spam = (i % 26) + 'a';
    write(STDOUT_FILENO, &spam , 1);
	}

}

int main(int argc, char* argv[]) {
  create(atoi(argv[1]));
  return 0;
}
