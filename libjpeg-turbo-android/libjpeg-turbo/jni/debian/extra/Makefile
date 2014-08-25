CFLAGS = -O2 -Wall -g
CC = cc
INSTALL = install -m755 -o root -g root
INSTALLDIR = install -m755 -o root -g root -d
DESTDIR =
prefix  = /usr/local
bindir  = $(prefix)/bin
mandir  = $(prefix)/share/man/man1

all: jpegexiforient

jpegexiforient: jpegexiforient.c
	$(CC) $(CFLAGS) -o jpegexiforient jpegexiforient.c
clean:
	-rm -f jpegexiforient
install:
	$(INSTALLDIR) $(DESTDIR)$(bindir) 
	$(INSTALLDIR) $(DESTDIR)$(mandir) 
	$(INSTALL) jpegexiforient    $(DESTDIR)$(bindir)
	$(INSTALL) jpegexiforient.1  $(DESTDIR)$(mandir)
	$(INSTALL) exifautotran      $(DESTDIR)$(bindir)
	$(INSTALL) exifautotran.1    $(DESTDIR)$(mandir)
