
FLAGS += -g -Wall
CC = gcc

ifndef MICROSERVICE_DIR
MICROSERVICE_DIR = ../png_microservice/
endif

LIBS = $(MICROSERVICE_DIR)lib/crc32.c $(MICROSERVICE_DIR)lib/png.c

all: png-extract

png-extractGIF: $(MICROSERVICE_DIR)png-extract.c $(MICROSERVICE_DIR)png-extract-main.c $(LIBS)
	$(CC) $^ $(FLAGS) -o $@

clean:
	rm -rf *.dSYM/ png-extract
