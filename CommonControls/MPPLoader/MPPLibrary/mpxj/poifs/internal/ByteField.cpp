#include "stdafx.h"
#include "ByteField.h"

/**
    * construct the ByteField with its offset into its containing
    * BYTE array and a default value of 0
    *
    * @param offset of the field within its BYTE array
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

ByteField::ByteField(int offset)
	:ByteField(offset, _default_value)
{

}

/**
    * construct the ByteField with its offset into its containing
    * BYTE array and initialize its value
    *
    * @param offset of the field within its BYTE array
    * @param value the initial value
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

ByteField::ByteField(int offset, BYTE value)
{
    if (offset < 0)
    {
        throw toolkit::Exception(L"offset cannot be negative");
    }
    _offset = offset;
    set(value);
}

/**
    * Construct the ByteField with its offset into its containing
    * BYTE array and initialize its value from its BYTE array
    *
    * @param offset of the field within its BYTE array
    * @param data the BYTE array to read the value from
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is not
    *            within the range of 0..(data.length - 1)
    */

ByteField::ByteField(int offset, ByteArrayPtr data)
	:ByteField(offset)
{
    readFromBytes(data);
}

/**
    * construct the ByteField with its offset into its containing
    * BYTE array, initialize its value, and write its value to its
    * BYTE array
    *
    * @param offset of the field within its BYTE array
    * @param value the initial value
    * @param data the BYTE array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is not
    *            within the range of 0..(data.length - 1)
    */

ByteField::ByteField(int offset, BYTE value, ByteArrayPtr data)
	:ByteField(offset, value)
{
    writeToBytes(data);
}

/**
    * get the ByteField's current value
    *
    * @return current value
    */

BYTE ByteField::get()
{
    return _value;
}

/**
    * set the ByteField's current value
    *
    * @param value to be set
    */

void ByteField::set(BYTE value)
{
    _value = value;
}

/**
    * set the ByteField's current value and write it to a BYTE array
    *
    * @param value to be set
    * @param data the BYTE array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of the BYTE array's range
    */

void ByteField::set(BYTE value, ByteArrayPtr data)
{
    set(value);
    writeToBytes(data);
}

/* ********** START implementation of FixedField ********** */

/**
    * set the value from its offset into an array of bytes
    *
    * @param data the BYTE array from which the value is to be read
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of range of the bte array
    */

void ByteField::readFromBytes(ByteArrayPtr data)
{
    _value = (*data)[ _offset ];
}

/**
    * set the value from an InputStream
    *
    * @param stream the InputStream from which the value is to be
    *               read
    *
    * @exception BufferUnderruntoolkit::Exception if there is not enough data
    *            available from the InputStream
    * @exception IOtoolkit::Exception if an IOtoolkit::Exception is thrown from reading
    *            the InputStream
    */

void ByteField::readFromStream(BwProj::toolkit::IDataReadStream* stream)
{
    // TODO - are these ~Field used / necessary
	int ib = stream->ReadByte();
    if (ib < 0) {
    	throw new toolkit::Exception(L"BufferUnderflow");
    }
    _value = (BYTE) ib;
}

/**
    * write the value out to an array of bytes at the appropriate
    * offset
    *
    * @param data the array of bytes to which the value is to be
    *             written
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of the BYTE array's range
    */

void ByteField::writeToBytes(ByteArrayPtr data)
{
    (*data)[ _offset ] = _value;
}

/**
    * return the value as a String
    *
    * @return the value as a String
    */

CString ByteField::toString()
{
	return CStringToolkit::IntToStr(_value);
}