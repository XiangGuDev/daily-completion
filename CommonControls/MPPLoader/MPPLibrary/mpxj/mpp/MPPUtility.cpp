#include "stdafx.h"
#include "MPPUtility.h"
#include "Var2Data.h"
#include "..\ProjectFile.h"
#include "..\common\DateHelper.h"
#include "..\common\NumberHelper.h"
#include "..\java\StringBuilder.h"

const BYTE MPPUtility::PASSWORD_MASK[] =
{
	60, 30, 48, 2, 6, 14, 8, 22,
	44, 12, 38, 10, 62, 16, 34, 24
};

const char MPPUtility::HEX_DIGITS[] =
{
	'0', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', 'A', 'B', 'C', 'D',
	'E', 'F'
};

const DatePtr MPPUtility::EPOCH_DATE = DateHelper::getTimestampFromLong(EPOCH); //DateHelper::getFIRST_DATE();// getTimestampFromLong(EPOCH);

/**
* Private constructor to prevent instantiation.
*/
MPPUtility::MPPUtility()
{
    // constructor to prevent instantiation
}

// 从数组中读字节转换成字符串（单字节）
CString MPPUtility::ByteArray2String(ByteArray2* data, int start, int length)
{
	char* text = new char[length + 1];
	memcpy_s(text, length + 1, &(data->get()[start]), length);
	text[length] = NULL;
	CString ret(text);
	delete[] text;
	return ret;
}

/**
* This method decodes a BYTE array with the given encryption code
* using XOR encryption.
* 
* @param data Source data
* @param encryptionCode Encryption code
*/
void MPPUtility::decodeBuffer(ByteArray2* data, BYTE encryptionCode)
{
    for (size_t i = 0; i < data->length(); i++)
    {
		(*data)[i] = (BYTE)((*data)[i] ^ encryptionCode);
    }
}

/**
* Decode the password from the given data. Will decode the data block as well. 
* 
* @param data encrypted data block
* @param encryptionCode encryption code
* 
* @return password
*/
CString MPPUtility::decodePassword(ByteArray2* data, BYTE encryptionCode)
{
    CString result;

    if (data->length() < MINIMUM_PASSWORD_DATA_LENGTH)
    {
		return result;
    }

    MPPUtility::decodeBuffer(data, encryptionCode);

	StringBuilder buffer;// = new StringBuilder();
    char c;

    for (int i = 0; i < sizeof(PASSWORD_MASK); i++)
    {
		int index = PASSWORD_MASK[i];
		c = (char) (*data)[index];

		if (c == 0)
		{
			break;
		}
		buffer.append(c);
    }

    result = buffer.toString();
    return (result);
}

/**
* This method extracts a portion of a BYTE array and writes it into
* another BYTE array.
*
* @param data Source data
* @param offset Offset into source data
* @param size Required size to be extracted from the source data
* @param buffer Destination buffer
* @param bufferOffset Offset into destination buffer
*/
void MPPUtility::getByteArray2(ByteArray2* data, int offset, int size, ByteArray2* buffer, int bufferOffset)
{
	BYTE* src = data->get();
	BYTE* desc = buffer->get();

	memcpy_s(&desc[bufferOffset], size, &src[offset], size);

    //System.arraycopy(data, offset, buffer, bufferOffset, size);
}

/**
* This method reads a single BYTE from the input array.
*
* @param data BYTE array of data
* @param offset offset of BYTE data in the array
* @return BYTE value
*/
int MPPUtility::getByte(ByteArray2* data, int offset)
{
    int result = (*data)[offset] & 0xFF;
    return result;
}

/**
* This method reads a single BYTE from the input array.
* The BYTE is assumed to be at the start of the array.
*
* @param data BYTE array of data
* @return BYTE value
*/
int MPPUtility::getByte(ByteArray2* data)
{
    return (getByte(data, 0));
}

/**
* This method reads a two BYTE integer from the input array.
*
* @param data the input array
* @param offset offset of integer data in the array
* @return integer value
*/
int MPPUtility::getShort(ByteArray2* data, int offset)
{
    int result = 0;
    int i = offset;
    for (int shiftBy = 0; shiftBy < 16; shiftBy += 8)
    {
        result |= ((*data)[i] & 0xff) << shiftBy;
        ++i;
    }
    return result;
}

/**
* This method reads a two BYTE integer from the input array.
* The integer is assumed to be at the start of the array.
*
* @param data the input array
* @return integer value
*/
int MPPUtility::getShort(ByteArray2* data)
{
    return (getShort(data, 0));
}

