#include "stdafx.h"
#include "DatatypeConverter.h"
#include "..\common\StringHelper.h"
#include "XsdDuration.h"
#include "MSPDIWriter.h"
//#include "..\GarbageCollector.h"

DatatypeConverter::DatatypeConverter()
:BIGDECIMAL_ONE(new BigDecimal(1))
{

}

DatatypeConverter::~DatatypeConverter()
{
	BIGDECIMAL_ONE.reset();
	DATE_FORMAT.reset();
	TIME_FORMAT.reset();
	NUMBER_FORMAT.reset();
	//DATE_FORMAT.reset();
}

/**
* Print an extended attribute currency value->
*
* @param value currency value
* @return string representation
*/
CString DatatypeConverter::printExtendedAttributeCurrency(NumberPtr value)
{
    return (value == NULL ? _T("") : getNumberFormat()->format(value->doubleValue() * 100));
}

/**
* Parse an extended attribute currency value->
*
* @param value string representation
* @return currency value
*/
NumberPtr DatatypeConverter::parseExtendedAttributeCurrency(CString value)
{
    NumberPtr result = NULL;

    if (value.GetLength())
    {
		result = NumberPtr(new Number(_ttof(value) / 100));
    }
    return result;
}

/**
* Print an extended attribute numeric value->
*
* @param value numeric value
* @return string representation
*/
CString DatatypeConverter::printExtendedAttributeNumber(NumberPtr value)
{
    return (getNumberFormat()->format(value->doubleValue()));
}

/**
* Parse and extended attribute numeric value->
*
* @param value string representation
* @return numeric value
*/
NumberPtr DatatypeConverter::parseExtendedAttributeNumber(CString value)
{
	return NumberPtr(new Number(_ttof(value)));
}

/**
* Print an extended attribute bool value->
*
* @param value bool value
* @return string representation
*/
CString DatatypeConverter::printExtendedAttributeBoolean(bool value)
{
    return (value ? L"1" : L"0");
}

/**
* Parse an extended attribute bool value->
*
* @param value string representation
* @return bool value
*/
bool DatatypeConverter::parseExtendedAttributeBoolean(CString value)
{
    return ((value == L"1" ? true : false));
}

/**
* Print an extended attribute date value->
*
* @param value date value
* @return string representation
*/
CString DatatypeConverter::printExtendedAttributeDate(DatePtr value)
{
    return (value == NULL ? _T("") : getDateFormat()->format(value));
}

/**
* Parse an extended attribute date value->
*
* @param value string representation
* @return date value
*/
DatePtr DatatypeConverter::parseExtendedAttributeDate(CString value)
{
    DatePtr result = NULL;

    if (value.GetLength() > 0)
    {
		result = getDateFormat()->parse(value);
    }

    return (result);
}

/**
* Print an extended attribute value->
*
* @param writer parent MSPDIWriter* instance
* @param value attribute value
* @param type type of the value being passed
* @return string representation
*/
CString DatatypeConverter::printExtendedAttribute(MSPDIWriter* writer, ObjectPtr value, DataType type)
{
    CString result;

	VariantPtr varValue = std::dynamic_pointer_cast<Variant>(value);

    if (type == DataType::DT_DATE)
    {
		result = printExtendedAttributeDate(std::dynamic_pointer_cast<Date>(value));
    }
    else if (varValue && varValue->type () == VT_BOOL)
    {
		result = printExtendedAttributeBoolean(varValue->boolValue());
    }
    else if (std::dynamic_pointer_cast<Duration2>(value))
	{
		result = printDuration(writer, std::dynamic_pointer_cast<Duration2>(value));
	}
	else if (type == DataType::DT_CURRENCY)
	{
		result = printExtendedAttributeCurrency(varValue);
	}
	else if (NumberHelper::isNumber(varValue))
	{
		result = printExtendedAttributeNumber(varValue);
	}
	else
	{
		result = value->toString();
	}

    return (result);
}

/**
* Parse an extended attribute value->
*
* @param file parent file
* @param mpx parent entity
* @param value string value
* @param mpxFieldID field ID
* @param durationFormat duration format associated with the extended attribute
*/
void DatatypeConverter::parseExtendedAttribute(ProjectFile* file, FieldContainer* mpx, 
	CString value, FieldType* mpxFieldID, TimeUnit durationFormat)
{
	if (mpxFieldID == NULL)
		return;

    switch (mpxFieldID->getDataType())
    {
		case DT_STRING:
		{
			mpx->set(mpxFieldID, VariantPtr(new Variant(value)));
			break;
		}

		case DT_DATE:
		{
			mpx->set(mpxFieldID, parseExtendedAttributeDate(value));
			break;
		}

		case DT_CURRENCY:
		{
			mpx->set(mpxFieldID, parseExtendedAttributeCurrency(value));
			break;
		}

		case DT_BOOLEAN:
		{
			mpx->set(mpxFieldID, NumberPtr(new Number(parseExtendedAttributeBoolean(value))));
			break;
		}

		case DT_NUMERIC:
		{
			mpx->set(mpxFieldID, parseExtendedAttributeNumber(value));
			break;
		}

		case DT_DURATION:
		{
			mpx->set(mpxFieldID, parseDuration(file, durationFormat, value));
			break;
		}

		default:
		{
			break;
		}
    }
}

