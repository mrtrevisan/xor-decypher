#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utils.h"
#include "lib.h"

#define TAM 1
#define KEY_GUESSES 4

// NOMES: Andriza Campanhol, Pedro Henrique da Silva Hinerasky, Mauro Roberto Trevisan e Ramon Godoy Izidoro

// void insert_norm(float norm, int keysize, float* norm_array, int* keysize_array, int max_size) {
//     if (norm < norm_array[max_size - 1]) {
//         int i = max_size - 1;
//         while (i > 0 && norm_array[i - 1] > norm) {
//             norm_array[i] = norm_array[i - 1];
//             keysize_array[i] = keysize_array[i - 1];
//             i--;
//         }
//         norm_array[i] = norm;
//         keysize_array[i] = keysize;
//     }
// }

// char** separate_in_blocks(char *cipherText, int keysize)
// {
//     int cipherTextSize = strlen(cipherText);
//     int blocks = cipherTextSize / keysize;

//     if(cipherTextSize % keysize != 0) {
//         blocks++;
//     }

//     char **block = (char **)malloc(blocks * sizeof(char *));
//     for (int i = 0; i < blocks; i++)
//     {
//         block[i] = (char *)malloc(keysize * sizeof(char) + 1);
//         strncpy(block[i], cipherText + i * keysize, keysize);
        
//         if(strlen(block[i]) < keysize)
//         {
//             int j = strlen(block[i]);
//             for(; j < keysize; j++) {
//                 block[i][j] = ' ';
//             }
//         }

//         block[i][keysize] = '\0';  
//         // printf("block len %d: %d\n", i, strlen(block[i]));     
//     }

//     return block;
// }

// char** transpose_blocks(char **blocks, int keysize, int blocksSize)
// {
//     char **transposed = (char **)malloc(keysize * sizeof(char *));
//     for (int i = 0; i < keysize; i++)
//     {
//         transposed[i] = (char *)malloc(blocksSize * sizeof(char) + 1);
//         for (int j = 0; j < blocksSize; j++)
//         {
//             transposed[i][j] = blocks[j][i];
//         }
//         transposed[i][blocksSize] = '\0';
//     }

//     return transposed;
// }

// char** generateCombinations(char **arrays, int n, double* total) {
//     int totalCombinations = pow(KEY_GUESSES, n); // 5^N combinations
//     char** combinations = malloc(sizeof(char*) * totalCombinations);
//     char current[n + 1];

//     for (int i = 0; i < totalCombinations; i++) {
//         int num = i;
//         for (int j = 0; j < n; j++) {
//             int index = num % KEY_GUESSES;
//             current[j] = arrays[j][index];
//             num /= KEY_GUESSES;
//         }
//         current[n] = '\0';
//         combinations[i] = malloc(sizeof(char) * (n+1));
//         strcpy(combinations[i], current);
//         // printf("%s\n", current);
//     }
//     *total = totalCombinations; 
//     return combinations;
// }

// void decipher2(char *cipherText, int keysize)
// {
//     keysize = 4;

//     char** blocks = separate_in_blocks(cipherText, keysize);
//     int blocksSize = strlen(cipherText) / keysize;
//     if (strlen(cipherText) % keysize != 0)
//     {
//         blocksSize++;
//     }

//     char** transposed = transpose_blocks(blocks, keysize, blocksSize);

//     char* key = malloc(sizeof(char) * keysize + 1);
//     char** bestKeys = malloc(sizeof(char*) * keysize);

//     // for(int i = 0; i < keysize; i++) {
//     //     bestKeys[i] = decipher(transposed[i]);
//     //     // key[i] = bestKey;
//     //     // printf("Bloco: %s\n Possíveis Chaves para o bloco:\n", transposed[i]);

//     //     // for (int j = 0; j < KEY_ARR_TAM; j++) {
//     //     //     printf("%c\n", bestKeys[i][j]);
//     //     // }
//     // }
    
//     // double total_combinations = 0;
//     // char** combinations = generateCombinations(bestKeys, keysize, &total_combinations);
    
//     double bestcount = 0;
//     char* bestRawText = NULL;
//     char* bestKey = NULL;