/**
* This method reads a four BYTE integer from the input array.
*
* @param data the input array
* @param offset offset of integer data in the array
* @return integer value
*/
int MPPUtility::getInt(ByteArray2* data, int offset)
{
    int result = 0;
    int i = offset;
    for (int shiftBy = 0; shiftBy < 32; shiftBy += 8)
    {
        result |= ((*data)[i] & 0xff) << shiftBy;
        ++i;
    }
    return result;
}

/**
* This method reads a four BYTE integer from the input array.
* The integer is assumed to be at the start of the array.
*
* @param data the input array
* @return integer value
*/
int MPPUtility::getInt(ByteArray2* data)
{
    return (getInt(data, 0));
}

/**
* This method reads an eight BYTE integer from the input array.
*
* @param data the input array
* @param offset offset of integer data in the array
* @return integer value
*/
__int64 MPPUtility::getLong(ByteArray2* data, int offset)
{
	__int64 result = 0;
    int i = offset;
    for (int shiftBy = 0; shiftBy < 64; shiftBy += 8)
    {
		result |= ((__int64)(*data)[i] & 0xff) << shiftBy;
        ++i;
    }
    return result;
}

/**
* This method reads a six BYTE long from the input array.
*
* @param data the input array
* @param offset offset of integer data in the array
* @return integer value
*/
long MPPUtility::getLong6(ByteArray2* data, int offset)
{
    long result = 0;
    int i = offset;
    for (int shiftBy = 0; shiftBy < 48; shiftBy += 8)
    {
        result |= ((long) ((*data)[i] & 0xff) << shiftBy);
        ++i;
    }
    return result;
}

/**
* This method reads a six BYTE long from the input array.
* The integer is assumed to be at the start of the array.
*
* @param data the input array
* @return integer value
*/
long MPPUtility::getLong6(ByteArray2* data)
{
    return (getLong6(data, 0));
}

/**
* This method reads a eight BYTE integer from the input array.
* The integer is assumed to be at the start of the array.
*
* @param data the input array
* @return integer value
*/
__int64 MPPUtility::getLong(ByteArray2* data)
{
    return (getLong(data, 0));
}

/**
* This method reads an eight BYTE double from the input array.
*
* @param data the input array
* @param offset offset of double data in the array
* @return double value
*/
double MPPUtility::getDouble(ByteArray2* data, int offset)
{
	__int64 value = getLong(data, offset);

	return NumberHelper::longBitsToDouble(value);
	//return *((double*)&value);
	//return (Double.longBitsToDouble());
}

/**
* This method reads an eight BYTE double from the input array.
* The double is assumed to be at the start of the array.
*
* @param data the input array
* @return double value
*/
double MPPUtility::getDouble(ByteArray2* data)
{
	__int64 value = getLong(data, 0);
	return NumberHelper::longBitsToDouble(value);
	//return *((double*)&value);
    //return (Double.longBitsToDouble(getLong(data, 0)));
}

/**
* Reads a UUID/GUID from a data block.
* 
* @param data data block
* @param offset offset into the data block
* @return UUID instance
*/
GUID MPPUtility::getGUID(ByteArray2* data, int offset)
{
	GUID result = { 0 };
    if (data && data->length() > 15)
    {
        __int64 long1 = 0;
		long1 |= ((__int64)((*data)[offset + 3])) << 56;
		long1 |= ((__int64)((*data)[offset + 2])) << 48;
		long1 |= ((__int64)((*data)[offset + 1])) << 40;
		long1 |= ((__int64)((*data)[offset + 0])) << 32;
		long1 |= ((__int64)((*data)[offset + 5])) << 24;
		long1 |= ((__int64)((*data)[offset + 4])) << 16;
		long1 |= ((__int64)((*data)[offset + 7])) << 8;
		long1 |= ((__int64)((*data)[offset + 6])) << 0;

		__int64 long2 = 0;
		long2 |= ((__int64)((*data)[offset + 8])) << 56;
		long2 |= ((__int64)((*data)[offset + 9])) << 48;
		long2 |= ((__int64)((*data)[offset + 10])) << 40;
		long2 |= ((__int64)((*data)[offset + 11])) << 32;
		long2 |= ((__int64)((*data)[offset + 12])) << 24;
		long2 |= ((__int64)((*data)[offset + 13])) << 16;
		long2 |= ((__int64)((*data)[offset + 14])) << 8;
		long2 |= ((__int64)((*data)[offset + 15])) << 0;

		*((__int64*)&result.Data1) = long1;
		*((__int64*)result.Data4) = long2;
		//result = new GUID(long1, long2);
    }
    return result;
}

