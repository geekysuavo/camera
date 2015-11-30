
all:
	$(MAKE) -C chsqc-ca/scripts
	$(MAKE) -C chsqc-cacb/scripts
	$(MAKE) -C nhsqc-gb1/scripts
	$(MAKE) -C nhsqc-ubq-sigma/scripts
	$(MAKE) -C nhsqc-ubq-lambda/scripts
	$(MAKE) -C hnco/scripts
	$(MAKE) -C hnco-ext/scripts
	$(MAKE) -C noesy/scripts

clean:
	$(MAKE) -C chsqc-ca/scripts $@
	$(MAKE) -C chsqc-cacb/scripts $@
	$(MAKE) -C nhsqc-gb1/scripts $@
	$(MAKE) -C nhsqc-ubq-sigma/scripts $@
	$(MAKE) -C nhsqc-ubq-lambda/scripts $@
	$(MAKE) -C hnco/scripts $@
	$(MAKE) -C hnco-ext/scripts $@
	$(MAKE) -C noesy/scripts $@

distclean: clean
	rm -f */ser

again: clean all

