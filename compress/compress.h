#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include <stdio.h>

FILE *compress_file(FILE *in);
FILE *compress_file_from_path(const char *in_path);

#endif