/**
* Reads a date value. Note that an NA is represented as 65535 in the
* MPP file. We represent this in Java using a NULL value. The actual
* value in the MPP file is number of days since 31/12/1983.
*
* @param data BYTE array of data
* @param offset location of data as offset into the array
* @return date value
*/
DatePtr MPPUtility::getDate(ByteArray2* data, int offset)
{
    DatePtr result;
    long days = getShort(data, offset);

    if (days != 65535)
    {
        result = DateHelper::getDateFromLong(EPOCH + (days * MS_PER_DAY));
    }

    return (result);
}

/**
* Reads a time value. The time is represented as tenths of a
* minute since midnight.
*
* @param data BYTE array of data
* @param offset location of data as offset into the array
* @return time value
*/
DatePtr MPPUtility::getTime(ByteArray2* data, int offset)
{
    int time = getShort(data, offset) / 10;
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(EPOCH_DATE);
    cal->set(Calendar::HOUR_OF_DAY, (time / 60));
    cal->set(Calendar::MINUTE, (time % 60));
    cal->set(Calendar::SECOND, 0);
    //cal->set(Calendar::MILLISECOND, 0);
    return (cal->getTime());
}

/**
* Reads a time value. The time is represented as tenths of a
* minute since midnight.
*
* @param data BYTE array of data
* @return time value
*/
DatePtr MPPUtility::getTime(ByteArray2* data)
{
    return getTime(data, 0);
}

/**
* Reads a duration value in milliseconds. The time is represented as
* tenths of a minute since midnight.
*
* @param data BYTE array of data
* @param offset location of data as offset into the array
* @return duration value
*/
long MPPUtility::getDuration(ByteArray2* data, int offset)
{
    return ((getShort(data, offset) * MS_PER_MINUTE) / 10);
}

/**
* Reads a combined date and time value.
*
* @param data BYTE array of data
* @param offset location of data as offset into the array
* @return time value
*/
DatePtr MPPUtility::getTimestamp(ByteArray2* data, int offset)
{
    DatePtr result;

    long days = getShort(data, offset + 2);

    if (!(days == 0 || days == 65535))
    {
        long time = getShort(data, offset);
        if (time == 65535)
        {
			time = 0;
        }
		__int64 time64v1 = days * MS_PER_DAY;
		__int64 time64v2 = (time * MS_PER_MINUTE) / 10;
		__int64 time64v3 = EPOCH + time64v1 + time64v2;
		result = DateHelper::getTimestampFromLong(time64v3);
    }

    return (result);
}

/**
* Reads a combined date and time value expressed in tenths of a minute.
*
* @param data BYTE array of data
* @param offset location of data as offset into the array
* @return time value
*/
DatePtr MPPUtility::getTimestampFromTenths(ByteArray2* data, int offset)
{
    long ms = ((long) getInt(data, offset)) * 6000;
    return DateHelper::getTimestampFromLong(EPOCH + ms);
}

/**
* Reads a combined date and time value.
* The value is assumed to be at the start of the array.
*
* @param data BYTE array of data
* @return time value
*/
DatePtr MPPUtility::getTimestamp(ByteArray2* data)
{
    return getTimestamp(data, 0);
}

/**
* Reads a string of two BYTE characters from the input array.
* This method assumes that the string finishes either at the
* end of the array, or when char zero is encountered.
* The value is assumed to be at the start of the array.
*
* @param data BYTE array of data
* @return string value
*/
CString MPPUtility::getUnicodeString(ByteArray2* data)
{
    return getUnicodeString(data, 0);
}

/**
* Reads a string of two BYTE characters from the input array.
* This method assumes that the string finishes either at the
* end of the array, or when char zero is encountered.
* The value starts at the position specified by the offset
* parameter.
*
* @param data BYTE array of data
* @param offset start point of unicode string
* @return string value
*/
CString MPPUtility::getUnicodeString(ByteArray2* data, int offset)
{
	if (!data) return L"";

	Array2<TCHAR> text(data->length() / 2 + 1);
	memcpy_s(text.get(), text.length() * sizeof(TCHAR), data->get(), data->length());
	text[text.length()-1] = 0;

	return text.get();

	//StringBuilder buffer;// = new StringBuilder();
 //   for (size_t loop = offset; loop < (data->length() - 1); loop += 2)
 //   {
	//	TCHAR c = (TCHAR)getShort(data, loop);

	//	if (c == 0)
	//	{
	//		break;
	//	}

	//	buffer.append(c);
 //   }

 //   return buffer.toString();
}

