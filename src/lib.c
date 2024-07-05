#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lib.h"

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
int countDigraphs(char *rawText)
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

float scoreRawText(const char *plaintext, int len) {
    float frequencies[256] = {0};
    frequencies['A'] = 14.63; frequencies['B'] = 1.04; frequencies['C'] = 3.88;
    frequencies['D'] = 4.99; frequencies['E'] = 12.57; frequencies['F'] = 1.02;
    frequencies['G'] = 1.30; frequencies['H'] = 1.28; frequencies['I'] = 6.18;
    frequencies['J'] = 0.40; frequencies['K'] = 0.02; frequencies['L'] = 2.78;
    frequencies['M'] = 4.74; frequencies['N'] = 5.05; frequencies['O'] = 10.73;
    frequencies['P'] = 2.52; frequencies['Q'] = 1.20; frequencies['R'] = 6.53;
    frequencies['S'] = 7.81; frequencies['T'] = 4.34; frequencies['U'] = 4.63;
    frequencies['V'] = 1.67; frequencies['W'] = 0.01; frequencies['X'] = 0.21;
    frequencies['Y'] = 0.01; frequencies['Z'] = 0.47;
    frequencies['a'] = 14.63; frequencies['b'] = 1.04; frequencies['c'] = 3.88;
    frequencies['d'] = 4.99; frequencies['e'] = 12.57; frequencies['f'] = 1.02;
    frequencies['g'] = 1.30; frequencies['h'] = 1.28; frequencies['i'] = 6.18;
    frequencies['j'] = 0.40; frequencies['k'] = 0.02; frequencies['l'] = 2.78;
    frequencies['m'] = 4.74; frequencies['n'] = 5.05; frequencies['o'] = 10.73;
    frequencies['p'] = 2.52; frequencies['q'] = 1.20; frequencies['r'] = 6.53;
    frequencies['s'] = 7.81; frequencies['t'] = 4.34; frequencies['u'] = 4.63;
    frequencies['v'] = 1.67; frequencies['w'] = 0.01; frequencies['x'] = 0.21;
    frequencies['y'] = 0.01; frequencies['z'] = 0.47;

    float score = 0.0;
    for (int i = 0; i < len; ++i) {
        score += frequencies[(int)plaintext[i]];
    }
    return score;
}

void analyzeRawText(char* rawText, char key, float* bestScores, char* bestKeys)
{
    int len = strlen(rawText);
    float score = scoreRawText(rawText, len);
    
    for (int i = 0; i < 5; i++)
    {
        if (score > bestScores[i])
        {
            for (int j = 4; j > i; j--)
            {
                bestScores[j] = bestScores[j - 1];
                bestKeys[j] = bestKeys[j - 1];
            }
            bestScores[i] = score;
            bestKeys[i] = key;
            break;
        }
    }
}

char* decipher(char *cipherText)
{
    int len = strlen(cipherText);
    char **keys = createKeys(len);

    char* bestKeys = (char*)malloc(5 * sizeof(char) + 1);    
    float bestScores[5] = {-1, -1, -1, -1, -1};

    for(int i=0; i<5; i++) {
        bestKeys[i] = 'a';
    }
    bestKeys[5] = '\0';

    for (int i = 0; i < 26; i++)
    {
        char *key = hex_encode(keys[i]);
        char *res = xorHexStrings(cipherText, key);

        if (res)
        {
            char *rawText = hex_decode(res);

            // if(keys[i][0] == 'Y')
            //     printf("%s\n", rawText);

            // if(keys[i][0] == 'U')
            //     printf("%s\n", rawText);

            analyzeRawText(rawText, keys[i][0], bestScores, bestKeys);

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
    
    return bestKeys;
}