/**
* Prints a currency symbol position value->
*
* @param value CurrencySymbolPosition instance
* @return currency symbol position
*/
CString DatatypeConverter::printCurrencySymbolPosition(CurrencySymbolPosition value)
{
    CString result;

    switch (value)
    {
        case BEFORE:
        {
			result = L"0";
			break;
        }

        case AFTER:
        {
			result = L"1";
			break;
        }

        case BEFORE_WITH_SPACE:
        {
			result = L"2";
			break;
        }

        case AFTER_WITH_SPACE:
        {
			result = L"3";
			break;
        }
		default:
			result = L"0";
			break;
    }

    return (result);
}

/**
* Parse a currency symbol position value->
*
* @param value currency symbol position
* @return CurrencySymbolPosition instance
*/
CurrencySymbolPosition DatatypeConverter::parseCurrencySymbolPosition(CString value)
{
    CurrencySymbolPosition result = CurrencySymbolPosition::BEFORE;

    switch (_ttol(value))
    {
        case 0:
        {
			result = CurrencySymbolPosition::BEFORE;
			break;
        }

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
    }

    return (result);
}

/**
* Print an accrue type.
*
* @param value AccrueType instance
* @return accrue type value
*/
CString DatatypeConverter::printAccrueType(AccrueType value)
{
	return StringHelper::toString((int)value);// value == NULL ? AccrueType::ACCRUE_PRORATED : value);
}

/**
* Parse an accrue type.
*
* @param value accrue type value
* @return AccrueType instance
*/
AccrueType DatatypeConverter::parseAccrueType(CString value)
{
    return (AccrueType) _ttol(value);
}

/**
* Print a resource type.
*
* @param value Resource2Type instance
* @return resource type value
*/
CString DatatypeConverter::printResource2Type(Resource2Type value)
{
	return StringHelper::toString((int)value);// value == NULL ? Resource2Type::RT_WORK : value);
}

/**
* Parse a resource type.
*
* @param value resource type value
* @return Resource2Type instance
*/
Resource2Type DatatypeConverter::parseResource2Type(CString value)
{
    return (Resource2Type)_ttol(value);
}

/**
* Print a work group.
*
* @param value WorkGroup2 instance
* @return work group value
*/
CString DatatypeConverter::printWorkGroup2(WorkGroup2 value)
{
	return StringHelper::toString((int)value);// value == NULL ? WorkGroup2::WG_DEFAULT : value);
}

/**
* Parse a work group.
*
* @param value work group value
* @return WorkGroup2 instance
*/
WorkGroup2 DatatypeConverter::parseWorkGroup2(CString value)
{
    return (WorkGroup2)_ttol(value);
}

/**
* Print a work contour.
*
* @param value WorkContour instance
* @return work contour value
*/
CString DatatypeConverter::printWorkContour(WorkContour value)
{
	return StringHelper::toString((int)value);// == NULL ? WorkContour::FLAT : value);
}

/**
* Parse a work contour.
*
* @param value work contour value
* @return WorkContour instance
*/
WorkContour DatatypeConverter::parseWorkContour(CString value)
{
    return (WorkContour)_ttol(value);
}

/**
* Print a booking type.
*
* @param value BookingType instance
* @return booking type value
*/
CString DatatypeConverter::printBookingType(BookingType value)
{
	if (value == NULL_BOOKINGTYPE)
		return L"";
	return StringHelper::toString((int)value);// == NULL ? BookingType::COMMITTED : value);
}

/**
* Parse a booking type.
*
* @param value booking type value
* @return BookingType instance
*/
BookingType DatatypeConverter::parseBookingType(CString value)
{
    return (BookingType)_ttol(value);
}

/**
* Print a task type.
*
* @param value TaskType instance
* @return task type value
*/
CString DatatypeConverter::printTaskType(TaskType value)
{
	return StringHelper::toString((int)value);// == NULL ? TaskType::FIXED_UNITS : value);
}

/**
* Parse a task type.
*
* @param value task type value
* @return TaskType instance
*/
TaskType DatatypeConverter::parseTaskType(CString value)
{
    return (TaskType)_ttol(value);
}

/**
* Print an earned value method.
*
* @param value EarnedValueMethod instance
* @return earned value method value
*/
BigIntegerPtr DatatypeConverter::printEarnedValueMethod(EarnedValueMethod value)
{
	return BigIntegerPtr(new BigInteger((int)value));// == NULL ? EarnedValueMethod::PERCENT_COMPLETE : value);
}

/**
* Parse an earned value method.
*
* @param value earned value method
* @return EarnedValueMethod instance
*/
EarnedValueMethod DatatypeConverter::parseEarnedValueMethod(NumberPtr value)
{
    return (EarnedValueMethod)(value->intValue());
}