/**
* Reads a string of two BYTE characters from the input array.
* This method assumes that the string finishes either at the
* end of the array, or when char zero is encountered, or
* when a string of a certain length in bytes has been read.
* The value starts at the position specified by the offset
* parameter.
*
* @param data BYTE array of data
* @param offset start point of unicode string
* @param length length in bytes of the string
* @return string value
*/
CString MPPUtility::getUnicodeString(ByteArray2* data, int offset, int length)
{
	StringBuilder buffer;// = new StringBuilder();
    char c;
	size_t loop = offset;
    int byteLength = 0;

    while (loop < (data->length() - 1) && byteLength < length)
    {
        c = (char) getShort(data, loop);

        if (c == 0)
        {
			break;
        }

        buffer.append(c);

        loop += 2;
        byteLength += 2;
    }

    return buffer.toString();
}

/**
* Reads a string of single BYTE characters from the input array.
* This method assumes that the string finishes either at the
* end of the array, or when char zero is encountered.
* The value is assumed to be at the start of the array.
*
* @param data BYTE array of data
* @return string value
*/
CString MPPUtility::getString(ByteArray2* data)
{
    return (getString(data, 0));
}

/**
* Reads a string of single BYTE characters from the input array.
* This method assumes that the string finishes either at the
* end of the array, or when char zero is encountered.
* Reading begins at the supplied offset into the array.
*
* @param data BYTE array of data
* @param offset offset into the array
* @return string value
*/
CString MPPUtility::getString(ByteArray2* data, int offset)
{
	StringBuilder buffer;// = new StringBuilder();

	for (size_t loop = 0; offset + loop < data->length(); loop++)
    {
		TCHAR c = (TCHAR)(*data)[offset + loop];

        if (c == 0)
        {
			break;
        }

        buffer.append(c);
    }

    return buffer.toString();
}

/**
* Reads a duration value. This method relies on the fact that
* the units of the duration have been specified elsewhere.
*
* @param value DurationPtr value
* @param type type of units of the duration
* @return DurationPtr instance
*/
DurationPtr MPPUtility::getDuration(int value, TimeUnit type)
{
    return getDuration((double) value, type);
}

/**
* Reads a color value represented by three bytes, for R, G, and B
* components, plus a flag BYTE indicating if this is an automatic color. 
* Returns NULL if the color type is "Automatic".
* 
* @param data BYTE array of data
* @param offset offset into array
* @return new Color instance
*/
COLORREF MPPUtility::getColor(ByteArray2* data, int offset)
{
	COLORREF result = NULL;

    if (getByte(data, offset + 3) == 0)
    {
        int r = getByte(data, offset);
        int g = getByte(data, offset + 1);
        int b = getByte(data, offset + 2);
        result = RGB(r, g, b);
    }

    return result;
}

/**
* Reads a duration value. This method relies on the fact that
* the units of the duration have been specified elsewhere.
*
* @param value DurationPtr value
* @param type type of units of the duration
* @return DurationPtr instance
*/
DurationPtr MPPUtility::getDuration(double value, TimeUnit type)
{
    double duration;
    // Value is given in 1/10 of minute
    switch (type)
    {
        case MINUTES:
        case ELAPSED_MINUTES:
        {
        duration = value / 10;
        break;
        }

        case HOURS:
        case ELAPSED_HOURS:
        {
        duration = value / 600; // 60 * 10
        break;
        }

        case DAYS:
        {
        duration = value / 4800; // 8 * 60 * 10
        break;
        }

        case ELAPSED_DAYS:
        {
        duration = value / 14400; // 24 * 60 * 10
        break;
        }

        case WEEKS:
        {
        duration = value / 24000; // 5 * 8 * 60 * 10
        break;
        }

        case ELAPSED_WEEKS:
        {
        duration = value / 100800; // 7 * 24 * 60 * 10
        break;
        }

        case MONTHS:
        {
        duration = value / 96000; // 
        break;
        }

        case ELAPSED_MONTHS:
        {
        duration = value / 432000; // 30 * 24 * 60 * 10
        break;
        }

        default:
        {
        duration = value;
        break;
        }
    }

    return Duration2::getInstance(duration, type);
}

/**
* This method converts between the duration units representation
* used in the MPP file, and the standard MPX duration units.
* If the supplied units are unrecognised, the units default to days.
*
* @param type MPP units
* @return MPX units
*/
TimeUnit MPPUtility::getDurationTimeUnits(int type)
{
	return getDurationTimeUnits(type, (TimeUnit)NULL);
}

