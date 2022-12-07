#include "gcloud_access_token_auth_client_log.h"
#include "gcloud_access_token_auth_client.h"

static int gcloud_access_token_auth_client_init(
                      char    *unused1 __attribute__((unused)), 
                      size_t  unused2  __attribute__((unused)), 
                      int     unused3  __attribute__((unused)), 
                      va_list unused4  __attribute__((unused)))
{
  gcloud_access_token_auth_log("%s init", PLUGIN_DESCRIPTION);
  return 0;
}

static int gcloud_access_token_auth_client_deinit()
{
  gcloud_access_token_auth_log("%s deinit", PLUGIN_DESCRIPTION);
  return 0;
}

static int gcloud_access_token_auth_client_handler(MYSQL_PLUGIN_VIO *vio, MYSQL *mysql)
{
  int pkt_len;
  unsigned char *pkt;

  if (!vio || !mysql) {
    gcloud_access_token_auth_log_error("Bug?");
    return CR_ERROR;
  }

  if (mysql->passwd[0]) {
    gcloud_access_token_auth_log("Password provided using it");
    return vio->write_packet(vio, (const unsigned char *) mysql->passwd, (int)strlen(mysql->passwd) ) ? CR_ERROR : CR_OK;
  }

  FILE *gcloud;
  char buf[2048];
  size_t nread;
  static char *cmd = "gcloud auth print-access-token 2>&1";

	gcloud = popen(cmd, "r");
	if (gcloud) {
    nread = fread(buf, sizeof(buf), 1, gcloud);
  }

  gcloud_access_token_auth_log("Got access token: %s (%d)", buf, strlen(buf));

  mysql->passwd = strdup(buf);
  if (!mysql->passwd) {
    gcloud_access_token_auth_log_error("Cannt allocate mysql->passwd");
    return CR_AUTH_PLUGIN_ERROR;
  }
  
  int res = vio->write_packet(vio, (const unsigned char *) buf, (int)strlen(buf) );
  if(res) { 
    gcloud_access_token_auth_log_error("write_packet error: %d", res);
    return CR_ERROR;
  }

  return CR_OK;
}

mysql_declare_client_plugin(AUTHENTICATION)
  PLUGIN_NAME,
  "Eugene Paniot",
  PLUGIN_DESCRIPTION,
  {0,1,0},
  "GPL",
  NULL,
  gcloud_access_token_auth_client_init,
  gcloud_access_token_auth_client_deinit,
  NULL, // options
  NULL, // get_options
  gcloud_access_token_auth_client_handler // authenticate_user
mysql_end_client_plugin;
