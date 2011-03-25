#include "strscan.h"

int
main(int argc, char **argv)
{
  static const char * const s1 = "hello, world!";
  static const char * const s2 = "lowerUPPERcamelCase123num";
  scn_scanner *sc;
  char c, match[100];
  const char *buf, *chrset;
  size_t buflen;

  sc = scn_new(s1, strlen(s1));

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

  /* scan char */
  if (scn_scan_chr(sc, ','))
    printf("scan comma\n");
  else
    printf("error\n");

  /* no match */
  if (scn_scan_chr(sc, ","))
    printf("error\n");
  scn_peek(sc, 0, &c);
  printf("peek 0 -> %c\n", c);

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

  if (scn_scan_chrset(sc, ", ", NULL) > 0)
    printf("scan \", \"\n");
  else
    printf("error\n");

  if ((buflen = scn_scan_str(sc, "world", &buf)) > 0)
    printf("scan %s (%lu)\n", buf, buflen);
  else
    printf("error\n");
  scn_free(sc);

  sc = scn_new(s2, strlen(s2));
  if ((buflen = scn_scan_chrclsset(sc, "l", &buf)) > 0)
    printf("scan lowcases -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  if ((buflen = scn_scan_chrclsset(sc, "u", &buf)) > 0)
    printf("scan upcases -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  if ((buflen = scn_scan_chrclsset(sc, "a", &buf)) > 0)
    printf("scan alphas -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  scn_set_pos(sc, 0);
  if ((buflen = scn_scan_upto_chrset(sc, "123", &buf)) > 0)
    printf("scan upto chrset \"123\" -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  scn_set_pos(sc, 0);
  if ((buflen = scn_scan_upto_chrclsset(sc, "d", &buf)) > 0)
    printf("scan upto digit -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  scn_set_pos(sc, 0);
  if ((buflen = scn_scan_upto_str(sc, "123", &buf)) > 0)
    printf("scan upto str \"123\" -> %s (%lu)\n", buf, buflen);
  else
    printf("error\n");

  scn_free(sc);
  return 0;
}

