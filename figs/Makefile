
TARGETS= 1 2 3 4 5 6 7 8 s1 s2 s3 toc

PNG=$(addsuffix .png,$(addprefix final/figure-,$(TARGETS)))
TIF=$(addsuffix .tif,$(addprefix final/figure-,$(TARGETS)))

TIFOPTS=  -density 1000x1000
TIFOPTS+= -units PixelsPerInch
TIFOPTS+= -colorspace CMYK
TIFOPTS+= -compress LZW

.SUFFIXES: .png .tif

all: final $(PNG) $(TIF)

final:
	install -d $@

final/figure-1.png: conv-by-method/output.png
	cp $^ $@

final/figure-2.png: conv-bench/output.png
	cp $^ $@

final/figure-3.png: conv-by-def/output.png
	cp $^ $@

final/figure-4.png: spec-by-def/output.png
	cp $^ $@

final/figure-5.png: conv-by-lambda/output.png
	cp $^ $@

final/figure-6.png: nonlin/output.png
	cp $^ $@

final/figure-7.png: split/output.png
	cp $^ $@

final/figure-8.png: decoup/output.png
	cp $^ $@

final/figure-s1.png: nhsqc/output.png
	cp $^ $@

final/figure-s2.png: chsqc/output.png
	cp $^ $@

final/figure-s3.png: hnco/output.png
	cp $^ $@

final/figure-toc.png: toc/output.png
	cp $^ $@

%.png:
	$(MAKE) -C $(dir $@)

.png.tif:
	convert $(TIFOPTS) $^ $@

clean:
	rm -f $(PNG) $(TIF)

distclean: clean
	$(MAKE) -C bench clean
	$(MAKE) -C conv-by-def clean
	$(MAKE) -C conv-by-lambda clean
	$(MAKE) -C conv-by-method clean
	$(MAKE) -C conv-by-steps clean
	$(MAKE) -C decoup clean
	$(MAKE) -C hnco clean
	$(MAKE) -C chsqc clean
	$(MAKE) -C nhsqc clean
	$(MAKE) -C nonlin clean
	$(MAKE) -C spec-by-def clean
	$(MAKE) -C split clean
	$(MAKE) -C toc clean

again: clean all

