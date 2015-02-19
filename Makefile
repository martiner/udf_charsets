
PREFIX = /usr/lib
SOURCE = udf_charsets.c
SOFILE = udf_charsets.so
MAKEFILE = Makefile

CC = gcc
INC = ../include

all: $(SOURCE)
	gcc -I$(INC) -shared -o $(SOFILE) $(SOURCE)

install: $(SOFILE)
	cp $(SOFILE) $(PREFIX)

version: $(SOURCE) $(Makefile)
	perl -ne 'print $$1 if /\$$VERSION\s*=\s*(\d.*?);/' $(SOURCE) > version

dist: version
	VERSION=`cat version` ; rm -rf udf_charsets-$$VERSION ; mkdir udf_charsets-$$VERSION && cp `cat MANIFEST` udf_charsets-$$VERSION && rm -f udf_charsets-$$VERSION.tar* && tar cvf udf_charsets-$$VERSION.tar udf_charsets-$$VERSION && gzip udf_charsets-$$VERSION.tar && mv udf_charsets-$$VERSION.tar.gz ..
	
clean:
	rm -rf $(SOFILE)
