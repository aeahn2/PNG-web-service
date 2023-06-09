#include "png-hide.h" 

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 4) {
    printf("Usage: %s <PNG Source File> <Insert File> <PNG Output File>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_hide(argv[1], argv[2], argv[3]);
}