//     // strcpy(key, combinations[i]);

//     key[0] = 'U';
//     key[1] = 'F';
//     key[2] = 'S';
//     key[3] = 'M';
//     key[keysize] = '\0';

//     char* res = NULL;

//     printf("Size: %d\nKey: %s\n", keysize, key);

//     printf("Blocksize: %d\n", blocksSize);

//     for (int i=0; i < blocksSize; i++) {
//         char* encoded_block = base64_decode(blocks[i]);
//         char *deciphered = xorHexStrings(hex_encode(encoded_block), hex_encode(key));
//         printf("%s", hex_to_base64(deciphered));
//         // if (res == NULL)
//         // {
//         //     res = malloc((strlen(deciphered) + 1)*sizeof(char));
//         //     strcpy(res, deciphered);
//         // }
//         // else
//         // {
//         //     char *temp = malloc((strlen(res) + strlen(deciphered) + 1)*sizeof(char));
//         //     strcpy(temp, res);
//         //     strcat(temp, deciphered);
//         //     free(res);
//         //     res = temp;
//         // }
//         free(deciphered);
//         free(encoded_block);
//     }

//     if (res)
//     {
//         char *rawText = hex_decode(res);
//         printf("\n");
//         // printf("Keysize: %d\n", keysize);
//         // printf("Key: %s\n", key);
//         printf("Deciphered text: %s\n", rawText);
//         printf("\n");

//         // double count = calculate_score(rawText);

//         // if (count > bestcount)
//         //     {
//         //         bestcount = count;
//         //         if (bestRawText == NULL) {
//         //             bestRawText = malloc(strlen(rawText) * sizeof(char));
//         //         }
//         //         strcpy(bestRawText, rawText);
//         //         bestKey = key;
//         // }

//         free(rawText);
//     }

//     free(res);
    
//     // for (int i = 0; i < total_combinations; i++)
//     // {
//         // strcpy(key, combinations[i]);
//         // key[keysize] = '\0';

//         // char *encoded_key = hex_encode(key);

//         // char* res = NULL;

//         // printf("Size: %d\nKey: %s\n", keysize, key);

//         // for (int i=0; i < blocksSize; i++) {
//         //     char* encoded_block = base64_to_hex(blocks[i]);
//         //     char *deciphered = xorHexStrings(encoded_block, encoded_key);
//         //     if (res == NULL)
//         //     {
//         //         res = malloc((strlen(deciphered) + 1)*sizeof(char));
//         //         strcpy(res, deciphered);
//         //     }
//         //     else
//         //     {
//         //         char *temp = malloc((strlen(res) + strlen(deciphered) + 1)*sizeof(char));
//         //         strcpy(temp, res);
//         //         strcat(temp, deciphered);
//         //         free(res);
//         //         res = temp;
//         //     }
//         //     free(deciphered);
//         //     free(encoded_block);
//         // }

//         // if (res)
//         // {
//         //     char *rawText = hex_decode(res);
//         //     // printf("\n");
//         //     // printf("Keysize: %d\n", keysize);
//         //     // printf("Key: %s\n", key);
//         //     // printf("Deciphered text: %s\n", rawText);
//         //     // printf("\n");

//         //     double count = calculate_score(rawText);

//         //     if (count > bestcount)
//         //         {
//         //             bestcount = count;
//         //             if (bestRawText == NULL) {
//         //                 bestRawText = malloc(strlen(rawText) * sizeof(char));
//         //             }
//         //             strcpy(bestRawText, rawText);
//         //             bestKey = key;
//         //     }

//         //     free(rawText);
//         // }

//         // free(res);
//         // free(encoded_key);
//     // }

//     printf("Best key: %s\n", bestKey);
//     printf("Deciphered: %s\n", bestRawText);
    
//     free(bestRawText);
//     free(key);

//     for(int i = 0; i < keysize; i++) {
//         free(transposed[i]);
//         free(bestKeys[i]);
//     }
//     free(transposed);
//     free(bestKeys);
    
//     // for (int i = 0; i < total_combinations; i++) {
//     //     free(combinations[i]);
//     // }
//     // free(combinations);
    
