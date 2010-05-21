#ifndef VBUCKET_H
#define VBUCKET_H

typedef void* VBUCKET_CONFIG_HANDLE;

/* Creation and destruction */
VBUCKET_CONFIG_HANDLE vbucket_config_parse_file(const char *filename);
VBUCKET_CONFIG_HANDLE vbucket_config_parse_string(const char *data);
void vbucket_config_destroy(VBUCKET_CONFIG_HANDLE h);

/* Misc */
const char *vbucket_get_error(void);

/* Config access */
int vbucket_config_get_num_replicas(VBUCKET_CONFIG_HANDLE h);
int vbucket_config_get_num_vbuckets(VBUCKET_CONFIG_HANDLE h);
int vbucket_config_get_num_servers(VBUCKET_CONFIG_HANDLE h);
const char *vbucket_config_get_server(VBUCKET_CONFIG_HANDLE h, int i);

/* VBucket access */
int vbucket_get_vbucket_by_key(VBUCKET_CONFIG_HANDLE h, const void *key, size_t nkey);
int vbucket_get_master(VBUCKET_CONFIG_HANDLE h, int id);
int vbucket_get_replica(VBUCKET_CONFIG_HANDLE h, int id, int n);

#endif
