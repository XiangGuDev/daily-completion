#pragma once

#include "POIFSConstants.h"
#include "LittleEndianConsts.h"

/**
 * Constants used in reading/writing the Header block
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class HeaderBlockConstants
{
public:
    static const __int64 _signature = 0xE11AB1A1E011CFD0L;
	static const int  _bat_array_offset = 0x4c;
	static const int  _max_bats_in_header =
        (POIFSConstants::SMALLER_BIG_BLOCK_SIZE - _bat_array_offset)
        / LittleEndianConsts::INT_SIZE; // If 4k blocks, rest is blank

    // Note - in Microsoft terms:
    //  BAT ~= FAT
    //  SBAT ~= MiniFAT
    //  XBAT ~= DIFat
    
    // useful offsets
	static const int  _signature_offset = 0;
	static const int  _bat_count_offset = 0x2C;
	static const int  _property_start_offset = 0x30;
	static const int  _sbat_start_offset = 0x3C;
	static const int  _sbat_block_count_offset = 0x40;
	static const int  _xbat_start_offset = 0x44;
	static const int  _xbat_count_offset = 0x48;
};   // end interface HeaderBlockConstants