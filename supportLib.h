/* Downloaded from https://repo.progsbase.com - Code Developed Using progsbase. */
#include <stdlib.h>
#include <stdio.h>

unsigned char *DoubleArrayToByteArray(double *data, size_t length);
void WriteToFile(double *data, size_t dataLength, char *filename);
double *ByteArrayToDoubleArray(unsigned char *data, size_t length);
