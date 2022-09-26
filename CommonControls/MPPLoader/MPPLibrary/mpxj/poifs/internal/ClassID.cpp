#include "stdafx.h"
#include "ClassID.h"

/**
*  <p>Creates a {@link ClassID} and initializes its value with
*  0x00 bytes.</p>
*/
ClassID::ClassID()
{
	bytes.reset (new ByteArray2(LENGTH));
	for (int i = 0; i < LENGTH; i++)
		(*bytes)[i] = 0x00;
}

/**
    *  <p>Creates a {@link ClassID} and reads its value from a byte
    *  array.</p>
    *
    * @param src The byte array to read from.
    * @param offset The offset of the first byte to read.
    */
ClassID::ClassID(ByteArrayPtr src, int offset)
{
    read(src, offset);
}

/**
    * <p>Creates a {@link ClassID} from a human-readable representation of the Class ID in standard 
    * format <code>"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"</code>.</p>
    * 
    * @param externalForm representation of the Class ID represented by this object.
    */
ClassID::ClassID(CString externalForm)
{
	bytes.reset(new ByteArray2(LENGTH));
	ASSERT(false);
    //CString clsStr = externalForm.replaceAll("[{}-]", "");
    //for (int i=0; i<clsStr.length(); i+=2) {
    //    bytes[i/2] = (byte)Integer.parseInt(clsStr.substring(i, i+2), 16);
    //}
}

/**
    * @return The number of bytes occupied by this object in the byte
    * stream.
    */
int ClassID::length()
{
    return LENGTH;
}

/**
    * <p>Gets the bytes making out the class ID. They are returned in
    * correct order, i.e. big-endian.</p>
    *
    * @return the bytes making out the class ID.
    */
ByteArrayPtr ClassID::getBytes()
{
    return bytes;
}



/**
    * <p>Sets the bytes making out the class ID.</p>
    *
    * @param bytes The bytes making out the class ID in big-endian format. They
    * are copied without their order being changed.
    */
void ClassID::setBytes(ByteArrayPtr bytes)
{
    for (size_t i = 0; i < this->bytes->length(); i++)
        (*this->bytes)[i] = (*bytes)[i];
}



/**
    * <p>Reads the class ID's value from a byte array by turning
    * little-endian into big-endian.</p>
    *
    * @param src The byte array to read from
    *
    * @param offset The offset within the <var>src</var> byte array
    *
    * @return A byte array containing the class ID.
    */
ByteArrayPtr ClassID::read(ByteArrayPtr src, int offset)
{
	bytes.reset(new ByteArray2(LENGTH));

    /* Read double word. */
    (*bytes)[0] = (*src)[3 + offset];
	(*bytes)[1] = (*src)[2 + offset];
	(*bytes)[2] = (*src)[1 + offset];
	(*bytes)[3] = (*src)[0 + offset];

    /* Read first word. */
	(*bytes)[4] = (*src)[5 + offset];
	(*bytes)[5] = (*src)[4 + offset];

    /* Read second word. */
	(*bytes)[6] = (*src)[7 + offset];
	(*bytes)[7] = (*src)[6 + offset];

    /* Read 8 bytes. */
    for (int i = 8; i < 16; i++)
		(*bytes)[i] = (*src)[i + offset];

    return bytes;
}



/**
    * <p>Writes the class ID to a byte array in the
    * little-endian format.</p>
    *
    * @param dst The byte array to write to.
    *
    * @param offset The offset within the <var>dst</var> byte array.
    *
    * @exception ArrayStoretoolkit::Exception if there is not enough room for the class
    * ID 16 bytes in the byte array after the <var>offset</var> position.
    */
void ClassID::write(ByteArrayPtr dst, int offset)
{
    /* Check array size: */
    if (dst->length() < 16)
        throw toolkit::Exception(L"Destination ByteArrayPtr must have room for at least 16 bytes");

    /* Write double word. */
	(*dst)[0 + offset] = (*bytes)[3];
	(*dst)[1 + offset] = (*bytes)[2];
	(*dst)[2 + offset] = (*bytes)[1];
	(*dst)[3 + offset] = (*bytes)[0];

    /* Write first word. */
	(*dst)[4 + offset] = (*bytes)[5];
	(*dst)[5 + offset] = (*bytes)[4];

    /* Write second word. */
	(*dst)[6 + offset] = (*bytes)[7];
	(*dst)[7 + offset] = (*bytes)[6];

    /* Write 8 bytes. */
    for (int i = 8; i < 16; i++)
		(*dst)[i + offset] = (*bytes)[i];
}