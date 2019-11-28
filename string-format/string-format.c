#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "string-format.h"

#define CDEV_NAME "/dev/string-format"

#ifdef IN_ANDROID_APP
#include <errno.h>
#include <android/log.h>

#define TAG "string-format"
#define print_error(prompt)  __android_log_print(ANDROID_LOG_ERROR, TAG, "%s: %s[%d]: %s", __func__, prompt, errno, strerror(errno))
#define print_info(...)      __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#else
#define errno 0
#define print_error(prompt)      fprintf(stderr, CDEV_NAME "%s: %s[%d]: %s", __func__, prompt, errno, strerror(errno))
#define print_info(...)          ((void)0)
#endif


string_format_t string_format_open(void)
{
    int fd = open(CDEV_NAME, O_RDWR);
    if (fd < 0)
        print_error("\"open\" ERROR");
    else
        print_info(CDEV_NAME "open(\"%s\", %d) OK", CDEV_NAME, O_RDWR);
    
    return fd;
}

char *string_format_cmd(char *dest, const char *src, int maxsize, string_format_t sf, string_format_cmd_t cmd) 
{
    int len = strlen(src) + 1;

    if (ioctl(sf, _IO('s', cmd)) < 0) {
        print_error("\"ioctl\" ERROR");
        return NULL;
    } else
        print_info("ioctl(%d, %d) OK", sf, cmd);


    if (write(sf, src, len) < 0) {
        print_error("\"write\" ERROR");
        return NULL;
    } else
        print_info("write(%d, \"%s\", %d) OK", sf, src, len);


    if (read(sf, dest, maxsize) < 0) {
        print_error("\"read\" ERROR");
        return NULL;
    } else
        print_info("read(%d, \"%s\", %d) OK", sf, dest, maxsize);

    return dest;
}

int string_format_close(string_format_t sf)
{
    int res = close(sf);
    if (res < 0)
        print_error("\"close\" ERROR");
    else
        print_info("read(%d) OK", sf);
    return res;
}
