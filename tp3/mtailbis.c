#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

#define ERROR "mtail: illegal argument\nusage: ./mtail [file] [number (int)]\n"
#define TAILLE_BUF 10


void tail_utile(const char* file, const int lignes) {

  char octet;
  int flux, indice, j, k;
  char *tampon = (char *) malloc(lignes*LINE_MAX);
  
  indice = 0;
  j = 0;
  k = 0;
  
  assert((flux = open(file,O_RDONLY)) != -1);

  /* Ecriture de chaque ligne dans chaque case du tableau tampon.
   Les lignes "inutiles" sont écrasées */
  while(read(flux, &octet, sizeof(char)) == 1) {
    tampon[indice*LINE_MAX + j] = octet;
    j++;
    if(octet == '\n') {
      tampon[indice*LINE_MAX + j]='\0'; 
      indice = ((indice + 1)%lignes); 
      j=0;
    }
  }

  /* Affichage des lignes, case par case de tampon */
  for(j = 0; j < lignes ; j++) {
    k = 0;
    octet = tampon[indice*LINE_MAX + k];
    while (octet != '\0' && octet != EOF) {
      octet = tampon[indice*LINE_MAX + k];
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
  int i = bufsize;
  while ((i >= 0) && (nb_lines < ntail)) {
    if (buffer[i] == '\0') {
      nb_lines++;
    }
    i--;
  }
  if (nb_lines == ntail) {
    return i;
  }
  else {
    *nlines = nb_lines;
    return -1;
  }
}


int
tail_before_pos
(int fd, int ntail)
{
  int i, nlines;
  char buf[TAILLE_BUF];

  lseek(fd, (-2*TAILLE_BUF), SEEK_CUR);
  assert(read(fd, buf, TAILLE_BUF) == TAILLE_BUF);

  i = index_tail_buffer(buf, TAILLE_BUF, ntail, &nlines);
  
  if (i == -1) {
    return tail_before_pos(fd, (ntail-nlines));
  }

  else {  /*  impression des lignes */
    while (fread(STDIN_FILENO, sizeof(char), TAILLE_BUF,(FILE *) fd)) {};
  }

  return 0;
}

int
tail_efficace
(const char* file, const int lignes)
{
  /* pas oublier de mettre lseek TAILLE_BUF apres la fin du fichier */
  /* ouvrir fichier, placer lseek, tail_before_pos */
  
  return 0;
}


int
main
(int argc, const char * argv[])
{
  if (argc != 3) { printf(ERROR); return 255;} 
  tail_utile(argv[2],atoi(argv[1])); 
  printf("\n-----------------------------\n");
  /* tail_efficace(argv[2],atoi(argv[1]));
  return 0;
}