/**
* This method converts between the duration units representation
* used in the MPP file, and the standard MPX duration units.
* If the supplied units are unrecognised, the units default to days.
*
* @param type MPP units
* @param projectDefaultDurationUnits default duration units for this project
* @return MPX units
*/
TimeUnit MPPUtility::getDurationTimeUnits(int type, TimeUnit projectDefaultDurationUnits)
{
    TimeUnit units;

    switch (type & DURATION_UNITS_MASK)
    {
        case 3:
        {
        units = TimeUnit::MINUTES;
        break;
        }

        case 4:
        {
        units = TimeUnit::ELAPSED_MINUTES;
        break;
        }

        case 5:
        {
        units = TimeUnit::HOURS;
        break;
        }

        case 6:
        {
        units = TimeUnit::ELAPSED_HOURS;
        break;
        }

        case 8:
        {
        units = TimeUnit::ELAPSED_DAYS;
        break;
        }

        case 9:
        {
        units = TimeUnit::WEEKS;
        break;
        }

        case 10:
        {
        units = TimeUnit::ELAPSED_WEEKS;
        break;
        }

        case 11:
        {
        units = TimeUnit::MONTHS;
        break;
        }

        case 12:
        {
        units = TimeUnit::ELAPSED_MONTHS;
        break;
        }

        case 19:
        {
        units = TimeUnit::PERCENT;
        break;
        }

        case 7:
        {
        units = TimeUnit::DAYS;
        break;
        }

        case 21:
        {
        units = projectDefaultDurationUnits == NULL ? TimeUnit::DAYS : projectDefaultDurationUnits;
        break;
        }

        default:
        {
        units = TimeUnit::DAYS;
        break;
        }
    }

    return (units);
}

/**
* Given a duration and the time units for the duration extracted from an MPP
* file, this method creates a new DurationPtr to represent the given
* duration. This instance has been adjusted to take into account the
* number of "hours per day" specified for the current project.
*
* @param file parent file
* @param duration duration length
* @param timeUnit duration units
* @return DurationPtr instance
*/
DurationPtr MPPUtility::getAdjustedDuration(ProjectFile* file, int duration, TimeUnit timeUnit)
{
    DurationPtr result;

    if (duration != -1)
    {
        switch (timeUnit)
        {
        case DAYS:
        {
			double unitsPerDay = file->getProjectHeader()->getMinutesPerDay()->doubleValue() * 10;// d;
            double totalDays = 0;
            if (unitsPerDay != 0)
            {
                totalDays = duration / unitsPerDay;
            }
            result = Duration2::getInstance(totalDays, timeUnit);
            break;
        }

        case ELAPSED_DAYS:
        {
            double unitsPerDay = 24 * 600;
            double totalDays = duration / unitsPerDay;
            result = Duration2::getInstance(totalDays, timeUnit);
            break;
        }

        case WEEKS:
        {
            double unitsPerWeek = file->getProjectHeader()->getMinutesPerWeek()->doubleValue() * 10;
            double totalWeeks = 0;
            if (unitsPerWeek != 0)
            {
                totalWeeks = duration / unitsPerWeek;
            }
            result = Duration2::getInstance(totalWeeks, timeUnit);
            break;
        }

        case ELAPSED_WEEKS:
        {
            double unitsPerWeek = (60 * 24 * 7 * 10);
            double totalWeeks = duration / unitsPerWeek;
            result = Duration2::getInstance(totalWeeks, timeUnit);
            break;
        }

        case MONTHS:
        {
			double unitsPerMonth = file->getProjectHeader()->getMinutesPerDay()->doubleValue() *
				file->getProjectHeader()->getDaysPerMonth()->doubleValue() * 10;// d;
            double totalMonths = 0;
            if (unitsPerMonth != 0)
            {
                totalMonths = duration / unitsPerMonth;
            }
            result = Duration2::getInstance(totalMonths, timeUnit);
            break;
        }

        case ELAPSED_MONTHS:
        {
            double unitsPerMonth = (60 * 24 * 30 * 10);
            double totalMonths = duration / unitsPerMonth;
            result = Duration2::getInstance(totalMonths, timeUnit);
            break;
        }

        default:
        {
            result = getDuration(duration, timeUnit);
            break;
        }
        }
    }
    return (result);
}

/**
* This method maps from the value used to specify default work units in the
* MPP file to a standard TimeUnit::
*
* @param value Default work units
* @return TimeUnit value
*/
TimeUnit MPPUtility::getWorkTimeUnits(int value)
{
    return (TimeUnit)(value - 1);
}

