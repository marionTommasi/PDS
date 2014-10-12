CC = gcc

CFLAGS = -Wall -Werror -ansi -pedantic

CFLAGS += -D_XOPEN_SOURCE=500

CFLAGS += -g

TEST_FILE = test.txt

STATS = mcat-tm.dat

SIZE = 69133742

all:plot

%:%.c
	$(CC) $(CFLAGS) -o $@ $<

run:mcat.sh mcat-scd filesize
	test -s $(TEST_FILE) || ./filesize $(SIZE) > $(TEST_FILE)
	./mcat.sh

plot:run.gnu
	test -s $(STATS) || make run
	gnuplot run.gnu

clean:
	rm -f -v *~ *.o
	if test -f $(TEST_FILE); then rm -f -v $(TEST_FILE); fi
	if test -f $(STATS); then rm -f -v $(STATS); fi
	if test -f filesize; then rm -f -v filesize; fi
	if test -f mcat-scd; then rm -f -v mcat-scd; fi
