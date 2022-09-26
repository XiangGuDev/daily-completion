#pragma once

#include "..\Duration.h"

class ProjectFile;
class Var2Data;

/**
 * This class provides common functionality used by each of the classes
 * that read the different sections of the MPP file.
 */
class MPPUtility
{
private:
   /**
    * Private constructor to prevent instantiation.
    */
	MPPUtility();

public:

	// 从数组中读字节转换成字符串（单字节）
	static CString ByteArray2String(ByteArray2* data, int start, int length);

   /**
    * This method decodes a BYTE array with the given encryption code
    * using XOR encryption.
    * 
    * @param data Source data
    * @param encryptionCode Encryption code
    */
	static void decodeBuffer(ByteArray2* data, BYTE encryptionCode);

   /**
    * Decode the password from the given data. Will decode the data block as well. 
    * 
    * @param data encrypted data block
    * @param encryptionCode encryption code
    * 
    * @return password
    */
   static CString decodePassword(ByteArray2* data, BYTE encryptionCode);

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
   static void getByteArray2(ByteArray2* data, int offset, int size, ByteArray2* buffer, int bufferOffset);

   /**
    * This method reads a single BYTE from the input array.
    *
    * @param data BYTE array of data
    * @param offset offset of BYTE data in the array
    * @return BYTE value
    */
   static int getByte(ByteArray2* data, int offset);

   /**
    * This method reads a single BYTE from the input array.
    * The BYTE is assumed to be at the start of the array.
    *
    * @param data BYTE array of data
    * @return BYTE value
    */
   static int getByte(ByteArray2* data);

   /**
    * This method reads a two BYTE integer from the input array.
    *
    * @param data the input array
    * @param offset offset of integer data in the array
    * @return integer value
    */
   static int getShort(ByteArray2* data, int offset);

   /**
    * This method reads a two BYTE integer from the input array.
    * The integer is assumed to be at the start of the array.
    *
    * @param data the input array
    * @return integer value
    */
   static int getShort(ByteArray2* data);

   /**
    * This method reads a four BYTE integer from the input array.
    *
    * @param data the input array
    * @param offset offset of integer data in the array
    * @return integer value
    */
   static int getInt(ByteArray2* data, int offset);

   /**
    * This method reads a four BYTE integer from the input array.
    * The integer is assumed to be at the start of the array.
    *
    * @param data the input array
    * @return integer value
    */
   static int getInt(ByteArray2* data);

   /**
    * This method reads an eight BYTE integer from the input array.
    *
    * @param data the input array
    * @param offset offset of integer data in the array
    * @return integer value
    */
   static __int64 getLong(ByteArray2* data, int offset);

   /**
    * This method reads a six BYTE long from the input array.
    *
    * @param data the input array
    * @param offset offset of integer data in the array
    * @return integer value
    */
   static long getLong6(ByteArray2* data, int offset);

   /**
    * This method reads a six BYTE long from the input array.
    * The integer is assumed to be at the start of the array.
    *
    * @param data the input array
    * @return integer value
    */
   static long getLong6(ByteArray2* data);

   /**
    * This method reads a eight BYTE integer from the input array.
    * The integer is assumed to be at the start of the array.
    *
    * @param data the input array
    * @return integer value
    */
   static __int64 getLong(ByteArray2* data);

   /**
    * This method reads an eight BYTE double from the input array.
    *
    * @param data the input array
    * @param offset offset of double data in the array
    * @return double value
    */
   static double getDouble(ByteArray2* data, int offset);

   /**
    * This method reads an eight BYTE double from the input array.
    * The double is assumed to be at the start of the array.
    *
    * @param data the input array
    * @return double value
    */
   static double getDouble(ByteArray2* data);

   /**
    * Reads a UUID/GUID from a data block.
    * 
    * @param data data block
    * @param offset offset into the data block
    * @return UUID instance
    */
   static GUID getGUID(ByteArray2* data, int offset);

