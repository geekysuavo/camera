
DATFILES=  nonlin.dat
DATFILES+= nonlin.corr

.SUFFIXES: .m .dat .corr

all: $(DATFILES)

.m.dat:
	octave -q $^ $@

.dat.corr:
	octave -q $(^:.dat=-corr.m) $^ $@

clean:
	rm -f $(DATFILES)

again: clean all

