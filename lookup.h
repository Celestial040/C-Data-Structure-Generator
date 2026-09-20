/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf -L ANSI-C -t -c -N lookup_key generate_key_value.gperf  */
/* Computed positions: -k'' */

#line 1 "generate_key_value.gperf"

#include <string.h>
#line 4 "generate_key_value.gperf"
struct KeyMapping { const char *name; int id; };

#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 9
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 9
#define MAX_HASH_VALUE 15
/* maximum key range = 7, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
/*ARGSUSED*/
static unsigned int
hash (register const char *str, register size_t len)
{
  (void) str;
  return len;
}

struct KeyMapping *
lookup_key (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct KeyMapping wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 7 "generate_key_value.gperf"
      {"utils_dir",       1},
#line 8 "generate_key_value.gperf"
      {"header_dir",      2},
      {""}, {""},
#line 6 "generate_key_value.gperf"
      {"status_header",   0},
      {""},
#line 9 "generate_key_value.gperf"
      {"struct_template", 3}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return (struct KeyMapping *) 0;
}
#line 10 "generate_key_value.gperf"

