#include "png-extract.h"

int png_extract(const char *png_filename, const char *dst_filename) {
  PNG *png = PNG_open(png_filename, "r");
  if (!png) { 
    return ERROR_INVALID_FILE; }
  FILE *dst_fp;
  dst_fp = fopen(dst_filename, "w"); 
  if (!dst_fp) {
    fclose(dst_fp);
    PNG_close(png); 
    return ERROR_INVALID_FILE;
  } 
  int fileFound = 0; 

  // Read chunks until reaching "IEND" or an invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }

    // Report data about the chunk to the command line:
    if ( strcmp(chunk.type, "file") == 0 ) {
      fileFound = 1; 
      // Write file data into destination file. 
      const void *file_data; 
      unsigned char *buffer = (unsigned char *)calloc(1, chunk.len);
      memcpy(buffer, chunk.data, chunk.len); 
      file_data = buffer; 
      fwrite(file_data, sizeof(unsigned char), chunk.len, dst_fp);  
      free(buffer); 
    }

    // Check for the "IEND" chunk to exit:
    if ( strcmp(chunk.type, "IEND") == 0 ) {
      PNG_free_chunk(&chunk);
      break;  
    }

    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }
  fclose(dst_fp); 
  PNG_close(png);
  if (fileFound) {
    return 0; 
  }
  return 4;
}
