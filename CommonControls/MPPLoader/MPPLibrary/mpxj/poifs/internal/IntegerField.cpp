#include "stdafx.h"
#include "IntegerField.h"
#include "LittleEndian.h"

/**
    * construct the IntegerField with its offset into its containing
    * byte array
    *
    * @param offset of the field within its byte array
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is
    *            negative
    */

IntegerField::IntegerField(int offset)
{
    if (offset < 0)
    {
        throw toolkit::Exception(L"negative offset");
    }
    _offset = offset;
}

/**
    * construct the IntegerField with its offset into its containing
    * byte array and initialize its value
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is
    *            negative
    */

IntegerField::IntegerField(int offset, __int32 value)
	:IntegerField(offset)
{
    set(value);
}

/**
    * Construct the IntegerField with its offset into its containing
    * byte array and initialize its value from its byte array
    *
    * @param offset of the field within its byte array
    * @param data the byte array to read the value from
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is not
    *            within the range of 0..(data.length - 1)
    */

IntegerField::IntegerField(int offset, ByteArrayPtr data)
	:IntegerField(offset)
{
    readFromBytes(data);
}

/**
    * construct the IntegerField with its offset into its containing
    * byte array, initialize its value, and write the value to a byte
    * array
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is
    *            negative or too large
    */

IntegerField::IntegerField(int offset, __int32 value, ByteArrayPtr data)
	:IntegerField(offset)
{
    set(value, data);
}

/**
    * get the IntegerField's current value
    *
    * @return current value
    */

__int32 IntegerField::get()
{
    return _value;
}

/**
    * set the IntegerField's current value
    *
    * @param value to be set
    */

void IntegerField::set(__int32 value)
{
    _value = value;
}

/**
    * set the IntegerField's current value and write it to a byte
    * array
    *
    * @param value to be set
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is too
    *            large
    */

void IntegerField::set(__int32 value, ByteArrayPtr data)
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
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is too
    *            large
    */

void IntegerField::readFromBytes(ByteArrayPtr data)
{
	_value = LittleEndian::getInt(data, _offset);
	// 4 byte
	//_value = *(long*)(&(*data)[_offset]);
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

void IntegerField::readFromStream(BwProj::toolkit::IDataReadStream* stream)
{
	_value = stream->ReadLong();
    //_value = LittleEndian::readInt(stream);
}

/**
    * write the value out to an array of bytes at the appropriate
    * offset
    *
    * @param data the array of bytes to which the value is to be
    *             written
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is too
    *            large
    */

void IntegerField::writeToBytes(ByteArrayPtr data)
{
	//*(long*)(&(*data)[_offset]) = _value;
    LittleEndian::putInt(data, _offset, _value);
}

/**
    * return the value as a String
    *
    * @return the value as a String
    */

CString IntegerField::toString()
{
	return CStringToolkit::IntToStr(_value);
}