#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned short version;

void readfile(FILE *, FILE *, int, int);

int dumpUnknown = 0;

int main(int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "+a")) != -1) {
		switch (opt) {
		case 'a':
			dumpUnknown = 1;
			break;
		default:
			fprintf(stderr, "Usage: %s [-a] file.TDT\n", argv[0]);
			return -1;
		}
	}
	if (optind >= argc) {
		fprintf(stderr, "Usage: %s [-a] file.TDT\n", argv[0]);
		return -1;
	}
	const char *infile = argv[optind];
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
	readfile(in, out, 0, 1);
	if (dumpUnknown) {
		// Show the rest of the file.... Painful, I know
		fprintf(out, "Unknown data:\n", "");
		unsigned char buffer[8];
		int i = 0, val = 0;
		for (;; i+= 8) {
			val = fread(buffer, 1, 8, in);
			if (val < 8) break;
			fprintf(out, "%s%02x %02x %02x %02x %02x %02x %02x %02x%s",
				i % 16 == 0 ? " " : "",
				buffer[0], buffer[1], buffer[2], buffer[3], buffer[4],
				buffer[5], buffer[6], buffer[7], i % 16 == 0 ? "  " : "\n");
		}
		if (i % 16 == 0) fprintf(out, " ");
		for (i = 0; i < val; i++) {
			fprintf(out, "%02x ", buffer[i]);
		}
		fprintf(out, "\n");
	}
	fclose(in);
	fclose(out);
}

const char *printTenantType(signed char type) {
	static const char *types[] = {"Floor", "__TYPE_1__", "__TYPE_2__",
		"Single Room", "Twin Room", "Hotel Suite", "Restaurant", "Office",
		"__TYPE_8__", "Condo", "Retail", "Parking Space", "Fast Food",
		"Medical Center", "Security", "Housekeeping", "__TYPE_16__", "SECOM",
		"Movie_18", "Movie_19", "Recycling_20", "Recycling_21", "__TYPE_22__",
		"__TYPE_23__", "Lobby", "__TYPE_25__", "__TYPE_26__", "__TYPE_27__",
		"__TYPE_28__", "Party Hall_29", "Party Hall_30", "Metro_31", "Metro_32",
		"Metro_33", "Movie_34", "Movie_35", "Cathedral_36", "Cathedral_37",
		"Cathedral_38", "Cathedral_39", "Cathedral_40", "__TYPE_41__",
		"structures", "__TYPE_43__", "__TYPE_44__", "Parking Ramp",
		"__TYPE_46__", "__TYPE_47__", "__TYPE_48__"};
	static char buf[128];
	unsigned char rawType = type < 0 ? -type : type;
	if (rawType >= sizeof(types)/sizeof(types[0]))
		sprintf(buf, "__TYPE_%hhd__", type);
	else
		sprintf(buf, "%s%s", types[rawType], type < 0 ? " (construction)" : "");
	return buf;
}

#define BEGIN_STRUCT(name) \
void read##name(FILE *in, FILE *out, int space, int important) { \
	void *buffer = malloc(100); \
	size_t curSize = 100;
#define ENSURE_SPACE(size) \
	do { \
	if (size > curSize) { \
		buffer = realloc(buffer, size); \
		curSize = size; \
	} \
	} while (0)
#define STRUCT_ELEMENT(type, str) \
	ENSURE_SPACE(sizeof(type)); \
	fread(buffer, sizeof(type), 1, in); \
	if (important) \
		fprintf(out, "%*s" #str ": %d\n", space, "", *(type*)(buffer));
#define STRUCT_SPECIAL_ELEMENT(type, str, print) \
	ENSURE_SPACE(sizeof(type)); \
	fread(buffer, sizeof(type), 1, in); \
	if (important) \
		fprintf(out, "%*s" #str ": %s\n", space, "", print(*(type*)buffer));
#define UNKNOWN_DATA(type, size) \
	if (important && dumpUnknown) {\
		fprintf(out, "%*sUnknown data:\n", space, ""); \
		uint8_t buffer[8]; \
		int i = 0; \
		for (; i + 8 < sizeof(type) * size; i+= 8) { \
			fread(buffer, 1, 8, in); \
			fprintf(out, "%*s%02x %02x %02x %02x %02x %02x %02x %02x%s", \
				i % 16 == 0 ? space + 1 : 0, "", \
				buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], \
				buffer[5], buffer[6], buffer[7], i % 16 == 0 ? "  " : "\n"); \
		} \
		if (i % 16 == 0) fprintf(out, "%*s", space + 1, ""); \
		for (; i < sizeof(type) * size; i++) { \
			fread(buffer, 1, 1, in); \
			fprintf(out, "%02x ", buffer[0]); \
		} \
		if (i % 16 != 0) fprintf(out, "\n"); \
	} else { \
		fseek(in, sizeof(type) * size, SEEK_CUR); \
	}
#define NAMED_STRUCT_ELEMENT(type, var, str) \
	type var; \
	fread(&var, sizeof(type), 1, in); \
	if (important) \
		fprintf(out, "%*s" #str ": %d\n", space, "", var);
#define STRUCT_ELEMENT_STRUCT_ARRAY(st, count, str) \
	{ \
		int i = 0; \
		for (i = 0; i < count; i++) { \
			if (important) \
				fprintf(out, "%*s" #str " %d:\n", space, "", i); \
			read ##st (in, out, space + 1, important); \
		} \
	}
#define STRUCT_ELEMENT_ARRAY(type, count, str) \
	if (sizeof(type) * count > curSize) { \
		buffer = realloc(buffer, sizeof(type) * count); \
		curSize = sizeof(type) * count; \
	} \
	fread(buffer, sizeof(type), count, in); \
	if (important) { \
		int i = 0; \
		for (i = 0; i < count; i++) { \
			fprintf(out, "%*s" #str " %d: %d\n", space, "", i, \
				((type*)buffer)[i]); \
		} \
	}
#define END_STRUCT(name) \
}

#define OUTPUT_STRUCT
#include "file.h"
