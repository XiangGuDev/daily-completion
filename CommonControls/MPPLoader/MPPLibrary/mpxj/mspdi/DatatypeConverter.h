#pragma once

#include "..\ProjectFile.h"

class MSPDIWriter;

/**
 * This class contains methods used to perform the datatype conversions
 * required to read and write MSPDI files.
 */
class DatatypeConverter
{
public:
	DatatypeConverter();
	~DatatypeConverter();

   /**
    * Print an extended attribute currency value.
    *
    * @param value currency value
    * @return string representation
    */
	CString printExtendedAttributeCurrency(NumberPtr value);

   /**
    * Parse an extended attribute currency value.
    *
    * @param value string representation
    * @return currency value
    */
	NumberPtr parseExtendedAttributeCurrency(CString value);

   /**
    * Print an extended attribute numeric value.
    *
    * @param value numeric value
    * @return string representation
    */
	CString printExtendedAttributeNumber(NumberPtr value);

   /**
    * Parse and extended attribute numeric value.
    *
    * @param value string representation
    * @return numeric value
    */
	NumberPtr parseExtendedAttributeNumber(CString value);

   /**
    * Print an extended attribute bool value.
    *
    * @param value bool value
    * @return string representation
    */
	CString printExtendedAttributeBoolean(bool value);

   /**
    * Parse an extended attribute bool value.
    *
    * @param value string representation
    * @return bool value
    */
	bool parseExtendedAttributeBoolean(CString value);

   /**
    * Print an extended attribute date value.
    *
    * @param value date value
    * @return string representation
    */
	CString printExtendedAttributeDate(DatePtr value);

   /**
    * Parse an extended attribute date value.
    *
    * @param value string representation
    * @return date value
    */
	DatePtr parseExtendedAttributeDate(CString value);

   /**
    * Print an extended attribute value.
    *
    * @param writer parent MSPDIWriter* instance
    * @param value attribute value
    * @param type type of the value being passed
    * @return string representation
    */
	CString printExtendedAttribute(MSPDIWriter* writer, ObjectPtr value, DataType type);

   /**
    * Parse an extended attribute value.
    *
    * @param file parent file
    * @param mpx parent entity
    * @param value string value
    * @param mpxFieldID field ID
    * @param durationFormat duration format associated with the extended attribute
    */
	void parseExtendedAttribute(ProjectFile* file, FieldContainer* mpx,
		CString value, FieldType* mpxFieldID, TimeUnit durationFormat);

   /**
    * Prints a currency symbol position value.
    *
    * @param value CurrencySymbolPosition instance
    * @return currency symbol position
    */
	CString printCurrencySymbolPosition(CurrencySymbolPosition value);

   /**
    * Parse a currency symbol position value.
    *
    * @param value currency symbol position
    * @return CurrencySymbolPosition instance
    */
	CurrencySymbolPosition parseCurrencySymbolPosition(CString value);

   /**
    * Print an accrue type.
    *
    * @param value AccrueType instance
    * @return accrue type value
    */
	CString printAccrueType(AccrueType value);

   /**
    * Parse an accrue type.
    *
    * @param value accrue type value
    * @return AccrueType instance
    */
	AccrueType parseAccrueType(CString value);

   /**
    * Print a resource type.
    *
    * @param value Resource2Type instance
    * @return resource type value
    */
	CString printResource2Type(Resource2Type value);

   /**
    * Parse a resource type.
    *
    * @param value resource type value
    * @return Resource2Type instance
    */
	Resource2Type parseResource2Type(CString value);

   /**
    * Print a work group.
    *
    * @param value WorkGroup2 instance
    * @return work group value
    */
	CString printWorkGroup2(WorkGroup2 value);

   /**
    * Parse a work group.
    *
    * @param value work group value
    * @return WorkGroup2 instance
    */
	WorkGroup2 parseWorkGroup2(CString value);

   /**
    * Print a work contour.
    *
    * @param value WorkContour instance
    * @return work contour value
    */
	CString printWorkContour(WorkContour value);

