using namespace std;


#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstdio>
#include <string.h>
#include <string>
#include "common.h"

#define LINE_TYPE '-'
#define LINE_SHORT_LEN 4
#define LINE_MAX_LEN   76
#define LINE_TRAILING_LEN(header) ((LINE_MAX_LEN - string(header).size()) - LINE_SHORT_LEN -2)

#define LINE_COMPLETE (string( LINE_MAX_LEN, LINE_TYPE).c_str())

#define LINE_HEADER(header) (string(string( LINE_SHORT_LEN, LINE_TYPE) + ' ' + string(header) + ' ' + string(LINE_TRAILING_LEN(header), LINE_TYPE)).c_str())

#define INDENT(level) (string( level, ' ' ))

#define WARNING_INDENT(level) (string(level, '*'))

#define TIMESTR_SIZE	64

static void dividerWithText (FILE *fd, const char *text)
{
	fprintf(fd, "\n%s\n", LINE_HEADER(text));
}

void edividerWithText (const char *text)
{
	dividerWithText(stderr, text);
//	if ( fplog != NULL ) dividerWithText(fplog, text);
}

static void divider (FILE * fd)
{
	fprintf(fd, "%s\n", LINE_COMPLETE);
}

void edivider ()
{
	divider(stderr);
//	if ( fplog != NULL ) divider(fplog);
}

int eprintf (const char *format, ...)
{
	va_list va;
	int rv;

	va_start(va, format);
	rv= vfprintf(stderr, format, va);
	va_end(va);
#if 0
	if ( fplog != NULL ) {
		time_t ts;
		struct tm timetm;
		char timestr[TIMESTR_SIZE];	

		/* Don't timestamp a single "\n" */
		if ( !(strlen(format) == 1 && format[0] == '\n') ) {
			time(&ts);
			timetm= *localtime(&ts);

			/* If you change this format, you _may_ need to change TIMESTR_SIZE */
			if ( strftime(timestr, TIMESTR_SIZE, "%b %e %Y %T", &timetm) == 0 ) {
				/* oops */
				timestr[0]= 0;
			}
			fprintf(fplog, "%s ", timestr);
		}
		va_start(va, format);
		rv= vfprintf(fplog, format, va);
		va_end(va);
	}
#endif
	return rv;
}

int eputs (const char *s)
{
//	if ( fplog != NULL ) fputs(s, fplog);
	return fputs(s, stderr);
}