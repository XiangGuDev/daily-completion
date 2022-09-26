#include "stdafx.h"
#include "Props.h"
#include "MPPUtility.h"

/**
* Retrieve property data as a BYTE array.
*
* @param type Type identifier
* @return  BYTE array of data
*/
ByteArrayPtr Props::getByteArray2(int type)
{
	ByteArrayPtr result;
	m_map.Lookup(type, result);
	return result;
}

/**
* Retrieves a BYTE value from the property data.
*
* @param type Type identifier
* @return BYTE value
*/
BYTE Props::getByte(int type)
{
	BYTE result = 0;

	ByteArrayPtr item = getByteArray2(type);
	if (item && item->length() > 0)
	{
		result = (*item)[0];
	}

	return (result);
}

/**
* Retrieves a short int value from the property data.
*
* @param type Type identifier
* @return short int value
*/
int Props::getShort(int type)
{
	int result = 0;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getShort(item.get());
	}

	return (result);
}

/**
* Retrieves an integer value from the property data.
*
* @param type Type identifier
* @return integer value
*/
int Props::getInt(int type)
{
	int result = 0;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getInt(item.get());
	}

	return (result);
}

/**
* Retrieves a double value from the property data.
*
* @param type Type identifier
* @return double value
*/
double Props::getDouble(int type)
{
	double result = 0;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getDouble(item.get());
	}

	return (result);
}

/**
* Retrieves a timestamp from the property data.
*
* @param type Type identifier
* @return timestamp
*/
DatePtr Props::getTime(int type)
{
	DatePtr result;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getTime(item.get());
	}

	return (result);
}

/**
* Retrieves a timestamp from the property data.
*
* @param type Type identifier
* @return timestamp
*/
DatePtr Props::getTimestamp(int type)
{
	DatePtr result;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getTimestamp(item.get());
	}

	return (result);
}

/**
* Retrieves a bool value from the property data.
*
* @param type Type identifier
* @return bool value
*/
bool Props::getBoolean(int type)
{
	bool result = false;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = !(MPPUtility::getShort(item.get()) == 0);
	}

	return (result);
}

/**
* Retrieves a string value from the property data.
*
* @param type Type identifier
* @return string value
*/
CString Props::getUnicodeString(int type)
{
	CString result;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getUnicodeString(item.get());
	}

	return (result);
}

/**
* Retrieves a date value from the property data.
*
* @param type Type identifier
* @return string value
*/
DatePtr Props::getDate(int type)
{
	DatePtr result;

	ByteArrayPtr item = getByteArray2(type);
	if (item)
	{
		result = MPPUtility::getDate(item.get(), 0);
	}

	return (result);
}

/**
* Retrieve the set of keys represented by this instance.
*
* @return key set
*/
std::shared_ptr<Set<int>> Props::keySet()
{
	return (m_map.keySet());
}

///**
//* This method dumps the contents of this properties block as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this block
//*/
//CString Props::toString()
//{
//	StringWriter sw = new StringWriter();
//	PrintWriter pw = new PrintWriter(sw);
//
//	pw.println("BEGIN Props");
//
//	for (int key : m_map.keySet())
//	{
//		pw.println("   Key: " + key + " Value: ");
//		pw.println(MPPUtility::hexdump(m_map.get(key), true, 16, "      "));
//	}
//
//	pw.println("END Props");
//
//	pw.println();
//	pw.close();
//	return (sw.toString());
//}