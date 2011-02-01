#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short u16;

#define NE_RSCTYPE_CURSOR             0x8001
#define NE_RSCTYPE_BITMAP             0x8002
#define NE_RSCTYPE_ICON               0x8003
#define NE_RSCTYPE_MENU               0x8004
#define NE_RSCTYPE_DIALOG             0x8005
#define NE_RSCTYPE_STRING             0x8006
#define NE_RSCTYPE_FONTDIR            0x8007
#define NE_RSCTYPE_FONT               0x8008
#define NE_RSCTYPE_ACCELERATOR        0x8009
#define NE_RSCTYPE_RCDATA             0x800a
#define NE_RSCTYPE_GROUP_CURSOR       0x800c
#define NE_RSCTYPE_GROUP_ICON         0x800e

 typedef struct _NAMEINFO {
     u16 rnOffset;
     u16 rnLength;
     u16 rnFlags;
     u16 rnID;
     u16 rnHandle;
     u16 rnUsage;
 } NAMEINFO;

 typedef struct _TYPEINFO {
     u16        rtTypeID;
     u16        rtResourceCount;
     int        rtReserved;
     NAMEINFO    *rtNameInfo;
 } TYPEINFO;

void output_bmp(char *contents, int length, FILE *out);
void output_ico(char *contents, int length, FILE *out);
void output_cur(char *contents, int length, FILE *out);

struct outtype {
	u16 typeID;
	const char *fext;
	void (*out)(char *contents, int length, FILE *out);
} outtypes[] = {
	{NE_RSCTYPE_BITMAP, "bmp", output_bmp},
	{NE_RSCTYPE_GROUP_ICON, "ico", output_ico},
	{NE_RSCTYPE_GROUP_CURSOR, "cur", output_cur},
	{0, NULL, NULL}
};

char *outDir;
FILE *readFile;
u16 segmentOffset;
int sizeShift;

char *getTypeName(u16 typeId) {
	static const char *typeNames[] = {
		"ERROR_8000", "CURSOR", "BITMAP", "ICON", "MENU", "DIALOG", "STRING",
		"FONTDIR", "FONT", "ACCELERATOR", "RCDATA", "ERROR_800b",
		"GROUP_CURSOR", "ERROR_800d", "GROUP_ICON", "ERROR_800f"
	};
	if (typeId & 0x8000)
		return strdup(typeNames[typeId & 0xf]);
	long pos = ftell(readFile);
	fseek(readFile, segmentOffset + typeId, SEEK_SET);
	char length;
	fread(&length, 1, 1, readFile);
	char *buf = malloc(length + 1);
	fread(buf, length, 1, readFile);
	buf[length] = 0;
	fseek(readFile, pos, SEEK_SET);
	return buf;
}
char *getNameName(u16 typeId) {
	if (typeId & 0x8000) {
		char buf[10];
		sprintf(buf, "%hd", typeId & 0x7fff);
		return strdup(buf);
	}
	long pos = ftell(readFile);
	fseek(readFile, segmentOffset + typeId, SEEK_SET);
	char length;
	fread(&length, 1, 1, readFile);
	char *buf = malloc(length + 1);
	fread(buf, length, 1, readFile);
	buf[length] = 0;
	fseek(readFile, pos, SEEK_SET);
	return buf;
}

TYPEINFO *readTypeInfo() {
	TYPEINFO *t = malloc(sizeof(TYPEINFO));
	fread(t, 2, 4, readFile);
	t->rtNameInfo = malloc(sizeof(NAMEINFO) * t->rtResourceCount);
	fread(t->rtNameInfo, sizeof(NAMEINFO), t->rtResourceCount, readFile);
	return t;
}

