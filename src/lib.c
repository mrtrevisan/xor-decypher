#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
int digraph_analyze(char *rawText)
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

double calculate_score(const char *text) {
    double portuguese_freq[26] = {
        14.63, // A
        1.04,  // B
        3.88,  // C
        4.99,  // D
        12.57, // E
        1.02,  // F
        1.30,  // G
        1.28,  // H
        6.18,  // I
        0.40,  // J
        0.02,  // K (not typically used in Portuguese words, very low frequency)
        2.78,  // L
        4.74,  // M
        5.05,  // N
        10.73, // O
        2.52,  // P
        1.20,  // Q
        6.53,  // R
        7.81,  // S
        4.34,  // T
        4.63,  // U
        1.67,  // V
        0.01,  // W (not typically used in Portuguese words, very low frequency)
        0.21,  // X
        0.01,  // Y (not typically used in Portuguese words, very low frequency)
        0.47   // Z
    };
    int letter_count[26] = {0};
    int total_letters = 0;

    // Count the frequency of each letter in the decoded text
    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            letter_count[toupper(text[i]) - 'A']++;
            total_letters++;
        }
    }

    // Calculate the score based on the difference from the English frequency table
    double score = 0.0;
    for (int i = 0; i < 26; i++) {
        double observed_freq = (double)letter_count[i] / total_letters * 100;
        score += (observed_freq - portuguese_freq[i]) * (observed_freq - portuguese_freq[i]);
    }

    return score;
}

char decipher(char *cipherText)
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
            //int count = analyzeRawText(rawText);
            int count = calculate_score(rawText);

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
        free(bestRawText);
    }

    return bestKey;
}