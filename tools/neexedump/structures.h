/*
 *  structures.h
 *  neexedump
 *
 *  Created by Fabian Schuiki on 29.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include <stdint.h>

typedef struct {
	uint16_t signatureWord;
	uint8_t linkerVersion;
	uint8_t linkerRevision;
	uint16_t entryTableOffset;
	uint16_t numberOfEntryTableBytes;
	uint32_t fileCRC;
	uint16_t flags;
	uint16_t automaticDataSegmentNumber;
	uint16_t initialAddedDynamicHeapSize;
	uint16_t initialAddedStackSize;
	uint32_t segmentNumberOfCSIP;
	uint32_t segmentNumberOfSSSP;
	uint16_t numberOfSegmentTableEntries;
	uint16_t numberOfModuleReferenceTableEntries;
	uint16_t numberOfNonResidentNameTableBytes;
	uint16_t segmentTableOffset;
	uint16_t resourceTableOffset;
	uint16_t residentNameTableOffset;
	uint16_t moduleReferenceTableOffset;
	uint16_t importedNamesTableOffset;
	uint32_t nonResidentNameTableOffset;
	uint16_t numberOfMovableEntryTableEntries;
	uint16_t logicalSectorAlignmentShift;
	uint16_t numberOfResourceEntries;
	uint8_t executableType; //0x02 = WINDOWS
	uint8_t _reserved[9];
} __attribute__((__packed__)) SegmentedEXEHeader_struct;

typedef struct {
	uint16_t logicalSectorOffset;
	uint16_t lengthOfSegment;
	uint16_t flags;
	uint16_t minimumAllocationSize;
} __attribute__((__packed__)) SegmentTableEntry;

typedef struct {
	uint16_t typeID;
	uint16_t numberOfResources;
	uint32_t _reserved;
} __attribute__((__packed__)) ResourceTableEntry;

typedef struct {
	uint16_t offset;
	uint16_t length;
	uint16_t flags;
	uint16_t resourceID;
	uint32_t _reserved;
} __attribute__((__packed__)) ResourceEntry;