/**
* This method maps the currency symbol position from the
* representation used in the MPP file to the representation
* used by MPX.
*
* @param value MPP symbol position
* @return MPX symbol position
*/
CurrencySymbolPosition MPPUtility::getSymbolPosition(int value)
{
    CurrencySymbolPosition result;

    switch (value)
    {
        case 1:
        {
        result = CurrencySymbolPosition::AFTER;
        break;
        }

        case 2:
        {
        result = CurrencySymbolPosition::BEFORE_WITH_SPACE;
        break;
        }

        case 3:
        {
        result = CurrencySymbolPosition::AFTER_WITH_SPACE;
        break;
        }

        case 0:
        default:
        {
        result = CurrencySymbolPosition::BEFORE;
        break;
        }
    }

    return (result);
}

/**
* Utility method to remove ampersands embedded in names.
*
* @param name name text
* @return name text without embedded ampersands
*/
CString MPPUtility::removeAmpersands(CString name)
{
    if (name.Left(1) == L"&")
    {
		StringBuilder sb;// = new StringBuilder();
		int index = 0;
		TCHAR c;

		while (index < name.GetLength())
		{
			c = name.GetAt(index);
			if (c != '&')
			{
				sb.append(c);
			}
			++index;
		}

		name = sb.toString();
    }

    return (name);
}

/**
* Utility method to read a percentage value.
* 
* @param data data block
* @param offset offset into data block
* @return percentage value
*/
double MPPUtility::getPercentage(ByteArray2* data, int offset)
{
    int value = MPPUtility::getShort(data, offset);
    double result = 0;
    if (value >= 0 && value <= 100)
    {
		result = value;// NumberHelper::getDouble(value);
    }
    return result;
}

/**
* This method allows a subsection of a BYTE array to be copied.
*
* @param data source data
* @param offset offset into the source data
* @param size length of the source data to copy
* @return new BYTE array containing copied data
*/
ByteArray2* MPPUtility::cloneSubArray(ByteArray2* data, int offset, int size)
{
	ByteArray2* newData = new ByteArray2(size);
    //System.arraycopy(data, offset, newData, 0, size);
	memcpy_s(newData->get(), size, &(data->get()[offset]), size);
    return newData;
}

///**
//* This method generates a formatted version of the data contained
//* in a BYTE array. The data is written both in hex, and as ASCII
//* characters.
//*
//* @param buffer data to be displayed
//* @param offset offset of start of data to be displayed
//* @param length length of data to be displayed
//* @param ascii flag indicating whether ASCII equivalent chars should also be displayed
//* @return formatted string
//*/
//CString MPPUtility::hexdump(ByteArray2* buffer, int offset, int length, bool ascii)
//{
//	StringBuilder sb;// = new StringBuilder();
//
//	if (!buffer)
//		return L"";
//
//    char c;
//    int loop;
//    int count = offset + length;
//
//    for (loop = offset; loop < count; loop++)
//    {
//		sb.append(L" ");
//		sb.append(HEX_DIGITS[((*buffer)[loop] & 0xF0) >> 4]);
//		sb.append(HEX_DIGITS[(*buffer)[loop] & 0x0F]);
//    }
//
//    if (ascii == true)
//    {
//		sb.append("   ");
//
//		for (loop = offset; loop < count; loop++)
//		{
//			c = (char)(*buffer)[loop];
//
//			if ((c > 200) || (c < 27))
//			{
//				c = ' ';
//			}
//
//			sb.append(c);
//		}
//    }
//
//    return (sb.toString());
//}

///**
//* This method generates a formatted version of the data contained
//* in a BYTE array. The data is written both in hex, and as ASCII
//* characters.
//*
//* @param buffer data to be displayed
//* @param ascii flag indicating whether ASCII equivalent chars should also be displayed
//* @return formatted string
//*/
//CString MPPUtility::hexdump(ByteArray2* buffer, bool ascii)
//{
//    int length = 0;
//
//    if (buffer)
//    {
//        length = buffer->length();
//    }
//
//    return (hexdump(buffer, 0, length, ascii));
//}

