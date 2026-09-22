/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf -L ANSI-C -t -c -N struct_template_keyword -H struct_template_hash -W struct_template_wordlist struct_template_keyword.gperf  */
/* Computed positions: -k'' */

#line 1 "struct_template_keyword.gperf"

#include <string.h>
#line 5 "struct_template_keyword.gperf"
struct TemplateKeyMapping { const char *name; int id; };

#define TOTAL_KEYWORDS 3
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 14
/* maximum key range = 11, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
/*ARGSUSED*/
static unsigned int
struct_template_hash (register const char *str, register size_t len)
{
  (void) str;
  return len;
}

struct TemplateKeyMapping *
struct_template_keyword (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct TemplateKeyMapping struct_template_wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 9 "struct_template_keyword.gperf"
      {"type",2},
      {""}, {""}, {""}, {""},
#line 8 "struct_template_keyword.gperf"
      {"base_name",1},
      {""}, {""}, {""}, {""},
#line 7 "struct_template_keyword.gperf"
      {"data_structure",0}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = struct_template_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = struct_template_wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &struct_template_wordlist[key];
        }
    }
  return (struct TemplateKeyMapping *) 0;
}
#line 10 "struct_template_keyword.gperf"

#undef TOTAL_KEYWORDS
#undef MIN_WORD_LENGTH
#undef MAX_WORD_LENGTH
#undef MIN_HASH_VALUE
#undef MAX_HASH_VALUE