   /**
    * Reads a date value. Note that an NA is represented as 65535 in the
    * MPP file. We represent this in Java using a null value. The actual
    * value in the MPP file is number of days since 31/12/1983.
    *
    * @param data BYTE array of data
    * @param offset location of data as offset into the array
    * @return date value
    */
   static DatePtr getDate(ByteArray2* data, int offset);

   /**
    * Reads a time value. The time is represented as tenths of a
    * minute since midnight.
    *
    * @param data BYTE array of data
    * @param offset location of data as offset into the array
    * @return time value
    */
   static DatePtr getTime(ByteArray2* data, int offset);

   /**
    * Reads a time value. The time is represented as tenths of a
    * minute since midnight.
    *
    * @param data BYTE array of data
    * @return time value
    */
   static DatePtr getTime(ByteArray2* data);

   /**
    * Reads a duration value in milliseconds. The time is represented as
    * tenths of a minute since midnight.
    *
    * @param data BYTE array of data
    * @param offset location of data as offset into the array
    * @return duration value
    */
   static long getDuration(ByteArray2* data, int offset);

   /**
    * Reads a combined date and time value.
    *
    * @param data BYTE array of data
    * @param offset location of data as offset into the array
    * @return time value
    */
   static DatePtr getTimestamp(ByteArray2* data, int offset);

   /**
    * Reads a combined date and time value expressed in tenths of a minute.
    *
    * @param data BYTE array of data
    * @param offset location of data as offset into the array
    * @return time value
    */
   static DatePtr getTimestampFromTenths(ByteArray2* data, int offset);

   /**
    * Reads a combined date and time value.
    * The value is assumed to be at the start of the array.
    *
    * @param data BYTE array of data
    * @return time value
    */
   static DatePtr getTimestamp(ByteArray2* data);

   /**
    * Reads a string of two BYTE characters from the input array.
    * This method assumes that the string finishes either at the
    * end of the array, or when char zero is encountered.
    * The value is assumed to be at the start of the array.
    *
    * @param data BYTE array of data
    * @return string value
    */
   static CString getUnicodeString(ByteArray2* data);

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
   static CString getUnicodeString(ByteArray2* data, int offset);

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
   static CString getUnicodeString(ByteArray2* data, int offset, int length);

   /**
    * Reads a string of single BYTE characters from the input array.
    * This method assumes that the string finishes either at the
    * end of the array, or when char zero is encountered.
    * The value is assumed to be at the start of the array.
    *
    * @param data BYTE array of data
    * @return string value
    */
   static CString getString(ByteArray2* data);

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
   static CString getString(ByteArray2* data, int offset);

   /**
    * Reads a duration value. This method relies on the fact that
    * the units of the duration have been specified elsewhere.
    *
    * @param value DurationPtr value
    * @param type type of units of the duration
    * @return DurationPtr instance
    */
   static DurationPtr getDuration(int value, TimeUnit type);

   /**
    * Reads a color value represented by three bytes, for R, G, and B
    * components, plus a flag BYTE indicating if this is an automatic color. 
    * Returns null if the color type is "Automatic".
    * 
    * @param data BYTE array of data
    * @param offset offset into array
    * @return new Color instance
    */
   static COLORREF getColor(ByteArray2* data, int offset);

   /**
    * Reads a duration value. This method relies on the fact that
    * the units of the duration have been specified elsewhere.
    *
    * @param value DurationPtr value
    * @param type type of units of the duration
    * @return DurationPtr instance
    */
   static DurationPtr getDuration(double value, TimeUnit type);

   /**
    * This method converts between the duration units representation
    * used in the MPP file, and the standard MPX duration units.
    * If the supplied units are unrecognised, the units default to days.
    *
    * @param type MPP units
    * @return MPX units
    */
   static TimeUnit getDurationTimeUnits(int type);

