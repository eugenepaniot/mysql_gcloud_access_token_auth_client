#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mysql/client_plugin.h>
#include <mysql.h>

void gcloud_access_token_auth_log(const char *format,...);
void gcloud_access_token_auth_log_error(const char *format,...);