/**
* Print units.
*
* @param value units value
* @return units value
*/
BigDecimalPtr DatatypeConverter::printUnits(NumberPtr value)
{
	return BigDecimalPtr(value == NULL ? new BigDecimal(BIGDECIMAL_ONE->intValue()) : new BigDecimal(value->doubleValue() / 100));
}

/**
* Parse units.
*
* @param value units value
* @return units value
*/
NumberPtr DatatypeConverter::parseUnits(NumberPtr value)
{
	return NumberPtr((value == NULL ? NULL : new Number(value->doubleValue() * 100)));
}

/**
* Print time unit.
*
* @param value TimeUnit instance
* @return time unit value
*/
BigIntegerPtr DatatypeConverter::printTimeUnit(TimeUnit value)
{
	return BigIntegerPtr(new BigInteger(value + 1));// value == NULL ? TimeUnit::DAYS + 1 : value + 1);
}

/**
* Parse time unit.
*
* @param value time unit value
* @return TimeUnit instance
*/
TimeUnit DatatypeConverter::parseTimeUnit(NumberPtr value)
{
    return (TimeUnit)(value->intValue() - 1);
}

/**
* Print date.
*
* @param value DatePtr value
* @return CalendarPtr value
*/
CalendarPtr DatatypeConverter::printDate(DatePtr value)
{
	CalendarPtr cal;

    if (value != NULL)
    {
        cal = Calendar::getInstance();
        cal->setTime(value);
        //cal->set(Calendar::MILLISECOND, 0);
        cal->set(Calendar::SECOND, 0);
    }

    return (cal);
}

/**
* Parse date.
*
* @param value CalendarPtr value
* @return DatePtr value
*/
DatePtr DatatypeConverter::parseDate(CalendarPtr value)
{
    DatePtr result;

    if (value != NULL)
    {
        //CalendarPtr cal = Calendar::getInstance();
        //cal->set(Calendar::YEAR, value->get(Calendar::YEAR));
        //cal->set(Calendar::MONTH, value->get(Calendar::MONTH));
        //cal->set(Calendar::DAY_OF_MONTH, value->get(Calendar::DAY_OF_MONTH));
        //cal->set(Calendar::HOUR_OF_DAY, value->get(Calendar::HOUR_OF_DAY));
        //cal->set(Calendar::MINUTE, value->get(Calendar::MINUTE));
        //cal->set(Calendar::SECOND, value->get(Calendar::SECOND));
        //cal->set(Calendar::MILLISECOND, value->get(Calendar::MILLISECOND));
        //result = cal->getTime();
		result = value->getTime();
    }

    return (result);
}

/**
* Print time.
*
* @param value time value
* @return calendar value
*/
CalendarPtr DatatypeConverter::printTime(DatePtr value)
{
    CalendarPtr cal = NULL;

    if (value != NULL)
    {
        cal = Calendar::getInstance();
        cal->setTime(value);
        //cal->set(Calendar::MILLISECOND, 0);
        cal->set(Calendar::SECOND, 0);
    }

    return (cal);
}

CString DatatypeConverter::printTime2T(DatePtr value)
{
	return printTime(printTime(value));
}

/**
* Parse time.
*
* @param value CalendarPtr value
* @return time value
*/
DatePtr DatatypeConverter::parseTime(CalendarPtr value)
{
    DatePtr result = NULL;

    if (value != NULL)
    {
        CalendarPtr cal = Calendar::getInstance();
        cal->set(Calendar::HOUR_OF_DAY, value->get(Calendar::HOUR_OF_DAY));
        cal->set(Calendar::MINUTE, value->get(Calendar::MINUTE));
        cal->set(Calendar::SECOND, value->get(Calendar::SECOND));
        //cal->set(Calendar::MILLISECOND, value->get(Calendar::MILLISECOND));
        result = cal->getTime();
    }

    return (result);
}

/**
* Parse work units.
*
* @param value work units value
* @return TimeUnit instance
*/
TimeUnit DatatypeConverter::parseWorkUnits(BigIntegerPtr value)
{
    TimeUnit result = TimeUnit::HOURS;

	if (value == NULL)
		return result;
 
    switch (value->intValue())
    {
    case 1:
    {
        result = TimeUnit::MINUTES;
        break;
    }

    case 3:
    {
        result = TimeUnit::DAYS;
        break;
    }

    case 4:
    {
        result = TimeUnit::WEEKS;
        break;
    }

    case 5:
    {
        result = TimeUnit::MONTHS;
        break;
    }

    case 7:
    {
        result = TimeUnit::YEARS;
        break;
    }

    default:
    case 2:
    {
        result = TimeUnit::HOURS;
        break;
    }
    }

    return (result);
}

