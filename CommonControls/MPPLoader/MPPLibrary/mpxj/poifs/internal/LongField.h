#pragma once

#include "FixedField.h"

/**
 * representation of a long (16-bit) field at a fixed location within
 * a byte array
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class LongField : FixedField
{
public:
    /**
     * construct the LongField with its offset into its containing
     * byte array
     *
     * @param offset of the field within its byte array
     *
     * @exception ArrayIndexOutOfBoundsException if offset is negative
     */

	LongField(int offset);

    /**
     * construct the LongField with its offset into its containing
     * byte array and initialize its value
     *
     * @param offset of the field within its byte array
     * @param value the initial value
     *
     * @exception ArrayIndexOutOfBoundsException if offset is negative
     */

	LongField(int offset, __int64 value);

    /**
     * Construct the LongField with its offset into its containing
     * byte array and initialize its value from its byte array
     *
     * @param offset of the field within its byte array
     * @param data the byte array to read the value from
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is not
     *            within the range of 0..(data.length - 1)
     */

	LongField(int offset, ByteArrayPtr data);

    /**
     * construct the LongField with its offset into its containing
     * byte array, initialize its value, and write the value to a byte
     * array
     *
     * @param offset of the field within its byte array
     * @param value the initial value
     * @param data the byte array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if offset is negative
     */

	LongField(int offset, __int64 value, ByteArrayPtr data);

    /**
     * get the LongField's current value
     *
     * @return current value
     */

	__int64 get();

    /**
     * set the LongField's current value
     *
     * @param value to be set
     */

	void set(__int64 value);

    /**
     * set the LongField's current value and write it to a byte array
     *
     * @param value to be set
     * @param data the byte array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of range
     */

	void set(__int64 value, ByteArrayPtr data);

    /**
     * set the value from its offset into an array of bytes
     *
     * @param data the byte array from which the value is to be read
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of range
     */

	void readFromBytes(ByteArrayPtr data);

    /**
     * set the value from an InputStream
     *
     * @param stream the InputStream from which the value is to be
     *               read
     *
     * @exception BufferUnderrunException if there is not enough data
     *            available from the InputStream
     * @exception IOException if an IOException is thrown from reading
     *            the InputStream
     */

	void readFromStream(BwProj::toolkit::IDataReadStream* stream);

    /**
     * write the value out to an array of bytes at the appropriate
     * offset
     *
     * @param data the array of bytes to which the value is to be
     *             written
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of range
     */

	void writeToBytes(ByteArrayPtr data);

    /**
     * return the value as a CString
     *
     * @return the value as a CString
     */

	CString toString();

private:
	__int64	_value;
	int		_offset;
};   // end class LongField