///**
//* This method generates a formatted version of the data contained
//* in a BYTE array. The data is written both in hex, and as ASCII
//* characters. The data is organised into fixed width columns.
//*
//* @param buffer data to be displayed
//* @param ascii flag indicating whether ASCII equivalent chars should also be displayed
//* @param columns number of columns
//* @param prefix prefix to be added before the start of the data
//* @return formatted string
//*/
//CString MPPUtility::hexdump(ByteArray2* buffer, bool ascii, int columns, CString prefix)
//{
//	StringBuilder sb;// = new StringBuilder();
//	if (!buffer)
//		return L"";
//
//    int index = 0;
//    DecimalFormat df = new DecimalFormat("00000");
//
//    while (index < buffer->length())
//    {
//		if (index + columns > buffer->length())
//		{
//			columns = buffer->length() - index;
//		}
//
//		sb.append(prefix);
//		sb.append(df.format(index));
//		sb.append(":");
//		sb.append(hexdump(buffer, index, columns, ascii));
//		sb.append('\n');
//
//		index += columns;
//    }
//
//    return sb.toString();
//}

///**
//* This method generates a formatted version of the data contained
//* in a BYTE array. The data is written both in hex, and as ASCII
//* characters. The data is organised into fixed width columns.
//* 
//* @param buffer data to be displayed
//* @param offset offset into buffer
//* @param length number of bytes to display
//* @param ascii flag indicating whether ASCII equivalent chars should also be displayed
//* @param columns number of columns
//* @param prefix prefix to be added before the start of the data
//* @return formatted string
//*/
//CString MPPUtility::hexdump(ByteArray2* buffer, int offset, int length, bool ascii, int columns, CString prefix)
//{
//	StringBuilder sb;// = new StringBuilder();
//	if (!buffer) return L"";
//
//    int index = offset;
//    DecimalFormat df = new DecimalFormat("00000");
//
//    while (index < (offset + length))
//    {
//		if (index + columns > (offset + length))
//		{
//			columns = (offset + length) - index;
//		}
//
//		sb.append(prefix);
//		sb.append(df.format(index - offset));
//		sb.append(":");
//		sb.append(hexdump(buffer, index, columns, ascii));
//		sb.append('\n');
//
//		index += columns;
//    }
//
//    return sb.toString();
//}

///**
//* Writes a hex dump to a file for a large BYTE array.
//*
//* @param fileName output file name
//* @param data target data
//*/
//void MPPUtility::fileHexDump(CString fileName, ByteArray2* data)
//{
//    try
//    {
//        FileOutputStream os = new FileOutputStream(fileName);
//        os.write(hexdump(data, true, 16, "").getBytes());
//        os.close();
//    }
//
//    catch (Exception& ex)
//    {
//        ex.printStackTrace();
//    }
//}

///**
//* Writes a hex dump to a file from a POI input stream.
//* Note that this assumes that the complete size of the data in
//* the stream is returned by the available() method.
//*
//* @param fileName output file name
//* @param is input stream
//*/
//void MPPUtility::fileHexDump(CString fileName, InputStream is)
//{
//    try
//    {
//        ByteArray2* data = new BYTE[is.available()];
//        is.read(data);
//        fileHexDump(fileName, data);
//    }
//
//    catch (Exception& ex)
//    {
//        ex.printStackTrace();
//    }
//}

///**
//* Writes a large BYTE array to a file.
//*
//* @param fileName output file name
//* @param data target data
//*/
//void MPPUtility::fileDump(CString fileName, ByteArray2* data)
//{
//    try
//    {
//        FileOutputStream os = new FileOutputStream(fileName);
//        os.write(data);
//        os.close();
//    }
//    catch (Exception& ex)
//    {
//        ex.printStackTrace();
//    }
//}

