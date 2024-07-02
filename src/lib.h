#ifndef LIB_H
#define LIB_H

void string_convert(int mode);

void createArray(char *array, char c, int size);

char **createKeys(int size);

int analyzeRawText(char *rawText);

char decipher(char *cipherText);

#endif