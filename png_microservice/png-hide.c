#include "png-hide.h" 
#include "lib/crc32.h"

int png_hide(const char *png_filename_source, const char *insert_filename, const char *png_filename_out) {
  // Open the file specified in argv[1] for reading and argv[2] for writing:
  PNG *png = PNG_open(png_filename_source, "r");
  if (!png) { return ERROR_INVALID_FILE; }

  PNG *out = PNG_open(png_filename_out, "w");

  // Create file chunk to hide
  FILE *fp = fopen(insert_filename, "r"); 
  long file_len; 
  fseek(fp, 0L, SEEK_END); 
  file_len = ftell(fp);
  rewind(fp); 

  PNG_Chunk file_chunk;
  file_chunk.len = 0; 
  file_chunk.len = file_len; 
  strcpy(file_chunk.type, "file"); 
  file_chunk.data = (unsigned char *)calloc(file_len,1); 
  fread(file_chunk.data, 1, file_len, fp);  

  // Calculate CRC for file_chunk
  const void *data; 
  unsigned char crc_buffer[file_len + 4]; 
  memcpy(crc_buffer, file_chunk.type, 4);
  memcpy(crc_buffer + 4, file_chunk.data, file_len);
  data = crc_buffer; 

  file_chunk.crc = 0; 
  crc32(data, file_len + 4, &file_chunk.crc); 
  fclose(fp); 

  int shouldHide = 1; 

  // Read chunks until reaching "IEND" or in invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      PNG_close(out);
      return ERROR_INVALID_CHUNK_DATA;
    }

    if ( strcmp(chunk.type, "IHDR") == 0 ) {
      PNG_write(out, &chunk); 
    }
    // Check for the "IEND" chunk to exit:
    else if ( strcmp(chunk.type, "IEND") == 0 ) {
      PNG_write(out, &chunk); 
      PNG_free_chunk(&chunk);
      break;  
    }
    PNG_write(out, &chunk);
    if (shouldHide) {
      PNG_write(out, &file_chunk); 
      shouldHide = 0;  
    }
    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }
  PNG_free_chunk(&file_chunk);
  PNG_close(out);
  PNG_close(png);
  return 0;
}