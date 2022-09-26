#pragma once

#include "MpxjEnum.h"

class ProjectHeader;

/**
 * This represents time durations as specified in an MPX file->
 */
class COMMONCONTROLS_API Duration2 : public ComparableT<Duration2>, public Object2
{
public:
	Duration2();

	/**
	 * Constructs an instance of this class from a duration amount and
	 * time unit type.
	 *
	 * @param duration amount of duration
	 * @param type time unit of duration
	 */
	Duration2(double duration, TimeUnit type);

	/**
	 * Constructs an instance of this class from a duration amount and
	 * time unit type.
	 *
	 * @param duration amount of duration
	 * @param type time unit of duration
	 */
	Duration2(int duration, TimeUnit type);

	Duration2(const Duration2& src);
	Duration2& operator = (const Duration2& src);

	/**
	 * This method is used to retrieve the size of the duration.
	 *
	 * @return size of the duration
	 */
	double getDuration();

	/**
	 * This method is used to retrieve the type of units the duration
	 * is expressed in. The valid types of units are found in the TimeUnit
	 * class.
	 *
	 * @return type of units
	 */
	TimeUnit getUnits();

	/**
	 * This method provides an <i>approximate</i> conversion between duration
	 * units. It does take into account the project defaults for number of hours
	 * in a day and a week, but it does not take account of calendar details.
	 * The results obtained from it should therefore be treated with caution.
	 *
	 * @param type target duration type
	 * @param defaults project header containing default values
	 * @return new DurationPtr instance
	 */
	std::shared_ptr<Duration2> convertUnits(TimeUnit type, ProjectHeader* defaults);

	/**
	 * This method provides an <i>approximate</i> conversion between duration
	 * units. It does take into account the project defaults for number of hours
	 * in a day and a week, but it does not take account of calendar details.
	 * The results obtained from it should therefore be treated with caution.
	 *
	 * @param duration duration value
	 * @param fromUnits units to convert from
	 * @param toUnits units to convert to
	 * @param defaults project header containing default values
	 * @return new DurationPtr instance
	 */
	static std::shared_ptr<Duration2> convertUnits(double duration, TimeUnit fromUnits,
		TimeUnit toUnits, ProjectHeader* defaults);

	/**
	 * This method provides an <i>approximate</i> conversion between duration
	 * units. It does take into account the project defaults for number of hours
	 * in a day and a week, but it does not take account of calendar details.
	 * The results obtained from it should therefore be treated with caution.
	 *
	 * @param duration duration value
	 * @param fromUnits units to convert from
	 * @param toUnits units to convert to
	 * @param minutesPerDay number of minutes per day
	 * @param minutesPerWeek number of minutes per week
	 * @param daysPerMonth number of days per month
	 * @return new DurationPtr instance
	 */
	static std::shared_ptr<Duration2> convertUnits(double duration, TimeUnit fromUnits,
		TimeUnit toUnits, double minutesPerDay, double minutesPerWeek, double daysPerMonth);

	/**
	 * Retrieve an DurationPtr instance. Use shared objects to
	 * represent common values for memory efficiency.
	 *
	 * @param duration duration value
	 * @param type duration type
	 * @return DurationPtr instance
	 */
	static std::shared_ptr<Duration2> getInstance(double duration, TimeUnit type);

	/**
	 * Retrieve an DurationPtr instance. Use shared objects to
	 * represent common values for memory efficiency.
	 *
	 * @param duration duration value
	 * @param type duration type
	 * @return DurationPtr instance
	 */
	static std::shared_ptr<Duration2> getInstance(int duration, TimeUnit type);

	//  /**
	//   * {@inheritDoc}
	//   */
	//bool equals(Object o);

	///**
	// * {@inheritDoc}
	// */
	//int hashCode();

	/**
	 * {@inheritDoc}
	 */
	int compareTo(Duration2* rhs);

	/**
	 * Equality test for duration component of a DurationPtr instance.
	 * Note that this does not take into account the units - use with care!
	 *
	 * @param rhs duration to compare
	 * @return true if duration components are equal, within the allowable delta
	 */
	bool durationComponentEquals(Duration2* rhs);

	/**
	 * Equality test for two duration values.
	 *
	 * @param lhs duration value
	 * @param rhs duration value
	 * @return true if duration values are equal, within the allowable delta
	 */
	static bool durationValueEquals(double lhs, double rhs);

	/**
	 * {@inheritDoc}
	 */
	virtual CString toString();

private:
	static void initZeroDurations();
	/**
	 * DurationPtr amount.
	 */
	double m_duration;

	/**
	 * DurationPtr type.
	 */
	TimeUnit m_units;

	static Duration2 ZERO_DURATIONS[14];
	//static DurationPtr* ZERO_DURATIONS[14];
};

typedef std::shared_ptr<Duration2> DurationPtr;
