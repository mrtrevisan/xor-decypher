#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lib.h"

void string_convert(int mode)
{
    if (mode == 1)
    {
        const char *base64_string = "QWNvcmRhUGVkcmluaG9RdWVob2pldGVtY2FtcGVvbmF0bw==";
        char *hex_string = base64_to_hex(base64_string);

        if (hex_string)
        {
            printf("Base64: %s\n", base64_string);
            printf("Hexadecimal: %s\n", hex_string);
            free(hex_string);
        }
    }
    else
    {
        const char *hex_string = "41636f72646150656472696e686f517565686f6a6574656d63616d70656f6e61746f";
        char *base64_string = hex_to_base64(hex_string);

        if (base64_string)
        {
            printf("Hexadecimal: %s\n", hex_string);
            printf("Base64: %s\n", base64_string);
            free(base64_string);
        }
    }

    return;
}

void createArray(char *array, char c, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = c;
    }
    array[size] = '\0';
}

char **createKeys(int size)
{
    char **keys = (char **)malloc(26 * sizeof(char *));

    for (int i = 0; i < 26; i++)
    {
        keys[i] = (char *)malloc(size * sizeof(char) + 1);
        createArray(keys[i], (char)(i + 'A'), size);
    }

    return keys;
}

// digraph
int analyzeRawText(char *rawText)
{
    int len = strlen(rawText);
    int count = 0;

    char *vowels = "AEIOUaeiou";
    char *consonants = "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";

    for (int i = 0; i < len - 1; i++)
    {
        if (strchr(consonants, rawText[i]) != NULL && strchr(vowels, rawText[i + 1]) != NULL)
        {
            count++;
        }
    }
    return count;
}

void decipher(char *cipherText)
{
    int len = strlen(cipherText);

    char **keys = createKeys(len);

    char *bestRawText = (char *)malloc(len * sizeof(char) + 1);
    char bestKey = 'a';
    int bestCount = -1;

    for (int i = 0; i < 26; i++)
    {
        char *key = hex_encode(keys[i]);
        char *res = xorHexStrings(cipherText, key);

        if (res)
        {
            char *rawText = hex_decode(res);
            int count = analyzeRawText(rawText);

            if (count > bestCount)
            {
                bestCount = count;
                strcpy(bestRawText, rawText);
                bestKey = keys[i][0];
            }

            free(rawText);
            free(res);
            free(key);
        }
    }

    for (int i = 0; i < 26; i++)
    {
        free(keys[i]);
    }
    free(keys);

    if (bestRawText)
    {
        printf("Key: %c\n", bestKey);
        printf("Raw Text: %s\n", bestRawText);
        free(bestRawText);
    }
}