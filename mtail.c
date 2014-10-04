#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


int 
naif_tail
(int n, const char * file)
{
  int fd;
  char buf;
  int pos;
  int cpt = 0;
  
  fd = open(file, O_RDONLY);
  assert(fd != -1);
  
  while (read(fd, &buf, sizeof(char)) == 1){ /* calcul du nombre de lignes */
    if (buf == '\n') {
      cpt++;
    }
  }
  
  close(fd);
  
  fd = open(file, O_RDONLY);
  assert(fd != -1);
  
  pos = cpt-n;
  cpt = 0;
  
  while (read(fd, &buf, sizeof(char)) == 1) { /* impression des lignes */
    if(cpt >= pos) {
      write(STDIN_FILENO, &buf, sizeof(char));
    }
    if (buf == '\n') {
      cpt++;
    }
  }
  
  close(fd);
  
  return 0;
}

void tail_utile(const char* file, const int lignes) {
  char tampon[LINE_MAX][LINE_MAX]; /* Nombre arbitraire, à préciser dans le README */
  char octet;
  int flux,indice = 0,j = 0,k = 0;

  assert((flux = open(file,O_RDONLY)) != -1);

  /* Ecriture de chaque ligne dans chaque case du tableau tampon.
   Les lignes "inutiles" sont écrasées */
  while(read(flux, &octet, sizeof(char)) == 1) {
    tampon[indice][j++] = octet;
    if(octet == '\n') {
      tampon[indice][j]='\0'; 
      indice = ((indice + 1)%lignes); 
      j=0;
    }
  }

  /* Affichage des lignes, case par case de tampon */
  for(j = 0; j < lignes ; j++) {
    k = 0;
    octet = tampon[indice][k];
    while (octet != '\0' && octet != EOF) {
      octet = tampon[indice][k];
      write(STDIN_FILENO,&octet,sizeof(char));
      k++;
    }
    indice = (indice + 1)%lignes;
  }

  close(flux);
}
 
int
index_tail_buffer
(const char *buffer, int bufsize, int ntail, int *nlines)
{
  int nb_lines;
  int i = 0;
  while ((i <= bufsize) && (nb_lines < ntail)) {
    if (buffer[i] == '\0') {
      nb_lines++;
    }
    i++;
  }
  if (nb_lines == ntail) {
    return nb_lines;
  }
  else {
    *nlines = bufsize;
    return -1;
  }
}


int
tail_before_pos
(int fd, unsigned int pos, int ntail)
{
  
}


int
main
(int argc, const char * argv[])
{
  naif_tail(atoi(argv[1]), argv[2]);
  return 0;
}
