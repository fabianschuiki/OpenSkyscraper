#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short version;

void readfile(FILE *, FILE *, int);

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s file.TDT\n", argv[0]);
		return -1;
	}
	const char *infile = argv[1];
	char *outfile = strdup(infile);
	char *munge;
	if (!(munge = strstr(outfile, ".tdt")) &&
			!(munge = strstr(outfile, ".TDT"))) {
		fprintf(stderr, "Error: bad file %s\n", infile);
		return -1;
	}
	strcpy(munge, ".txt");

	FILE *in = fopen(infile, "rb");
	FILE *out = fopen(outfile, "w");
	if (!in || !out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	fread(&version, sizeof(version), 1, in);
	readfile(in, out, 0);
	fclose(in);
	fclose(out);
}

#define BEGIN_STRUCT(name) \
void read##name(FILE *in, FILE *out, int space) { \
	void *buffer = malloc(100); \
	size_t curSize = 100;
#define STRUCT_ELEMENT(type, str) \
	if (sizeof(type) > curSize) { \
		buffer = realloc(buffer, sizeof(type)); \
		curSize = sizeof(type); \
	} \
	fread(buffer, sizeof(type), 1, in); \
	fprintf(out, "%*s" #str ": %d\n", space, "", *(type*)(buffer));
#define UNKNOWN_DATA(type, size) \
	fseek(in, sizeof(type) * size, SEEK_CUR);
#define NAMED_STRUCT_ELEMENT(type, var, str) \
	type var; \
	fread(&var, sizeof(type), 1, in); \
	fprintf(out, "%*s" #str ": %d\n", space, "", var);
#define STRUCT_ELEMENT_STRUCT_ARRAY(st, count, str) \
	{ \
		int i = 0; \
		for (i = 0; i < count; i++) { \
			fprintf(out, "%*s" #str " %d:\n", space, "", i); \
			read ##st (in, out, space + 1); \
		} \
	}
#define STRUCT_ELEMENT_ARRAY(type, count, str) \
	if (sizeof(type) * count > curSize) { \
		buffer = realloc(buffer, sizeof(type) * count); \
		curSize = sizeof(type) * count; \
	} \
	fread(buffer, sizeof(type), count, in); \
	{ \
		int i = 0; \
		for (i = 0; i < count; i++) { \
			fprintf(out, "%*s" #str " %d: %d\n", space + 1, "", i, \
				((type*)buffer)[i]); \
		} \
	}
#define END_STRUCT(name) \
}

#include "file.h"
