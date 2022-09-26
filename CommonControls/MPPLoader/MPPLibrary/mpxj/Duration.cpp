#include "stdafx.h"
#include "Duration.h"
#include "ProjectHeader.h"
#include "common\NumberHelper.h"

//#include "StaticResource2Clear.h"

Duration2 Duration2::ZERO_DURATIONS[14];

Duration2::Duration2()
{
	m_duration = 0;
	m_units = MINUTES;
}
/**
* Constructs an instance of this class from a duration amount and
* time unit type.
*
* @param duration amount of duration
* @param type time unit of duration
*/
Duration2::Duration2(double duration, TimeUnit type)
{
	m_duration = duration;
	m_units = type;
}

/**
* Constructs an instance of this class from a duration amount and
* time unit type.
*
* @param duration amount of duration
* @param type time unit of duration
*/
Duration2::Duration2(int duration, TimeUnit type)
{
	m_duration = duration;
	m_units = type;
}

Duration2::Duration2(const Duration2& src)
{
	m_duration = src.m_duration;
	m_units = src.m_units;
}

Duration2& Duration2::operator = (const Duration2& src)
{
	m_duration = src.m_duration;
	m_units = src.m_units;

	return *this;
}

/**
* This method is used to retrieve the size of the duration.
*
* @return size of the duration
*/
double Duration2::getDuration()
{
	return (m_duration);
}

/**
* This method is used to retrieve the type of units the duration
* is expressed in. The valid types of units are found in the TimeUnit
* class.
*
* @return type of units
*/
TimeUnit Duration2::getUnits()
{
	return (m_units);
}

/**
* This method provides an <i>approximate</i> conversion between duration
* units. It does take into account the project defaults for number of hours
* in a day and a week, but it does not take account of calendar details.
* The results obtained from it should therefore be treated with caution.
*
* @param type target duration type
* @param defaults project header containing default values
* @return new Duration instance
*/
DurationPtr Duration2::convertUnits(TimeUnit type, ProjectHeader* defaults)
{
	return convertUnits(m_duration, m_units, type, defaults);
}

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
* @return new Duration instance
*/
DurationPtr Duration2::convertUnits(double duration,
	TimeUnit fromUnits, TimeUnit toUnits, ProjectHeader* defaults)
{
	return convertUnits(duration, fromUnits, toUnits,
		defaults->getMinutesPerDay()->doubleValue(),
		defaults->getMinutesPerWeek()->doubleValue(),
		defaults->getDaysPerMonth()->doubleValue());
}

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
* @return new Duration instance
*/
DurationPtr Duration2::convertUnits(double duration, TimeUnit fromUnits,
	TimeUnit toUnits, double minutesPerDay, double minutesPerWeek, double daysPerMonth)
{
	switch (fromUnits)
	{
		case YEARS:
		{
			duration *= (minutesPerWeek * 52);
			break;
		}

		case ELAPSED_YEARS:
		{
			duration *= (60 * 24 * 7 * 52);
			break;
		}

		case MONTHS:
		{
			duration *= (minutesPerDay * daysPerMonth);
			break;
		}

		case ELAPSED_MONTHS:
		{
			duration *= (60 * 24 * 30);
			break;
		}

		case WEEKS:
		{
			duration *= minutesPerWeek;
			break;
		}

		case ELAPSED_WEEKS:
		{
			duration *= (60 * 24 * 7);
			break;
		}

		case DAYS:
		{
			duration *= minutesPerDay;
			break;
		}

		case ELAPSED_DAYS:
		{
			duration *= (60 * 24);
			break;
		}

		case HOURS:
		case ELAPSED_HOURS:
		{
			duration *= 60;
			break;
		}

		default:
		{
			 break;
		}
	}

	if (toUnits != TimeUnit::MINUTES && toUnits != TimeUnit::ELAPSED_MINUTES)
	{
		switch (toUnits)
		{
			case HOURS:
			case ELAPSED_HOURS:
			{
				duration /= 60;
				break;
			}

			case DAYS:
			{
				if (minutesPerDay != 0)
				{
					duration /= minutesPerDay;
				}
				else
				{
					duration = 0;
				}
				break;
			}

			case ELAPSED_DAYS:
			{
				duration /= (60 * 24);
				break;
			}

			case WEEKS:
			{
				if (minutesPerWeek != 0)
				{
					duration /= minutesPerWeek;
				}
				else
				{
					duration = 0;
				}
				break;
			}

			case ELAPSED_WEEKS:
			{
				duration /= (60 * 24 * 7);
				break;
			}

			case MONTHS:
			{
				if (minutesPerDay != 0 && daysPerMonth != 0)
				{
					duration /= (minutesPerDay * daysPerMonth);
				}
				else
				{
					duration = 0;
				}
				break;
			}

			case ELAPSED_MONTHS:
			{
				duration /= (60 * 24 * 30);
				break;
			}

			case YEARS:
			{
				if (minutesPerWeek != 0)
				{
					duration /= (minutesPerWeek * 52);
				}
				else
				{
					duration = 0;
				}
				break;
			}

			case ELAPSED_YEARS:
			{
				duration /= (60 * 24 * 7 * 52);
				break;
			}

			default:
			{
				break;
			}
		}
	}

	return Duration2::getInstance(duration, toUnits);
}