//     for(int i = 0; i < blocksSize; i++) {
//         free(blocks[i]);
//     }
//     free(blocks);
// }
float score_plaintext(const uint8_t *plaintext, size_t len) {
    /* Frequência das letras de acordo com https://web.archive.org/web/20210430083032/https://www.gta.ufrj.br/grad/06_2/alexandre/criptoanalise.html */
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
    size_t i;
    for (i = 0; i < len; ++i) {
        score += frequencies[plaintext[i]];
    }
    return score;
}

void break_ciphertext(char* ciphertext, int keysize)
{

}

void apply_xor_single_char(const uint8_t *input, uint8_t key, size_t len, uint8_t **output) {
    *output = malloc(len);
    size_t i;
    for (i = 0; i < len; i++) {
        (*output)[i] = input[i] ^ key;
    }
    (*output)[len] = '\0';
}

void decrypt_with_single_char_xor(const uint8_t *ciphertext, size_t len, uint8_t *key) {
    float best_score = 0.0;
    uint8_t best_key = 0;
    uint8_t *plaintext_candidate;

    int k;
    for (k = 0; k < 256; ++k) {
        apply_xor_single_char(ciphertext, (uint8_t)k, len, &plaintext_candidate);
        float current_score = score_plaintext(plaintext_candidate, len);
        if (current_score > best_score) {
            best_score = current_score;
            best_key = (uint8_t)k;
        }
        free(plaintext_candidate);
    }
    printf("KEY: %c\n",best_key);

    *key = best_key;
    // apply_xor_single_char(ciphertext, best_key, len, output);
}

void transpose_blocks(char* cipherText, int keySize) {
    int length = strlen(cipherText);
    int numBlocks = (length + keySize - 1) / keySize;  // Calcula o número de blocos
    uint8_t *key;
    // Cria uma matriz para armazenar os blocos transpostos
    char** transposed = (char**)malloc(keySize * sizeof(char*));
    for (int i = 0; i < keySize; i++) {
        transposed[i] = (char*)malloc(numBlocks + 1);  // +1 para o caractere nulo
    }

    // Inicializa a matriz transposta com '\0'
    for (int i = 0; i < keySize; i++) {
        for (int j = 0; j < numBlocks; j++) {
            transposed[i][j] = '\0';
        }
    }

    // Preenche a matriz transposta
    for (int i = 0; i < length; i++) {
        int blockIndex = i / keySize;
        int byteIndex = i % keySize;
        transposed[byteIndex][blockIndex] = cipherText[i];
    }

    // Imprime os blocos transpostos
    for (int i = 0; i < keySize; i++) {
        printf("Bloco %d: %s\n", i + 1, transposed[i]);
        //char best_key = decipher(base64_to_hex(transposed[i]));
        decrypt_with_single_char_xor(transposed[i], strlen(transposed[i]), &key);

        printf("Melhor chave: %u\n", key);
    }

    // Libera a memória alocada
    for (int i = 0; i < keySize; i++) {
        free(transposed[i]);
    }
    free(transposed);
}

