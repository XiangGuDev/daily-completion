#pragma once

#include "POIFSBigBlockSize.h"

/**
 * <p>A repository for constants shared by POI classes.</p>
 */
class POIFSConstants
{
public:
    /** Most files use 512 bytes as their big block size */
    static const int SMALLER_BIG_BLOCK_SIZE = 0x0200;
	static const POIFSBigBlockSize SMALLER_BIG_BLOCK_SIZE_DETAILS;// (SMALLER_BIG_BLOCK_SIZE, (short)9);
    /** Some use 4096 bytes */
    static const int LARGER_BIG_BLOCK_SIZE = 0x1000;
	static const POIFSBigBlockSize LARGER_BIG_BLOCK_SIZE_DETAILS;// (LARGER_BIG_BLOCK_SIZE, (short)12);
    
    /** How big a block in the small block stream is. Fixed size */
    static const int SMALL_BLOCK_SIZE = 0x0040; 
    
    /** How big a single property is */
    static const int PROPERTY_SIZE  = 0x0080;
    
    /** 
     * The minimum size of a document before it's stored using 
     *  Big Blocks (normal streams). Smaller documents go in the 
     *  Mini Stream (SBAT / Small Blocks)
     */
    static const int BIG_BLOCK_MINIMUM_DOCUMENT_SIZE = 0x1000;
    
    /** The highest sector number you're allowed, 0xFFFFFFFA */
    static const int LARGEST_REGULAR_SECTOR_NUMBER = -5;
    
    /** Indicates the sector holds a DIFAT block (0xFFFFFFFC) */
    static const int DIFAT_SECTOR_BLOCK   = -4;
    /** Indicates the sector holds a FAT block (0xFFFFFFFD) */
    static const int FAT_SECTOR_BLOCK   = -3;
    /** Indicates the sector is the end of a chain (0xFFFFFFFE) */
    static const int END_OF_CHAIN   = -2;
    /** Indicates the sector is not used (0xFFFFFFFF) */
    static const int UNUSED_BLOCK   = -1;
    
    /** The first 4 bytes of an OOXML file, used in detection */
	static const BYTE OOXML_FILE_HEADER[];// = { 0x50, 0x4b, 0x03, 0x04 };
};   // end interface POIFSConstants;