/**
* Print work units.
*
* @param value TimeUnit instance
* @return work units value
*/
BigIntegerPtr DatatypeConverter::printWorkUnits(TimeUnit value)
{
    int result = 0;

    //if (value == NULL)
    //{
    //    value = TimeUnit::HOURS;
    //}

    switch (value)
    {
        case MINUTES:
        {
        result = 1;
        break;
        }

        case DAYS:
        {
        result = 3;
        break;
        }

        case WEEKS:
        {
        result = 4;
        break;
        }

        case MONTHS:
        {
        result = 5;
        break;
        }

        case YEARS:
        {
        result = 7;
        break;
        }

        default:
        case HOURS:
        {
        result = 2;
        break;
        }
    }

	return BigIntegerPtr(new BigInteger(result));
}

/**
* Parse a duration->
*
* @param file parent file
* @param defaultUnits default time units for the resulting duration
* @param value duration value
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDuration(ProjectFile* file, TimeUnit defaultUnits, CString value)
{
    DurationPtr result;

	if (value.GetLength() == 0)
		return result;

	XsdDuration xsd(value);// = new XsdDuration(value);
    TimeUnit units = TimeUnit::DAYS;

    if (xsd.getSeconds() != 0 || xsd.getMinutes() != 0)
    {
		units = TimeUnit::MINUTES;
    }

    if (xsd.getHours() != 0)
    {
		units = TimeUnit::HOURS;
    }

    if (xsd.getDays() != 0)
    {
		units = TimeUnit::DAYS;
    }

    if (xsd.getMonths() != 0)
    {
		units = TimeUnit::MONTHS;
    }

    if (xsd.getYears() != 0)
    {
		units = TimeUnit::YEARS;
    }

    double duration = 0;

    switch (units)
    {
		case TimeUnit::YEARS:
		{
			//
			// Calculate the number of years
			//
			duration += xsd.getYears();
			duration += ((double) xsd.getMonths() / 12);
			duration += ((double) xsd.getDays() / 365);
			duration += ((double) xsd.getHours() / (365 * 24));
			duration += ((double) xsd.getMinutes() / (365 * 24 * 60));
			duration += (xsd.getSeconds() / (365 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::ELAPSED_YEARS:
		{
			//
			// Calculate the number of years
			//
			duration += xsd.getYears();
			duration += ((double) xsd.getMonths() / 12);
			duration += ((double) xsd.getDays() / 365);
			duration += ((double) xsd.getHours() / (365 * 24));
			duration += ((double) xsd.getMinutes() / (365 * 24 * 60));
			duration += (xsd.getSeconds() / (365 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::MONTHS:
		{
			//
			// Calculate the number of months
			//
			duration += (xsd.getYears() * 12);
			duration += xsd.getMonths();
			duration += ((double) xsd.getDays() / 30);
			duration += ((double) xsd.getHours() / (30 * 24));
			duration += ((double) xsd.getMinutes() / (30 * 24 * 60));
			duration += (xsd.getSeconds() / (30 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::ELAPSED_MONTHS:
		{
			//
			// Calculate the number of months
			//
			duration += (xsd.getYears() * 12);
			duration += xsd.getMonths();
			duration += ((double) xsd.getDays() / 30);
			duration += ((double) xsd.getHours() / (30 * 24));
			duration += ((double) xsd.getMinutes() / (30 * 24 * 60));
			duration += (xsd.getSeconds() / (30 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::WEEKS:
		{
			//
			// Calculate the number of weeks
			//
			duration += (xsd.getYears() * 52);
			duration += (xsd.getMonths() * 4);
			duration += ((double) xsd.getDays() / 7);
			duration += ((double) xsd.getHours() / (7 * 24));
			duration += ((double) xsd.getMinutes() / (7 * 24 * 60));
			duration += (xsd.getSeconds() / (7 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::ELAPSED_WEEKS:
		{
			//
			// Calculate the number of weeks
			//
			duration += (xsd.getYears() * 52);
			duration += (xsd.getMonths() * 4);
			duration += ((double) xsd.getDays() / 7);
			duration += ((double) xsd.getHours() / (7 * 24));
			duration += ((double) xsd.getMinutes() / (7 * 24 * 60));
			duration += (xsd.getSeconds() / (7 * 24 * 60 * 60));
			break;
		}

		case TimeUnit::DAYS:
		{
			//
			// Calculate the number of days
			//
			duration += (xsd.getYears() * 365);
			duration += (xsd.getMonths() * 30);
			duration += xsd.getDays();
			duration += ((double) xsd.getHours() / 24);
			duration += ((double) xsd.getMinutes() / (24 * 60));
			duration += (xsd.getSeconds() / (24 * 60 * 60));
			break;
		}

		case TimeUnit::ELAPSED_DAYS:
		{
			//
			// Calculate the number of days
			//
			duration += (xsd.getYears() * 365);
			duration += (xsd.getMonths() * 30);
			duration += xsd.getDays();
			duration += ((double) xsd.getHours() / 24);
			duration += ((double) xsd.getMinutes() / (24 * 60));
			duration += (xsd.getSeconds() / (24 * 60 * 60));
			break;
		}

		case TimeUnit::HOURS:
		case TimeUnit::ELAPSED_HOURS:
		{
			//
			// Calculate the number of hours
			//
			duration += (xsd.getYears() * (365 * 24));
			duration += (xsd.getMonths() * (30 * 24));
			duration += (xsd.getDays() * 24);
			duration += xsd.getHours();
			duration += ((double) xsd.getMinutes() / 60);
			duration += (xsd.getSeconds() / (60 * 60));
			break;
		}

		case TimeUnit::MINUTES:
		case TimeUnit::ELAPSED_MINUTES:
		{
			//
			// Calculate the number of minutes
			//
			duration += (xsd.getYears() * (365 * 24 * 60));
			duration += (xsd.getMonths() * (30 * 24 * 60));
			duration += (xsd.getDays() * (24 * 60));
			duration += (xsd.getHours() * 60);
			duration += xsd.getMinutes();
			duration += (xsd.getSeconds() / 60);
			break;
		}

		default:
		{
			break;
		}
    }

    //
    // Convert from a duration in hours to a duration
    // expressed in the default duration units
    //
	ProjectHeader* header = file->getProjectHeader();
    if (defaultUnits == NULL)
    {
		defaultUnits = header->getDefaultDurationUnits();
    }

    result = Duration2::convertUnits(duration, units, defaultUnits, header);

    return (result);
}

/**
* Print duration->
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
CString DatatypeConverter::printDuration(MSPDIWriter* writer, DurationPtr duration)
{
    CString result;

    if (duration != NULL /*&& duration->getDuration() != 0*/)
    {
        result = printDurationMandatory(writer, duration);
    }

    return (result);
}

