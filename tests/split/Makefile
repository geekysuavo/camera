
DATFILES=split.dat

.SUFFIXES: .m .dat

all: $(DATFILES)

.m.dat:
	octave -q $^ $@

clean:
	rm -f $(DATFILES)

again: clean all