/**
* Retrieve an Duration instance. Use shared objects to
* represent common values for memory efficiency.
*
* @param duration duration value
* @param type duration type
* @return Duration instance
*/
DurationPtr Duration2::getInstance(double duration, TimeUnit type)
{
	if (duration == 0)
	{
		initZeroDurations();
		return DurationPtr (new Duration2 (ZERO_DURATIONS[type]));
	}
	else
	{
		return DurationPtr(new Duration2(duration, type));
	}
}

/**
* Retrieve an Duration instance. Use shared objects to
* represent common values for memory efficiency.
*
* @param duration duration value
* @param type duration type
* @return Duration instance
*/
DurationPtr Duration2::getInstance(int duration, TimeUnit type)
{
	//Duration result;
	if (duration == 0)
	{
		initZeroDurations();
		return DurationPtr(new Duration2(ZERO_DURATIONS[type]));
	}
	else
	{
		return DurationPtr(new Duration2(duration, type));
	}
}

///**
//* {@inheritDoc}
//*/
//bool Duration::equals(Object o)
//{
//    bool result = false;
//    if (o instanceof Duration)
//    {
//        Duration rhs = (Duration) o;
//        result = durationComponentEquals(rhs) && m_units == rhs.m_units;
//    }
//    return result;
//}

///**
//* {@inheritDoc}
//*/
//int Duration::hashCode()
//{
//	return (m_units.getValue() + (int)m_duration);
//}

/**
* {@inheritDoc}
*/
int Duration2::compareTo(Duration2* rhs)
{
	DurationPtr temp;
	if (m_units != rhs->m_units)
	{
		temp = convertUnits(rhs->m_duration, rhs->m_units, m_units, (8 * 60), (5 * 8 * 60), 20);
		rhs = temp.get();
	}

	return durationComponentEquals(rhs) ? 0 : m_duration < rhs->m_duration ? -1 : 1;
}

/**
* Equality test for duration component of a Duration instance.
* Note that this does not take into account the units - use with care!
*
* @param rhs duration to compare
* @return true if duration components are equal, within the allowable delta
*/
bool Duration2::durationComponentEquals(Duration2* rhs)
{
	return durationValueEquals(m_duration, rhs->m_duration);
}

/**
* Equality test for two duration values.
*
* @param lhs duration value
* @param rhs duration value
* @return true if duration values are equal, within the allowable delta
*/
bool Duration2::durationValueEquals(double lhs, double rhs)
{
	return NumberHelper::equals(lhs, rhs, 0.00001);
}

/**
* {@inheritDoc}
*/
CString Duration2::toString()
{
	CString strDur;
	strDur.Format(L"%0.3f", m_duration);
	strDur = CStringToolkit::TrimNumberString(strDur);
	return (strDur + TimeUnit2Text(m_units));
}

void Duration2::initZeroDurations()
{
	if (ZERO_DURATIONS[TimeUnit::HOURS].getUnits() == TimeUnit::HOURS)
		return;

	ZERO_DURATIONS[TimeUnit::MINUTES] = Duration2(0, TimeUnit::MINUTES);
	ZERO_DURATIONS[TimeUnit::HOURS] = Duration2(0, TimeUnit::HOURS);
	ZERO_DURATIONS[TimeUnit::DAYS] = Duration2(0, TimeUnit::DAYS);
	ZERO_DURATIONS[TimeUnit::WEEKS] = Duration2(0, TimeUnit::WEEKS);
	ZERO_DURATIONS[TimeUnit::MONTHS] = Duration2(0, TimeUnit::MONTHS);
	ZERO_DURATIONS[TimeUnit::YEARS] = Duration2(0, TimeUnit::YEARS);
	ZERO_DURATIONS[TimeUnit::PERCENT] = Duration2(0, TimeUnit::PERCENT);
	ZERO_DURATIONS[TimeUnit::ELAPSED_MINUTES] = Duration2(0, TimeUnit::ELAPSED_MINUTES);
	ZERO_DURATIONS[TimeUnit::ELAPSED_HOURS] = Duration2(0, TimeUnit::ELAPSED_HOURS);
	ZERO_DURATIONS[TimeUnit::ELAPSED_DAYS] = Duration2(0, TimeUnit::ELAPSED_DAYS);
	ZERO_DURATIONS[TimeUnit::ELAPSED_WEEKS] = Duration2(0, TimeUnit::ELAPSED_WEEKS);
	ZERO_DURATIONS[TimeUnit::ELAPSED_MONTHS] = Duration2(0, TimeUnit::ELAPSED_MONTHS);
	ZERO_DURATIONS[TimeUnit::ELAPSED_YEARS] = Duration2(0, TimeUnit::ELAPSED_YEARS);
	ZERO_DURATIONS[TimeUnit::ELAPSED_PERCENT] = Duration2(0, TimeUnit::ELAPSED_PERCENT);

	//class ZeroClear : public ClearProxy
	//{
	//public:
	//	void Clear()
	//	{
	//		for (int i = 0; i < 14; i++)
	//			ZERO_DURATIONS[i].reset();
	//	}
	//};

	//StaticResource2Clear::GetInstance()->Register(new ZeroClear());
}