///**
//* Dump out all the possible variables within the given data block.
//*
//* @param file current project file
//* @param data data to dump from
//* @param dumpShort true to dump all the data as shorts
//* @param dumpInt true to dump all the data as ints
//* @param dumpDouble true to dump all the data as Doubles
//* @param dumpTimeStamp true to dump all the data as TimeStamps
//* @param dumpDuration true to dump all the data as Durations (long)
//* @param dumpDate true to dump all the data as Dates
//* @param dumpTime true to dump all the data as Dates (time)
//* @param dumpAdjustedDuration true to dump all data as adjusted durations
//*/
//void MPPUtility::dataDump(ProjectFile file, ByteArray2* data, bool dumpShort, bool dumpInt, bool dumpDouble, 
//	bool dumpTimeStamp, bool dumpDuration, bool dumpDate, bool dumpTime, bool dumpAdjustedDuration)
//{
//	if (!data) return;
//
//    System.out.println(MPPUtility::hexdump(data, false, 16, ""));
//
//    for (int i = 0; i < data->length(); i++)
//    {
//		if (dumpShort)
//		{
//			try
//			{
//				int sh = MPPUtility::getShort(data, i);
//				System.out.println(i + ":" + sh);
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions
//			}
//		}
//		if (dumpInt)
//		{
//			try
//			{
//				int sh = MPPUtility::getInt(data, i);
//				System.out.println(i + ":" + sh);
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpDouble)
//		{
//			try
//			{
//				double d = MPPUtility::getDouble(data, i);
//				System.out.println(i + ":" + d);
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpTimeStamp)
//		{
//			try
//			{
//				DatePtr d = MPPUtility::getTimestamp(data, i);
//				if (d != NULL)
//				{
//					System.out.println(i + ":" + d.toString());
//				}
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpDuration)
//		{
//			try
//			{
//				long d = MPPUtility::getDuration(data, i);
//				System.out.println(i + ":" + d);
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpDate)
//		{
//			try
//			{
//				DatePtr d = MPPUtility::getDate(data, i);
//				if (d != NULL)
//				{
//					System.out.println(i + ":" + d.toString());
//				}
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpTime)
//		{
//			try
//			{
//				DatePtr d = MPPUtility::getTime(data, i);
//				System.out.println(i + ":" + d.toString());
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions      	    	      
//			}
//		}
//		if (dumpAdjustedDuration)
//		{
//			try
//			{
//				System.out.println(i + ":" + MPPUtility::getAdjustedDuration(file, 
//					MPPUtility::getInt(data, i), TimeUnit::DAYS));
//			}
//			catch (Exception ex)
//			{
//				// Silently ignore exceptions                   
//			}
//		}
//    }
//}
//
///**
//* Dump out all the possible variables within the given data block.
//*
//* @param data data to dump from
//* @param id unique ID
//* @param dumpShort true to dump all the data as shorts
//* @param dumpInt true to dump all the data as ints
//* @param dumpDouble true to dump all the data as Doubles
//* @param dumpTimeStamp true to dump all the data as TimeStamps
//* @param dumpUnicodeString true to dump all the data as Unicode strings
//* @param dumpString true to dump all the data as strings
//*/
//void MPPUtility::varDataDump(Var2DataPtr data, int id, bool dumpShort, bool dumpInt, 
//	bool dumpDouble, bool dumpTimeStamp, bool dumpUnicodeString, bool dumpString)
//{
//    System.out.println("VARDATA");
//    for (int i = 0; i < 500; i++)
//    {
//        if (dumpShort)
//        {
//        try
//        {
//            int sh = data.getShort(id, Integer.valueOf(i));
//            System.out.println(i + ":" + sh);
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//        if (dumpInt)
//        {
//        try
//        {
//            int sh = data.getInt(id, Integer.valueOf(i));
//            System.out.println(i + ":" + sh);
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//        if (dumpDouble)
//        {
//        try
//        {
//            double d = data.getDouble(id, Integer.valueOf(i));
//            System.out.println(i + ":" + d);
//            System.out.println(i + ":" + d / 60000);
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//        if (dumpTimeStamp)
//        {
//        try
//        {
//            DatePtr d = data.getTimestamp(id, Integer.valueOf(i));
//            if (d != NULL)
//            {
//                System.out.println(i + ":" + d.toString());
//            }
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//        if (dumpUnicodeString)
//        {
//        try
//        {
//            CString s = data.getUnicodeString(id, Integer.valueOf(i));
//            if (s != NULL)
//            {
//                System.out.println(i + ":" + s);
//            }
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//        if (dumpString)
//        {
//        try
//        {
//            CString s = data.getString(id, Integer.valueOf(i));
//            if (s != NULL)
//            {
//                System.out.println(i + ":" + s);
//            }
//        }
//        catch (Exception ex)
//        {
//            // Silently ignore exceptions
//        }
//        }
//    }
//}

///**
//* Dumps the contents of a structured block made up from a header
//* and fixed sized records.
//* 
//* @param headerSize header zie
//* @param blockSize block size
//* @param data data block
//*/
//void MPPUtility::dumpBlockData(int headerSize, int blockSize, ByteArray2* data)
//{
//    if (data != NULL)
//    {
//        System.out.println(MPPUtility::hexdump(data, 0, headerSize, false));
//        int index = headerSize;
//        while (index < data->length())
//        {
//			System.out.println(MPPUtility::hexdump(data, index, blockSize, false));
//			index += blockSize;
//        }
//    }
//}

/**
* Get the epoch date.
* 
* @return epoch date.
*/
DatePtr MPPUtility::getEpochDate()
{
    return EPOCH_DATE;
}