   /**
    * Parse a work contour.
    *
    * @param value work contour value
    * @return WorkContour instance
    */
	WorkContour parseWorkContour(CString value);

   /**
    * Print a booking type.
    *
    * @param value BookingType instance
    * @return booking type value
    */
	CString printBookingType(BookingType value);

   /**
    * Parse a booking type.
    *
    * @param value booking type value
    * @return BookingType instance
    */
	BookingType parseBookingType(CString value);

   /**
    * Print a task type.
    *
    * @param value TaskType instance
    * @return task type value
    */
	CString printTaskType(TaskType value);

   /**
    * Parse a task type.
    *
    * @param value task type value
    * @return TaskType instance
    */
	TaskType parseTaskType(CString value);

   /**
    * Print an earned value method.
    *
    * @param value EarnedValueMethod instance
    * @return earned value method value
    */
	BigIntegerPtr printEarnedValueMethod(EarnedValueMethod value);

   /**
    * Parse an earned value method.
    *
    * @param value earned value method
    * @return EarnedValueMethod instance
    */
	EarnedValueMethod parseEarnedValueMethod(NumberPtr value);

   /**
    * Print units.
    *
    * @param value units value
    * @return units value
    */
	BigDecimalPtr printUnits(NumberPtr value);

   /**
    * Parse units.
    *
    * @param value units value
    * @return units value
    */
	NumberPtr parseUnits(NumberPtr value);

   /**
    * Print time unit.
    *
    * @param value TimeUnit instance
    * @return time unit value
    */
	BigIntegerPtr printTimeUnit(TimeUnit value);

   /**
    * Parse time unit.
    *
    * @param value time unit value
    * @return TimeUnit instance
    */
	TimeUnit parseTimeUnit(NumberPtr value);

   /**
    * Print date.
    *
    * @param value DatePtr value
    * @return CalendarPtr value
    */
	CalendarPtr printDate(DatePtr value);

   /**
    * Parse date.
    *
    * @param value CalendarPtr value
    * @return DatePtr value
    */
	DatePtr parseDate(CalendarPtr value);

   /**
    * Print time.
    *
    * @param value time value
    * @return calendar value
    */
	CalendarPtr printTime(DatePtr value);
	CString printTime2T(DatePtr value);

   /**
    * Parse time.
    *
    * @param value CalendarPtr value
    * @return time value
    */
	DatePtr parseTime(CalendarPtr value);

   /**
    * Parse work units.
    *
    * @param value work units value
    * @return TimeUnit instance
    */
	TimeUnit parseWorkUnits(BigIntegerPtr value);

   /**
    * Print work units.
    *
    * @param value TimeUnit instance
    * @return work units value
    */
	BigIntegerPtr printWorkUnits(TimeUnit value);

   /**
    * Parse a duration.
    *
    * @param file parent file
    * @param defaultUnits default time units for the resulting duration
    * @param value duration value
    * @return DurationPtr instance
    */
	DurationPtr parseDuration(ProjectFile* file, TimeUnit defaultUnits, CString value);

   /**
    * Print duration.
    *
    * Note that Microsoft's xsd:duration parser implementation does not
    * appear to recognise durations other than those expressed in hours.
    * We use the compatibility flag to determine whether the output
    * is adjusted for the benefit of Microsoft Project.
    *
    * @param writer parent MSPDIWriter* instance
    * @param duration DurationPtr value
    * @return xsd:duration value
    */
	CString printDuration(MSPDIWriter* writer, DurationPtr duration);

   /**
    * Print duration.
    *
    * Note that Microsoft's xsd:duration parser implementation does not
    * appear to recognise durations other than those expressed in hours.
    * We use the compatibility flag to determine whether the output
    * is adjusted for the benefit of Microsoft Project.
    *
    * @param writer parent MSPDIWriter* instance
    * @param duration DurationPtr value
    * @return xsd:duration value
    */
	CString printDurationMandatory(MSPDIWriter* writer, DurationPtr duration);

   /**
    * Print duration time units.
    *
    * @param duration DurationPtr value
    * @param estimated is this an estimated duration
    * @return time units value 
    */
	BigIntegerPtr printDurationTimeUnits(DurationPtr duration, bool estimated);

