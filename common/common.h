#ifndef _COMMON_H
#define _COMMON_H

#define PREFIX_LEN		4
#define CLIENT_KEY_PREFIX	"ID__"
#define RA_PREFIX		"RA__"
#define ERR_PREFIX		"ERR_"
#define OK_PREFIX		"OK__"
#define DATA_WRITE_PREFIX	"DW__"
#define END_PREFIX		"END_"

enum RA_state {NOT_STARTED, STARTED, W_FOR_MSG01, W_FOR_MSG2, W_FOR_MSG3, W_FOR_MSG4};

void edividerWithText(const char *text);
void edivider();

/*
void dividerWithText(FILE *fd, const char *text);
void divider(FILE *fd);
*/

int eprintf(const char *format, ...);
int eputs(const char *s);

#endif