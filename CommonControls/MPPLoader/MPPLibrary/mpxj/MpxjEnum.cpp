#include "stdafx.h"
#include "MpxjEnum.h"

/**
* Represents the recurrence type.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(RecurrenceType)
{
	{ DAILY, _T("Daily") },
	{ WEEKLY, _T("Weekly") },
	{ MONTHLY, _T("Monthly") },
	{ YEARLY, _T("Yearly") },
	{ (RecurrenceType)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(RecurrenceType);

/**
* Instances of this class represent enumerated resource type values.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(Resource2Type)
{
	{ RT_MATERIAL, _T("Material") },
	{ RT_WORK, _T("Work") },
	{ RT_COST, _T("Cost") },
	{ (Resource2Type)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(Resource2Type);

/**
* This class is used to represent a relation type. It provides a mapping
* between the textual description of a relation type found in an MPX
* file, and an enumerated representation that can be more easily manipulated
* programatically.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(RelationType)
{
	{ FINISH_FINISH, _T("FF") },
	{ FINISH_START, _T("FS") },
	{ START_FINISH, _T("SF") },
	{ START_START, _T("SS") },
	{ (RelationType)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(RelationType);

/**
* This class contains utility functions allowing time unit specifications
* to be parsed and formatted.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(TimeUnit)
{
	{ MINUTES, _T("m") },
	{ HOURS, _T("h") },
	{ DAYS, _T("d") },
	{ WEEKS, _T("w") },
	{ MONTHS, _T("mo") },
	{ PERCENT, _T("%") },
	{ YEARS, _T("y") },
	{ ELAPSED_MINUTES, _T("em") },
	{ ELAPSED_HOURS, _T("eh") },
	{ ELAPSED_DAYS, _T("ed") },
	{ ELAPSED_WEEKS, _T("ew") },
	{ ELAPSED_MONTHS, _T("emo") },
	{ ELAPSED_YEARS, _T("ey") },
	{ ELAPSED_PERCENT, _T("e%") },
	{ (TimeUnit)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(TimeUnit);

/**
* This class represents the enumeration of the valid types of view.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(MppViewType)
{
	{ VIEW_UNKNOWN, L"UNKNOWN" },
	{ VIEW_GANTT_CHART, L"GANTT_CHART" },
	{ VIEW_NETWORK_DIAGRAM, L"NETWORK_DIAGRAM" },
	{ VIEW_RELATIONSHIP_DIAGRAM, L"RELATIONSHIP_DIAGRAM" },
	{ VIEW_TASK_FORM, L"TASK_FORM" },
	{ VIEW_TASK_SHEET, L"TASK_SHEET" },
	{ VIEW_RESOURCE_FORM, L"RESOURCE_FORM" },
	{ VIEW_RESOURCE_SHEET, L"RESOURCE_SHEET" },
	{ VIEW_RESOURCE_GRAPH, L"RESOURCE_GRAPH" },
	{ VIEW_TASK_DETAILS_FORM, L"TASK_DETAILS_FORM" },
	{ VIEW_TASK_NAME_FORM, L"TASK_NAME_FORM" },
	{ VIEW_RESOURCE_NAME_FORM, L"RESOURCE_NAME_FORM" },
	{ VIEW_CALENDAR, L"CALENDAR" },
	{ VIEW_TASK_USAGE, L"TASK_USAGE" },
	{ VIEW_RESOURCE_USAGE, L"RESOURCE_USAGE" },
	{ (MppViewType)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(MppViewType);

/**
* Represents the pattern used to fill a group.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(BackgroundPattern)
{
	{ BP_TRANSPARENT, L"Transparent" },
	{ BP_SOLID, L"Solid" },
	{ BP_LIGHTDOTTED, L"Light Dotted" },
	{ BP_DOTTED, L"Dotted" },
	{ BP_HEAVYDOTTED, L"Heavy Dotted" },
	{ BP_BACKSLASH, L"Back Slash" },
	{ BP_FORWARDSLASH, L"Forward Slash" },
	{ BP_INVERSEBACKSLASH, L"Inverse Back Slash" },
	{ BP_INVERSEFORWARDSLASH, L"Inverse Forward Slash" },
	{ BP_LIGHTVERTICALSTRIPE, L"Light Vertical Stripe" },
	{ BP_HEAVYVERTICALSTRIPE, L"Heavy Vertical Stripe" },
	{ BP_CHECKERED, L"Checkered" },
	{ BP_DENSEFORWARDSLASH, L"Dense Forward Slash" },
	{ BP_INVERSECHECKERED, L"Inverse Checkered" },
	{ (BackgroundPattern)NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION2(BackgroundPattern);


/**
* This class represents the grid line styles used by Microsoft Project.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(LineStyle2)
{
	{ LS_NONE, L"None" },
	{ LS_SOLID, L"Solid" },
	{ LS_DOTTED1, L"Dotted1" },
	{ LS_DOTTED2, L"Dotted2" },
	{ LS_DASHED, L"Dashed" },
	{ (LineStyle2)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(LineStyle2);

/**
* Class representing how links are drawn on a Gantt chart.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(LinkStyle)
{
	{ NONE, L"None" },
	{ END_TOP, L"End Top" },
	{ END_START, L"End Start" },
	{ (LinkStyle)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(LinkStyle);


/**
* This enum represents the colors used by Microsoft Project.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(ColorType)
{
	{ BLACK, L"Black", RGB(0, 0, 0) },
	{ RED, L"Red", RGB(255, 0, 0) },
	{ YELLOW, L"Yellow", RGB(255, 255, 0) },
	{ LIME, L"Lime", RGB(148, 255, 148) },
	{ AQUA, L"Aqua", RGB(194, 220, 255) },
	{ BLUE, L"Blue", RGB(0, 0, 255) },
	{ FUSCHIA, L"Fuschia", RGB(255, 0, 255) },
	{ WHITE, L"White", RGB(255, 255, 255) },
	{ MAROON, L"Maroon", RGB(128, 0, 0) },
	{ GREEN, L"Green", RGB(0, 128, 0) },
	{ OLIVE, L"Olive", RGB(128, 128, 0) },
	{ NAVY, L"Navy", RGB(0, 0, 128) },
	{ PURPLE, L"Purple", RGB(128, 0, 128) },
	{ TEAL, L"Teal", RGB(0, 128, 128) },
	{ GRAY, L"Gray", RGB(128, 128, 128) },
	{ SILVER, L"Silver", RGB(192, 192, 192) },
	{ AUTOMATIC, L"Automatic", NULL },
	{ (ColorType)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(ColorType);

/**
* Class representing the label alignment on a Gantt chart timescale.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(TimescaleAlignment)
{
	{ TIMEALIGN_LEFT, L"Left" },
	{ TIMEALIGN_CENTER, L"Center" },
	{ TIMEALIGN_RIGHT, L"Right" },
	{ (TimescaleAlignment)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(TimescaleAlignment);


/**
* This class represents daily, weekly or monthly time intervals.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(Interval)
{
	{ INTERVAL_DAILY, L"Daily" },
	{ INTERVAL_WEEKLY, L"Weekly" },
	{ INTERVAL_MONTHLY, L"Monthly" },
	{ (Interval)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(Interval);

/**
* Class representing how non-working time is shown on a Gantt chart.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(NonWorkingTimeStyle)
{
	{ BEHIND, L"Behind" },
	{ IN_FRONT, L"In Front" },
	{ DO_NOT_DRAW, L"Do Not Draw" },
	{ (NonWorkingTimeStyle)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(NonWorkingTimeStyle);

/**
* Represents the pattern used to fill the middle section of a Gantt bar.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(ChartPattern)
{
	{ CP_TRANSPARENT, L"Transparent" },
	{ CP_SOLID, L"Solid" },
	{ CP_LIGHTDOTTED, L"Light Dotted" },
	{ CP_DOTTED, L"Dotted" },
	{ CP_HEAVYDOTTED, L"Heavy Dotted" },
	{ CP_BACKSLASH, L"Back Slash" },
	{ CP_FORWARDSLASH, L"Forward Slash" },
	{ CP_CHECKERED, L"Checkered" },
	{ CP_VERTICALSTRIPE, L"Vertical Stripe" },
	{ CP_HORIZONTALSTRIPE, L"Horizontal Stripe" },
	{ CP_GRID, L"Grid" },
	{ CP_SOLIDHAIRY, L"Solid Hairy" },
	{ (ChartPattern)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(ChartPattern);

/**
* Class representing the units which may be shown on a Gantt chart timescale.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(TimescaleUnits)
{
	{ TU_NONE, L"None" },
	{ TU_MINUTES, L"Minutes" },
	{ TU_HOURS, L"Hours" },
	{ TU_DAYS, L"Days" },
	{ TU_WEEKS, L"Weeks" },
	{ TU_THIRDS_OF_MONTHS, L"Thirds of Months" },
	{ TU_MONTHS, L"Months" },
	{ TU_QUARTERS, L"Quarters" },
	{ TU_HALF_YEARS, L"Half Years" },
	{ TU_YEARS, L"Years" },
	{ (TimescaleUnits)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(TimescaleUnits);

/**
* Enumeration representing the formats which may be shown on a Gantt chart timescale.
*/
BEGIN_ENUMTYPENAME_CONVERSION2(TimescaleFormat)
{
	{TF_NONE, L"None" },

	//
	// Years
	//
	{ YEAR_YYYY, L"2002, 2003, ..." },
	{ YEAR_XYY, L"'02, '03, ..." },
	{ YEAR_YY, L"02, 03, ..." },
	{ YEAR_YEAR_START, L"Year 1, Year 2 ... (From Start)" },
	{ YEAR_Y_START, L"Y1, Y2, Y3, ... (From Start)" },
	{ YEAR_1_START, L"1, 2, 3, 4, ...(From Start)" },
	{ YEAR_YEAR_END, L"Year 2, Year 1 ... (From End)" },
	{ YEAR_Y_END, L"Y3, Y2, Y1, ... (From End)" },
	{ YEAR_1_END, L"4, 3, 2, 1, ... (From End)" },

	//
	// Half years
	//
	{ HALFYEAR_1_HALF, L"1st Half, 2nd Half, ..." },
	{ HALFYEAR_HALF_1_YYYY, L"Half 1, 2002, Half2, 2002, ..." },
	{ HALFYEAR_HALF_1, L"Half 1, Half2, ..." },
	{ HALFYEAR_H1_XYY, L"H1 '02, H2 '02, ..." },
	{ HALFYEAR_H1, L"H1, H2, ..." },
	{ HALFYEAR_1, L"1, 2, ..." },
	{ HALFYEAR_1HYY, L"1H02, 2H02, ..." },
	{ HALFYEAR_HALF_1_START, L"Half 1, Half 2, Half 3, ... (From Start)" },
	{ HALFYEAR_H1_START, L"H1, H2, H3, ... (From Start)" },
	{ HALFYEAR_1_START, L"1, 2, 3, ... (From Start)" },
	{ HALFYEAR_HALF_1_END, L"Half 3, Half 2, Half 1, ... (From End)" },
	{ HALFYEAR_H1_END, L"H3, H2, H1, ... (From End)" },
	{ HALFYEAR_1_END, L"3, 2, 1, ... (From End)" },

	//
	// Quarters
	//
	{ QUARTER_1_QUARTER, L"1st Quarter" },
	{ QUARTER_QTR_1_YYYY, L"Qtr 1, 2002" },
	{ QUARTER_QTR_1, L"Qtr 1, Qtr2, ..." },
	{ QUARTER_Q1_XYY, L"Q1 '02, Q2 '02, ..." },
	{ QUARTER_Q1, L"Q1, Q2, ..." },
	{ QUARTER_1, L"1, 2, ..." },
	{ QUARTER_1QYY, L"1Q02, 2Q02, ..." },
	{ QUARTER_QUARTER_1_START, L"Quarter 1, Quarter 2, ...(From Start)" },
	{ QUARTER_Q1_START, L"Q1, Q2, Q3, Q4, ... (From Start)" },
	{ QUARTER_1_START, L"1, 2, 3, 4, ... (From Start)" },
	{ QUARTER_QUARTER_1_END, L"Quarter 2 Quarter 1, ... (From End)" },
	{ QUARTER_Q1_END, L"Q4, Q3, Q2, Q1, ... (From End)" },
	{ QUARTER_1_END, L"4, 3, 2, 1, (From End)" },

	//
	// Months
	//
	{ MONTHS_MMMM_YYYY, L"January 2002" },
	{ MONTHS_MMM_XYY, L"Jan '02" },
	{ MONTHS_MMMM, L"January" },
	{ MONTHS_MMM, L"Jan, Feb, ..." },
	{ MONTHS_M, L"J, F, ..." },
	{ MONTHS_1, L"1, 2, ..." },
	{ MONTHS_1_XYY, L"1 '02" },
	{ MONTHS_1SYY, L"1/02" },
	{ MONTHS_MONTH_1_START, L"Month 1, Month 2, ... (From Start)" },
	{ MONTHS_M1_START, L"M1, M2, M3, ... (From Start)" },
	{ MONTHS_1_START, L"1, 2, 3, 4, ... (From Start)" },
	{ MONTHS_MONTH_1_END, L"Month 2 Month 1, ... (From End)" },
	{ MONTHS_M1_END, L"M3, M2, M1, ... (From End)" },
	{ MONTHS_1_END, L"4, 3, 2, 1, ... (From End)" },

	//
	// Thirds of months
	//
	{ TRIMONTHS_1, L"1, 11, 21, ..." },
	{ TRIMONTHS_B, L"B, M, E, ..." },
	{ TRIMONTHS_BEGINNING, L"Beginning, Middle, End, ..." },
	{ TRIMONTHS_MS1, L"1/1, 1/11, 1/21, ..." },
	{ TRIMONTHS_MSB, L"1/B, 1/M, 1/E, ..." },
	{ TRIMONTHS_MMMM_BEGINNING, L"January Beginning, January Middle, ..." },
	{ TRIMONTHS_MMM_1, L"Jan 1, Jan 11, Jan 21, ..." },
	{ TRIMONTHS_MMM_B, L"Jan B, Jan M, Jan E, ..." },
	{ TRIMONTHS_MMMM_1, L"January 1, January 11, January 21, ..." },
	{ TRIMONTHS_MS1SYY, L"1/1/02, 1/11/02, 1/21/02, ..." },
	{ TRIMONTHS_MSBSYY, L"1/B/02, 1/M/02, 1/E/02, ..." },
	{ TRIMONTHS_MMM_1_X02, L"Jan 1/02, Jan 11/02, Jan 21/02, ..." },
	{ TRIMONTHS_MMM_B_X02, L"Jan B/02, Jan M/02, Jan E/02, ..." },
	{ TRIMONTHS_MMMM_1_YYYY, L"January 1, 2002, January 11, 2002, ..." },
	{ TRIMONTHS_MMMM_BEGINNING_YYYY, L"January Beginning, ..." },

	//
	// Weeks
	//
	{ WEEKS_MMMM_DD_YYYY, L"January 27, 2007" },
	{ WEEKS_MMM_DD_XYY, L"January 27, '02" },
	{ WEEKS_MMMM_DD, L"January 27" },
	{ WEEKS_MMM_DD, L"Jan 27, Feb 3, ..." },
	{ WEEKS_MDD, L"J 27, F 3, ..." },
	{ WEEKS_MSDDSYY, L"1/27/02, 2/3/02, ..." },
	{ WEEKS_MSDD, L"1/27, 2/3, ..." },
	{ WEEKS_DD, L"27, 3, ..." },
	{ WEEKS_DDD_DD, L"Sun 27" },
	{ WEEKS_DDD_MSDDSYY, L"Sun 1/27/02" },
	{ WEEKS_DDD_MMMM_DD_XYY, L"Sun January 27, '02" },
	{ WEEKS_DDD_MMM_DD_XYY, L"Sun Jan 27, '02" },
	{ WEEKS_DDD_MMMM_DD, L"Sun January 27" },
	{ WEEKS_DDD_MMM_DD, L"Su Jan 27" },
	{ WEEKS_MMM_W, L"S Jan 27" },
	{ WEEKS_D_MMM_DD, L"Sun J 27" },
	{ WEEKS_DDD_M_DD, L"Su J 27" },
	{ WEEKS_DD_M_DD, L"S J 27" },
	{ WEEKS_D_M_DD, L"Sun 1/27" },
	{ WEEKS_DDD_MSDD, L"Su 1/27" },
	{ WEEKS_DD_MSDD, L"S 1/27" },
	{ WEEKS_D_MSDD, L"1, 2, ...52, 1, 2" },
	{ WEEKS_W, L"Sun 1, ..., Sun 52, Sun 1,..." },
	{ WEEKS_DDD_W, L"1 1, ..., 7 1, 1 2, ..., 7 52" },
	{ WEEKS_D_W, L"Week 1, Week 2, ... (From Start)" },
	{ WEEKS_WEEK_1_START, L"W1, W2, ... (From start)" },
	{ WEEKS_W1_START, L"1, 2, 3, 4, ... (From start)" },
	{ WEEKS_1_START, L"Week 2, Week 1, ... (From end)" },
	{ WEEKS_WEEK_1_END, L"W4, W3, W2, W1, ... (From end)" },
	{ WEEKS_W1_END, L"4, 3, 2, 1, ... (From end)" },
	{ WEEKS_1_END, L"Mon Jan 28, '02" },

	//
	// Days
	//
	{ DAYS_DDD_MMM_DD_XYY, L"Mon January 28" },
	{ DAYS_DDD_MMMM_DD, L"Mon Jan 28" },
	{ DAYS_DDD_MMM_DD, L"Mon J 28" },
	{ DAYS_DDD_M_DD, L"Mo J 28" },
	{ DAYS_DD_M_DD, L"M J 28" },
	{ DAYS_D_M_DD, L"Mon 1/28" },
	{ DAYS_DDD_MSDD, L"Mo 1/28" },
	{ DAYS_DD_MSDD, L"M 1/28" },
	{ DAYS_D_MSDD, L"Mon 28" },
	{ DAYS_DDD_DD, L"Mo 28" },
	{ DAYS_DD_DD, L"M 28" },
	{ DAYS_D_DD, L"M28" },
	{ DAYS_DXDD, L"Jan 28, '02" },
	{ DAYS_DD_MMM, L"28 Jan" },
	{ DAYS_DDD_DD_XYY, L"Jan 28, Jan 29, ... " },
	{ DAYS_M_DD, L"J28, J29, ... " },
	{ DAYS_DDDD, L"Sunday, Monday, ..." },
	{ DAYS_DDD, L"Sun, Mon, Tue, ..." },
	{ DAYS_DD, L"Su, Mo, Tu, ..." },
	{ DAYS_D, L"S, M, T, ..." },
	{ DAYS_MSDDSYY, L"1/28/02, 1/29/02, ... " },
	{ DAYS_DDD_MSDDSYY, L"Mon 1/28/02, Tue 1/29/02, ... " },
	{ DAYS_MSDD, L"1/28, 1/29, ... " },
	{ DAYS_1, L"1, 2, ..." },
	{ DAYS_128_YYYY, L"128 2002 (Day of Year)" },
	{ DAYS_128_XYY, L"128 '02 (Day of Year)" },
	{ DAYS_128, L"128 (Day of Year)" },
	{ DAYS_DAY_1_START, L"Day 1, Day 2, ...  (From Start)" },
	{ DAYS_D1_START, L"D1, D2, D3, ...  (From start)" },
	{ DAYS_1_START, L"1, 2, 3, 4, ...  (From start)" },
	{ DAYS_DAY_1_END, L"Day 2, Day 1, ...  (From end)" },
	{ DAYS_D1_END, L"D3, D2, D1, ...  (From end)" },
	{ DAYS_1_END, L"4, 3, 2, 1, ...  (From start)" },

	//
	// Hours
	//
	{ HOURS_DDD_MMM_DD_HH_AM, L"Mon Jan 28, 11 AM" },
	{ HOURS_MMM_DD_HH_AM, L"Jan 28, 11 AM" },
	{ HOURS_MSDD_HH_AM, L"1/28, 11 AM" },
	{ HOURS_HHMM_AM, L"11:00 AM, 12:00 PM, ... " },
	{ HOURS_HH_AM, L"11 AM, 12 PM, ... " },
	{ HOURS_HH, L"11, 12, ... " },
	{ HOURS_HOUR_1_START, L"Hour 1, Hour 2, ...  (From Start)" },
	{ HOURS_H1_START, L"H1, H2, H3, ...  (From start)" },
	{ HOURS_1_START, L"1, 2, 3, 4, ...  (From start)" },
	{ HOURS_HOUR_1_END, L"Hour 2, Hour 1, ...  (From end)" },
	{ HOURS_H1_END, L"H3, H2, H1, ...  (From end)" },
	{ HOURS_1_END, L"4, 3, 2, 1, ...  (From start)" },

	//
	// Minutes
	//
	{ MINUTES_HHMM_AM, L"1:45 PM, 1:46 PM, ... " },
	{ MINUTES_MM, L"45, 46, 47, ... " },
	{ MINUTES_MINUTE_1_START, L"Minute 1, Minute 2, ...  (From Start)" },
	{ MINUTES_M1_START, L"M1, M2, M3, ...  (From start)" },
	{ MINUTES_1_START, L"1, 2, 3, 4, ...  (From start)" },
	{ MINUTES_MINUTE_1_END, L"Minute 2, Minute 1, ...  (From end)" },
	{ MINUTES_M1_END, L"M3, M2, M1, ...  (From end)" },
	{ MINUTES_1_END, L"4, 3, 2, 1, ...  (From start)" },
	{ (TimescaleFormat)NULL, NULL }
};
END_ENUMTYPENAME_CONVERSION2(TimescaleFormat);