#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utils.h"
#include "lib.h"

#define MIN_KEYSIZE 2
#define MAX_KEYSIZE 40

int get_best_keysize(char* cipherText)
{
    int bestKeysize = 0;
    double bestNormalized = DBL_MAX;

    for (int keysize = MIN_KEYSIZE; keysize <= MAX_KEYSIZE; keysize++)
    {
        char* str1 = malloc(keysize * sizeof(char) + 1);
        strncpy(str1, cipherText, keysize);

        char* str2 = malloc(keysize * sizeof(char) + 1);
        strncpy(str2, cipherText + keysize, keysize);

        // printf("str1 : %s | str2 : %s\n", str1, str2);
        int hamming = hamming_distance(str1, str2);

        double normalized_hamming_distance = (double)hamming / (double)keysize;

        if (normalized_hamming_distance < bestNormalized)
        {
            bestNormalized = normalized_hamming_distance;
            bestKeysize = keysize;
        }
        
        // printf("Resultado do hamming normalizado: %f\n", normalized_hamming_distance);

        free(str1);
        free(str2);
    }
    
    return bestKeysize;
}

char** transpose_blocks(char* cipherText, int keySize) {
    int lengthCipherText = strlen(cipherText);
    int nBlocks = (lengthCipherText + keySize - 1) / keySize;  
    
    char** transposed = (char**)malloc(keySize * sizeof(char*));
    // char* transposed[keysize]
    for (int i = 0; i < keySize; i++) {
        transposed[i] = (char*)malloc(nBlocks + 1);  
    }

    for (int i = 0; i < keySize; i++) {
        for (int j = 0; j < nBlocks; j++) {
            transposed[i][j] = '\0';
        }
    }

    for (int i = 0; i < lengthCipherText; i++) {
        int blockIndex = i / keySize;
        int byteIndex = i % keySize;
        transposed[byteIndex][blockIndex] = cipherText[i];
    }

    return transposed;
}

