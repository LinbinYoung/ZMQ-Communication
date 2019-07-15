#ifndef _COMMON_H
#define _COMMON_H

#include "unistd.h"

#define PREFIX_LEN		4
#define REMOTE_ATT	"ID__"
#define PARAMETER "PAR_"
#define CALCULATION "CAL_"

enum RA_state {NOT_STARTED, STARTED, W_FOR_MSG01, W_FOR_MSG2, W_FOR_MSG3, W_FOR_MSG4};

typedef struct STUDNET{
    char port[5];
    int age;
    int score;
    char name[5];
    char SID[5];
}STD;

typedef struct TEACHER{
    char name[5];
    char SID[8];
}TEA;

void edividerWithText(const char *text);
void edivider();

/*
void dividerWithText(FILE *fd, const char *text);
void divider(FILE *fd);
*/

int eprintf(const char *format, ...);
int eputs(const char *s);

#endif