   /**
    * This method converts between the duration units representation
    * used in the MPP file, and the standard MPX duration units.
    * If the supplied units are unrecognised, the units default to days.
    *
    * @param type MPP units
    * @param projectDefaultDurationUnits default duration units for this project
    * @return MPX units
    */
   static TimeUnit getDurationTimeUnits(int type, TimeUnit projectDefaultDurationUnits);

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
   static DurationPtr getAdjustedDuration(ProjectFile* file, int duration, TimeUnit timeUnit);

   /**
    * This method maps from the value used to specify default work units in the
    * MPP file to a standard TimeUnit.
    *
    * @param value Default work units
    * @return TimeUnit value
    */
   static TimeUnit getWorkTimeUnits(int value);

   /**
    * This method maps the currency symbol position from the
    * representation used in the MPP file to the representation
    * used by MPX.
    *
    * @param value MPP symbol position
    * @return MPX symbol position
    */
   static CurrencySymbolPosition getSymbolPosition(int value);

   /**
    * Utility method to remove ampersands embedded in names.
    *
    * @param name name text
    * @return name text without embedded ampersands
    */
   static CString removeAmpersands(CString name);

   /**
    * Utility method to read a percentage value.
    * 
    * @param data data block
    * @param offset offset into data block
    * @return percentage value
    */
   static double getPercentage(ByteArray2* data, int offset);

   /**
    * This method allows a subsection of a BYTE array to be copied.
    *
    * @param data source data
    * @param offset offset into the source data
    * @param size length of the source data to copy
    * @return new BYTE array containing copied data
    */
   static ByteArray2* cloneSubArray(ByteArray2* data, int offset, int size);

   ///**
   // * This method generates a formatted version of the data contained
   // * in a BYTE array. The data is written both in hex, and as ASCII
   // * characters.
   // *
   // * @param buffer data to be displayed
   // * @param offset offset of start of data to be displayed
   // * @param length length of data to be displayed
   // * @param ascii flag indicating whether ASCII equivalent chars should also be displayed
   // * @return formatted string
   // */
   //static CString hexdump(ByteArray2* buffer, int offset, int length, bool ascii);

   ///**
   // * This method generates a formatted version of the data contained
   // * in a BYTE array. The data is written both in hex, and as ASCII
   // * characters.
   // *
   // * @param buffer data to be displayed
   // * @param ascii flag indicating whether ASCII equivalent chars should also be displayed
   // * @return formatted string
   // */
   //static CString hexdump(ByteArray2* buffer, bool ascii);

   ///**
   // * This method generates a formatted version of the data contained
   // * in a BYTE array. The data is written both in hex, and as ASCII
   // * characters. The data is organised into fixed width columns.
   // *
   // * @param buffer data to be displayed
   // * @param ascii flag indicating whether ASCII equivalent chars should also be displayed
   // * @param columns number of columns
   // * @param prefix prefix to be added before the start of the data
   // * @return formatted string
   // */
   //static CString hexdump(ByteArray2* buffer, bool ascii, int columns, CString prefix);

   ///**
   // * This method generates a formatted version of the data contained
   // * in a BYTE array. The data is written both in hex, and as ASCII
   // * characters. The data is organised into fixed width columns.
   // * 
   // * @param buffer data to be displayed
   // * @param offset offset into buffer
   // * @param length number of bytes to display
   // * @param ascii flag indicating whether ASCII equivalent chars should also be displayed
   // * @param columns number of columns
   // * @param prefix prefix to be added before the start of the data
   // * @return formatted string
   // */
   //static CString hexdump(ByteArray2* buffer, int offset, int length, bool ascii, int columns, CString prefix);

   ///**
   // * Writes a hex dump to a file for a large BYTE array.
   // *
   // * @param fileName output file name
   // * @param data target data
   // */
   //static void fileHexDump(CString fileName, ByteArray2* data);

   ///**
   // * Writes a hex dump to a file from a POI input stream.
   // * Note that this assumes that the complete size of the data in
   // * the stream is returned by the available() method.
   // *
   // * @param fileName output file name
   // * @param is input stream
   // */
   //static void fileHexDump(CString fileName, BwProj::toolkit::IDataReadStream* is);

