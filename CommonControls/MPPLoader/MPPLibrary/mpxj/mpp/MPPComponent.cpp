#include "stdafx.h"
#include "MPPComponent.h"
#include "MPPUtility.h"

/**
* Constructor.
*/
MPPComponent::MPPComponent()
{
    // protected constructor to prevent instantiation
}

/**
* This method reads a single byte from the input stream.
*
* @param is the input stream
* @return byte value
* @throws IOException on file read error or EOF
*/
int MPPComponent::readByte(BwProj::toolkit::IDataReadStream* is)
{
	BYTE value = 0; DWORD readBytes = 0;
	if (!is->Read(&value, sizeof(BYTE), readBytes))
	{
		throw std::bad_exception();
	}

	return value;
}

/**
* This method reads a two byte integer from the input stream.
*
* @param is the input stream
* @return integer value
* @throws IOException on file read error or EOF
*/
int MPPComponent::readShort(BwProj::toolkit::IDataReadStream* is)
{
	ByteArray2 data(2);
	DWORD readBytes = 0;
	if (!is->Read(data.get(), sizeof(short), readBytes))
	{
		throw std::bad_exception();
	}

	return MPPUtility::getShort(&data);
}

/**
* This method reads a four byte integer from the input stream.
*
* @param is the input stream
* @return byte value
* @throws IOException on file read error or EOF
*/
long MPPComponent::readInt(BwProj::toolkit::IDataReadStream* is)
{
	ByteArray2 data(4);
	DWORD readBytes = 0;
	if (!is->Read(data.get(), sizeof(long), readBytes))
	{
		throw std::bad_exception();
	}

	return MPPUtility::getInt(&data);
}

/**
* This method reads a byte array from the input stream.
*
* @param is the input stream
* @param size number of bytes to read
* @return byte array
* @throws IOException on file read error or EOF
*/
ByteArrayPtr MPPComponent::readByteArray2(BwProj::toolkit::IDataReadStream* is, int size)
{
	ByteArrayPtr value(new ByteArray2(size)); DWORD readBytes = 0;
	if (!is->Read(value->get(), size, readBytes))
	{
		throw std::bad_exception();
	}

	return value;

    //ByteArrayPtr buffer = new byte[size];
    //if (is.read(buffer) != buffer.length)
    //{
    //    throw new EOFException();
    //}
    //return (buffer);
}