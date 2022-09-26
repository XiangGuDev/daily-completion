#pragma once

#include "MPPComponent.h"

/**
 * This class represents the common structure of Props files found in
 * Microsoft Project MPP files. The MPP8 and MPP9 file formats both
 * implement Props files slightly differently, so this class contains
 * the shared implementation detail, with specific implementations for
 * MPP8 and MPP9 Props files found in the Props8 and Props9 classes.
 */
class Props : public MPPComponent
{
public:
	/**
	 * Retrieve property data as a BYTE array.
	 *
	 * @param type Type identifier
	 * @return  BYTE array of data
	 */
	ByteArrayPtr getByteArray2(int type);

	/**
	 * Retrieves a BYTE value from the property data.
	 *
	 * @param type Type identifier
	 * @return BYTE value
	 */
	BYTE getByte(int type);

	/**
	 * Retrieves a short int value from the property data.
	 *
	 * @param type Type identifier
	 * @return short int value
	 */
	int getShort(int type);

	/**
	 * Retrieves an integer value from the property data.
	 *
	 * @param type Type identifier
	 * @return integer value
	 */
	int getInt(int type);

	/**
	 * Retrieves a double value from the property data.
	 *
	 * @param type Type identifier
	 * @return double value
	 */
	double getDouble(int type);

	/**
	 * Retrieves a timestamp from the property data.
	 *
	 * @param type Type identifier
	 * @return timestamp
	 */
	DatePtr getTime(int type);

	/**
	 * Retrieves a timestamp from the property data.
	 *
	 * @param type Type identifier
	 * @return timestamp
	 */
	DatePtr getTimestamp(int type);

	/**
	 * Retrieves a bool value from the property data.
	 *
	 * @param type Type identifier
	 * @return bool value
	 */
	bool getBoolean(int type);

	/**
	 * Retrieves a string value from the property data.
	 *
	 * @param type Type identifier
	 * @return string value
	 */
	CString getUnicodeString(int type);

	/**
	 * Retrieves a date value from the property data.
	 *
	 * @param type Type identifier
	 * @return string value
	 */
	DatePtr getDate(int type);

	/**
	 * Retrieve the set of keys represented by this instance.
	 *
	 * @return key set
	 */
	std::shared_ptr<Set<int>> keySet();

	///**
	// * This method dumps the contents of this properties block as a CString.
	// * Note that this facility is provided as a debugging aid.
	// *
	// * @return formatted contents of this block
	// */
	//virtual CString toString();

	/**
	 * Data types.
	 */
	static const long PROJECT_START_DATE = 37748738;
	static const long PROJECT_FINISH_DATE = 37748739;
	static const long SCHEDULE_FROM = 37748740;
	static const long DEFAULT_CALENDAR_NAME = 37748750;
	static const long CURRENCY_SYMBOL = 37748752;
	static const long CURRENCY_PLACEMENT = 37748753;
	static const long CURRENCY_DIGITS = 37748754;

	static const long DURATION_UNITS = 37748757;
	static const long WORK_UNITS = 37748758;
	static const long TASK_UPDATES_RESOURCE = 37748761;
	static const long SPLIT_TASKS = 37748762;
	static const long START_TIME = 37748764;
	static const long MINUTES_PER_DAY = 37748765;
	static const long MINUTES_PER_WEEK = 37748766;
	static const long STANDARD_RATE = 37748767;
	static const long OVERTIME_RATE = 37748768;
	static const long END_TIME = 37748769;

	static const long WEEK_START_DAY = 37748773;
	static const long FISCAL_YEAR_START_MONTH = 37748780;
	static const long HONOR_CONSTRAINTS = 37748794;
	static const long FISCAL_YEAR_START = 37748801;
	static const long EDITABLE_ACTUAL_COSTS = 37748802;

	static const long DAYS_PER_MONTH = 37753743;

	static const long CURRENCY_CODE = 37753787;

	static const long CALCULATE_MULTIPLE_CRITICAL_PATHS = 37748793;

	static const long TASK_FIELD_NAME_ALIASES = 1048577;
	static const long RESOURCE_FIELD_NAME_ALIASES = 1048578;

	static const long PASSWORD_FLAG = 893386752;

	static const long PROTECTION_PASSWORD_HASH = 893386756;

	static const long WRITE_RESERVATION_PASSWORD_HASH = 893386757;

	static const long ENCRYPTION_CODE = 893386759;

	static const long STATUS_DATE = 37748805;

	static const long SUBPROJECT_COUNT = 37748868;
	static const long SUBPROJECT_DATA = 37748898;
	static const long SUBPROJECT_TASK_COUNT = 37748900;

	static const long DEFAULT_CALENDAR_HOURS = 37753736;

	static const long TASK_FIELD_ATTRIBUTES = 37753744;

	static const long FONT_BASES = 54525952;

	static const long AUTO_FILTER = 893386767;

	static const long PROJECT_FILE_PATH = 893386760;

	static const long HYPERLINK_BASE = 37748810;

	static const long RESOURCE_CREATION_DATE = 205521219;

	static const long SHOW_PROJECT_SUMMARY_TASK = 54525961;

	static const long TASK_FIELD_MAP = 131092;
	static const long TASK_FIELD_MAP2 = 50331668;
	static const long ENTERPRISE_CUSTOM_FIELD_MAP = 37753797; // MPP14 37753768?      

	static const long RESOURCE_FIELD_MAP = 131093;
	static const long RESOURCE_FIELD_MAP2 = 50331669;

	static const long RELATION_FIELD_MAP = 131094;

	static const long ASSIGNMENT_FIELD_MAP = 131095;
	static const long ASSIGNMENT_FIELD_MAP2 = 50331671;

	static const long BASELINE_DATE = 37753749;
	static const long BASELINE1_DATE = 37753750;
	static const long BASELINE2_DATE = 37753751;
	static const long BASELINE3_DATE = 37753752;
	static const long BASELINE4_DATE = 37753753;
	static const long BASELINE5_DATE = 37753754;
	static const long BASELINE6_DATE = 37753755;
	static const long BASELINE7_DATE = 37753756;
	static const long BASELINE8_DATE = 37753757;
	static const long BASELINE9_DATE = 37753758;
	static const long BASELINE10_DATE = 37753759;

protected:
	Map2<int, ByteArrayPtr> m_map;// = new TreeMap<int, ByteArrayPtr>();
};

typedef std::shared_ptr<Props> PropsPtr;