/**
* Print duration->
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
CString DatatypeConverter::printDurationMandatory(MSPDIWriter* writer, DurationPtr duration)
{
    CString result;

    if (duration != NULL)
    {
        TimeUnit durationType = duration->getUnits();

        if (durationType == TimeUnit::HOURS || durationType == TimeUnit::ELAPSED_HOURS)
        {
			result = XsdDuration(duration).toString();
        }
        else
        {
			duration = duration->convertUnits(TimeUnit::HOURS, writer->getProjectFile()->getProjectHeader());
			result = XsdDuration(duration).toString();
        }
    }

    return (result);
}

/**
* Print duration time units.
*
* @param duration DurationPtr value
* @param estimated is this an estimated duration
* @return time units value 
*/
BigIntegerPtr DatatypeConverter::printDurationTimeUnits(DurationPtr duration, bool estimated)
{
    BigIntegerPtr result;

    if (duration != NULL)
    {
        result = printDurationTimeUnits(duration->getUnits(), estimated);
    }

    return (result);
}

/**
* Parse currency.
*
* @param value currency value
* @return currency value
*/
BigDecimalPtr DatatypeConverter::parseCurrency(NumberPtr value)
{
	return BigDecimalPtr(value == NULL ? NULL : new BigDecimal(value->doubleValue() / 100));
}

/**
* Print currency.
*
* @param value currency value
* @return currency value
*/
BigDecimalPtr DatatypeConverter::printCurrency(NumberPtr value)
{
	if (!value) return value;
	if (abs(value->doubleValue()*100) == 0)
		return BigDecimalPtr();

	return BigDecimalPtr(new BigDecimal(value->doubleValue() * 100));
}

/**
* Parse duration time units.
*
* Note that we don't differentiate between confirmed and unconfirmed
* durations. Unrecognised duration types are default to hours.
*
* @param value BigIntegerPtr value
* @return DurationPtr units
*/
TimeUnit DatatypeConverter::parseDurationTimeUnits(BigIntegerPtr value)
{
    return parseDurationTimeUnits(value, TimeUnit::HOURS);
}