int main()
{
    char* cipherText = "FBM9JCMjIT48IjIpMAA2KTA0MiExIwAsOzIyADQ0OiwzKTosJTQ6IDAvISwgKDo7MDQgJDEnNygzIzcoJyc/LicvMik0KDwkOzI2PzwpISs6NDIpMDM+LDYnIyQhJz8vJycgJDkjOj80AyA+MCAyOTo0Nj0nIyAoOzI8OCArPiwnJTwkODY8PyEnPTkwKDw9JykwKCY1PCkwLz05MDQ6IicvKSw2JzwpOiM9PjwoPDg7LyUoJzU6OTQ0OiIlMzEhPCU8IzoEISwmLz8oNik9OScvMTg8MyMsJyc8HzwpFD80KDcoMSkAODkyPD87JyE+MCkjPzwrNiQnKRY+ISc3IjEnFSgxIyEsNic8LDYpPTk0NDAiOCImLCYzPSQjIyE+PCIyKTA1NSgxIyEsPDUWPiEnICgxLzIpNCM+HjQoJywYJyEkNBQ6IhI0MiMxIzciBjM/DycnICQ5JDIkJzQ8DjQrPC88KDIOPCIyKTATPSQjIyE+PDIyPzwnAz86IDY+JikhBzo1NgA0NDosOyk3LAcpMCU0ADohPSk8IzEjMi46KCcoNiMyIDQvPD8lJyE5MCI2PiAnICwhLyUkMSc3KCYnMCwxIz4kNicgKDQiPiQ7LyA5JycnJCMnIB06NSA4PCc6IzEnJz8wNTAsODY6Kzo0MikwNTYpMDM+KDgAISgxIyEkNikEKCYyIyU0KjYjICs2IAUnPyAwLyEsMScgADw1ICIwNTYiIDIhIjArECw2LjwoPDQyKToVJiEUKjYgMSkgLjQrIyQ0Jyc4NCo2PiE0JjkgNDIoNik+PTo1JywlKSEpOjw2GDsvNywxIyAYOy8lKCc1Ojk0NDosJgU2IyE0PCkwByE5MDU2ATAyISwmBTYjITQ8KTAFOig7JTosJggyOSA0MiQmIxY1NDIyPhYjPTknKTcoFi82IzYvMj4HMyEsPDUQKDsyISIxIxAkMCgwJDQ1NywGJyYpMAU2IyE0PCkwBTooOyU6LCYVPC48Jzo+MA4mIDQoMj4WIz05Jyk3KBAiJi40JTIiFiM9OScpNygQIiYuNCUyIhMvICQ2JzYJMDUjIicyPD4WKT8oMi88HToqOjkwJT0kNikQKDsyISIxIwcoNig8ITohOiwWKT8oMi88GTAlPSQ2KRojMTMgOScvMiExIwAsOzIyADQ0OiwQNSMsNikeODkyOik8NTAkJSo6IzQ0NywAAAAAMCsAJDkwNiQnJx4sJzI6IyYTPSQxJzcoMSMWKSAlMi40KRojMyc9OTwqGj0wBz4sJyM/IhspNiMmLz0iJTQ2PjAoMCQ0KjwrMDQ2LjAlNiMhKTY5Jy89OTQjJiA2MyE+OjU3KDI0MikgJzAsOjU2IzEpPCMvIzA4JzU8PiYzIygnLzw/MDU3KCEjMCM6KjwqPCcnPzwoJywwNTY5MCI2ITwlNiM2LzI5IDQyPTkjPSwwKTo5MCgnLDAyISgmIjYvNCU7LCcjPywxKRIhMCs3JCY1PCw8KCA5PDImJDYnPCIzIyE5NCU2IyEpNj4wLyAuIDQgIiYiNj06NTQ/NCImLDYnPD4wKDciITQ6IyEnNjwgJyc/OiI2KTozJyInJzciNi89PCAjPTk0Iz0iIyM3KDgjIDknJzciMSkpKDEjNj4lIzAkNCo6NzQlMiIwMz49Jyk0PzQrMikwNjw+MSkmOTo0Mik6CDIoMTMwLDYnPC80NTouNCMnKDYoOi40NTIiIy89OTAjIjg0MiEiNjMhPjo1Jyg2KDouOjUjIiYrNik8KSA8ICcnPzoyNi47LzAiJjYyPzQjPT48KDwgMCI6IiArMDgnNTwpMCM9PjwoPCAwIjoiMDM+LiA0ICIxIzYpICUyLjQpOiMzJz05PCocLjo0IyIxLyAuMCgnKDAlPCMmMjo5IC83IjEjMj0nKSskOCc3LDgjPTkwMiEkOzIyIDwqNj4hMzcsOzI2PiYjPSk6MDojISM2PjAyNiA8Kj0sOCk3LDkvNywxIyM/MDU2IzYvMiEwMiEoJis6ITsnPiIxJz8kMSc3KDEjNiMmLz0iNCI6PiEnPS48Jxw8ICc3PzoiNj0wNSAiNCowIjsyMi46KzAoJyUyKTAlOiM2KT4kOTU2PyMvNyInIyAJMDUnKCYiPCQmKzohMCImNzAoJyImNTIiMSkwKDsyNj4wIjwkJis6ITApOjk6JTYjISkgPjQpJyg2KDouOic3IDwoOj4hNDI5PDA8PjArNikgJTIuNCkXIjAhISgmNTwpMDM+LiA0ICIxIzEsNi4yPzAqMik6Iz4OPCM9LjwnNywWKT49IDIyLjQpNigtLzQkMScmIDQ2ISgxLyA9OjU6LjQpNiwlMjopOiMgPTQ0Miw0NDYsNCo2IDEjJiA2KT0nICgnIjEjMCI4NjY5MCgwJDQ1Oyw3Lz8kMSc3KCYjMjk8MiYpMDUyPjA0NiA0IiI4PDQ6KTQ1NzgnJz05MCchKDQqOjc0JTIiMSkwOCc1PB06NDcoMy89JDYnPCIXJzAlNDQ2ITArECQwKDAkNCIyDjorIzghJzAsOiI2OzA1Nj8gKyM/OiA6PiYvPCM0KiI4NCo6KzwlMik6NjI/NCcjKCY3JiQmJzYpMDU2IyMpPzs8KzYjISk9LDQ0NiwxIzAiODYmOTQlMiIlJyEsOjYhIj8jJyIwJTwjJjIhODYnPCkwLjI/MTEyPzAjICIzMiQsJyMxLCYvMCIwMjIgNyM+PTQ0MiIgNTwpMDU6PiEjPiwmJTwgJTMnLDEpISQvJzciJiM+IiAyISwmJyEoNDU3LDQyOjs8IjIpMC4mIDQoMiwzLz4pMDA6LDcvPyQvJyEiICcmIDAoJywnJyM/OiImOTwwOik0IjYoNDcmLDkvNywxIzcoISk3IiYpIDk8Njw+MSMjPzolNik8KzYjISkgAzQTFR4YMjwpOiM0PzA1ICIxIyUoJiMhODg2ISIzLyA+PCk9LDklPCAxKT4kOy88KDYnIyw2LzcsMSMjLCcnJz80JDIhPSchIzQnISg0IjIOOisjOCEnMCw6IjY+MCglIjkwNiMxKSM/Oiw2OTo1Nyg2KT49IDIyKTo0Nj4wNTo+ISM+LCYiNi46KyM4IScwLDo2ISIyNDIgNDU2Pjw1Jyg4JyApMC89Kzo0Piw2JzwsISM9OTonPC40NDI5MDQ2LjoqPCo8JTw+OiU6LDkjNjk8JTwoJDM2KC0jIS40NSYsJicnJCMvNywxIyAjNDU8LjwjNywxIzAiODQ2PiUpPT40JDohPCIyKTAJESw2LjI/MCo2IAYvIDkwKzI+MSMaIzMpISA0JTIiMCkjPzogOj4mLzwjNCoiODAzICw2KT0uMC8nIiYjJyg2KDouNDU3KDwoNSInKzI5PCUyKCEjPD88JzcoJi8gOTArMj4lJyEsNik9OScvMTg8ND0sJik/ODYnPCkwNiEiNyo2IDQ1NyghNDI5NCs2IyEpNyw8KDUiJysyLjQpPSwmKSEqNCg6NzQlPCgmNjw/OCM6IjEnMCI7NSc/ICUyIjEjPiIxIz8iJiI2LCAyPCA0JTIiNikhPTo0Mjk8MDIIOSM3KCMjJygnIyA9PDQ6OTojPj0nIzYjMSM3Iic3Jig5LjY9MDQ+JCEnMCI7JTYvMDQwPzwnISg2KT0uJyMnJC8nIT0nKTkoISkgKTAyISwhJz4oOzI8KTQvPSs6ND4sNic8LjooIC48Iz05MCIyPiAnNTg7JTIiOycgIjYvNik0IjYoMSMyLjo0NyI2KT47NCo8PzA1Njk8JTw+ESk2KicjID46IjY4OCUmPyYpNyg3JzAlNDQ2ITQiPCg4FTo+ISM+LCYiNgQ7IDw/OCcwLDojNjU8ITopNDM+LCU0Nik8NSMiJi8wLDojMj0hLzciMDUjLCcnMiwnIzIsOSM+KTAzPi46KDk4OzI8KTAlPCAlIycoOyU6LCYuMi88KjopNCI2PjAnJyQhMzcoJicgKCcjPiwxNyYkJy83LCYiJj80KCcoNDQ2LDkvKSw2JzwpOiUmPyYpHSgmMjY+MCgnJDEpNj4hIzAsJS8nODkpNygzLz0oOjY2PzMvPz0nKTUkJjU6IjsnPykwNTYnNCI8KTA1MD8wMDYiJjQ2PCAvICQhKSA9Ji8wIjMvICQ2KSA8ICM3KCMjPj4wNCc/NCQyIT0nNyImJzwhOig0IjEpMDgnNTwoJyM/LDYvPCM0JyAuOScgPjA1NyglNDwvOSM+LCY3JighKTciMCEhKCY1PCkwMDYoJjIyPzQ2JyI0NDY+OiolKCc=";

    // float min_norms[TAM];
    // int min_keysize[TAM];

    // for(int i = 0; i < TAM; i++) {
    //     min_norms[i] = FLT_MAX;
    //     min_keysize[i] = 0;
    // }  

    // for (int keysize = 2; keysize <= 40; keysize++)
    // {
    //     char* str1 = malloc(keysize * sizeof(char) + 1);
    //     strncpy(str1, cipherText, keysize);

    //     char* str2 = malloc(keysize * sizeof(char) + 1);
    //     strncpy(str2, cipherText + keysize, keysize);

    //     // printf("str1 : %s | str2 : %s\n", str1, str2);
    //     int hamming = hamming_distance(str1, str2);
    //     float hamming_norm = (float)hamming / (float)keysize;

    //     insert_norm(hamming_norm, keysize, min_norms, min_keysize, TAM);

    //     free(str1);
    //     free(str2);
    // }

    // for (int i = 0; i < TAM; i++)
    // {
    //     printf("Keysize: %d %f\n", min_keysize[i], min_norms[i]);
    // }
    transpose_blocks(cipherText, 4);
    // for(int i = 0; i < TAM; i++) {
    //     decipher2(cipherText, min_keysize[i]);
    // }
}

