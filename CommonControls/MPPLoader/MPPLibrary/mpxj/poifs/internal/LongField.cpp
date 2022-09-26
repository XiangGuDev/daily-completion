#include "stdafx.h"
#include "LongField.h"
#include "LittleEndian.h"

/**
    * construct the LongField with its offset into its containing
    * byte array
    *
    * @param offset of the field within its byte array
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

LongField::LongField(int offset)
{
    if (offset < 0)
    {
        throw toolkit::Exception(L"Illegal offset");
    }
    _offset = offset;
}

/**
    * construct the LongField with its offset into its containing
    * byte array and initialize its value
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

LongField::LongField(int offset, __int64 value)
:LongField(offset)
{
    set(value);
}

/**
    * Construct the LongField with its offset into its containing
    * byte array and initialize its value from its byte array
    *
    * @param offset of the field within its byte array
    * @param data the byte array to read the value from
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is not
    *            within the range of 0..(data.length - 1)
    */

LongField::LongField(int offset, ByteArrayPtr data)
:LongField(offset)
{
    readFromBytes(data);
}

/**
    * construct the LongField with its offset into its containing
    * byte array, initialize its value, and write the value to a byte
    * array
    *
    * @param offset of the field within its byte array
    * @param value the initial value
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if offset is negative
    */

LongField::LongField(int offset, __int64 value, ByteArrayPtr data)
:LongField(offset)
{
    set(value, data);
}

/**
    * get the LongField's current value
    *
    * @return current value
    */

__int64 LongField::get()
{
    return _value;
}

/**
    * set the LongField's current value
    *
    * @param value to be set
    */

void LongField::set(__int64 value)
{
    _value = value;
}

/**
    * set the LongField's current value and write it to a byte array
    *
    * @param value to be set
    * @param data the byte array to write the value to
    *
    * @exception ArrayIndexOutOfBoundstoolkit::Exception if the offset is out
    *            of range
    */

void LongField::set(__int64 value, ByteArrayPtr data)
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

void LongField::readFromBytes(ByteArrayPtr data)
{
    _value = LittleEndian::getLong(data, _offset);
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

void LongField::readFromStream(BwProj::toolkit::IDataReadStream* stream)
{
    //_value = LittleEndian::readLong(stream);
	_value = stream->ReadI64();
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

void LongField::writeToBytes(ByteArrayPtr data)
{
    LittleEndian::putLong(data, _offset, _value);
}

/**
    * return the value as a CString
    *
    * @return the value as a CString
    */

CString LongField::toString()
{
	CString str;
	str.Format(_T("%Lld"), _value);
	return str;
}