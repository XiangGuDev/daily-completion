#pragma once

/**
 * behavior of a field at a fixed location within a byte array
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class FixedField
{

    /**
     * set the value from its offset into an array of bytes
     *
     * @param data the byte array from which the value is to be read
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of the array's valid index range
     */

	virtual void readFromBytes(ByteArrayPtr data) = 0;

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

	virtual void readFromStream(BwProj::toolkit::IDataReadStream* stream) = 0;

    /**
     * write the value out to an array of bytes at the appropriate
     * offset
     *
     * @param data the array of bytes to which the value is to be
     *             written
     *
     * @exception ArrayIndexOutOfBoundsException if the offset is out
     *            of the array's valid index range
     */

	virtual void writeToBytes(ByteArrayPtr data) = 0;

    /**
     * return the value as a String
     *
     * @return the value as a String
     */

    virtual CString toString() = 0;
};   // end interface FixedField