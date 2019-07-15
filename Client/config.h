#if !defined(CONFIG_H)
#define CONFIG_H

#include <sgx_key_exchange.h>
#include <sgx_report.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#define SET_OPT(x,y)	x|=y
#define CLEAR_OPT(x,y)	x=x&~y
#define OPT_ISSET(x,y)	x&y

#define OPT_IAS_PROD	0x01
#define OPT_NO_PROXY	0x02

typedef struct config_struct {
	sgx_spid_t spid;
	uint16_t quote_type;
	EVP_PKEY *service_private_key;
	char *proxy_server;
	char ca_bundle[256];
	char *user_agent;
	char *cert_file;
	char *cert_key_file;
	unsigned int proxy_port;
	unsigned char kdk[16];
	char *cert_type[4];
	X509_STORE *store;
	X509 *signing_ca;
	unsigned int apiver;
	int strict_trust;
	uint32_t flags;
	int n_clusters; //for kmeans
	int max_iters;
	int data_dims;
	int data_rows;
	double lambda;
	double learning_rate;
	char *model;
	char data_file_path[1024];
	char server[256];
	int check_mrenclave;
	uint8_t mrenclave[32];
} config_t;

#endif
