
#ifndef H_STR2AV
#define H_STR2AV

typedef enum {
    STR2AV_OK       = 0,
    STR2AV_UNBALANCED_QUOTE
} str_to_argv_err_t;

#ifdef __cplusplus
extern "C"
#endif
str_to_argv_err_t str2av(char const * str, int * argc_p, char *** argv_p);

#endif
