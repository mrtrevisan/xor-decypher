#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lib.h"

// NOMES: Andriza Campanhol, Pedro Henrique da Silva Hinerasky, Mauro Roberto Trevisan e Ramon Godoy Izidoro

int main()
{
    printf("################################-PARTE 1-##################################\n\n");
    string_convert(1);

    printf("\n\n");

    string_convert(2);

    printf("\n################################-PARTE 2-##################################\n\n");

    char *str = "41636f72646150656472696e686f517565686f6a6574656d63616d70656f6e61746f";
    char *key = "0b021e0701003e0a0d060c0807063d1a0b0f0e060a1a020c0f0e03170403010f130e";
    char *res = xorHexStrings(str, key);

    if (res)
    {
        printf("Ciphertext: %s", res);
        free(res);
        printf("\n");
    }

    printf("\n################################-PARTE 3-##################################\n\n");

    char *cipherText = "072c232c223d2c3e3e2c2328232538202e2c3f3f223d223f2c3c3824072c232c223d2c3e3e2c2328232538202b24212028232c191b1b222e283c382828233f22212c2238393f222e242a2c3f3f223d223f2c2408232c22292c2f22212c3d3f223c38283b2c242c2e222339282e283f002c243e38203d22382e2228202c243e38203e282e38212239283f2024232c002c3e38202122382e223d222928393f222e22232c283e3c3824232c19382922243e3e22272c2b2c373d2c3f3928292c3f223924232c082c3f223924232c272c2b2c373d2c3f392829283b222e281c3828392820242928242c3e392c22202229283f232c3e082220283e202225222028203c38283b243b242c232c3e2e2c3b283f232c3e";
    decipher(cipherText);

    printf("\n");

    return 0;
}