   ///**
   // * Writes a large BYTE array to a file.
   // *
   // * @param fileName output file name
   // * @param data target data
   // */
   //static void fileDump(CString fileName, ByteArray2* data);

   ///**
   // * Dump out all the possible variables within the given data block.
   // *
   // * @param file current project file
   // * @param data data to dump from
   // * @param dumpShort true to dump all the data as shorts
   // * @param dumpInt true to dump all the data as ints
   // * @param dumpDouble true to dump all the data as Doubles
   // * @param dumpTimeStamp true to dump all the data as TimeStamps
   // * @param dumpDuration true to dump all the data as Durations (long)
   // * @param dumpDate true to dump all the data as Dates
   // * @param dumpTime true to dump all the data as Dates (time)
   // * @param dumpAdjustedDuration true to dump all data as adjusted durations
   // */
   //static void dataDump(ProjectFile* file, ByteArray2* data, bool dumpShort,
	  // bool dumpInt, bool dumpDouble, bool dumpTimeStamp, bool dumpDuration,
	  // bool dumpDate, bool dumpTime, bool dumpAdjustedDuration);

   ///**
   // * Dump out all the possible variables within the given data block.
   // *
   // * @param data data to dump from
   // * @param id unique ID
   // * @param dumpShort true to dump all the data as shorts
   // * @param dumpInt true to dump all the data as ints
   // * @param dumpDouble true to dump all the data as Doubles
   // * @param dumpTimeStamp true to dump all the data as TimeStamps
   // * @param dumpUnicodeString true to dump all the data as Unicode strings
   // * @param dumpString true to dump all the data as strings
   // */
   //static void varDataDump(std::shared_ptr<Var2Data> data, int id, bool dumpShort,bool dumpInt, 
	  // bool dumpDouble, bool dumpTimeStamp, bool dumpUnicodeString, bool dumpString);

   ///**
   // * Dumps the contents of a structured block made up from a header
   // * and fixed sized records.
   // * 
   // * @param headerSize header zie
   // * @param blockSize block size
   // * @param data data block
   // */
   //static void dumpBlockData(int headerSize, int blockSize, ByteArray2* data);

   /**
    * Get the epoch date.
    * 
    * @return epoch date.
    */
   static DatePtr getEpochDate();

private:
   /**
    * Epoch date for MPP date calculation is 31/12/1983. This constant
    * is that date expressed in milliseconds using the Java date epoch.
	* 441763200000L为1/1/1984，1983Date对象不支持
    */
	static const time_t EPOCH = 441676800L/*000L*/;// 441763200000L;// 441676800000L;

   /**
    * Epoch DatePtr as a DatePtr instance.
    */
   static const DatePtr EPOCH_DATE;// = DateHelper.getTimestampFromLong(EPOCH);

   /**
    * Number of milliseconds per day.
    */
   static const time_t MS_PER_DAY = 24 * 60 * 60;// *1000;

   /**
    * Number of milliseconds per minute.
    */
   static const time_t MS_PER_MINUTE = 60;// *1000;

   /**
    * Constants used to convert bytes to hex digits.
    */
   static const char HEX_DIGITS[];

   /**
    * Mask used to remove flags from the duration units field.
    */
   static const int DURATION_UNITS_MASK = 0x1F;

   /**
   * The mask used by Project to hide the password. The data must first
   * be decoded using the XOR key and then the password can be read by reading
   * the characters in given order starting with 1 and going up to 16.
   *
   * 00000: 00 00 04 00 00 00 05 00 07 00 12 00 10 00 06 00
   * 00016: 14 00 00 00 00 00 08 00 16 00 00 00 00 00 02 00
   * 00032: 00 00 15 00 00 00 11 00 00 00 00 00 00 00 09 00
   * 00048: 03 00 00 00 00 00 00 00 00 00 00 00 01 00 13 00
   */
   static const BYTE PASSWORD_MASK[];

   static const int MINIMUM_PASSWORD_DATA_LENGTH = 64;
};