   /**
    * Parse currency.
    *
    * @param value currency value
    * @return currency value
    */
	BigDecimalPtr parseCurrency(NumberPtr value);

   /**
    * Print currency.
    *
    * @param value currency value
    * @return currency value
    */
	BigDecimalPtr printCurrency(NumberPtr value);

   /**
    * Parse duration time units.
    *
    * Note that we don't differentiate between confirmed and unconfirmed
    * durations. Unrecognised duration types are default to hours.
    *
    * @param value BigIntegerPtr value
    * @return DurationPtr units
    */
	TimeUnit parseDurationTimeUnits(BigIntegerPtr value);

   /**
    * Parse duration time units.
    *
    * Note that we don't differentiate between confirmed and unconfirmed
    * durations. Unrecognised duration types are default the supplied default value.
    *
    * @param value BigIntegerPtr value
    * @param defaultValue if value is null, use this value as the result 
    * @return DurationPtr units
    */
	TimeUnit parseDurationTimeUnits(BigIntegerPtr value, TimeUnit defaultValue);

   /**
    * Print duration time units.
    *
    * Note that we don't differentiate between confirmed and unconfirmed
    * durations. Unrecognised duration types are default to hours.
    *
    * @param value DurationPtr units
    * @param estimated is this an estimated duration
    * @return BigIntegerPtr value
    */
	BigIntegerPtr printDurationTimeUnits(TimeUnit value, bool estimated);

   /**
    * Parse priority.
    *
    *
    * @param priority priority value
    * @return Priority* instance
    */
	PriorityPtr parsePriority(BigIntegerPtr priority);

   /**
    * Print priority.
    *
    * @param priority Priority* instance
    * @return priority value
    */
	BigIntegerPtr printPriority(PriorityPtr priority);

   /**
    * Parse duration represented in thousandths of minutes.
    *
    * @param value duration value
    * @return DurationPtr instance
    */
	DurationPtr parseDurationInThousanthsOfMinutes(NumberPtr value);

   /**
    * Parse duration represented in tenths of minutes.
    *
    * @param value duration value
    * @return DurationPtr instance
    */
	DurationPtr parseDurationInTenthsOfMinutes(NumberPtr value);

   /**
    * Parse duration represented in thousandths of minutes. 
    * 
    * @param header project header
    * @param value duration value
    * @param targetTimeUnit required output time units
    * @return DurationPtr instance
    */
	DurationPtr parseDurationInThousanthsOfMinutes(
		ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit);

   /**
    * Parse duration represented as tenths of minutes.
    * 
    * @param header project header
    * @param value duration value
    * @param targetTimeUnit required output time units
    * @return DurationPtr instance
    */
	DurationPtr parseDurationInTenthsOfMinutes(
		ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit);

   /**
    * Print duration in thousandths of minutes.
    *
    * @param duration DurationPtr instance
    * @return duration in thousandths of minutes
    */
	BigIntegerPtr printDurationInIntegerThousandthsOfMinutes(DurationPtr duration);

   /**
    * Print duration in thousandths of minutes.
    *
    * @param duration DurationPtr instance
    * @return duration in thousandths of minutes
    */
	BigDecimalPtr printDurationInDecimalThousandthsOfMinutes(DurationPtr duration);

   /**
    * Print duration in tenths of minutes.
    *
    * @param duration DurationPtr instance
    * @return duration in tenths of minutes
    */
	BigIntegerPtr printDurationInIntegerTenthsOfMinutes(DurationPtr duration);

   /**
    * Print rate.
    *
    * @param rate RatePtr instance
    * @return rate value
    */
	BigDecimalPtr printRate(RatePtr rate);

   /**
    * Parse rate.
    *
    * @param value rate value
    * @return RatePtr instance
    */
	RatePtr parseRate(BigDecimalPtr value);

   /**
    * Print a day.
    *
    * @param day Day instance
    * @return day value
    */
	BigIntegerPtr printDay(Day day);

   /**
    * Parse a day.
    *
    * @param value day value
    * @return Day instance
    */
	Day parseDay(NumberPtr value);

