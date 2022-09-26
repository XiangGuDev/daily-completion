#pragma once

#include "Date.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Calendar
//
class Calendar : public Object2
{
public:
	enum FIELD
	{
		//ERA = 0,
		YEAR = 1,
		//MONTH = 2,
		//WEEK_OF_YEAR = 3,
		//WEEK_OF_MONTH = 4,
		//DATE = 5,
		//DAY_OF_MONTH = 5,
		DAY_OF_YEAR = 6,
		DAY_OF_WEEK = 7,
		//DAY_OF_WEEK_IN_MONTH = 8,
		//AM_PM = 9,
		//HOUR = 10,
		HOUR_OF_DAY = 11,
		MINUTE = 12,
		SECOND = 13,
		//MILLISECOND = 14,
		//ZONE_OFFSET = 15,
	};

	Calendar();

	static std::shared_ptr<Calendar> getInstance();

	void setTime(DatePtr date);

	DatePtr getTime();

	int getFirstDayOfWeek();

	int get(FIELD field);

	void set(FIELD field, int value);

	/**
	* 得到指定字段的最大值
	* @param field	目前只支持 DAY_OF_YEAR
	*/
	int getActualMaximum(FIELD field);

	/**
	* add()函数会在逻辑上改变其它字段，使结果正确。
	* @param field	目前只支持 DAY_OF_YEAR
	*/
	void add(FIELD field, int value);

	/**
	* roll()函数只会比相应的字段进行处理，不会智能的对其它字段也进行逻辑上的改变。
	* @param field	目前只支持 YEAR
	*/
	void roll(FIELD field, int value);

	virtual CString toString();

protected:
#ifdef _DEBUG
	CString _textDate;
#endif

protected:
	time_t	_time;	// 当前设置的时间
};

typedef std::shared_ptr<Calendar> CalendarPtr;

/****************************************************************************************
Java 根据指定日期计算所在周的周一和周日
1.
5.      private static void convertWeekByDate(Date time) {
6.
7.          SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd"); //设置时间格式
8.          Calendar cal = Calendar.getInstance();
9.          cal.setTime(time);
10.         //判断要计算的日期是否是周日，如果是则减一天计算周六的，否则会出问题，计算到下一周去了
11.         int dayWeek = cal.get(Calendar.DAY_OF_WEEK);//获得当前日期是一个星期的第几天
12.         if(1 == dayWeek) {
13.             cal.add(Calendar.DAY_OF_MONTH, -1);
14.         }
15.         System.out.println("要计算日期为:"+sdf.format(cal.getTime())); //输出要计算日期
16.         cal.setFirstDayOfWeek(Calendar.MONDAY);//设置一个星期的第一天，按中国的习惯一个星期的第一天是星期一
17.         int day = cal.get(Calendar.DAY_OF_WEEK);//获得当前日期是一个星期的第几天
18.         cal.add(Calendar.DATE, cal.getFirstDayOfWeek()-day);//根据日历的规则，给当前日期减去星期几与一个星期第一天的差值
19.         String imptimeBegin = sdf.format(cal.getTime());
20.         System.out.println("所在周星期一的日期："+imptimeBegin);
21.         cal.add(Calendar.DATE, 6);
22.         String imptimeEnd = sdf.format(cal.getTime());
23.         System.out.println("所在周星期日的日期："+imptimeEnd);
24.
25.     }


Calendar中add()和roll()函数的用法
一、取某个时间点后的整点时刻。例如1984年7月7日15:23:05后的整点时刻即为1984-07-07 16:00:00。
实现如下：
Calendar calendar = Calendar.getInstance();
calendar.add(Calendar.HOUR_OF_DAY, 1);//小时上加1
calendar.set(Calendar.MINUTE, 0);//分钟设为0
calendar.set(Calendar.SECOND, 0);//秒钟设为0

二、取某个日历之前的某个月。例如要取2002年1月12号之前1个月的时间，应该是2001年12月12日。
Calendar calendar = Calendar.getInstance();
calendar.set(2002, 0, 12);//代表2002年1月12日
calendar.add(Calendar.MONTH, -1);//这样就将日期设置成了2001年12月12日。
System.out.println(calendar.getTime().toString());

但是这种情况如果用roll()来实现。如以下代码：
Calendar calendar = Calendar.getInstance();
calendar.set(2002, 0, 12);//代表2002年1月12日
calendar.roll(Calendar.MONTH, -1);//这样就将日期设置成了2002年12月12日。
System.out.println(calendar.getTime().toString());

结论：roll()函数处理，只会比相应的字段进行处理，不会智能的对其它字段也进行逻辑上的改变。
但是add()函数会在逻辑上改变其它字段，使结果正确。

注：Calendar中的Month是从0-11的，0代表1月，11代表12月。

****************************************************************************************/