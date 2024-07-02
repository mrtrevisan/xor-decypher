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
    char *output = (char *)malloc(length * 2 + 1);
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
    int output_length = (length + 1) / 2;

    char *output = (char *)malloc(output_length + 1);
    if (!output)
        return NULL;

    int i;
    for (i = 0; i < length - 1; i += 2)
    {
        sscanf(input + i, "%2hhx", &output[i / 2]);
    }

    if (length % 2 != 0)
    {
        sscanf(input + i, "%1hhx", &output[i / 2]);
    }

    output[output_length] = '\0';

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

char *xorHexStrings(const char *plainText, const char *key)
{
    size_t len = strlen(plainText) / 2;
    char *plainTextBytes;
    char *keyBytes;
    char *cypherTextBytes = (char *)malloc(len * sizeof(char) + 1);

    plainTextBytes = hex_decode(plainText);
    keyBytes = hex_decode(key);

    for (size_t i = 0; i < len; i++)
    {
        cypherTextBytes[i] = plainTextBytes[i] ^ keyBytes[i];
    }

    cypherTextBytes[len] = '\0';

    char *cypherText = hex_encode(cypherTextBytes);

    free(plainTextBytes);
    free(keyBytes);
    free(cypherTextBytes);

    return cypherText;
}

char* read_file_to_string(const char* filename) {
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);  

    char* content = malloc(file_size + 1);  
    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, file_size, file);
    if (read_size != file_size) {
        free(content);
        fclose(file);
        return NULL;
    }

    content[file_size] = '\0';

    fclose(file);

    return content;
}

int count_different_bits(char c1, char c2) {
    int diff = 0;
    unsigned char xor = c1 ^ c2;  

    while (xor) {
        diff += xor & 1;
        xor >>= 1;
    }

    return diff;
}

int hamming_distance(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int max_len = len1 > len2 ? len1 : len2;
    int distance = 0;

    for (int i = 0; i < max_len; i++) {
        char c1 = i < len1 ? str1[i] : 0; 
        char c2 = i < len2 ? str2[i] : 0;

        distance += count_different_bits(c1, c2);
    }

    return distance;
}