// Etapa 4: quebrar uma mensagem cifrada com uma chave desconhecida, de tamanho maior que 1.
// Utilize o seguinte método:
// [FEITO] - KEYSIZE deve ser o tamanho estimado da chave; tente valores de 2 a (digamos) 40.
// [FEITO] - Escreva uma função para computar a distância de Hamming (Hamming distance/edit distance) entre duas strings. Distância de Hamming é o número de bits diferentes entre as strings. (Ex.: a distância Hamming entre "sistema" e "abobora" é 18)
// [FEITO] - Para cada KEYSIZE, crie um bloco formado pelos primeiros KEYSIZE bytes e um segundo bloco formado pelos KEYSIZE bytes seguintes e encontre a distância Hamming entre eles. Normalize o resultado dividindo por KEYSIZE. (Ex.: se o ciphertext for "SDUDEHQV" e KEYSIZE for 3, o primeiro bloco seria "SDU" e o segundo "DEH" e a distância seria 9, normalizando: 9/3 = 3)
// [FEITO] - O KEYSIZE com a menor distância Hamming normalizada é, provavelmente, o tamanho da chave correta. (use 5 menores valores, para começar, até calibrar seu código. Também é possível usar 4 ou mais blocos de tamanho KEYSIZE e fazer a média das distâncias encontradas, por exemplo.)
// [FEITO] - Agora que você tem o tamanho provável da chave, quebre o ciphertext em pedaços de comprimento KEYSIZE. Transponha estes blocos: crie um novo bloco que é formado pelo primeiro byte de cada bloco, outro que é formado pelo segundo byte de cada bloco e assim por diante. KEYSIZE informa qual o número de blocos a serem criados.
// - Ataque cada novo bloco como se ele fosse uma cifragem com chave de um caracter (etapa 1 acima).
// - Para cada novo bloco, a melhor chave de 1 caracter encontrada é, provavelmente, a chave de decriptação dele. Junte estas sub-chaves e encontre a chave usada na encriptação.
