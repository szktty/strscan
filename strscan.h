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
static inline _Bool scn_chrclscmp(const char *clsset, char c);

static inline size_t scn_scan_chr(scn_scanner *sc, char c, const char **into);
static inline size_t scn_scan_chrset(scn_scanner *sc,
    const char *chrset, const char **into);
static inline size_t scn_scan_chrclsset(scn_scanner *sc,
    const char *clsset, const char **into);
static inline size_t scn_scan_str(scn_scanner *sc, const char *s,
    const char **into);
static inline size_t scn_scan_strn(scn_scanner *sc, const char *s,
    size_t len, const char **into);
static inline size_t scn_scan_stri(scn_scanner *sc, const char *s,
    const char **into);
static inline size_t scn_scan_strin(scn_scanner *sc, const char *s,
    size_t len, const char **into);

static inline size_t scn_scan_upto_chr(scn_scanner *sc, char c,
    const char **into);
static inline size_t scn_scan_upto_chrset(scn_scanner *sc,
    const char *chrset, const char **into);
static inline size_t scn_scan_upto_chrclsset(scn_scanner *sc,
    const char *clsset, const char **into);
static inline size_t scn_scan_upto_str(scn_scanner *sc, const char *s,
    const char **into);
static inline size_t scn_scan_upto_strn(scn_scanner *sc, const char *s,
    size_t len, const char **into);
static inline size_t scn_scan_upto_stri(scn_scanner *sc, const char *s,
    const char **into);
static inline size_t scn_scan_upto_strin(scn_scanner *sc, const char *s,
    size_t len, const char **into);

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
scn_scan_chr(scn_scanner *sc, char c, const char **into)
{
  size_t i;
  char pc;

  i = 0;
  while (scn_peek(sc, i, &pc) && pc == c)
    i++;

  if (i > 0) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    sc->pos += i;
    return i;
  } else
    return 0;
}

static inline size_t
scn_scan_chrset(scn_scanner *sc, const char *chrset,
    const char **into)
{
  size_t i;
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
scn_chrclscmp(const char *clsset, char c)
{
  char cls;

  while ((cls = *clsset++) != '\0') {
    switch (cls) {
    case 'a':
      /* alpha */
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
        return true;
      break;

    case 'A':
      /* non alpha */
      if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')))
        return true;
      break;

    case 'd':
      /* digit */
      if ('0' <= c && c <= '9')
        return true;
      break;

    case 'D':
      /* non digit */
      if (!('0' <= c && c <= '9'))
        return true;
      break;

    case 'h':
      /* alphanum + '-'  */
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
          ('0' <= c && c <= '9') || c == '-')
        return true;
      break;

    case 'H':
      /* non alphanum + non '-' */
      if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') || c == '-'))
        return true;
      break;

    case 'l':
      /* lower case */
      if ('a' <= c && c <= 'z')
        return true;
      break;

    case 'L':
      /* non lower case */
      if (!('a' <= c && c <= 'z'))
        return true;
      break;

    case 'n':
      /* alphanum */
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
          ('0' <= c && c <= '9'))
        return true;
      break;

    case 'N':
      /* non alphanum */
      if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9')))
        return true;
      break;

    case 's':
      /* spaces */
      if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f')
        return true;
      break;

    case 'S':
      /* non spaces */
      if (!(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f'))
        return true;
      break;

    case 'u':
      /* upper case */
      if ('A' <= c && c <= 'Z')
        return true;
      break;

    case 'U':
      /* non upper case */
      if (!('A' <= c && c <= 'Z'))
        return true;
      break;

    case 'w':
      /* alphanum + '_'  */
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
          ('0' <= c && c <= '9') || c == '_')
        return true;
      break;

    case 'W':
      /* non alphanum + non '_'  */
      if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
          ('0' <= c && c <= '9') || c == '_')
        return true;
      break;

    default:
      if (c == cls)
        return true;
      break;
    }
  }
  return false;
}

static inline size_t
scn_scan_chrclsset(scn_scanner *sc, const char *clsset, const char **into)
{
  size_t i;
  char c;

  i = 0;
  while (scn_peek(sc, i, &c) && scn_chrclscmp(clsset, c))
    i++;

  if (i > 0) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    sc->pos += i;
    return i;
  } else
    return 0;
}

static inline size_t
scn_scan_str(scn_scanner *sc, const char *s, const char **into)
{
  return scn_scan_strn(sc, s, strlen(s), into);
}

static inline size_t
scn_scan_strn(scn_scanner *sc, const char *s, size_t len, const char **into)
{
  size_t i;
  char c;

  for (i = 0; i < len; i++) {
    if (!(scn_peek(sc, i, &c) && c == s[i]))
      return 0;
  }

  if (into != NULL)
    *into = sc->s.cs + sc->pos;
  sc->pos += len;
  return len;
}

static inline size_t
scn_scan_upto_chr(scn_scanner *sc, char c, const char **into)
{
  size_t rest, i;
  char pc;

  i = 0;
  while (scn_peek(sc, i, &c) && pc != c)
    i++;

  if (i > 0) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    sc->pos += i;
    return i;
  } else
    return 0;
}

static inline size_t
scn_scan_upto_chrset(scn_scanner *sc, const char *chrset,
    const char **into)
{
  size_t rest, i;
  char c;

  i = 0;
  while (scn_peek(sc, i, &c) && !scn_chrcmp(chrset, c))
    i++;

  if (i > 0) {
    sc->pos += i;
    return scn_scan_chrset(sc, chrset, into);
  } else
    return 0;
}

static inline size_t
scn_scan_upto_chrclsset(scn_scanner *sc, const char *clsset,
    const char **into)
{
  size_t i;
  char c;

  i = 0;
  while (scn_peek(sc, i, &c) && !scn_chrclscmp(clsset, c))
    i++;

  if (i > 0) {
    sc->pos += i;
    return scn_scan_chrclsset(sc, clsset, into);
  } else
    return 0;
}

static inline size_t
scn_scan_upto_str(scn_scanner *sc, const char *s, const char **into)
{
  return scn_scan_upto_strn(sc, s, strlen(s), into);
}

static inline size_t
scn_scan_upto_strn(scn_scanner *sc, const char *s, size_t len,
    const char **into)
{
  size_t i, j;
  char c;

  i = 0;
  while (scn_peek(sc, i, &c)) {
    if (c == *s) {
      for (j = 1; j < len; j++) {
        if (!(scn_peek(sc, i+j, &c) && c == s[j]))
          goto not_match;
      }

      if (into != NULL)
        *into = sc->s.cs + sc->pos;
      sc->pos += i;
      return i;

not_match:
      i += j;
    } else
      i++;
  }
  return 0;
}

#endif /* STRSCAN_H */