/**
* Parse duration time units.
*
* Note that we don't differentiate between confirmed and unconfirmed
* durations. Unrecognised duration types are default the supplied default value->
*
* @param value BigIntegerPtr value
* @param defaultValue if value is NULL, use this value as the result 
* @return DurationPtr units
*/
TimeUnit DatatypeConverter::parseDurationTimeUnits(BigIntegerPtr value, TimeUnit defaultValue)
{
    TimeUnit result = defaultValue;

	if (!value)
		return result;

    switch (value->intValue())
    {
    case 3:
    case 35:
    {
        result = TimeUnit::MINUTES;
        break;
    }

    case 4:
    case 36:
    {
        result = TimeUnit::ELAPSED_MINUTES;
        break;
    }

    case 5:
    case 37:
    {
        result = TimeUnit::HOURS;
        break;
    }

    case 6:
    case 38:
    {
        result = TimeUnit::ELAPSED_HOURS;
        break;
    }

    case 7:
    case 39:
    case 53:
    {
        result = TimeUnit::DAYS;
        break;
    }

    case 8:
    case 40:
    {
        result = TimeUnit::ELAPSED_DAYS;
        break;
    }

    case 9:
    case 41:
    {
        result = TimeUnit::WEEKS;
        break;
    }

    case 10:
    case 42:
    {
        result = TimeUnit::ELAPSED_WEEKS;
        break;
    }

    case 11:
    case 43:
    {
        result = TimeUnit::MONTHS;
        break;
    }

    case 12:
    case 44:
    {
        result = TimeUnit::ELAPSED_MONTHS;
        break;
    }

    case 19:
    case 51:
    {
        result = TimeUnit::PERCENT;
        break;
    }

    case 20:
    case 52:
    {
        result = TimeUnit::ELAPSED_PERCENT;
        break;
    }

    default:
    {
        result = PARENT_FILE->getProjectHeader()->getDefaultDurationUnits();
        break;
    }
    }

    return (result);
}

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
BigIntegerPtr DatatypeConverter::printDurationTimeUnits(TimeUnit value, bool estimated)
{
    int result;

    //if (value == NULL)
    //{
    //    value = TimeUnit::HOURS;
    //}

    switch (value)
    {
        case MINUTES:
        {
        result = (estimated ? 35 : 3);
        break;
        }

        case ELAPSED_MINUTES:
        {
        result = (estimated ? 36 : 4);
        break;
        }

        case ELAPSED_HOURS:
        {
        result = (estimated ? 38 : 6);
        break;
        }

        case DAYS:
        {
        result = (estimated ? 39 : 7);
        break;
        }

        case ELAPSED_DAYS:
        {
        result = (estimated ? 40 : 8);
        break;
        }

        case WEEKS:
        {
        result = (estimated ? 41 : 9);
        break;
        }

        case ELAPSED_WEEKS:
        {
        result = (estimated ? 42 : 10);
        break;
        }

        case MONTHS:
        {
        result = (estimated ? 43 : 11);
        break;
        }

        case ELAPSED_MONTHS:
        {
        result = (estimated ? 44 : 12);
        break;
        }

        case PERCENT:
        {
        result = (estimated ? 51 : 19);
        break;
        }

        case ELAPSED_PERCENT:
        {
        result = (estimated ? 52 : 20);
        break;
        }

        default:
        case HOURS:
        {
        result = (estimated ? 37 : 5);
        break;
        }
    }

	return BigIntegerPtr(new BigInteger(result));
}

/**
* Parse priority.
*
*
* @param priority priority value
* @return Priority instance
*/
PriorityPtr DatatypeConverter::parsePriority(BigIntegerPtr priority)
{
    return (priority == NULL ? NULL : Priority::getInstance(priority->intValue()));
}

/**
* Print priority.
*
* @param priority Priority instance
* @return priority value
*/
BigIntegerPtr DatatypeConverter::printPriority(PriorityPtr priority)
{
    int result = Priority::MEDIUM;

    if (priority != NULL)
    {
        result = priority->getValue();
    }

	return BigIntegerPtr(new BigInteger(result));
}

/**
* Parse duration represented in thousandths of minutes.
*
* @param value duration value
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDurationInThousanthsOfMinutes(NumberPtr value)
{
    return parseDurationInFractionsOfMinutes(NULL, value, TimeUnit::MINUTES, 1000);
}

/**
* Parse duration represented in tenths of minutes.
*
* @param value duration value
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDurationInTenthsOfMinutes(NumberPtr value)
{
    return parseDurationInFractionsOfMinutes(NULL, value, TimeUnit::MINUTES, 10);
}

/**
* Parse duration represented in thousandths of minutes. 
* 
* @param header project header
* @param value duration value
* @param targetTimeUnit required output time units
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDurationInThousanthsOfMinutes(
	ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit)
{
    return parseDurationInFractionsOfMinutes(header, value, targetTimeUnit, 1000);
}

/**
* Parse duration represented as tenths of minutes.
* 
* @param header project header
* @param value duration value
* @param targetTimeUnit required output time units
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDurationInTenthsOfMinutes(
	ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit)
{
    return parseDurationInFractionsOfMinutes(header, value, targetTimeUnit, 10);
}

/**
* Print duration in thousandths of minutes.
*
* @param duration DurationPtr instance
* @return duration in thousandths of minutes
*/
BigIntegerPtr DatatypeConverter::printDurationInIntegerThousandthsOfMinutes(DurationPtr duration)
{
    BigIntegerPtr result;
    if (duration != NULL && duration->getDuration() != 0)
    {
		result.reset (new BigInteger((long) printDurationFractionsOfMinutes(duration, 1000)));
    }
    return result;
}

/**
* Print duration in thousandths of minutes.
*
* @param duration DurationPtr instance
* @return duration in thousandths of minutes
*/
BigDecimalPtr DatatypeConverter::printDurationInDecimalThousandthsOfMinutes(DurationPtr duration)
{
    BigDecimalPtr result;
    if (duration != NULL && duration->getDuration() != 0)
    {
		result.reset (new BigDecimal(printDurationFractionsOfMinutes(duration, 1000)));
    }
    return result;
}

