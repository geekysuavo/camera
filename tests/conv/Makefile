
DATFILES=  conv-by-method.dat
DATFILES+= conv-by-lambda.dat
DATFILES+= conv-by-steps.dat
DATFILES+= conv-by-def.dat
DATFILES+= spec-by-def.dat

.SUFFIXES: .m .dat

all: $(DATFILES)

.m.dat:
	octave -q $^ $@

clean:
	rm -f $(DATFILES)

again: clean all

