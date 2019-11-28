#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// adb logcat -v time -b kernel

#define errno 0
#define isspace(c)  ((c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == ' ')

#include <string-format/string-format.h>

#define MAX_BUF_SIZE    1024

struct cmd_data_t {
    const char *cmd_string;
    string_format_cmd_t cmd;
} cmd_data[] = {
    {"keep", SF_KEEP},
    {"upper", SF_TO_UPPER},
    {"lower", SF_TO_LOWER},
    {"toggle", SF_TOGGLE},
    {"title", SF_TO_TITLE},
    {"reverse", SF_REVERSE},

    {NULL, 0},
};


void cmd_help(void)
{
    puts("cammand help:");
    puts("  keep       ");
    puts("  upper      ");
    puts("  lower      ");
    puts("  toggle     ");
    puts("  tittle      ");
    puts("");
}


bool get_cmd_and_args(char *buf, char **pcmd, char **pargs)
{
    char *cmd;
    char *args;
    char *_args;

    cmd = buf;
    while (isspace(*cmd))  cmd++;
    if (*cmd == 0)
        return false;
    args = cmd;

    while (!isspace(*args))  args++;
    if (*args) {
        *args++ = 0;
        while (isspace(*args))  args++;
        if (*args) {
            _args = args;
            while (*_args) _args++;
            _args--;
            while (isspace(*_args)) _args--;
            *++_args = 0;

        } else
            args = NULL;
    } else 
        args = NULL;

    *pcmd = cmd;
    *pargs = args;
    return true;
}

void resolve_cmd(string_format_t sf, char *buf, const char *cmd, const char *args, const char *argv)
{
    for (int i = 0; cmd_data[i].cmd_string; i++) {
        if (!strcmp(cmd, cmd_data[i].cmd_string)) {
            if (args == NULL) {
                fprintf(stderr, "%s: error: cammamd \"%s\" need arguments.\n", argv, cmd);
                return;
            }

            printf("cammand: \"%s\"\ninput:   \"%s\"\n", cmd, args);
            if (string_format_cmd(buf, args, MAX_BUF_SIZE, sf, cmd_data[i].cmd))
                printf("output:  \"%s\"\n", buf);
            else 
                fprintf(stderr, "%s: error: \"string_format_cmd\" return NULL.\n", argv);
            return;
        }
    }

    if (!strcmp(cmd, "exit")) {
        if (string_format_close(sf) < 0) {
            fprintf(stderr, "%s: error: string_format_close faild[%d]: %s.\n", argv, errno, strerror(errno));
            exit(1);
        }
        exit(0);
    } else if (!strcmp(cmd, "help")) {
        cmd_help();
    } else 
        fprintf(stderr, "%s: error: unknown cammand \"%s\".\n", argv, cmd);
}

int main(int argc, char *argv[])
{
    string_format_t sf = 0;
    char input_buf[MAX_BUF_SIZE + 1];
    char buf[MAX_BUF_SIZE + 1];
    char *cmd;
    char *args;

    if ((sf = string_format_open()) < 0) {
        fprintf(stderr, "%s: error: string_format_open faild[%d]: %s.\n", argv[0], errno, strerror(errno));
        return 1;
    }

    while (1) {
        printf(">>> "); 
        fflush(stdout);
        fgets(input_buf, MAX_BUF_SIZE, stdin);
        
        if (get_cmd_and_args(input_buf, &cmd, &args))
            resolve_cmd(sf, buf, cmd, args, argv[0]);
    }

    return 0;
}
