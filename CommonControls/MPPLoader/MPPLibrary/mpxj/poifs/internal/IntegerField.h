#pragma once

#include "FixedField.h"

/**
 * representation of an integer (32-bit) field at a fixed location
 * within a byte array
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class IntegerField : public FixedField
{
public:
    /**
     * construct the IntegerField with its offset into its containing
     * byte array
     *
     * @param offset of the field within its byte array
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is
     *            negative
     */

	IntegerField(int offset);

    /**
     * construct the IntegerField with its offset into its containing
     * byte array and initialize its value
     *
     * @param offset of the field within its byte array
     * @param value the initial value
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is
     *            negative
     */

	IntegerField(int offset, __int32 value);

    /**
     * Construct the IntegerField with its offset into its containing
     * byte array and initialize its value from its byte array
     *
     * @param offset of the field within its byte array
     * @param data the byte array to read the value from
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is not
     *            within the range of 0..(data.length - 1)
     */

	IntegerField(int offset, ByteArrayPtr data);

    /**
     * construct the IntegerField with its offset into its containing
     * byte array, initialize its value, and write the value to a byte
     * array
     *
     * @param offset of the field within its byte array
     * @param value the initial value
     * @param data the byte array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is
     *            negative or too large
     */

	IntegerField(int offset, __int32 value, ByteArrayPtr data);

    /**
     * get the IntegerField's current value
     *
     * @return current value
     */

	__int32 get();

    /**
     * set the IntegerField's current value
     *
     * @param value to be set
     */

	void set(__int32 value);

    /**
     * set the IntegerField's current value and write it to a byte
     * array
     *
     * @param value to be set
     * @param data the byte array to write the value to
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is too
     *            large
     */

	void set(__int32 value, ByteArrayPtr data);

    /* ********** START implementation of FixedField ********** */

    /**
     * set the value from its offset into an array of bytes
     *
     * @param data the byte array from which the value is to be read
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is too
     *            large
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
     * @exception ArrayIndexOutOfBoundsException if the offset is too
     *            large
     */

	void writeToBytes(ByteArrayPtr data);

    /**
     * return the value as a String
     *
     * @return the value as a String
     */

	CString toString();

private:
	__int32	_value;
	int _offset;

    /* **********  END  implementation of FixedField ********** */
};   // end class IntegerField