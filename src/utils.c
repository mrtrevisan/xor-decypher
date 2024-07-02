#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int base64_char_index(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return -1;
}

char *base64_encode(const char *input)
{
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *output, *p;
    int i, remainder, enc_len;

    int length = strlen(input);

    remainder = length % 3;
    enc_len = (length / 3) * 4 + (remainder ? 4 : 0);

    output = (char *)malloc(enc_len + 1);
    if (!output)
        return NULL;

    p = output;

    for (i = 0; i < length - 2; i += 3)
    {
        *p++ = base64_chars[input[i] >> 2];
        *p++ = base64_chars[((input[i] & 0x03) << 4) | (input[i + 1] >> 4)];
        *p++ = base64_chars[((input[i + 1] & 0x0f) << 2) | (input[i + 2] >> 6)];
        *p++ = base64_chars[input[i + 2] & 0x3f];
    }

    if (remainder == 1)
    {
        *p++ = base64_chars[input[i] >> 2];
        *p++ = base64_chars[(input[i] & 0x03) << 4];
        *p++ = '=';
        *p++ = '=';
    }
    else if (remainder == 2)
    {
        *p++ = base64_chars[input[i] >> 2];
        *p++ = base64_chars[((input[i] & 0x03) << 4) | (input[i + 1] >> 4)];
        *p++ = base64_chars[(input[i + 1] & 0x0f) << 2];
        *p++ = '=';
    }

    *p = '\0';
    return output;
}

char *base64_decode(const char *input)
{
    int input_length = strlen(input);
    int output_length = (input_length / 4) * 3;

    char *output = (char *)malloc(output_length + 1);

    if (input[input_length - 1] == '=')
        output_length--;
    if (input[input_length - 2] == '=')
        output_length--;

    int i, j;
    for (i = 0, j = 0; i < input_length;)
    {
        int sextet_a = input[i] == '=' ? 0 & i++ : base64_char_index(input[i++]);
        int sextet_b = input[i] == '=' ? 0 & i++ : base64_char_index(input[i++]);
        int sextet_c = input[i] == '=' ? 0 & i++ : base64_char_index(input[i++]);
        int sextet_d = input[i] == '=' ? 0 & i++ : base64_char_index(input[i++]);

        int triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < output_length)
            output[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < output_length)
            output[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < output_length)
            output[j++] = (triple >> 0 * 8) & 0xFF;
    }
    output[output_length] = '\0';
    return output;
}

char *hex_encode(const char *input)
{
    size_t length = strlen(input);
    char *output = (char *)malloc(length * 2 + 1); // Espaço para string hexadecimal + '\0'
    if (!output)
        return NULL;

    for (size_t i = 0; i < length; i++)
    {
        sprintf(output + (i * 2), "%02x", (unsigned char)input[i]);
    }

    return output;
}

char *hex_decode(const char *input)
{
    int length = strlen(input);
    int output_length = (length + 1) / 2; // Espaço para os bytes decodificados

    char *output = (char *)malloc(output_length + 1); // Espaço para os bytes decodificados + '\0'
    if (!output)
        return NULL;

    int i;
    for (i = 0; i < length - 1; i += 2)
    {
        sscanf(input + i, "%2hhx", &output[i / 2]);
    }

    // Se o comprimento for ímpar, trate o último caractere hexadecimal sozinho
    if (length % 2 != 0)
    {
        sscanf(input + i, "%1hhx", &output[i / 2]);
    }

    output[output_length] = '\0'; // Adiciona o terminador nulo

    return output;
}

char *base64_to_hex(const char *base64_string)
{
    char *decoded_bytes = base64_decode(base64_string);

    if (!decoded_bytes)
    {
        printf("Erro ao decodificar a string Base64.\n");
        return NULL;
    }

    char *hex_string = hex_encode(decoded_bytes);

    free(decoded_bytes);
    return hex_string;
}

char *hex_to_base64(const char *hex_string)
{
    char *decoded_bytes = hex_decode(hex_string);

    if (!decoded_bytes)
    {
        printf("Erro ao decodificar a string Base64.\n");
        return NULL;
    }

    char *base64_string = base64_encode(decoded_bytes);

    free(decoded_bytes);
    return base64_string;
}

// Função para aplicar XOR em duas strings hexadecimais de mesmo tamanho
char *xorHexStrings(const char *plainText, const char *key)
{
    size_t len = strlen(plainText) / 2;
    char *plainTextBytes;
    char *keyBytes;
    char *cypherTextBytes = (char *)malloc(len * sizeof(char) + 1);

    // Converte as strings hexadecimais para arrays de bytes
    plainTextBytes = hex_decode(plainText);
    keyBytes = hex_decode(key);

    // Aplica XOR byte a byte
    for (size_t i = 0; i < len; i++)
    {
        cypherTextBytes[i] = plainTextBytes[i] ^ keyBytes[i];
    }

    cypherTextBytes[len] = '\0';

    // Converte o resultado de volta para uma string hexadecimal
    char *cypherText = hex_encode(cypherTextBytes);

    // Libera a memória alocada
    free(plainTextBytes);
    free(keyBytes);
    free(cypherTextBytes);

    return cypherText;
}