void dumpTypeInfo(TYPEINFO *t) {
	char *tname = getTypeName(t->rtTypeID);
	printf("Resource type %s, count %d\n", tname, t->rtResourceCount);
	int i;
	for (i = 0; i < t->rtResourceCount; i++) {
		struct outtype *writer = outtypes;
		while (writer->fext) {
			if (t->rtTypeID == writer->typeID)
				break;
			writer++;
		}
		char *nname = getNameName(t->rtNameInfo[i].rnID);
		int offset = t->rtNameInfo[i].rnOffset * sizeShift;
		int length = t->rtNameInfo[i].rnLength * sizeShift;
		char fatbuf[256];
		sprintf(fatbuf, "%s/%s.%s", outDir, nname,
			(writer->fext ? writer->fext : tname));
		printf("\tName %d: offset: %x, length: %x, flags: %x to %s\n",
				i, offset, length, t->rtNameInfo[i].rnFlags, fatbuf);
		long pos = ftell(readFile);
		fseek(readFile, offset, SEEK_SET);
		FILE *f = fopen(fatbuf, "wb");
		char *buffer = malloc(length);
		fread(buffer, length, 1, readFile);
		if (writer->fext)
			writer->out(buffer, length, f);
		else
			fwrite(buffer, length, 1, f);
		fclose(f);
		free(buffer);
		fseek(readFile, pos, SEEK_SET);
		free(nname);
	}
	free(tname);
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <NE file> <output directory>\n",
				argv[0]);
		return 1;
	}
	outDir = argv[2];
	readFile = fopen(argv[1], "rb");
	// XXX: 0x250 offset is currently hardcoded, should be reading the real
	// windows header offset...
	fseek(readFile, 0x250 + 0x24, SEEK_SET);
	fread(&segmentOffset, 2, 1, readFile);
	segmentOffset += 0x250;
	fseek(readFile, segmentOffset, SEEK_SET);
	u16 alignShift;
	fread(&alignShift, 2, 1, readFile);
	sizeShift = 1 << alignShift;

	TYPEINFO *t;
	while (1) {
		t = readTypeInfo();
		if (t->rtTypeID == 0)
			break;
		dumpTypeInfo(t);
		free(t->rtNameInfo);
		free(t);
	}
	free(t->rtNameInfo);
	free(t);
}

#include<stdint.h>

typedef struct {
	uint32_t filesz;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t bmp_offset;
} bmp_fileheader;

typedef struct {
  uint32_t header_sz;
  int32_t width;
  int32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
} bmp_infoheader;

void output_bmp(char *contents, int length, FILE *out) {
	bmp_fileheader header = {length + sizeof(bmp_fileheader) + 2, 0, 0,
		sizeof(bmp_fileheader) + sizeof(bmp_infoheader) + 2};
	bmp_infoheader *info = (bmp_infoheader *)contents;
	if (info->ncolors != 0)
		header.bmp_offset += 4 * info->ncolors;
	else if (info->bitspp <= 8)
		header.bmp_offset += 4 * (1 << info->bitspp);
	fwrite("BM", 2, 1, out);
	fwrite(&header, sizeof(bmp_fileheader), 1, out);
	fwrite(contents, length, 1, out);
}

typedef struct {
	uint16_t reserved;
	uint16_t imageType;
	uint16_t numImages;
} ico_fileheader;

typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t color;
	uint8_t reserved;
	uint16_t colorPlane;
	uint16_t bbp;
	uint32_t bitmapSize;
	uint32_t bitmapData;
} ico_imageentry;

typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t color;
	uint8_t reserved;
	uint16_t colorPlane;
	uint16_t bbp;
	uint32_t bitmapSize;
	uint16_t resID;
} __attribute__((packed)) ico_groupentry;

// Note: to guarantee that this works, you'll need to run the program twice,
// once to generate the .ICON and .CURSOR files, the second time to make usable
// .ico and .cur files

void output_icoOrCur(char *contents, int length, FILE *out, const char *fext) {
	ico_fileheader *header = (ico_fileheader*)contents;
	uint32_t offset = sizeof(ico_fileheader) +
		sizeof(ico_imageentry)*header->numImages;
	fwrite(header, sizeof(ico_fileheader), 1, out);
	int i = 0;
	for (i = 0; i < header->numImages; i++) {
		ico_groupentry *entry = (ico_groupentry*)(contents +
			sizeof(ico_fileheader) + sizeof(ico_imageentry) * i);
		ico_imageentry outentry = { entry->width,
			entry->height, entry->color, entry->reserved, entry->colorPlane,
			entry->bbp, entry->bitmapSize, offset };
		offset += entry->bitmapSize;
		fwrite(&outentry, sizeof(ico_imageentry), 1, out);
	}
	for (i = 0; i < header->numImages; i++) {
		ico_groupentry *entry = (ico_groupentry*)(contents +
			sizeof(ico_fileheader) + sizeof(ico_imageentry) * i);
		char fatbuf[256];
		sprintf(fatbuf, "%s/%d.%s", outDir, entry->resID, fext);
		FILE *ico = fopen(fatbuf, "rb");
		char *contents = malloc(entry->bitmapSize);
		fread(contents, entry->bitmapSize, 1, ico);
		fwrite(contents, entry->bitmapSize, 1, out);
		free(contents);
	}
}

void output_ico(char *contents, int length, FILE *out) {
	output_icoOrCur(contents, length, out, "ICON");
}
void output_cur(char *contents, int length, FILE *out) {
	output_icoOrCur(contents, length, out, "CURSOR");
}
