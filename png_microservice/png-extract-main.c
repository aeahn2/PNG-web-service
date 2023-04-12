#include "png-extract.h"

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <File Name>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_extract(argv[1], argv[2]);
}
