#ifndef STRSCAN_H
#define STRSCAN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SCN_MALLOC      malloc
#define SCN_FREE        free

typedef struct scn_scanner scn_scanner;

struct scn_scanner {
  /* string to be skipped. not needed to be NUL terminated */
  union {
    char *s;
    const char *cs;
  } s;
  size_t len;
  size_t pos;
  _Bool nocopy;
};

static inline scn_scanner *scn_new(const char *s, size_t len);
static inline scn_scanner *scn_new_nocopy(const char *s, size_t len);
static inline void scn_init(scn_scanner *sc, const char *s, size_t len);
static inline void scn_init_nocopy(scn_scanner *sc, const char *s, size_t len);
static inline void scn_free(scn_scanner *sc);
static inline void scn_final(scn_scanner *sc);

static inline const char *scn_str(scn_scanner *sc);
static inline size_t scn_pos(scn_scanner *sc);
static inline void scn_set_pos(scn_scanner *sc, size_t pos);
static inline void scn_forward(scn_scanner *sc, size_t len);
static inline _Bool scn_atend(scn_scanner *sc);

static inline const char *scn_curptr(scn_scanner *sc);
static inline _Bool scn_peek(scn_scanner *sc, size_t i, char *into);
static inline _Bool scn_next(scn_scanner *sc, char *into);
static inline _Bool scn_chrcmp(const char *chrset, char c);

static inline size_t scn_scan_chrset(scn_scanner *sc,
    const char *chrset, const char **into, size_t *into_len);
static inline _Bool scn_scan_str(scn_scanner *sc, const char *s,
    const char **into, size_t *into_len);
static inline _Bool scn_scan_strn(scn_scanner *sc, const char *s,
    size_t len, const char **into, size_t *into_len);
static inline _Bool scn_scan_lowcases(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_upcases(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_alphas(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_alphanums(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_word(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_digits(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_spaces(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_crlf(scn_scanner *sc,
    char **into, size_t *into_len);
static inline _Bool scn_scan_hdr(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);

static inline _Bool scn_scan_upto_chrset(scn_scanner *sc,
    const char *chrset, char **into, size_t *into_len);
static inline _Bool scn_scan_upto_str(scn_scanner *sc,
    const char *s, char **into, size_t *into_len);
static inline _Bool scn_scan_upto_strn(scn_scanner *sc,
    const char *s, size_t len, char **into, size_t *into_len);

static inline scn_scanner *
scn_new(const char *s, size_t len)
{
  scn_scanner *sc;

  sc = (scn_scanner *)SCN_MALLOC(sizeof(scn_scanner));
  scn_init(sc, s, len);
  return sc;
}

static inline scn_scanner *
scn_new_nocopy(const char *s, size_t len)
{
  scn_scanner *sc;

  sc = (scn_scanner *)SCN_MALLOC(sizeof(scn_scanner));
  scn_init_nocopy(sc, s, len);
  return sc;
}

static inline void
scn_init(scn_scanner *sc, const char *s, size_t len)
{
  sc->s.s = (char *)SCN_MALLOC(len);
  strncpy(sc->s.s, s, len);
  sc->len = len;
  sc->pos = 0;
  sc->nocopy = false;
}

static inline void
scn_init_nocopy(scn_scanner *sc, const char *s, size_t len)
{
  sc->s.cs = s;
  sc->len = len;
  sc->pos = 0;
  sc->nocopy = true;
}

static inline void
scn_free(scn_scanner *sc)
{
  scn_final(sc);
  SCN_FREE(sc);
}

static inline void
scn_final(scn_scanner *sc)
{
  if (!sc->nocopy)
    SCN_FREE(sc->s.s);
}

static inline const char *
scn_str(scn_scanner *sc)
{
  return sc->s.cs;
}

static inline size_t
scn_pos(scn_scanner *sc)
{
  return sc->pos;
}

static inline void
scn_set_pos(scn_scanner *sc, size_t pos)
{
  sc->pos = pos;
}

static inline void
scn_forward(scn_scanner *sc, size_t len)
{
  sc->pos += len;
}

static inline _Bool
scn_atend(scn_scanner *sc)
{
  return sc->pos >= sc->len;
}

static inline const char *
scn_curptr(scn_scanner *sc)
{
  return sc->s.cs + sc->pos;
}

static inline _Bool
scn_peek(scn_scanner *sc, size_t i, char *into)
{
  if (sc->pos+i < sc->len) {
    if (into != NULL)
      *into = sc->s.cs[sc->pos+i];
    return true;
  } else
    return false;
}

static inline _Bool
scn_next(scn_scanner *sc, char *into)
{
  if (sc->pos < sc->len) {
    if (into != NULL)
      *into = sc->s.cs[sc->pos++];
    return true;
  } else
    return false;
}

static inline _Bool
scn_chrcmp(const char *chrset, char c)
{
  while (*chrset != '\0') {
   if (*chrset++ == c)
     return true;
  }
  return false;
}

static inline size_t
scn_scan_chrset(scn_scanner *sc, const char *chrset,
    const char **into, size_t *into_len)
{
  size_t rest, i;
  char c;

  i = 0;
  while (scn_peek(sc, i, &c) && scn_chrcmp(chrset, c))
    i++;

  if (i > 0) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    sc->pos += i;
    return i;
  } else
    return 0;
}

static inline _Bool
scn_scan_str(scn_scanner *sc, const char *s,
    const char **into, size_t *into_len)
{
  return scn_scan_strn(sc, s, strlen(s), into, into_len);
}

static inline _Bool
scn_scan_strn(scn_scanner *sc, const char *s, size_t len,
    const char **into, size_t *into_len)
{
  size_t i;
  char c;

  for (i = 0; i < len; i++) {
    if (!(scn_peek(sc, i, &c) && c == s[i]))
      return false;
  }

  if (into != NULL)
    *into = sc->s.cs + sc->pos;
  if (into_len != NULL)
    *into_len = len;
  sc->pos += len;
  return true;
}

/*
static inline _Bool
scn_scan_crlf(scn_scanner *sc)
{
  return scn_scan_str(sc, "\r\n", NULL);
}

*/

#endif /* STRSCAN_H */

