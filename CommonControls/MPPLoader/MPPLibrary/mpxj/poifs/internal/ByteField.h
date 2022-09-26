#pragma once

#include "FixedField.h"

/**
 * representation of a BYTE (8-bit) field at a fixed location within a
 * BYTE array
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class ByteField : FixedField
{
public:
    /**
     * construct the ByteField with its offset into its containing
     * BYTE array and a default value of 0
     *
     * @param offset of the field within its BYTE array
     *
     * @exception ArrayIndexOutOfBoundsException if offset is negative
     */

	ByteField(int offset);

    /**
     * construct the ByteField with its offset into its containing
     * BYTE array and initialize its value
     *
     * @param offset of the field within its BYTE array
     * @param value the initial value
     *
     * @exception ArrayIndexOutOfBoundsException if offset is negative
     */

	ByteField(int offset, BYTE value);

    /**
     * Construct the ByteField with its offset into its containing
     * BYTE array and initialize its value from its BYTE array
     *
     * @param offset of the field within its BYTE array
     * @param data the BYTE array to read the value from
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is not
     *            within the range of 0..(data.length - 1)
     */

	ByteField(int offset, ByteArrayPtr data);

    /**
     * construct the ByteField with its offset into its containing
     * BYTE array, initialize its value, and write its value to its
     * BYTE array
     *
     * @param offset of the field within its BYTE array
     * @param value the initial value
     * @param data the BYTE array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is not
     *            within the range of 0..(data.length - 1)
     */

	ByteField(int offset, BYTE value, ByteArrayPtr data);

    /**
     * get the ByteField's current value
     *
     * @return current value
     */

	BYTE get();

    /**
     * set the ByteField's current value
     *
     * @param value to be set
     */

	void set(BYTE value);

    /**
     * set the ByteField's current value and write it to a BYTE array
     *
     * @param value to be set
     * @param data the BYTE array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of the BYTE array's range
     */

	void set(BYTE value, ByteArrayPtr data);

    /* ********** START implementation of FixedField ********** */

    /**
     * set the value from its offset into an array of bytes
     *
     * @param data the BYTE array from which the value is to be read
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of range of the bte array
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
     *            of the BYTE array's range
     */

	void writeToBytes(ByteArrayPtr data);

    /**
     * return the value as a String
     *
     * @return the value as a String
     */

	CString toString();

private:
	static const BYTE	_default_value = 0;
	BYTE				_value;
	int					_offset;
    /* **********  END  implementation of FixedField ********** */
};   // end class ByteField