int main()
{
    char* cipherText = "FBM9JCMjIT48IjIpMAA2KTA0MiExIwAsOzIyADQ0OiwzKTosJTQ6IDAvISwgKDo7MDQgJDEnNygzIzcoJyc/LicvMik0KDwkOzI2PzwpISs6NDIpMDM+LDYnIyQhJz8vJycgJDkjOj80AyA+MCAyOTo0Nj0nIyAoOzI8OCArPiwnJTwkODY8PyEnPTkwKDw9JykwKCY1PCkwLz05MDQ6IicvKSw2JzwpOiM9PjwoPDg7LyUoJzU6OTQ0OiIlMzEhPCU8IzoEISwmLz8oNik9OScvMTg8MyMsJyc8HzwpFD80KDcoMSkAODkyPD87JyE+MCkjPzwrNiQnKRY+ISc3IjEnFSgxIyEsNic8LDYpPTk0NDAiOCImLCYzPSQjIyE+PCIyKTA1NSgxIyEsPDUWPiEnICgxLzIpNCM+HjQoJywYJyEkNBQ6IhI0MiMxIzciBjM/DycnICQ5JDIkJzQ8DjQrPC88KDIOPCIyKTATPSQjIyE+PDIyPzwnAz86IDY+JikhBzo1NgA0NDosOyk3LAcpMCU0ADohPSk8IzEjMi46KCcoNiMyIDQvPD8lJyE5MCI2PiAnICwhLyUkMSc3KCYnMCwxIz4kNicgKDQiPiQ7LyA5JycnJCMnIB06NSA4PCc6IzEnJz8wNTAsODY6Kzo0MikwNTYpMDM+KDgAISgxIyEkNikEKCYyIyU0KjYjICs2IAUnPyAwLyEsMScgADw1ICIwNTYiIDIhIjArECw2LjwoPDQyKToVJiEUKjYgMSkgLjQrIyQ0Jyc4NCo2PiE0JjkgNDIoNik+PTo1JywlKSEpOjw2GDsvNywxIyAYOy8lKCc1Ojk0NDosJgU2IyE0PCkwByE5MDU2ATAyISwmBTYjITQ8KTAFOig7JTosJggyOSA0MiQmIxY1NDIyPhYjPTknKTcoFi82IzYvMj4HMyEsPDUQKDsyISIxIxAkMCgwJDQ1NywGJyYpMAU2IyE0PCkwBTooOyU6LCYVPC48Jzo+MA4mIDQoMj4WIz05Jyk3KBAiJi40JTIiFiM9OScpNygQIiYuNCUyIhMvICQ2JzYJMDUjIicyPD4WKT8oMi88HToqOjkwJT0kNikQKDsyISIxIwcoNig8ITohOiwWKT8oMi88GTAlPSQ2KRojMTMgOScvMiExIwAsOzIyADQ0OiwQNSMsNikeODkyOik8NTAkJSo6IzQ0NywAAAAAMCsAJDkwNiQnJx4sJzI6IyYTPSQxJzcoMSMWKSAlMi40KRojMyc9OTwqGj0wBz4sJyM/IhspNiMmLz0iJTQ2PjAoMCQ0KjwrMDQ2LjAlNiMhKTY5Jy89OTQjJiA2MyE+OjU3KDI0MikgJzAsOjU2IzEpPCMvIzA4JzU8PiYzIygnLzw/MDU3KCEjMCM6KjwqPCcnPzwoJywwNTY5MCI2ITwlNiM2LzI5IDQyPTkjPSwwKTo5MCgnLDAyISgmIjYvNCU7LCcjPywxKRIhMCs3JCY1PCw8KCA5PDImJDYnPCIzIyE5NCU2IyEpNj4wLyAuIDQgIiYiNj06NTQ/NCImLDYnPD4wKDciITQ6IyEnNjwgJyc/OiI2KTozJyInJzciNi89PCAjPTk0Iz0iIyM3KDgjIDknJzciMSkpKDEjNj4lIzAkNCo6NzQlMiIwMz49Jyk0PzQrMikwNjw+MSkmOTo0Mik6CDIoMTMwLDYnPC80NTouNCMnKDYoOi40NTIiIy89OTAjIjg0MiEiNjMhPjo1Jyg2KDouOjUjIiYrNik8KSA8ICcnPzoyNi47LzAiJjYyPzQjPT48KDwgMCI6IiArMDgnNTwpMCM9PjwoPCAwIjoiMDM+LiA0ICIxIzYpICUyLjQpOiMzJz05PCocLjo0IyIxLyAuMCgnKDAlPCMmMjo5IC83IjEjMj0nKSskOCc3LDgjPTkwMiEkOzIyIDwqNj4hMzcsOzI2PiYjPSk6MDojISM2PjAyNiA8Kj0sOCk3LDkvNywxIyM/MDU2IzYvMiEwMiEoJis6ITsnPiIxJz8kMSc3KDEjNiMmLz0iNCI6PiEnPS48Jxw8ICc3PzoiNj0wNSAiNCowIjsyMi46KzAoJyUyKTAlOiM2KT4kOTU2PyMvNyInIyAJMDUnKCYiPCQmKzohMCImNzAoJyImNTIiMSkwKDsyNj4wIjwkJis6ITApOjk6JTYjISkgPjQpJyg2KDouOic3IDwoOj4hNDI5PDA8PjArNikgJTIuNCkXIjAhISgmNTwpMDM+LiA0ICIxIzEsNi4yPzAqMik6Iz4OPCM9LjwnNywWKT49IDIyLjQpNigtLzQkMScmIDQ2ISgxLyA9OjU6LjQpNiwlMjopOiMgPTQ0Miw0NDYsNCo2IDEjJiA2KT0nICgnIjEjMCI4NjY5MCgwJDQ1Oyw3Lz8kMSc3KCYjMjk8MiYpMDUyPjA0NiA0IiI4PDQ6KTQ1NzgnJz05MCchKDQqOjc0JTIiMSkwOCc1PB06NDcoMy89JDYnPCIXJzAlNDQ2ITArECQwKDAkNCIyDjorIzghJzAsOiI2OzA1Nj8gKyM/OiA6PiYvPCM0KiI4NCo6KzwlMik6NjI/NCcjKCY3JiQmJzYpMDU2IyMpPzs8KzYjISk9LDQ0NiwxIzAiODYmOTQlMiIlJyEsOjYhIj8jJyIwJTwjJjIhODYnPCkwLjI/MTEyPzAjICIzMiQsJyMxLCYvMCIwMjIgNyM+PTQ0MiIgNTwpMDU6PiEjPiwmJTwgJTMnLDEpISQvJzciJiM+IiAyISwmJyEoNDU3LDQyOjs8IjIpMC4mIDQoMiwzLz4pMDA6LDcvPyQvJyEiICcmIDAoJywnJyM/OiImOTwwOik0IjYoNDcmLDkvNywxIzcoISk3IiYpIDk8Njw+MSMjPzolNik8KzYjISkgAzQTFR4YMjwpOiM0PzA1ICIxIyUoJiMhODg2ISIzLyA+PCk9LDklPCAxKT4kOy88KDYnIyw2LzcsMSMjLCcnJz80JDIhPSchIzQnISg0IjIOOisjOCEnMCw6IjY+MCglIjkwNiMxKSM/Oiw2OTo1Nyg2KT49IDIyKTo0Nj4wNTo+ISM+LCYiNi46KyM4IScwLDo2ISIyNDIgNDU2Pjw1Jyg4JyApMC89Kzo0Piw2JzwsISM9OTonPC40NDI5MDQ2LjoqPCo8JTw+OiU6LDkjNjk8JTwoJDM2KC0jIS40NSYsJicnJCMvNywxIyAjNDU8LjwjNywxIzAiODQ2PiUpPT40JDohPCIyKTAJESw2LjI/MCo2IAYvIDkwKzI+MSMaIzMpISA0JTIiMCkjPzogOj4mLzwjNCoiODAzICw2KT0uMC8nIiYjJyg2KDouNDU3KDwoNSInKzI5PCUyKCEjPD88JzcoJi8gOTArMj4lJyEsNik9OScvMTg8ND0sJik/ODYnPCkwNiEiNyo2IDQ1NyghNDI5NCs2IyEpNyw8KDUiJysyLjQpPSwmKSEqNCg6NzQlPCgmNjw/OCM6IjEnMCI7NSc/ICUyIjEjPiIxIz8iJiI2LCAyPCA0JTIiNikhPTo0Mjk8MDIIOSM3KCMjJygnIyA9PDQ6OTojPj0nIzYjMSM3Iic3Jig5LjY9MDQ+JCEnMCI7JTYvMDQwPzwnISg2KT0uJyMnJC8nIT0nKTkoISkgKTAyISwhJz4oOzI8KTQvPSs6ND4sNic8LjooIC48Iz05MCIyPiAnNTg7JTIiOycgIjYvNik0IjYoMSMyLjo0NyI2KT47NCo8PzA1Njk8JTw+ESk2KicjID46IjY4OCUmPyYpNyg3JzAlNDQ2ITQiPCg4FTo+ISM+LCYiNgQ7IDw/OCcwLDojNjU8ITopNDM+LCU0Nik8NSMiJi8wLDojMj0hLzciMDUjLCcnMiwnIzIsOSM+KTAzPi46KDk4OzI8KTAlPCAlIycoOyU6LCYuMi88KjopNCI2PjAnJyQhMzcoJicgKCcjPiwxNyYkJy83LCYiJj80KCcoNDQ2LDkvKSw2JzwpOiUmPyYpHSgmMjY+MCgnJDEpNj4hIzAsJS8nODkpNygzLz0oOjY2PzMvPz0nKTUkJjU6IjsnPykwNTYnNCI8KTA1MD8wMDYiJjQ2PCAvICQhKSA9Ji8wIjMvICQ2KSA8ICM3KCMjPj4wNCc/NCQyIT0nNyImJzwhOig0IjEpMDgnNTwoJyM/LDYvPCM0JyAuOScgPjA1NyglNDwvOSM+LCY3JighKTciMCEhKCY1PCkwMDYoJjIyPzQ2JyI0NDY+OiolKCc=";
    int bestKeySize = get_best_keysize(cipherText);
    
    char** transposed = transpose_blocks(cipherText, bestKeySize);

    for (int i = 0; i < bestKeySize; i++)
    {
        //atacar bloco
        char* hex_transposed = base64_to_hex(transposed[i]);
        char* best_keys = decipher(hex_transposed);
        printf("Melhor chave para o bloco %d: %s\n", i, best_keys);
    }

    for (int i = 0; i < bestKeySize; i++) {
        free(transposed[i]);
    }
    free(transposed);
    
    printf("\n");
    return 0;
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