/**
* Print duration in tenths of minutes.
*
* @param duration DurationPtr instance
* @return duration in tenths of minutes
*/
BigIntegerPtr DatatypeConverter::printDurationInIntegerTenthsOfMinutes(DurationPtr duration)
{
    BigIntegerPtr result;

    if (duration != NULL && duration->getDuration() != 0)
    {
		result.reset (new BigInteger((long)printDurationFractionsOfMinutes(duration, 10)));
    }

    return result;
}

/**
* Parse duration represented as an arbitrary fraction of minutes.
* 
* @param header project header
* @param value duration value
* @param targetTimeUnit required output time units
* @param factor required fraction of a minute
* @return DurationPtr instance
*/
DurationPtr DatatypeConverter::parseDurationInFractionsOfMinutes(
	ProjectHeader* header, NumberPtr value, TimeUnit targetTimeUnit, int factor)
{
	DurationPtr result;
    if (value != NULL)
    {
        result = Duration2::getInstance(value->intValue() / factor, TimeUnit::MINUTES);
        if (targetTimeUnit != result->getUnits())
        {
			result = result->convertUnits(targetTimeUnit, header);
        }
    }

    return (result);
}

/**
* Print a duration represented by an arbitrary fraction of minutes.
* 
* @param duration DurationPtr instance
* @param factor required factor
* @return duration represented as an arbitrary fraction of minutes
*/
double DatatypeConverter::printDurationFractionsOfMinutes(DurationPtr duration, int factor)
{
    double result = 0;

    if (duration != NULL)
    {
        result = duration->getDuration();

        switch (duration->getUnits())
        {
        case HOURS:
        case ELAPSED_HOURS:
        {
            result *= 60;
            break;
        }

        case DAYS:
        {
            result *= (60 * 8);
            break;
        }

        case ELAPSED_DAYS:
        {
            result *= (60 * 24);
            break;
        }

        case WEEKS:
        {
            result *= (60 * 8 * 5);
            break;
        }

        case ELAPSED_WEEKS:
        {
            result *= (60 * 24 * 7);
            break;
        }

        case MONTHS:
        {
            result *= (60 * 8 * 5 * 4);
            break;
        }

        case ELAPSED_MONTHS:
        {
            result *= (60 * 24 * 30);
            break;
        }

        case YEARS:
        {
            result *= (60 * 8 * 5 * 52);
            break;
        }

        case ELAPSED_YEARS:
        {
            result *= (60 * 24 * 365);
            break;
        }

        default:
        {
            break;
        }
        }
    }

    result *= factor;

    return (result);
}

/**
* Print rate.
*
* @param rate RatePtr instance
* @return rate value
*/
BigDecimalPtr DatatypeConverter::printRate(RatePtr rate)
{
    BigDecimalPtr result;
    if (rate != NULL && rate->getAmount() != 0)
    {
        result.reset (new BigDecimal(rate->getAmount()));
    }
    return result;
}

/**
* Parse rate.
*
* @param value rate value
* @return RatePtr instance
*/
RatePtr DatatypeConverter::parseRate(BigDecimalPtr value)
{
    RatePtr result;

    if (value != NULL)
    {
        result.reset (new Rate(value->doubleValue(), TimeUnit::HOURS));
    }

    return (result);
}

/**
* Print a day.
*
* @param day Day instance
* @return day value
*/
BigIntegerPtr DatatypeConverter::printDay(Day day)
{
	return BigIntegerPtr(day == NULL ? NULL : new BigInteger(day - 1));
}

/**
* Parse a day.
*
* @param value day value
* @return Day instance
*/
Day DatatypeConverter::parseDay(NumberPtr value)
{
    return (Day)(NumberHelper::getInt(value) + 1);
}

/**
* Parse a constraint type.
*
* @param value constraint type value
* @return ConstraintType instance
*/
ConstraintType DatatypeConverter::parseConstraintType(NumberPtr value)
{
    return (ConstraintType)(value->intValue());
}

/**
* Print a constraint type.
*
* @param value ConstraintType instance
* @return constraint type value
*/
BigIntegerPtr DatatypeConverter::printConstraintType(ConstraintType value)
{
	return BigIntegerPtr(new BigInteger((int)value));// (value == NULL ? NULL : );
}

/**
* Print a task UID.
*
* @param value task UID
* @return task UID string
*/
CString DatatypeConverter::printTaskUID(int value)
{
    ProjectFile* file = PARENT_FILE;
    if (file != NULL)
    {
        file->fireTaskWrittenEvent(file->getTaskByUniqueID(value));
    }
    return StringHelper::toString(value);
}

/**
* Parse a task UID.
*
* @param value task UID string
* @return task UID
*/
int DatatypeConverter::parseTaskUID(CString value)
{
	return _ttol(value);
}

/**
* Print a resource UID.
*
* @param value resource UID value
* @return resource UID string
*/
CString DatatypeConverter::printResource2UID(int value)
{
    ProjectFile* file = PARENT_FILE;
    if (file != NULL)
    {
        file->fireResource2WrittenEvent(file->getResource2ByUniqueID(value));
    }
    return (StringHelper::toString(value));
}

