#pragma once

#include "LittleEndianConsts.h"

class LittleEndian : public LittleEndianConsts
{
public:
	static ByteArrayPtr getByteArray2(ByteArrayPtr data, int offset, int size);

	/**
	* get an int value from a byte array
	*
	* @param data
	*            the byte array
	* @param offset
	*            a starting offset into the byte array
	* @return the int (32-bit) value
	*/
	static __int32 getInt(ByteArrayPtr data)
	{
		return getInt(data, 0);
	}
	static __int32 getInt(ByteArrayPtr data, int offset);

	static void putInt(ByteArrayPtr data, int offset, __int32 value);

	/**
	* get a long value from a byte array
	*
	* @param data
	*            the byte array
	* @param offset
	*            a starting offset into the byte array
	* @return the long (64-bit) value
	*/
	static __int64 getLong(ByteArrayPtr data, int offset);

	static void putLong(ByteArrayPtr data, int offset, __int64 value);

	/**
	* get an unsigned int value from a byte array
	*
	* @param data
	*            the byte array
	* @return the unsigned int (32-bit) value in a long
	*/
	static long getUInt(ByteArrayPtr data);

	/**
	* get an unsigned int value from a byte array
	*
	* @param data
	*            the byte array
	* @param offset
	*            a starting offset into the byte array
	* @return the unsigned int (32-bit) value in a long
	*/
	static long getUInt(ByteArrayPtr data, int offset)
	{
		long retNum = getInt(data, offset);
		return retNum & 0x00FFFFFFFFl;
	}

	/**
	* get an unsigned short value from the beginning of a byte array
	*
	* @param data
	*            the byte array
	* @return the unsigned short (16-bit) value in an int
	*/
	static int getUShort(ByteArrayPtr data)
	{
		return getUShort(data, 0);
	}

	/**
	* get an unsigned short value from a byte array
	*
	* @param data
	*            the byte array
	* @param offset
	*            a starting offset into the byte array
	* @return the unsigned short (16-bit) value in an integer
	*/
	static int getUShort(ByteArrayPtr data, int offset)
	{
		int b0 = (*data)[offset] & 0xFF;
		int b1 = (*data)[offset + 1] & 0xFF;
		return (b1 << 8) + (b0 << 0);
	}

	/**
	* get a short value from the beginning of a byte array
	*
	* @param data
	*            the byte array
	* @return the short (16-bit) value
	*/
	static short getShort(ByteArrayPtr data)
	{
		return getShort(data, 0);
	}

	/**
	* get a short value from a byte array
	*
	* @param data
	*            the byte array
	* @param offset
	*            a starting offset into the byte array
	* @return the short (16-bit) value
	*/
	static short getShort(ByteArrayPtr data, int offset)
	{
		int b0 = (*data)[offset] & 0xFF;
		int b1 = (*data)[offset + 1] & 0xFF;
		return (short)((b1 << 8) + (b0 << 0));
	}

	static double getDouble(ByteArrayPtr data)
	{
		return getDouble(data, 0);
	}

	static double getDouble(ByteArrayPtr data, int offset)
	{
		long long val = getLong(data, offset);
		return *(double*)&val;
	}
};