   /**
    * Parse a constraint type.
    *
    * @param value constraint type value
    * @return ConstraintType instance
    */
	ConstraintType parseConstraintType(NumberPtr value);

   /**
    * Print a constraint type.
    *
    * @param value ConstraintType instance
    * @return constraint type value
    */
	BigIntegerPtr printConstraintType(ConstraintType value);

   /**
    * Print a task UID.
    *
    * @param value task UID
    * @return task UID string
    */
	CString printTaskUID(int value);

   /**
    * Parse a task UID.
    *
    * @param value task UID string
    * @return task UID
    */
	int parseTaskUID(CString value);

   /**
    * Print a resource UID.
    *
    * @param value resource UID value
    * @return resource UID string
    */
	CString printResource2UID(int value);

   /**
    * Parse a resource UID.
    *
    * @param value resource UID string
    * @return resource UID value
    */
	int parseResource2UID(CString value);

   /**
    * Print a bool.
    *
    * @param value bool
    * @return bool value
    */
	CString printBoolean(bool value);

   /**
    * Parse a bool.
    *
    * @param value bool
    * @return bool value
    */
	bool parseBoolean(CString value);

   /**
    * Print a time value.
    *
    * @param value time value
    * @return time value
    */
	CString printTime(CalendarPtr value);

   /**
    * Parse a time value.
    *
    * @param value time value
    * @return time value
    */
	CalendarPtr parseTime(CString value);

   /**
    * Print a date time value.
    *
    * @param value date time value
    * @return string representation
    */
	CString printDateTime(CalendarPtr value);
	CString printDateTime(DatePtr value);

   /**
    * Parse a date time value.
    *
    * @param value string representation
    * @return date time value
    */
	CalendarPtr parseDateTime(CString value);

   /**
    * Print method for a string: returns the string unchanged.
    * This is used to enable to string representation of an
    * xsd:datetime to be generated by MPXJ.
    * 
    * @param value string value
    * @return string value
    */
	CString printString(CString value);

   /**
    * Parse method for a string: returns the string unchanged.
    * This is used to enable to string representation of an
    * xsd:datetime to be processed by MPXJ.
   
    * @param value string value
    * @return string value
    */
	CString parseString(CString value);

   /**
    * This method is called to set the parent file for the current
    * write operation. This allows task and resource write events
    * to be captured and passed to any file listeners.
    *
    * @param file parent file instance
    */
	void setParentFile(ProjectFile* file);

private:
	/**
	* Parse duration represented as an arbitrary fraction of minutes.
	*
	* @param header project header
	* @param value duration value
	* @param targetTimeUnit required output time units
	* @param factor required fraction of a minute
	* @return DurationPtr instance
	*/
	DurationPtr parseDurationInFractionsOfMinutes(
		ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit, int factor);

	/**
	* Print a duration represented by an arbitrary fraction of minutes.
	*
	* @param duration DurationPtr instance
	* @param factor required factor
	* @return duration represented as an arbitrary fraction of minutes
	*/
	double printDurationFractionsOfMinutes(DurationPtr duration, int factor);

   /**
    * Retrieve a number formatter.
    *
    * @return NumberFormat* instance
    */
	const NumberFormat* getNumberFormat();

   /**
    * Retrieve a date formatter.
    *
    * @return DateFormat* instance
    */
	const DateFormat* getDateFormat();

   /**
    * Retrieve a time formatter.
    *
    * @return DateFormat* instance
    */
	const DateFormat* getTimeFormat();

private:
	std::shared_ptr<DateFormat> DATE_FORMAT;// = new ThreadLocal<DateFormat*>();
	std::shared_ptr<DateFormat> TIME_FORMAT;// = new ThreadLocal<DateFormat*>();
	std::shared_ptr<NumberFormat> NUMBER_FORMAT;// = new ThreadLocal<NumberFormat*>();
	BigDecimalPtr BIGDECIMAL_ONE;// = BigDecimal.valueOf(1);
	ProjectFile* PARENT_FILE;// = new ThreadLocal<ProjectFile>();
};