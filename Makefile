
# CC: compiler binary filename.
CC=gcc

# CFLAGS: compilation flags.
CFLAGS=-s -static -O3 -ffast-math -fopenmp -std=c99 -Wall -Wformat
CFLAGS+= -msse -msse2 -msse3

# LIBS: library linkage flags.
LIBS=-lfftw3f -lm

# USE_AVX: set to 'y' or 'n' to enable or disable AVX extensions.
USE_AVX=n
ifeq ($(USE_AVX),y)
  # enables AVX extensions.
  CFLAGS+= -mavx
else
  # disables AVX extensions and their warning messages.
  CFLAGS+= -Wno-psabi
endif

# installation configuration variables.
INSTALL=install
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man/man1

# BIN: binary output filenames.
BIN=bin/camera

# MAN: manual page filenames.
MAN=man/camera.1

# SRC, OBJ: basenames and filenames of output object files.
SRC=hx arr nmrpipe task sched util camera
OBJ=$(addsuffix .o,$(addprefix src/,$(SRC)))

# SUFFIXES: registered filename suffixes for implicit make rules.
.SUFFIXES: .c .o

# all: global, default make target.
all: $(BIN)

# BIN: binary linkage make target.
$(BIN): $(OBJ)
	@echo " LD $@"
	@$(INSTALL) -d $(dir $@)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# .c.o: source compilation make target.
.c.o:
	@echo " CC $^"
	@$(CC) $(CFLAGS) -c $^ -o $@

# install: target to install all generated output files.
install: install-bin install-man

# install-bin: target to install all binary files.
install-bin: $(BIN)
	@echo " INSTALL $(BIN)"
	@$(INSTALL) -d $(BINDIR)
	@$(INSTALL) $(BIN) $(BINDIR)

# install-man: target to install manual pages.
install-man: $(MAN)
	@echo " INSTALL $(MAN)"
	@$(INSTALL) -d $(MANDIR)
	@$(INSTALL) $(MAN) $(MANDIR)

# clean: target to remove all generated intermediate and output files.
clean:
	@echo " CLEAN"
	@rm -f $(BIN) $(OBJ)

# distclean: target to obliterate literally all output files.
distclean: clean
	@echo " DISTCLEAN"
	$(MAKE) -C data clean
	$(MAKE) -C tests clean
	$(MAKE) -C figs distclean

# again: target to fully recompile all sources.
again: clean all

# fixme: target to search all sources for 'fix me' statements.
fixme:
	@echo " FIXME"
	@grep -RHni --color fixme src/*.[ch] || echo " None found"

# lines: target to count lines of all sources.
lines:
	@echo " WC"
	@wc -l src/*.[ch]

