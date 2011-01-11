#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "structures.h"

int main (int argc, const char * argv[]) {
	if (argc != 2) return 1;
	const char * inpath = argv[1];
	char outpath[512];
	outpath[0] = 0;
	strcat(outpath, inpath);
	strcat(outpath, ".txt");
	char respath[512];
	respath[0] = 0;
	strcat(respath, inpath);
	strcat(respath, " Resources");
	
	FILE * fin = fopen(inpath, "rb");
	FILE * fout = fopen(outpath, "w");
	fprintf(fout, "--- %s ---\n\n", inpath);
	mkdir(respath, 0777);
	
	
	//Offset
	uint32_t shoff;
	fseek(fin, 0x3C, SEEK_SET);
	fread(&shoff, 1, 4, fin);
	fprintf(fout, "offset to segmented header = 0x%X (%i)\n\n\n", shoff, shoff);
	
	//Segmented EXE Header
	SegmentedEXEHeader_struct sh;
	fseek(fin, shoff, SEEK_SET);
	fread(&sh, sizeof(sh), 1, fin);
	fprintf(fout, "+++ SEGMENTED EXE HEADER +++\n");
	fprintf(fout, "entry table offset = 0x%X -> 0x%X\n", sh.entryTableOffset, sh.entryTableOffset + shoff);
	fprintf(fout, "entry table length = %i Bytes\n\n", sh.numberOfEntryTableBytes);
	fprintf(fout, "automatic data segment number = %i\n\n", sh.automaticDataSegmentNumber);
	fprintf(fout, "initial added dynamic heap size = %i\n", sh.initialAddedDynamicHeapSize);
	fprintf(fout, "initial added stack size = %i\n\n", sh.initialAddedStackSize);
	fprintf(fout, "segment table entries = %i\n", sh.numberOfSegmentTableEntries);
	fprintf(fout, "module reference table entries = %i\n", sh.numberOfModuleReferenceTableEntries);
	fprintf(fout, "non-resident name table length = %i Bytes\n\n", sh.numberOfNonResidentNameTableBytes);
	fprintf(fout, "segment table offset           = 0x%X -> 0x%X\n", sh.segmentTableOffset, sh.segmentTableOffset + shoff);
	fprintf(fout, "resource table offset          = 0x%X -> 0x%X\n", sh.resourceTableOffset, sh.resourceTableOffset + shoff);
	fprintf(fout, "resident name table offset     = 0x%X -> 0x%X\n", sh.residentNameTableOffset, sh.residentNameTableOffset + shoff);
	fprintf(fout, "module reference table offset  = 0x%X -> 0x%X\n", sh.moduleReferenceTableOffset, sh.moduleReferenceTableOffset + shoff);
	fprintf(fout, "imported names table offset    = 0x%X -> 0x%X\n", sh.importedNamesTableOffset, sh.importedNamesTableOffset + shoff);
	fprintf(fout, "non-resident name table offset = 0x%X -> 0x%X\n\n", sh.nonResidentNameTableOffset, sh.nonResidentNameTableOffset + shoff);
	fprintf(fout, "number of movable entries in entry table = %i\n", sh.numberOfMovableEntryTableEntries);
	fprintf(fout, "logical sector alignment shift = %i\n", sh.logicalSectorAlignmentShift);
	fprintf(fout, "number of resource entries = %i\n", sh.numberOfResourceEntries);
	fprintf(fout, "executable type = 0x%X\n\n\n", sh.executableType);
	
	//Segment table
	fprintf(fout, "+++ SEGMENT TABLE +++\n");
	fseek(fin, sh.segmentTableOffset + shoff, SEEK_SET);
	int i;
	for (i = 0; i < sh.numberOfSegmentTableEntries; i++) {
		SegmentTableEntry ste;
		unsigned int pos = ftell(fin);
		fread(&ste, sizeof(ste), 1, fin);
		fprintf(fout, "#%i %X\t 0x%X\t %i\t 0x%X\t %i\t", i + 1, pos, ste.logicalSectorOffset, ste.lengthOfSegment, ste.flags, ste.minimumAllocationSize);
		unsigned int type = ste.flags & 0x7;
		if (type == 0) fprintf(fout, "CODE");
		if (type == 1) fprintf(fout, "DATA");
		if (ste.flags & 0x0010) fprintf(fout, " MOVEABLE");
		if (ste.flags & 0x0040) fprintf(fout, " PRELOAD");
		if (ste.flags & 0x0100) fprintf(fout, " RELOCINFO");
		if (ste.flags & 0xF000)	fprintf(fout, " DISCARD");
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n\n");
	
	//Resource table
	fprintf(fout, "+++ RESOURCE TABLE +++\n");
	fseek(fin, sh.resourceTableOffset + shoff + 2, SEEK_SET);
	while (1) {
		ResourceTableEntry entry;
		fread(&entry, sizeof(entry), 1, fin);
		if (entry.typeID == 0) break;
		fprintf(fout, "#%X, %i resources\n", entry.typeID & 0x7FFF, entry.numberOfResources);
		char restypepath[512] = "";
		strcat(restypepath, respath);
		strcat(restypepath, "/");
		char restype[64];
		sprintf(restype, "%X", entry.typeID & 0x7FFF);
		strcat(restypepath, restype);
		mkdir(restypepath, 0777);
		
		for (i = 0; i < entry.numberOfResources; i++) {
			ResourceEntry resource;
			fread(&resource, sizeof(resource), 1, fin);
			fprintf(fout, "\t #%X\t 0x%X\t 0x%X\t 0x%X\t", resource.resourceID & ~0x8000, resource.offset, resource.length, resource.flags);
			if (resource.flags & 0x0010) fprintf(fout, " MOVEABLE");
			if (resource.flags & 0x0020) fprintf(fout, " PURE");
			if (resource.flags & 0x0040) fprintf(fout, " PRELOAD");
			fprintf(fout, "\n");
			
			int res_offset = resource.offset;
			int res_length = resource.length;
			res_offset <<= sh.logicalSectorAlignmentShift;
			res_length <<= sh.logicalSectorAlignmentShift;
			
			char respath[512] = "";
			strcat(respath, restypepath);
			strcat(respath, "/");
			char res[64];
			sprintf(res, "%X", resource.resourceID & ~0x8000);
			strcat(respath, res);
			
			FILE * fres = fopen(respath, "w");
			long jumpback = ftell(fin);
			fseek(fin, res_offset, SEEK_SET);
			void * res_buffer = malloc(res_length);
			fread(res_buffer, 1, res_length, fin);
			fseek(fin, jumpback, SEEK_SET);
			fwrite(res_buffer, 1, res_length, fres);
			fclose(fres);
			free(res_buffer);
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
	
	//Resource name table
	/*fprintf(fout, "  -- resource names\n");
	unsigned int strid = 0x8000;
	while (1) {
		uint8_t length;
		fread(&length, 1, 1, fin);
		if (length == 0) break;
		char * str = malloc(length + 1);
		str[length] = 0;
		fread(str, length, 1, fin);
		fprintf(fout, "#%X '%s'\n", strid++, str);
	}
	fprintf(fout, "\n\n");*/
	
	fclose(fin);
	fclose(fout);
    return 0;
}
