#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

/**
 * Roberto José Mahl e Jhuan Luis Assumpção
*/

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t base64_decode(const char *base64, uint8_t **output) {
    size_t len = strlen(base64);
    size_t padding = 0;
    
    if (base64[len - 1] == '=' && base64[len - 2] == '=')
        padding = 2;
    else if (base64[len - 1] == '=')
        padding = 1;

    size_t out_len = (len * 3) / 4 - padding;
    *output = malloc(out_len);
    
    int index = 0, sextet = 0, buffer = 0;
    size_t i;
    for (i = 0; i < len; ++i) {
        if (base64[i] == '=') break;
        
        int value = strchr(base64_table, base64[i]) - base64_table;
        buffer = (buffer << 6) | value;
        sextet += 6;
        
        if (sextet >= 8) {
            sextet -= 8;
            (*output)[index++] = (buffer >> sextet) & 0xFF;
        }
    }
    
    return out_len;
}

char* base64_encode(const uint8_t *data, size_t len) {
    size_t out_len = 4 * ((len + 2) / 3);
    char *output = malloc(out_len + 1);
    
    size_t index = 0;
    int sextet = 0, buffer = 0;
    size_t i;
    for (i = 0; i < len; ++i) {
        buffer = (buffer << 8) | data[i];
        sextet += 8;
        
        while (sextet >= 6) {
            sextet -= 6;
            output[index++] = base64_table[(buffer >> sextet) & 0x3F];
        }
    }
    
    if (sextet > 0) {
        buffer <<= 6 - sextet;
        output[index++] = base64_table[buffer & 0x3F];
    }
    
    while (index < out_len) {
        output[index++] = '=';
    }
    
    output[out_len] = '\0';
    return output;
}

char* raw_to_hex(const uint8_t *data, size_t len) {
    char *output = malloc(len * 2 + 1);
    size_t i;
    for (i = 0; i < len; ++i) {
        sprintf(output + i * 2, "%02x", data[i]);
    }
    output[len * 2] = '\0';
    return output;
}

size_t hex_to_raw(const char *hex, uint8_t **output) {
    size_t len = strlen(hex) / 2;
    *output = malloc(len);
    size_t i;
    for (i = 0; i < len; ++i) {
        sscanf(hex + 2 * i, "%2hhx", &(*output)[i]);
    }
    return len;
}