/**
* Parse a resource UID.
*
* @param value resource UID string
* @return resource UID value
*/
int DatatypeConverter::parseResource2UID(CString value)
{
	return _ttol(value);
}

/**
* Print a bool.
*
* @param value bool
* @return bool value
*/
CString DatatypeConverter::printBoolean(bool value)
{
    return (!value ? L"0" : L"1");
}

/**
* Parse a bool.
*
* @param value bool
* @return bool value
*/
bool DatatypeConverter::parseBoolean(CString value)
{
    return (value.GetLength() == 0 || value.GetAt(0) != '1' ? false : true);
}

/**
* Print a time value->
*
* @param value time value
* @return time value
*/
CString DatatypeConverter::printTime(CalendarPtr value)
{
    return (value == NULL ? L"" : getTimeFormat()->format(value->getTime()));
}

/**
* Parse a time value->
*
* @param value time value
* @return time value
*/
CalendarPtr DatatypeConverter::parseTime(CString value)
{
    CalendarPtr cal = NULL;
    if (value.GetLength() != 0)
    {
        cal = Calendar::getInstance();
        try
        {
			cal->setTime(getTimeFormat()->parse(value));
        }
        catch (toolkit::Exception&)
        {
			cal = NULL;
        }
    }
    return (cal);
}

/**
* Print a date time value->
*
* @param value date time value
* @return string representation
*/
CString DatatypeConverter::printDateTime(CalendarPtr value)
{
    return (value == NULL ? L"" : getDateFormat()->format(value->getTime()));
}

CString DatatypeConverter::printDateTime(DatePtr value)
{
	if (value == NULL) return L"";
	CalendarPtr cal = Calendar::getInstance();
	cal->setTime(value);
	cal->set(Calendar::SECOND, 0);
	return printDateTime(cal);
}

/**
* Parse a date time value->
*
* @param value string representation
* @return date time value
*/
CalendarPtr DatatypeConverter::parseDateTime(CString value)
{
    CalendarPtr result = NULL;

    if (value.GetLength() != 0)
    {
        try
        {
			result = Calendar::getInstance();
			result->setTime(getDateFormat()->parse(value));
        }
        catch (toolkit::Exception&)
        {
			result = NULL;
        }
    }

    return (result);
}

/**
* Print method for a string: returns the string unchanged.
* This is used to enable to string representation of an
* xsd:datetime to be generated by MPXJ.
* 
* @param value string value
* @return string value
*/
CString DatatypeConverter::printString(CString value)
{
    return (value);
}

/**
* Parse method for a string: returns the string unchanged.
* This is used to enable to string representation of an
* xsd:datetime to be processed by MPXJ.
   
* @param value string value
* @return string value
*/
CString DatatypeConverter::parseString(CString value)
{
    return (value);
}

/**
* This method is called to set the parent file for the current
* write operation. This allows task and resource write events
* to be captured and passed to any file listeners.
*
* @param file parent file instance
*/
void DatatypeConverter::setParentFile(ProjectFile* file)
{
    PARENT_FILE = file;
}

/**
* Retrieve a number formatter.
*
* @return NumberFormat instance
*/
const NumberFormat* DatatypeConverter::getNumberFormat()
{
	const NumberFormat* format = NUMBER_FORMAT.get();
    if (format == NULL)
    {
		NUMBER_FORMAT.reset (new NumberFormat(L"0.02d"));
        // XML numbers should use . as decimal separator and no grouping. 
        //format = new DecimalFormat("#.##", new DecimalFormatSymbols(Locale.US));
        //format.setGroup2ingUsed(false);
        //NUMBER_FORMAT.set(format);
		//GarbageCollector::add(new GcSharedPtr<NumberFormat>(NUMBER_FORMAT));
    }
	return NUMBER_FORMAT.get();
}

/**
* Retrieve a date formatter.
*
* @return DateFormat instance
*/
const DateFormat* DatatypeConverter::getDateFormat()
{
	const DateFormat* df = DATE_FORMAT.get();// .get();
    if (df == NULL)
    {
		DATE_FORMAT.reset (new DateFormat(L"%Y-%m-%dT%H:%M:%S"));
        //df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
        //df.setLenient(false);
		//GarbageCollector::add(new GcSharedPtr<DateFormat>(DATE_FORMAT));
    }
	return DATE_FORMAT.get();

}

/**
* Retrieve a time formatter.
*
* @return DateFormat instance
*/
const DateFormat* DatatypeConverter::getTimeFormat()
{
	const DateFormat* df = TIME_FORMAT.get();
    if (df == NULL)
    {
		TIME_FORMAT.reset (new DateFormat(L"%H:%M:%S"));
		TIME_FORMAT->setIgnoreDate(true);
        //df = new SimpleDateFormat("HH:mm:ss");
        //df.setLenient(false);
		//GarbageCollector::add(new GcSharedPtr<DateFormat>(TIME_FORMAT));
    }
	return TIME_FORMAT.get();

}