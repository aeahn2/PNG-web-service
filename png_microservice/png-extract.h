#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include <errno.h>
#ifdef __cplusplus
extern "C" {
#endif

int png_extract(const char *png_filename, const char *dst_filename);

#ifdef __cplusplus
}
#endif