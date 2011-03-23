#include "strscan.h"

int
main(int argc, char **argv)
{
  static const char * const s = "hello, world!";
  scn_scanner *sc;
  char c, match[100];
  const char *buf, *chrset;
  size_t buflen;

  sc = scn_new(s, strlen(s));

  /* current pointer */
  buf = scn_curptr(sc);
  printf("curptr -> \"%s\"\n", buf);

  /* peek a char */
  scn_peek(sc, 0, &c);
  printf("peek 0 -> %c\n", c);
  scn_peek(sc, 1, &c);
  printf("peek 1 -> %c\n", c);
  scn_peek(sc, 2, &c);
  printf("peek 2 -> %c\n", c);
  scn_peek(sc, 3, &c);
  printf("peek 3 -> %c\n", c);

  /* get next char */
  scn_next(sc, &c);
  printf("next -> %c\n", c);
  scn_next(sc, &c);
  printf("next -> %c\n", c);
  scn_next(sc, &c);
  printf("next -> %c\n", c);
  scn_next(sc, &c);
  printf("next -> %c\n", c);
  scn_next(sc, &c);
  printf("next -> %c\n", c);

  /* clear position */
  scn_set_pos(sc, 0);
  printf("pos = 0\n");

  /* scan characters from set */
  chrset = "abcdefghijklmnopqrstuvwxyz";
  buflen = scn_scan_chrset(sc, chrset, &buf);
  strncpy(match, buf, buflen);
  match[buflen] = '\0';
  printf("lowcase chrset -> %s (%lu)\n", buf, buflen);
  if (scn_scan_chrset(sc, chrset, &buf) > 0)
    printf("error\n");
  if (scn_scan_chrset(sc, ",", NULL) > 0)
    printf("scan \",\"\n");

  scn_free(sc);

  return 0;
}

