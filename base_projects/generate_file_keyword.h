/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf -L ANSI-C -t -c -N generate_file_keyword -H generate_file_hash -W generate_file_wordlist generate_file_keyword.gperf  */
/* Computed positions: -k'' */

#line 1 "generate_file_keyword.gperf"

#include <string.h>
#line 5 "generate_file_keyword.gperf"
struct GenKeyMapping { const char *name; int id; };

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
generate_file_hash (register const char *str, register size_t len)
{
  (void) str;
  return len;
}

struct GenKeyMapping *
generate_file_keyword (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct GenKeyMapping generate_file_wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 8 "generate_file_keyword.gperf"
      {"utils_dir",       1},
#line 9 "generate_file_keyword.gperf"
      {"header_dir",      2},
      {""}, {""},
#line 7 "generate_file_keyword.gperf"
      {"status_header",   0},
      {""},
#line 10 "generate_file_keyword.gperf"
      {"struct_template", 3}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = generate_file_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = generate_file_wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &generate_file_wordlist[key];
        }
    }
  return (struct GenKeyMapping *) 0;
}
#line 11 "generate_file_keyword.gperf"

#undef TOTAL_KEYWORDS
#undef MIN_WORD_LENGTH
#undef MAX_WORD_LENGTH
#undef MIN_HASH_VALUE
#undef MAX_HASH_VALUE
