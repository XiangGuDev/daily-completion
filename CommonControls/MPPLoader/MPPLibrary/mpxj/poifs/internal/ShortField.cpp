#include "stdafx.h"
#include "ShortField.h"


/**
    * construct the ShortField with its offset into its containing
    * byte array
    *
    * @param offset of the field within its byte array
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

ShortField::ShortField(int offset)
{
    if (offset < 0)
    {
        throw toolkit::Exception(L"Illegal offset");
    }
    _offset = offset;
}

/**
    * construct the ShortField with its offset into its containing
    * byte array and initialize its value
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

ShortField::ShortField(int offset, short value)
	:ShortField(offset)
{
    set(value);
}

/**
    * Construct the ShortField with its offset into its containing
    * byte array and initialize its value from its byte array
    *
    * @param offset of the field within its byte array
    * @param data the byte array to read the value from
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is not
    *            within the range of 0..(data.length - 1)
    */

ShortField::ShortField(int offset, ByteArrayPtr data)
	:ShortField(offset)
{
    readFromBytes(data);
}

/**
    * construct the ShortField with its offset into its containing
    * byte array, initialize its value, and write its value to its
    * byte array
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

ShortField::ShortField(int offset, short value, ByteArrayPtr data)
	:ShortField(offset)
{
    set(value, data);
}

/**
    * get the ShortField's current value
    *
    * @return current value
    */

short ShortField::get()
{
    return _value;
}

/**
    * set the ShortField's current value
    *
    * @param value to be set
    */

void ShortField::set(short value)
{
    _value = value;
}

/**
    * set the ShortField's current value and write it to a byte array
    *
    * @param value to be set
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of range
    */

void ShortField::set(short value, ByteArrayPtr data)
{
    _value = value;
    writeToBytes(data);
}

/* ********** START implementation of FixedField ********** */

/**
    * set the value from its offset into an array of bytes
    *
    * @param data the byte array from which the value is to be read
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of range
    */

void ShortField::readFromBytes(ByteArrayPtr data)
{
	_value = *(short*)(&(*data)[_offset]);
    //_value = LittleEndian.getShort(data, _offset);
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

void ShortField::readFromStream(BwProj::toolkit::IDataReadStream* stream)
{
	DWORD readBytes = 0;
	stream->Read(&_value, sizeof(short), readBytes);
    //_value = LittleEndian.readShort(stream);
}

/**
    * write the value out to an array of bytes at the appropriate
    * offset
    *
    * @param data the array of bytes to which the value is to be
    *             written
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of range
    */

void ShortField::writeToBytes(ByteArrayPtr data)
{
	*(short*)(&(*data)[_offset]) = _value;
	//LittleEndian.putShort(data, _offset, _value);
}

/**
    * return the value as a String
    *
    * @return the value as a String
    */

CString ShortField::toString()
{
    return CStringToolkit::IntToStr(_value);
}