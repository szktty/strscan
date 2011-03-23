#ifndef STRSCAN_H
#define STRSCAN_H

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

static inline _Bool scn_peek_chr(scn_scanner *sc, char *into);
static inline _Bool scn_peek_str(scn_scanner *sc, size_t len,
    const char **into);
static inline _Bool scn_next_chr(scn_scanner *sc, char *into);
static inline _Bool scn_next_str(scn_scanner *sc, size_t len,
    const char **into);
static inline _Bool scn_chrcmp(scn_scanner *sc, const char c1, const char c2);

/* forwards scan location if matched */
static inline size_t scn_scan_chrset(scn_scanner *sc,
    const char *chrset, char **into, size_t *into_len);
static inline _Bool scn_scan_str(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_strn(scn_scanner *sc, const char *s,
    size_t len, char **into, size_t *into_len);
static inline _Bool scn_scan_lowcases(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_upcases(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_alphas(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_alphanums(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_word(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_digits(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_spaces(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_scan_hdr(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);

static inline size_t scn_scan_upto_chrset(scn_scanner *sc,
    const char *chrset, char **into, size_t *into_len);
static inline _Bool scn_scan_upto_str(scn_scanner *sc,
    const char *s, char **into, size_t *into_len);
static inline _Bool scn_scan_upto_strn(scn_scanner *sc,
    const char *s, size_t len, char **into, size_t *into_len);

/* keeps current scan location whether matched or not */
static inline size_t scn_search_chrset(scn_scanner *sc,
    const char *chrset, char **into, size_t *into_len);
static inline _Bool scn_search_str(scn_scanner *sc, const char *s,
    char **into, size_t *into_len);
static inline _Bool scn_search_strn(scn_scanner *sc, const char *s,
    size_t len, char **into, size_t *into_len);

static inline size_t scn_search_upto_chrset(scn_scanner *sc,
    const char *chrset, char **into, size_t *into_len);
static inline _Bool scn_search_upto_str(scn_scanner *sc,
    const char *s, char **into, size_t *into_len);
static inline _Bool scn_search_upto_strn(scn_scanner *sc,
    const char *s, size_t len, char **into, size_t *into_len);

/* private */
static inline void _scn_copy(scn_scanner *sc, size_t start,
    size_t end, char **into, size_t *into_len);

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

static inline _Bool
scn_peek_chr(scn_scanner *sc, char *into)
{
  if (sc->pos+1 < sc->len) {
    if (into != NULL)
      *into = sc->s.cs[sc->pos+1];
    return true;
  } else
    return false;
}

static inline _Bool
scn_peek_str(scn_scanner *sc, size_t len, const char **into)
{
  if (sc->pos+len < sc->len) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    return true;
  } else
    return false;
}

static inline _Bool
scn_next_chr(scn_scanner *sc, char *into)
{
  if (sc->pos < sc->len) {
    if (into != NULL)
      *into = sc->s.cs[sc->pos++];
    return true;
  } else
    return false;
}

static inline _Bool
scn_next_str(scn_scanner *sc, size_t len, const char **into)
{
  if (sc->pos+len < sc->len) {
    if (into != NULL)
      *into = sc->s.cs + sc->pos;
    sc->pos += len;
    return true;
  } else
    return false;
}

static inline _Bool
scn_chrcmp(scn_scanner *sc, const char c1, const char c2)
{
  switch (c1) {
  case "\1":
    break;
  }
  return c1 == c2 ||
    (!sc->s.cs && (('a' <= c1 && c1 <= 'z' && c1 - 32 == c2) ||
                 ('A' <= c1 && c1 <= 'Z' && c1 + 32 == c2)));
}

static inline _Bool
scn_scan_chrset(scn_scanner *sc, const char *chrset, char **into, size_t *into_len)
{
  size_t start, len;
  char c;
  const char *t;

  start = sc->pos;
  while (scn_next(sc, &c)) {
    t = chrset;
    while (*t != '\0') {
      if (!scn_chrcmp(sc, c, *t)) {
        sc->pos--;
        break;
      }
      t++;
    }
    if (*t != '\0')
      goto not_match;
  }

  if (sc->pos > start) {
    len = sc->pos - start;
    if (into != NULL) {
      *into = (char *)SCN_MALLOC(len+1);
      strncpy(*into, sc->s.cs+start, len);
      (*into, size_t *into_len)[len] = '\0';
    }
    return true;
  }

not_match:
  sc->pos = start;
  return false;
}

static inline _Bool
scn_scan_str(scn_scanner *sc, const char *s, char **into, size_t *into_len)
{
  return scn_scan_strn(sc, s, strlen(s), into, size_t *into_len);
}

static inline _Bool
scn_scan_strn(scn_scanner *sc, const char *s, size_t len, char **into, size_t *into_len)
{
  size_t i, start;
  char c;

  start = sc->pos;
  for (i = 0; i < len; i++) {
    if (!(scn_next(sc, &c) && scn_chrcmp(sc, c, s[i])))
      goto not_match;
  }

  _scn_copy(sc, start, sc->pos, into, size_t *into_len);
  return true;

not_match:
  sc->pos = start;
  return false;
}

static inline _Bool
scn_scan_crlf(scn_scanner *sc)
{
  return scn_scan_str(sc, "\r\n", NULL);
}

static inline void
_scn_copy(scn_scanner *sc, size_t start, size_t end, char **into, size_t *into_len)
{
  size_t len;

  if (into != NULL) {
    len = end - start;
    *into = (char *)SCN_MALLOC(len+1);
    strncpy(*into, sc->s.cs+start, len);
    (*into, size_t *into_len)[len] = '\0';
  }
}

static inline _Bool
scn_scan_upto_chrset(scn_scanner *sc, const char *chrset, char **into, size_t *into_len)
{
  size_t start;
  char c;
  const char *t;

  start = sc->pos;
  while (scn_next(sc, &c)) {
    t = chrset;
    while (*t != '\0') {
      if (scn_chrcmp(sc, c, *t)) {
        sc->pos--;
        goto end;
      }
      t++;
    }
  }

end:
  if (sc->pos > start) {
    _scn_copy(sc, start, sc->pos, into, size_t *into_len);
    return true;
  } else {
    sc->pos = start;
    return false;
  }
}

static inline _Bool
scn_scan_upto_str(scn_scanner *sc, const char *s, char **into, size_t *into_len)
{
  size_t start;
  char c;
  const char *t;

  start = sc->pos;

begin:
  t = s;
  while (*t != '\0') {
    if (scn_next(sc, &c)) {
      if (!scn_chrcmp(sc, c, *t))
        goto begin;
    } else
      goto not_match;
  }

  if (sc->pos > start) {
    _scn_copy(sc, start, sc->pos, into, size_t *into_len);
    return true;
  }
 
not_match:
  sc->pos = start;
  return false;
}

static inline _Bool
scn_scan_upto_space(scn_scanner *sc, char **into, size_t *into_len)
{
  return scn_scan_upto_chrset(sc, " ", into, size_t *into_len);
}

static inline _Bool
scn_scan_upto_crlf(scn_scanner *sc, char **into, size_t *into_len)
{
  return scn_scan_upto_str(sc, "\r\n", into, size_t *into_len);
}

static inline _Bool
scn_scan_digits(scn_scanner *sc, char **into, size_t *into_len)
{
  size_t start;
  char c;

  start = sc->pos;
  while (scn_next(sc, &c)) {
    if (!('0' <= c && c <= '9')) {
      sc->pos--;
      break;
    }
  }

  if (sc->pos > start) {
    _scn_copy(sc, start, sc->pos, into, size_t *into_len);
    return true;
  } else {
    sc->pos = start;
    return false;
  }
}

static inline void
scn_skip(scn_scanner *sc)
{
  char c;

  if (sc->s.cskip != NULL) {
    while (scn_next(sc, &c)) {
      if (strchr(sc->s.cskip, c) == NULL) {
        sc->pos--;
        return;
      }
    }
  }
}

#endif /* STRSCAN_H */