void print_raw_as_binary(const uint8_t *data, size_t len) {
    size_t i;
    int j;
    for (i = 0; i < len; i++) {
        for (j = 7; j >= 0; j--) {
            printf("%d", (data[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void apply_xor(const uint8_t *input, const uint8_t *key, size_t len, uint8_t **output) {
    *output = malloc(len);
    size_t i;
    for (i = 0; i < len; i++) {
        (*output)[i] = input[i] ^ key[i];
    }
    (*output)[len] = '\0';
}

void apply_xor_single_char(const uint8_t *input, uint8_t key, size_t len, uint8_t **output) {
    *output = malloc(len);
    size_t i;
    for (i = 0; i < len; i++) {
        (*output)[i] = input[i] ^ key;
    }
    (*output)[len] = '\0';
}

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

void decrypt_with_single_char_xor(const uint8_t *ciphertext, size_t len, uint8_t *key, uint8_t **output) {
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

    *key = best_key;
    apply_xor_single_char(ciphertext, best_key, len, output);
}

int hamming_distance(const uint8_t *str1, const uint8_t *str2, size_t len) {
    int distance = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char xor = str1[i] ^ str2[i];
        
        while (xor) {
            distance += xor & 1;
            xor >>= 1;
        }
    }

    return distance;
}

void estimate_keysize(const uint8_t *ciphertext, size_t ciphertext_len, uint8_t min_keysize, uint8_t max_keysize, uint8_t *keysize_estimations, uint8_t estimations_number) {
    double min_distances[estimations_number];
    uint8_t min_keysizes[estimations_number];
    for (int i = 0; i < estimations_number; ++i) {
        min_distances[i] = DBL_MAX;
        min_keysizes[i] = 0;
    }

    for (uint8_t keysize = min_keysize; keysize <= max_keysize; ++keysize) {
        double total_distance = 0.0;
        int comparisons = 0;

        for (size_t i = 0; i + 2 * keysize <= ciphertext_len; i += keysize) {
            int distance = hamming_distance(ciphertext + i, ciphertext + i + keysize, keysize);
            total_distance += (double)distance / keysize;
            comparisons++;
        }

        double normalized_distance = total_distance / comparisons;

        for (int i = 0; i < estimations_number; ++i) {
            if (normalized_distance < min_distances[i]) {
                for (int j = estimations_number - 1; j > i; --j) {
                    min_distances[j] = min_distances[j - 1];
                    min_keysizes[j] = min_keysizes[j - 1];
                }
                min_distances[i] = normalized_distance;
                min_keysizes[i] = keysize;
                break;
            }
        }
    }

    memcpy(keysize_estimations, min_keysizes, estimations_number);
}

void transpose_blocks(const uint8_t *input, size_t input_len, size_t block_size, uint8_t **output) {
    for (size_t i = 0; i < block_size; ++i) {
        for (size_t j = 0; j * block_size + i < input_len; ++j) {
            output[i][j] = input[j * block_size + i];
        }
    }
}

void decrypt_repeating_key_xor(const uint8_t *ciphertext, size_t ciphertext_len, uint8_t keysize, uint8_t **key, uint8_t **plaintext) {
    uint8_t *transposed_blocks[keysize];
    for (uint8_t i = 0; i < keysize; ++i) {
        transposed_blocks[i] = malloc((ciphertext_len / keysize + 1) * sizeof(uint8_t));
    }

    transpose_blocks(ciphertext, ciphertext_len, keysize, transposed_blocks);

    *key = malloc(keysize);
    for (uint8_t i = 0; i < keysize; ++i) {
        decrypt_with_single_char_xor(transposed_blocks[i], ciphertext_len / keysize + 1, &((*key)[i]), &(*plaintext));
    }

    *plaintext = malloc(ciphertext_len);
    for (size_t i = 0; i < ciphertext_len; ++i) {
        (*plaintext)[i] = ciphertext[i] ^ (*key)[i % keysize];
    }

    for (uint8_t i = 0; i < keysize; ++i) {
        free(transposed_blocks[i]);
    }
}

int main() {
    char base64_input[] = "FBM9JCMjIT48IjIpMAA2KTA0MiExIwAsOzIyADQ0OiwzKTosJTQ6IDAvISwgKDo7MDQgJDEnNygzIzcoJyc/LicvMik0KDwkOzI2PzwpISs6NDIpMDM+LDYnIyQhJz8vJycgJDkjOj80AyA+MCAyOTo0Nj0nIyAoOzI8OCArPiwnJTwkODY8PyEnPTkwKDw9JykwKCY1PCkwLz05MDQ6IicvKSw2JzwpOiM9PjwoPDg7LyUoJzU6OTQ0OiIlMzEhPCU8IzoEISwmLz8oNik9OScvMTg8MyMsJyc8HzwpFD80KDcoMSkAODkyPD87JyE+MCkjPzwrNiQnKRY+ISc3IjEnFSgxIyEsNic8LDYpPTk0NDAiOCImLCYzPSQjIyE+PCIyKTA1NSgxIyEsPDUWPiEnICgxLzIpNCM+HjQoJywYJyEkNBQ6IhI0MiMxIzciBjM/DycnICQ5JDIkJzQ8DjQrPC88KDIOPCIyKTATPSQjIyE+PDIyPzwnAz86IDY+JikhBzo1NgA0NDosOyk3LAcpMCU0ADohPSk8IzEjMi46KCcoNiMyIDQvPD8lJyE5MCI2PiAnICwhLyUkMSc3KCYnMCwxIz4kNicgKDQiPiQ7LyA5JycnJCMnIB06NSA4PCc6IzEnJz8wNTAsODY6Kzo0MikwNTYpMDM+KDgAISgxIyEkNikEKCYyIyU0KjYjICs2IAUnPyAwLyEsMScgADw1ICIwNTYiIDIhIjArECw2LjwoPDQyKToVJiEUKjYgMSkgLjQrIyQ0Jyc4NCo2PiE0JjkgNDIoNik+PTo1JywlKSEpOjw2GDsvNywxIyAYOy8lKCc1Ojk0NDosJgU2IyE0PCkwByE5MDU2ATAyISwmBTYjITQ8KTAFOig7JTosJggyOSA0MiQmIxY1NDIyPhYjPTknKTcoFi82IzYvMj4HMyEsPDUQKDsyISIxIxAkMCgwJDQ1NywGJyYpMAU2IyE0PCkwBTooOyU6LCYVPC48Jzo+MA4mIDQoMj4WIz05Jyk3KBAiJi40JTIiFiM9OScpNygQIiYuNCUyIhMvICQ2JzYJMDUjIicyPD4WKT8oMi88HToqOjkwJT0kNikQKDsyISIxIwcoNig8ITohOiwWKT8oMi88GTAlPSQ2KRojMTMgOScvMiExIwAsOzIyADQ0OiwQNSMsNikeODkyOik8NTAkJSo6IzQ0NywAAAAAMCsAJDkwNiQnJx4sJzI6IyYTPSQxJzcoMSMWKSAlMi40KRojMyc9OTwqGj0wBz4sJyM/IhspNiMmLz0iJTQ2PjAoMCQ0KjwrMDQ2LjAlNiMhKTY5Jy89OTQjJiA2MyE+OjU3KDI0MikgJzAsOjU2IzEpPCMvIzA4JzU8PiYzIygnLzw/MDU3KCEjMCM6KjwqPCcnPzwoJywwNTY5MCI2ITwlNiM2LzI5IDQyPTkjPSwwKTo5MCgnLDAyISgmIjYvNCU7LCcjPywxKRIhMCs3JCY1PCw8KCA5PDImJDYnPCIzIyE5NCU2IyEpNj4wLyAuIDQgIiYiNj06NTQ/NCImLDYnPD4wKDciITQ6IyEnNjwgJyc/OiI2KTozJyInJzciNi89PCAjPTk0Iz0iIyM3KDgjIDknJzciMSkpKDEjNj4lIzAkNCo6NzQlMiIwMz49Jyk0PzQrMikwNjw+MSkmOTo0Mik6CDIoMTMwLDYnPC80NTouNCMnKDYoOi40NTIiIy89OTAjIjg0MiEiNjMhPjo1Jyg2KDouOjUjIiYrNik8KSA8ICcnPzoyNi47LzAiJjYyPzQjPT48KDwgMCI6IiArMDgnNTwpMCM9PjwoPCAwIjoiMDM+LiA0ICIxIzYpICUyLjQpOiMzJz05PCocLjo0IyIxLyAuMCgnKDAlPCMmMjo5IC83IjEjMj0nKSskOCc3LDgjPTkwMiEkOzIyIDwqNj4hMzcsOzI2PiYjPSk6MDojISM2PjAyNiA8Kj0sOCk3LDkvNywxIyM/MDU2IzYvMiEwMiEoJis6ITsnPiIxJz8kMSc3KDEjNiMmLz0iNCI6PiEnPS48Jxw8ICc3PzoiNj0wNSAiNCowIjsyMi46KzAoJyUyKTAlOiM2KT4kOTU2PyMvNyInIyAJMDUnKCYiPCQmKzohMCImNzAoJyImNTIiMSkwKDsyNj4wIjwkJis6ITApOjk6JTYjISkgPjQpJyg2KDouOic3IDwoOj4hNDI5PDA8PjArNikgJTIuNCkXIjAhISgmNTwpMDM+LiA0ICIxIzEsNi4yPzAqMik6Iz4OPCM9LjwnNywWKT49IDIyLjQpNigtLzQkMScmIDQ2ISgxLyA9OjU6LjQpNiwlMjopOiMgPTQ0Miw0NDYsNCo2IDEjJiA2KT0nICgnIjEjMCI4NjY5MCgwJDQ1Oyw3Lz8kMSc3KCYjMjk8MiYpMDUyPjA0NiA0IiI4PDQ6KTQ1NzgnJz05MCchKDQqOjc0JTIiMSkwOCc1PB06NDcoMy89JDYnPCIXJzAlNDQ2ITArECQwKDAkNCIyDjorIzghJzAsOiI2OzA1Nj8gKyM/OiA6PiYvPCM0KiI4NCo6KzwlMik6NjI/NCcjKCY3JiQmJzYpMDU2IyMpPzs8KzYjISk9LDQ0NiwxIzAiODYmOTQlMiIlJyEsOjYhIj8jJyIwJTwjJjIhODYnPCkwLjI/MTEyPzAjICIzMiQsJyMxLCYvMCIwMjIgNyM+PTQ0MiIgNTwpMDU6PiEjPiwmJTwgJTMnLDEpISQvJzciJiM+IiAyISwmJyEoNDU3LDQyOjs8IjIpMC4mIDQoMiwzLz4pMDA6LDcvPyQvJyEiICcmIDAoJywnJyM/OiImOTwwOik0IjYoNDcmLDkvNywxIzcoISk3IiYpIDk8Njw+MSMjPzolNik8KzYjISkgAzQTFR4YMjwpOiM0PzA1ICIxIyUoJiMhODg2ISIzLyA+PCk9LDklPCAxKT4kOy88KDYnIyw2LzcsMSMjLCcnJz80JDIhPSchIzQnISg0IjIOOisjOCEnMCw6IjY+MCglIjkwNiMxKSM/Oiw2OTo1Nyg2KT49IDIyKTo0Nj4wNTo+ISM+LCYiNi46KyM4IScwLDo2ISIyNDIgNDU2Pjw1Jyg4JyApMC89Kzo0Piw2JzwsISM9OTonPC40NDI5MDQ2LjoqPCo8JTw+OiU6LDkjNjk8JTwoJDM2KC0jIS40NSYsJicnJCMvNywxIyAjNDU8LjwjNywxIzAiODQ2PiUpPT40JDohPCIyKTAJESw2LjI/MCo2IAYvIDkwKzI+MSMaIzMpISA0JTIiMCkjPzogOj4mLzwjNCoiODAzICw2KT0uMC8nIiYjJyg2KDouNDU3KDwoNSInKzI5PCUyKCEjPD88JzcoJi8gOTArMj4lJyEsNik9OScvMTg8ND0sJik/ODYnPCkwNiEiNyo2IDQ1NyghNDI5NCs2IyEpNyw8KDUiJysyLjQpPSwmKSEqNCg6NzQlPCgmNjw/OCM6IjEnMCI7NSc/ICUyIjEjPiIxIz8iJiI2LCAyPCA0JTIiNikhPTo0Mjk8MDIIOSM3KCMjJygnIyA9PDQ6OTojPj0nIzYjMSM3Iic3Jig5LjY9MDQ+JCEnMCI7JTYvMDQwPzwnISg2KT0uJyMnJC8nIT0nKTkoISkgKTAyISwhJz4oOzI8KTQvPSs6ND4sNic8LjooIC48Iz05MCIyPiAnNTg7JTIiOycgIjYvNik0IjYoMSMyLjo0NyI2KT47NCo8PzA1Njk8JTw+ESk2KicjID46IjY4OCUmPyYpNyg3JzAlNDQ2ITQiPCg4FTo+ISM+LCYiNgQ7IDw/OCcwLDojNjU8ITopNDM+LCU0Nik8NSMiJi8wLDojMj0hLzciMDUjLCcnMiwnIzIsOSM+KTAzPi46KDk4OzI8KTAlPCAlIycoOyU6LCYuMi88KjopNCI2PjAnJyQhMzcoJicgKCcjPiwxNyYkJy83LCYiJj80KCcoNDQ2LDkvKSw2JzwpOiUmPyYpHSgmMjY+MCgnJDEpNj4hIzAsJS8nODkpNygzLz0oOjY2PzMvPz0nKTUkJjU6IjsnPykwNTYnNCI8KTA1MD8wMDYiJjQ2PCAvICQhKSA9Ji8wIjMvICQ2KSA8ICM3KCMjPj4wNCc/NCQyIT0nNyImJzwhOig0IjEpMDgnNTwoJyM/LDYvPCM0JyAuOScgPjA1NyglNDwvOSM+LCY3JighKTciMCEhKCY1PCkwMDYoJjIyPzQ2JyI0NDY+OiolKCc=";
    uint8_t *ciphertext;
    size_t ciphertext_len = base64_decode(base64_input, &ciphertext);

    uint8_t keysize_estimations[3];
    estimate_keysize(ciphertext, ciphertext_len, 2, 40, keysize_estimations, 3);

    printf("Estimated keysizes: ");
    for (int i = 0; i < 3; ++i) {
        printf("%d ", keysize_estimations[i]);
    }
    printf("\n");

    uint8_t *key;
    uint8_t *plaintext;
    decrypt_repeating_key_xor(ciphertext, ciphertext_len, keysize_estimations[0], &key, &plaintext);

    printf("Key: ");
    for (size_t i = 0; i < keysize_estimations[0]; ++i) {
        printf("%c", key[i]);
    }
    printf("\n");

    printf("Plaintext: ");
    for (size_t i = 0; i < ciphertext_len; ++i) {
        printf("%c", plaintext[i]);
    }
    printf("\n");

    free(ciphertext);
    free(key);
    free(plaintext);

    return 0;
}
