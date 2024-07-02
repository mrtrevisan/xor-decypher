#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "utils.h"
#include "lib.h"

#define TAM 5
// NOMES: Andriza Campanhol, Pedro Henrique da Silva Hinerasky, Mauro Roberto Trevisan e Ramon Godoy Izidoro

void insert_norm(float norm, int keysize, float* norm_array, int* keysize_array, int max_size) {
    if (norm < norm_array[max_size - 1]) {
        int i = max_size - 1;
        while (i > 0 && norm_array[i - 1] > norm) {
            norm_array[i] = norm_array[i - 1];
            keysize_array[i] = keysize_array[i - 1];
            i--;
        }
        norm_array[i] = norm;
        keysize_array[i] = keysize;
    }
}

char** separate_in_blocks(char *cipherText, int keysize)
{
    int cipherTextSize = strlen(cipherText);
    int blocks = cipherTextSize / keysize;

    if(cipherTextSize % keysize != 0) {
        blocks++;
    }

    char **block = (char **)malloc(blocks * sizeof(char *));
    for (int i = 0; i < blocks; i++)
    {
        block[i] = (char *)malloc(keysize * sizeof(char) + 1);
        strncpy(block[i], cipherText + i * keysize, keysize);
        
        if(strlen(block[i]) < keysize)
        {
            int j = strlen(block[i]);
            for(; j < keysize; j++) {
                block[i][j] = '=';
            }
        }

        block[i][keysize] = '\0';  
        // printf("block len %d: %d\n", i, strlen(block[i]));     
    }

    return block;
}

char** transpose_blocks(char **blocks, int keysize, int blocksSize)
{
    char **transposed = (char **)malloc(keysize * sizeof(char *));
    for (int i = 0; i < keysize; i++)
    {
        transposed[i] = (char *)malloc(blocksSize * sizeof(char) + 1);
        for (int j = 0; j < blocksSize; j++)
        {
            transposed[i][j] = blocks[j][i];
        }
        transposed[i][blocksSize] = '\0';
    }

    return transposed;
}

void decipher2(char *cipherText, int keysize)
{
    char** blocks = separate_in_blocks(cipherText, keysize);
    int blocksSize = strlen(cipherText) / keysize;
    if (strlen(cipherText) % keysize != 0)
    {
        blocksSize++;
    }

    char** transposed = transpose_blocks(blocks, keysize, blocksSize);

    char* key = malloc(sizeof(char) * keysize + 1);
    for(int i = 0; i < keysize; i++) {
        char bestKey = decipher(transposed[i]);
        key[i] = bestKey;
    }
    
    key[keysize] = '\0';
    
    char *encoded_key = hex_encode(key);

    char* res = NULL;

    for(int i=0; i < blocksSize; i++) {
        char* encoded_block = base64_to_hex(blocks[i]);
        char *deciphered = xorHexStrings(encoded_block, encoded_key);
        if (res == NULL)
        {
            res = malloc(strlen(deciphered) + 1);
            strcpy(res, deciphered);
        }
        else
        {
            char *temp = malloc(strlen(res) + strlen(deciphered) + 1);
            strcpy(temp, res);
            strcat(temp, deciphered);
            free(res);
            res = temp;
        }
        free(deciphered);
        free(encoded_block);
    }

    if (res)
    {
        char *rawText = hex_decode(res);
        printf("\n");
        printf("Keysize: %d\n", keysize);
        printf("Key: %s\n", key);
        printf("Deciphered text: %s\n", rawText);
        printf("\n");

        free(rawText);
    }
    free(res);
    free(key);
    free(encoded_key);
    for(int i = 0; i < keysize; i++) {
        free(transposed[i]);
    }
    free(transposed);
    for(int i = 0; i < blocksSize; i++) {
        free(blocks[i]);
    }
    free(blocks);
}

int main()
{
    char* cipherText = read_file_to_string("cifra.txt");

    float min_norms[TAM];
    int min_keysize[TAM];

    for(int i = 0; i < TAM; i++) {
        min_norms[i] = FLT_MAX;
        min_keysize[i] = 0;
    }  

    for (int keysize = 2; keysize <= 40; keysize++)
    {
        char* str1 = malloc(keysize * sizeof(char) + 1);
        strncpy(str1, cipherText, keysize);

        char* str2 = malloc(keysize * sizeof(char) + 1);
        strncpy(str2, cipherText + keysize, keysize);

        // printf("str1 : %s | str2 : %s\n", str1, str2);
        int hamming = hamming_distance(str1, str2);
        float hamming_norm = (float)hamming / (float)keysize;

        insert_norm(hamming_norm, keysize, min_norms, min_keysize, TAM);

        free(str1);
        free(str2);
    }

    for (int i = 0; i < TAM; i++)
    {
        printf("Keysize: %d %f\n", min_keysize[i], min_norms[i]);
    }

    for(int i = 0; i < TAM; i++) {
        decipher2(cipherText, min_keysize[i]);
    }

    free(cipherText);
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


