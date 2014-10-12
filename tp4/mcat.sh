MCAT=./mcat-scd

MCAT_INPUT=test.txt

TIME_FILE=mcat-tm.dat
    
TIME_CMD="/usr/bin/time"

TIME_OPT="-f %e %U %S"
    
rm -f $TIME_FILE && echo "# buffer real user sys" > $TIME_FILE
    
for size in `awk 'BEGIN { for( i=1; i<=8388608; i*=2 ) print i }'`; do
    export MCAT_BUFSIZ=$size
    echo -n $size>> $TIME_FILE && $TIME_CMD "$TIME_OPT" $MCAT $MCAT_INPUT $size > /dev/null 2>> $TIME_FILE
done
