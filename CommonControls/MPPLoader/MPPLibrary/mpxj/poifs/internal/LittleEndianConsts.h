#pragma once

/**
 * a repository for constants shared by classes within this package
 *
 * @author Marc Johnson
 * @author Andrew C. Oliver (acoliver at apache dot org)
 */

class LittleEndianConsts
{
public:
    // sizes of various numbers in this environment
    static const int BYTE_SIZE   = 1;
	static const int SHORT_SIZE = 2;
	static const int INT_SIZE = 4;
	static const int LONG_SIZE = 8;
	static const int DOUBLE_SIZE = 8;
};   // end public interface LittleEndianConsts