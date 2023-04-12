
FLAGS += -g -Wall
CC = gcc

ifndef MICROSERVICE_DIR
MICROSERVICE_DIR = ../png_microservice/
endif

LIBS = $(MICROSERVICE_DIR)lib/crc32.c $(MICROSERVICE_DIR)lib/png.c

all: png-extractGIF

png-extractGIF: $(MICROSERVICE_DIR)png-extractGIF.c $(MICROSERVICE_DIR)png-extractGIF-main.c $(LIBS)
	$(CC) $^ $(FLAGS) -o $@

clean:
	rm -rf *.dSYM/ png-extractGIF
