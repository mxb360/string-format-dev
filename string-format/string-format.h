#ifndef __STRING_FORMAT_STRING_FORMAT_H__
#define __STRING_FORMAT_STRING_FORMAT_H__

#define SF_KEEP             0
#define SF_TO_UPPER         1
#define SF_TO_LOWER         2
#define SF_TOGGLE           3
#define SF_TO_TITLE         4
#define SF_REVERSE          5

//#define IN_ANDROID_APP

typedef unsigned long string_format_cmd_t;

typedef int string_format_t;

#ifdef __cplusplus
extern "C" {
#endif

string_format_t string_format_open(void);

char *string_format_cmd(char *dest, const char *src, int maxsize, string_format_t sf,
                        string_format_cmd_t cmd);

int string_format_close(string_format_t sf);


#ifdef __cplusplus
}
#endif

#endif
