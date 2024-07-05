#ifndef LIB_H
#define LIB_H

void string_convert(int mode);

void createArray(char *array, char c, int size);

char **createKeys(int size);

void analyzeRawText(char* rawText, char key, float* bestScores, char* bestKeys);

char* decipher(char *cipherText);

#endif