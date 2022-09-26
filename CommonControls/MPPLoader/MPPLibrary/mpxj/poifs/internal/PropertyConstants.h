#pragma once

/**
 * Interface PropertyConstants
 */

class PropertyConstants
{
public:
    static const int  PROPERTY_TYPE_OFFSET = 0x42;

    // the property types
    static const BYTE DIRECTORY_TYPE	 = 1;
	static const BYTE DOCUMENT_TYPE		= 2;
	static const BYTE ROOT_TYPE			= 5;
};   // end public interface PropertyConstants