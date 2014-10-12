set title "Temps d'execution et taille du tampon"
set logscale x
set xlabel "taille du tampon (octets)"
set logscale y
set ylabel "temps (secondes)"
set style data linespoints
plot "mcat-tm.dat" using 1:2 title "real", \
     "mcat-tm.dat" using 1:3 title "user", \
     "mcat-tm.dat" using 1:4 title "sys"
pause -1  "Hit return to continue"
