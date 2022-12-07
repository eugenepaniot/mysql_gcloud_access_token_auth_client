#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "gcloud_access_token_auth_client_log.h"
#include "gcloud_access_token_auth_client.h"

void gcloud_access_token_auth_log(const char *format,...) {
    char *debug_var;
    va_list args;

    debug_var = getenv("GCLOUD_AUTH_DEBUG");
    if (!debug_var || strcmp(debug_var, "") == 0 || strcmp(debug_var, "0") == 0) {
        va_end(args);
        return;
    }

    va_start(args, format);
    fprintf(stderr, "[DEBUG][%s]: ", PLUGIN_NAME);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}


void gcloud_access_token_auth_log_error(const char *format,...) {
    char *debug_var;
    va_list args;

    va_start(args, format);
    fprintf(stderr, "[ERORR][%s]: ", PLUGIN_NAME);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

