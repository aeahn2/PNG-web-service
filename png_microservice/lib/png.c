#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//#include <zlib.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;


/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  PNG *png = (PNG*)malloc(sizeof(PNG));
  if (!png) {
    return NULL;
  }
  png->fp = fopen(filename, mode);
  if (!png->fp) {
    free(png);
    return NULL;
  }
  strncpy(png->mode, mode, 3);
  png->mode[3] = '\0';
  unsigned char sig_valid[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  if (strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0) {
    unsigned char sig[8];
    fread(sig, 1, 8, png->fp);
    if (memcmp(sig, sig_valid, 8) != 0) {
      fclose(png->fp);
      free(png);
      return NULL;
    }
  } else if (strcmp(mode, "w") == 0) {
    fwrite(sig_valid, 1, 8, png->fp);
  } else {
    fclose(png->fp);
    free(png);
    return NULL;
  }
  return png;
}


/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
  if (!png || !chunk) {
    return 0;
  }

  chunk->len = 0; 

  fread(&chunk->len, sizeof(uint32_t), 1, png->fp);
  chunk->len = ntohl(chunk->len);

  char default_type[5] = {'a','a','a','a','\0'}; 
  strcpy(chunk->type, default_type); 
  fread(chunk->type, sizeof(char), 4, png->fp);
  chunk->type[4] = '\0';

  if (chunk->len) {
    chunk->data = (unsigned char*)calloc(chunk->len, 1);
    fread(chunk->data, sizeof(unsigned char), chunk->len, png->fp);
  }
  
  fread(&chunk->crc, sizeof(uint32_t), 1, png->fp);
  chunk->crc = ntohl(chunk->crc);

  return chunk->len + 12;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written. 
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  if (!png || !chunk) {
    return 0;
  }

  // Write length of chunk
  uint32_t len = htonl(chunk->len);
  fwrite(&len, sizeof(uint32_t), 1, png->fp);

  // Write type of chunk
  fwrite(chunk->type, sizeof(char), 4, png->fp);

  // Write data of chunk
  fwrite(chunk->data, sizeof(unsigned char), chunk->len, png->fp);

  // Write the crc of the chunk
  const void *data; 
  unsigned char buffer[chunk->len + 4]; 
  memcpy(buffer, chunk->type, 4);
  memcpy(buffer + 4, chunk->data, chunk->len); 
  data = buffer; 

  chunk->crc = 0; 
  crc32(data, chunk->len + 4, &chunk->crc); 
  uint32_t crc_ = htonl(chunk->crc); 
  fwrite(&crc_, sizeof(uint32_t), 1, png->fp);

  return chunk->len + 12;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  if (!chunk) {
    return;
  }
  if (chunk->len) {
    free(chunk->data);
    chunk->data = NULL;
  }
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  if (!png) {
    return;
  }
  fclose(png->fp);
  free(png);
}