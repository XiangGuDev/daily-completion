#pragma once

#include "EnumType2Text.h"

/**
* Instances of this class represent enumerated file version values.
*/
enum FileVersion
{
	VERSION_1_0 = 1,
	VERSION_3_0 = 3,
	VERSION_4_0 = 4
};

/**
* Instances of this class represent enumerated versions of Microsoft
* Project which can read and write MSPDI files.
*/
enum SaveVersion
{
	Project2002=9,
	Project2003=9,
	Project2007=12,
	Project2010=14
};

/**
* This class is used to represent an accrue type. It provides a mapping
* between the textual description of a accrue type found in an MPX
* file, and an enumerated representation that can be more easily manipulated
* programmatically.
*/
enum AccrueType
{
	ACCRUE_START = 1,
	ACCRUE_END = 2,
	ACCRUE_PRORATED = 3
};

/**
* Instances of this class represent enumerated schedule from values.
*/
enum ScheduleFrom
{
	SCHEDULE_START = 0,
	SCHEDULE_FINISH = 1
};

/**
* Enumeration representing booking types.
*/
enum BookingType
{
	NULL_BOOKINGTYPE = -1,
	COMMITTED = 0,
	PROPOSED = 1
};

/**
* This class represents the data type of an attribute.
*/
enum DataType
{
	DT_STRING = 1,
	DT_DATE = 2,
	DT_CURRENCY = 3,
	DT_BOOLEAN = 4,
	DT_NUMERIC = 5,
	DT_DURATION = 6,
	DT_UNITS = 7,
	DT_PERCENTAGE = 8,
	DT_ACCRUE = 9,
	DT_CONSTRAINT = 10,
	DT_RATE = 11,
	DT_PRIORITY = 12,
	DT_RELATION_LIST = 13,
	DT_TASK_TYPE = 14,
	DT_RESOURCE_TYPE = 15,
	DT_TIME_UNITS = 15,
	DT_WORK = 16,
	DT_INTEGER = 17,
	DT_ASCII_STRING = 18,
	DT_SHORT = 19,
	DT_BINARY = 20,
	DT_DELAY = 21,
	DT_WORK_UNITS = 22,
	DT_WORKGROUP = 23,
	DT_GUID = 24,
	DT_RATE_UNITS = 25
};

/**
* Instances of this class represent enumerated Date order values.
*/
enum DateOrder
{
	MDY = 0,
	DMY = 1,
	YMD = 2
};

/**
* Instances of this class represent enumerated day values.
*/
enum Day
{
	SUNDAY = 1,
	MONDAY = 2,
	TUESDAY = 3,
	WEDNESDAY = 4,
	THURSDAY = 5,
	FRIDAY = 6,
	SATURDAY = 7
};

/**
* Instances of this class represent enumerated day values used as to
* define when progress lines are drawn.
*/
enum ProgressLineDay
{
	PLD_SUNDAY = 1,//Day.SUNDAY),
	PLD_MONDAY = 2,//Day.MONDAY),
	PLD_TUESDAY = 3,//Day.TUESDAY),
	PLD_WEDNESDAY = 4,//Day.WEDNESDAY),
	PLD_THURSDAY = 5,//Day.THURSDAY),
	PLD_FRIDAY = 6,//Day.FRIDAY),
	PLD_SATURDAY = 7,//Day.SATURDAY),
	PLD_DAY = 8, //null),
	PLD_WORKINGDAY = 9, //null),
	PLD_NONWORKINGDAY = 10,// null);
};//

/**
* This class is used to represent a the day type used by the project calendar.
*/
enum DayType
{
	NON_WORKING = 0,
	WORKING = 1,
	DEFAULT = 2
};

/**
* Instances of this class represent enumerated earned value method values.
*/
enum EarnedValueMethod
{
	PERCENT_COMPLETE = 0,
	PHYSICAL_PERCENT_COMPLETE = 1
};

/**
* Instances of this class represent enumerated date format values.
*/
enum ProjectDateFormat
{
	DD_MM_YY_HH_MM = 0,		//This format represents Dates in the form 25/12/98 12:56.
	DD_MM_YY = 1,			//This format represents Dates in the form 25/05/98.
	DD_MMMMM_YYYY_HH_MM = 2,//This format represents Dates in the form 13 December 2002 12:56.
	DD_MMMMM_YYYY = 3,		//This format represents Dates in the form 13 December 2002.
	DD_MMM_HH_MM = 4,		//This format represents Dates in the form 24 Nov 12:56.
	DD_MMM_YY = 5,			//This format represents Dates in the form 25 Aug '98.
	DD_MMMMM = 6,			//This format represents Dates in the form 25 September.
	DD_MMM = 7,				//This format represents Dates in the form 25 Aug.
	EEE_DD_MM_YY_HH_MM = 8,	//This format represents Dates in the form Thu 25/05/98 12:56.
	EEE_DD_MM_YY = 9,		//This format represents Dates in the form Wed 25/05/98.
	EEE_DD_MMM_YY = 10,		//This format represents Dates in the form Wed 25 Mar '98.
	EEE_HH_MM = 11,			//This format represents Dates in the form Wed 12:56.
	DD_MM = 12,				//This format represents Dates in the form 25/5.
	DD = 13,				//This format represents Dates in the form 23.
	HH_MM = 14,				//This format represents Dates in the form 12:56.
	EEE_DD_MMM = 15,		//This format represents Dates in the form Wed 23 Mar..
	EEE_DD_MM = 16,			//This format represents Dates in the form Wed 25/5.
	EEE_DD = 17,			//This format represents Dates in the form Wed 05.
	DD_WWW = 18,			//This format represents Dates in the form 5/W25.
	DD_WWW_YY_HH_MM = 19,	//This format represents Dates in the form 5/W25/98 12:56.
	DD_MM_YYYY = 20			//This format represents Dates in the form 25/05/1998.
};

/**
* Instances of this class represent enumerated time format values.
*/

enum ProjectTimeFormat
{
	TWELVE_HOUR = 0,
	TWENTY_FOUR_HOUR = 1
};

/**
* Instances of this class represent enumerated currency symbol position values.
*/
enum CurrencySymbolPosition
{
	AFTER = 0,
	BEFORE = 1,
	AFTER_WITH_SPACE = 2,
	BEFORE_WITH_SPACE = 3
};

/**
* Instances of this class represent enumerated task type values.
*/
enum TaskType
{
	FIXED_UNITS = 0,
	FIXED_DURATION = 1,
	FIXED_WORK = 2
};

/**
* This class is used to represent a constraint type. It provides a mapping
* between the textual description of a constraint type found in an MPX
* file, and an enumerated representation that can be more easily manipulated
* programatically.
*/
enum ConstraintType
{
	AS_SOON_AS_POSSIBLE=0,
	AS_LATE_AS_POSSIBLE=1,
	MUST_START_ON=2,
	MUST_FINISH_ON=3,
	START_NO_EARLIER_THAN=4,
	START_NO_LATER_THAN=5,
	FINISH_NO_EARLIER_THAN=6,
	FINISH_NO_LATER_THAN=7
};

/**
* Represents task mode values.
*/
enum TaskMode
{
	MANUALLY_SCHEDULED=0,
	AUTO_SCHEDULED=1
};

/**
* Instances of this class represent enumerated work group values.
*/
enum WorkGroup2
{
	WG_DEFAULT=0,
	WG_NONE = 1,
	WG_EMAIL = 2,
	WG_WEB = 3
};

/**
* Represents the style of the start and end sections of a Gantt bar.
*/
enum GanttBarStartEndType
{
	GANTTBAR_SOLID=0, //"Solid"),
	GANTTBAR_FRAMED = 1, //"Framed"),
	GANTTBAR_DASHED = 2, //"Dashed");
};

/**
* Represents the shape type used to draw the middle section of a Gantt bar.
*/
enum GanttBarMiddleShape
{
	GANTTBARMIDDLE_NONE=0, //"None"),
	ROUNDED=1, //"Rounded"),
	TOPTHINROUNDED=2, //"Top Thin Rounded"),
	MIDDLETHINROUNDED=3, //"Middle Thin Rounded"),
	BOTTOMTHINROUNDED=4, //"Bottom Thin Rounded"),
	TOPLINE=5, //"Top Line"),
	MIDDLELINE=6, //"Middle Line"),
	BOTTOMLINE=7, //"Bottom Line"),
};

/**
* Represents the shape at the start end end of a Gantt bar.
*/
enum GanttBarStartEndShape
{
	GANTTBARSTART_NONE=0, //None"),
	NORTHHOMEPLATE=1, //North Home Plate"),
	SOUTHHOMEPLATE=2, //South Home Plate"),
	DIAMOND=3, //Diamond"),
	UPARROW=4, //Up Arrow"),
	DOWNARROW=5, //Down Arrow"),
	RIGHTARROW=6, //Right Arrow"),
	LEFTARROW=7, //Left Arrow"),
	UPPOINTER=8, //Up Pointer"),
	SOUTHMINIHOMEPLATE=9, //South Mini Home Plate"),
	NORTHMINIHOMEPLATE=10, //North Mini Home Plate"),
	VERTICALBAR=11, //Vertical Bar"),
	SQUARE=12, //Square"),
	DIAMONDCIRCLED=13, //Diamond Circled"),
	DOWNPOINTER=14, //Down Pointer"),
	UPARROWCIRCLED=15, //Up Arrow Circled"),
	DOWNARROWCIRCLED=16, //Down Arrow Circled"),
	UPPOINTERCIRCLED=17, //Up Pointer Circled"),
	DOWNPOINTERCIRCLED=18, //Down Pointer Circled"),
	CIRCLE=19, //Circle"),
	STAR=20, //Star"),
	LEFTBRACKET=21, //Left Bracket"),
	RIGHTBRACKET=22, //Right Bracket"),
	LEFTGRADIENT=23, //Left Gradient"),
	RIGHTGRADIENT=24, //Right Gradient");
};

/**
* Enumeration representing the formats which may be shown on a Gantt chart timescale.
*/
enum GanttBarDateFormat
{
	GANTTDATEFMT_DEFAULT=0, //Default"),
	GANTTDATEFMT_DDMMYY_MMSS = 1, //28/01/02 12:33"),
	GANTTDATEFMT_DDMMYY = 2, //28/01/02"),
	GANTTDATEFMT_DDMMYYYY = 21, //28/01/2002"),
	GANTTDATEFMT_DD_MMMM_YYYY_HHMM = 3, //28 January 2002 12:33"),
	GANTTDATEFMT_DD_MMMM_YYYY = 4, //28 January 2002"),
	GANTTDATEFMT_DD_MMM_HHMM = 5, //28 Jan 12:33"),
	GANTTDATEFMT_DD_MMM_YY = 6, //28 Jan '02"),
	GANTTDATEFMT_DD_MMMM = 7, //28 January"),
	GANTTDATEFMT_DD_MMM = 8, //28 Jan"),
	GANTTDATEFMT_DDD_DDMMYY_HHMM = 9, //Mon 28/01/02 12:33"),
	GANTTDATEFMT_DDD_DDMMYY = 10, //Mon 28/01/02"),
	GANTTDATEFMT_DDD_DD_MMM_YY = 11, //Mon 28 Jan '02"),
	GANTTDATEFMT_DDD_HHMM = 12, //Mon 12:33"),
	GANTTDATEFMT_DDD_DD_MMM = 16, //Mon 28 Jan"),
	GANTTDATEFMT_DDD_DDMM = 17, //Mon 28/01"),
	GANTTDATEFMT_DDD_DD = 18, //Mon 28"),
	GANTTDATEFMT_DDMM = 13, //28/01"),
	GANTTDATEFMT_DD = 14, //28"),
	GANTTDATEFMT_HHMM = 15, //12:33"),
	GANTTDATEFMT_GANTTDATEFMT_MWW = 19, //1/W05"),
	GANTTDATEFMT_MWWYY_HHMM = 20, //1/W05/02 12:33");
};

/**
* This enum represents the colors used by Microsoft Project.
*/
enum ColorType
{
	BLACK=0,// "Black", Color.BLACK),
	RED=1,// "Red", Color.RED),
	YELLOW=2,// "Yellow", Color.YELLOW),
	LIME=3,// "Lime", new Color=148, 255, 148)),
	AQUA=4,// "Aqua", new Color=194, 220, 255)),
	BLUE=5, //"Blue", Color.BLUE),
	FUSCHIA=6,// "Fuschia", Color.MAGENTA),
	WHITE=7, //"White", Color.WHITE),
	MAROON=8, //"Maroon", new Color=128, 0, 0)),
	GREEN=9, //"Green", new Color(0, 128, 0)),
	OLIVE=10, //"Olive", new Color=128, 128, 0)),
	NAVY=11, //"Navy", new Color(0, 0, 128)),
	PURPLE=12,// "Purple", new Color=128, 0, 128)),
	TEAL=13, //"Teal", new Color(0, 128, 128)),
	GRAY=14,// "Gray", new Color=128, 128, 128)),
	SILVER=15, //"Silver", new Color=192, 192, 192)),
	AUTOMATIC=16, //"Automatic", null);
};
DECLARE_ENUMTYPENAME_CONVERSION2(ColorType);

/**
* This class represents daily, weekly or monthly time intervals.
*/
enum Interval
{
	INTERVAL_DAILY =0,// "Daily"),
	INTERVAL_WEEKLY = 1,// "Weekly"),
	INTERVAL_MONTHLY = 2,// "Monthly");
};
DECLARE_ENUMTYPENAME_CONVERSION2(Interval);

/**
* Class representing how non-working time is shown on a Gantt chart.
*/
enum NonWorkingTimeStyle
{
	BEHIND=0,// "Behind"),
	IN_FRONT=1,// "In Front"),
	DO_NOT_DRAW=2,// "Do Not Draw");
};
DECLARE_ENUMTYPENAME_CONVERSION2(NonWorkingTimeStyle);


/**
* Represents the pattern used to fill the middle section of a Gantt bar.
*/
enum ChartPattern// implements MpxjEnum
{
	CP_TRANSPARENT=0, //"Transparent"),
	CP_SOLID = 1, //"Solid"),
	CP_LIGHTDOTTED = 2, //"Light Dotted"),
	CP_DOTTED = 3, //"Dotted"),
	CP_HEAVYDOTTED = 4,// "Heavy Dotted"),
	CP_BACKSLASH = 5, //"Back Slash"),
	CP_FORWARDSLASH = 6, //"Forward Slash"),
	CP_CHECKERED = 7, //"Checkered"),
	CP_VERTICALSTRIPE = 8,// "Vertical Stripe"),
	CP_HORIZONTALSTRIPE = 9, //"Horizontal Stripe"),
	CP_GRID = 10, //"Grid"),
	CP_SOLIDHAIRY = 11, //"Solid Hairy");
};
DECLARE_ENUMTYPENAME_CONVERSION2(ChartPattern);

/**
* Represents the recurrence type.
*/
enum RecurrenceType
{
	DAILY =1,
	WEEKLY =4,
	MONTHLY =8,
	YEARLY =16,
};
DECLARE_ENUMTYPENAME_CONVERSION2(RecurrenceType);

/**
* Instances of this class represent enumerated resource type values.
*/
enum Resource2Type
{
	RT_MATERIAL = 0,
	RT_WORK = 1,
	RT_COST = 2,
};
DECLARE_ENUMTYPENAME_CONVERSION2(Resource2Type);

/**
* This class is used to represent a relation type. It provides a mapping
* between the textual description of a relation type found in an MPX
* file, and an enumerated representation that can be more easily manipulated
* programatically.
*/
enum RelationType
{
	FINISH_FINISH=0,
	FINISH_START=1,
	START_FINISH=2,
	START_START=3,
};
DECLARE_ENUMTYPENAME_CONVERSION2(RelationType);

/**
* This class contains utility functions allowing time unit specifications
* to be parsed and formatted.
*/
enum TimeUnit
{
	MINUTES = 0,
	HOURS = 1,
	DAYS = 2,
	WEEKS = 3,
	MONTHS = 4,
	PERCENT = 5,
	YEARS = 6,
	ELAPSED_MINUTES = 7,
	ELAPSED_HOURS = 8,
	ELAPSED_DAYS = 9,
	ELAPSED_WEEKS = 10,
	ELAPSED_MONTHS = 11,
	ELAPSED_YEARS = 12,
	ELAPSED_PERCENT = 13,
};
DECLARE_ENUMTYPENAME_CONVERSION2(TimeUnit);

/**
* This class represents the enumeration of the valid types of view.
*/
enum MppViewType
{
	VIEW_UNKNOWN = 0,
	VIEW_GANTT_CHART = 1,
	VIEW_NETWORK_DIAGRAM = 2,
	VIEW_RELATIONSHIP_DIAGRAM = 3,
	VIEW_TASK_FORM = 4,
	VIEW_TASK_SHEET = 5,
	VIEW_RESOURCE_FORM = 6,
	VIEW_RESOURCE_SHEET = 7,
	VIEW_RESOURCE_GRAPH = 8,
	VIEW_TASK_DETAILS_FORM = 10,
	VIEW_TASK_NAME_FORM = 11,
	VIEW_RESOURCE_NAME_FORM = 12,
	VIEW_CALENDAR = 13,
	VIEW_TASK_USAGE = 14,
	VIEW_RESOURCE_USAGE = 15,
};
DECLARE_ENUMTYPENAME_CONVERSION2(MppViewType);

/**
* Represents the pattern used to fill a group.
*/
enum BackgroundPattern
{
	BP_TRANSPARENT = 0,
	BP_SOLID = 1,
	BP_LIGHTDOTTED = 2,
	BP_DOTTED = 3,
	BP_HEAVYDOTTED = 4,
	BP_BACKSLASH = 5,
	BP_FORWARDSLASH = 6,
	BP_INVERSEBACKSLASH = 7,
	BP_INVERSEFORWARDSLASH = 8,
	BP_LIGHTVERTICALSTRIPE = 9,
	BP_HEAVYVERTICALSTRIPE = 10,
	BP_CHECKERED = 11,
	BP_DENSEFORWARDSLASH = 12,
	BP_INVERSECHECKERED = 13,
};
DECLARE_ENUMTYPENAME_CONVERSION2(BackgroundPattern);

/**
* This class represents the grid line styles used by Microsoft Project.
*/
enum LineStyle2
{
	LS_NONE = 0,
	LS_SOLID = 1,
	LS_DOTTED1 = 2,
	LS_DOTTED2 = 3,
	LS_DASHED = 4,
};
DECLARE_ENUMTYPENAME_CONVERSION2(LineStyle2);

/**
* Class representing how links are drawn on a Gantt chart.
*/
enum LinkStyle
{
	NONE=0,// "None"),
	END_TOP=1,// "End Top"),
	END_START=2,// "End Start"),
};
DECLARE_ENUMTYPENAME_CONVERSION2(LinkStyle);


/**
* Class representing the label alignment on a Gantt chart timescale.
*/
enum TimescaleAlignment
{
	TIMEALIGN_LEFT=0,
	TIMEALIGN_CENTER = 1,
	TIMEALIGN_RIGHT = 2,
};
DECLARE_ENUMTYPENAME_CONVERSION2(TimescaleAlignment);

/**
* Class representing the units which may be shown on a Gantt chart timescale.
*/
enum TimescaleUnits
{
	TU_NONE = -1,
	TU_MINUTES = 0,
	TU_HOURS = 1,
	TU_DAYS = 2,
	TU_WEEKS = 3,
	TU_THIRDS_OF_MONTHS = 4,
	TU_MONTHS = 5,
	TU_QUARTERS = 6,
	TU_HALF_YEARS = 7,
	TU_YEARS = 8
};
DECLARE_ENUMTYPENAME_CONVERSION2(TimescaleUnits);


/**
* Enumeration representing the formats which may be shown on a Gantt chart timescale.
*/
enum TimescaleFormat
{
	TF_NONE = 35,

	//
	// Years
	//
	YEAR_YYYY = 0,
	YEAR_XYY = 1,
	YEAR_YY = 78,
	YEAR_YEAR_START = 48,
	YEAR_Y_START = 73,
	YEAR_1_START = 74,
	YEAR_YEAR_END = 49,
	YEAR_Y_END = 71,
	YEAR_1_END = 72,

	//
	// Half years
	//
	HALFYEAR_1_HALF = 123,
	HALFYEAR_HALF_1_YYYY = 124,
	HALFYEAR_HALF_1 = 125,
	HALFYEAR_H1_XYY = 126,
	HALFYEAR_H1 = 127,
	HALFYEAR_1 = 128,
	HALFYEAR_1HYY = 129,
	HALFYEAR_HALF_1_START = 130,
	HALFYEAR_H1_START = 131,
	HALFYEAR_1_START = 132,
	HALFYEAR_HALF_1_END = 133,
	HALFYEAR_H1_END = 134,
	HALFYEAR_1_END = 135,

	//
	// Quarters
	//
	QUARTER_1_QUARTER = 2,
	QUARTER_QTR_1_YYYY = 3,
	QUARTER_QTR_1 = 5,
	QUARTER_Q1_XYY = 4,
	QUARTER_Q1 = 6,
	QUARTER_1 = 62,
	QUARTER_1QYY = 51,
	QUARTER_QUARTER_1_START = 46,
	QUARTER_Q1_START = 65,
	QUARTER_1_START = 66,
	QUARTER_QUARTER_1_END = 47,
	QUARTER_Q1_END = 63,
	QUARTER_1_END = 64,

	//
	// Months
	//
	MONTHS_MMMM_YYYY = 7,
	MONTHS_MMM_XYY = 8,
	MONTHS_MMMM = 9,
	MONTHS_MMM = 10,
	MONTHS_M = 11,
	MONTHS_1 = 57,
	MONTHS_1_XYY = 85,
	MONTHS_1SYY = 86,
	MONTHS_MONTH_1_START = 44,
	MONTHS_M1_START = 60,
	MONTHS_1_START = 61,
	MONTHS_MONTH_1_END = 45,
	MONTHS_M1_END = 58,
	MONTHS_1_END = 59,

	//
	// Thirds of months
	//
	TRIMONTHS_1 = 136,
	TRIMONTHS_B = 137,
	TRIMONTHS_BEGINNING = 138,
	TRIMONTHS_MS1 = 139,
	TRIMONTHS_MSB = 140,
	TRIMONTHS_MMMM_BEGINNING = 141,
	TRIMONTHS_MMM_1 = 142,
	TRIMONTHS_MMM_B = 143,
	TRIMONTHS_MMMM_1 = 144,
	TRIMONTHS_MS1SYY = 145,
	TRIMONTHS_MSBSYY = 146,
	TRIMONTHS_MMM_1_X02 = 147,
	TRIMONTHS_MMM_B_X02 = 148,
	TRIMONTHS_MMMM_1_YYYY = 149,
	TRIMONTHS_MMMM_BEGINNING_YYYY = 150,

	//
	// Weeks
	//
	WEEKS_MMMM_DD_YYYY=12, 
	WEEKS_MMM_DD_XYY=13, 
	WEEKS_MMMM_DD=14,
	WEEKS_MMM_DD=15, 
	WEEKS_MDD=89, 
	WEEKS_MSDDSYY=16, 
	WEEKS_MSDD=17,
	WEEKS_DD=87, 
	WEEKS_DDD_DD=88, 
	WEEKS_DDD_MSDDSYY=100, 
	WEEKS_DDD_MMMM_DD_XYY=102, 
	WEEKS_DDD_MMM_DD_XYY=101, 
	WEEKS_DDD_MMMM_DD=96,
	WEEKS_DDD_MMM_DD=93,
	WEEKS_MMM_W=94,
	WEEKS_D_MMM_DD=95, 
	WEEKS_DDD_M_DD=97, 
	WEEKS_DD_M_DD=98, 
	WEEKS_D_M_DD=99,
	WEEKS_DDD_MSDD=90, 
	WEEKS_DD_MSDD=91,
	WEEKS_D_MSDD=92, 
	WEEKS_W=50, 
	WEEKS_DDD_W=103, 
	WEEKS_D_W=104,
	WEEKS_WEEK_1_START=42, 
	WEEKS_W1_START=69, 
	WEEKS_1_START=70,
	WEEKS_WEEK_1_END=43, 
	WEEKS_W1_END=67, 
	WEEKS_1_END=68,

	//
	// Days
	//
	DAYS_DDD_MMM_DD_XYY=22, 
	DAYS_DDD_MMMM_DD=111,
	DAYS_DDD_MMM_DD=23,
	DAYS_DDD_M_DD=112,
	DAYS_DD_M_DD=113,
	DAYS_D_M_DD=114,
	DAYS_DDD_MSDD=108,
	DAYS_DD_MSDD=109,
	DAYS_D_MSDD=110, 
	DAYS_DDD_DD=105,
	DAYS_DD_DD=106,
	DAYS_D_DD=107,
	DAYS_DXDD=121, 
	DAYS_DD_MMM=25,
	DAYS_DDD_DD_XYY=24,
	DAYS_M_DD=115,
	DAYS_DDDD=18,
	DAYS_DDD=19,
	DAYS_DD=119,
	DAYS_D=20,
	DAYS_MSDDSYY=26, 
	DAYS_DDD_MSDDSYY=52,
	DAYS_MSDD=27,
	DAYS_1=21,
	DAYS_128_YYYY=117,
	DAYS_128_XYY=116, 
	DAYS_128=118, 
	DAYS_DAY_1_START=40, 
	DAYS_D1_START=55, 
	DAYS_1_START=56,
	DAYS_DAY_1_END=41,
	DAYS_D1_END=53,
	DAYS_1_END=54,

	//
	// Hours
	//
	HOURS_DDD_MMM_DD_HH_AM=28,
	HOURS_MMM_DD_HH_AM=29,
	HOURS_MSDD_HH_AM=120,
	HOURS_HHMM_AM=30, 
	HOURS_HH_AM=31, 
	HOURS_HH=32, 
	HOURS_HOUR_1_START=38, 
	HOURS_H1_START=78, 
	HOURS_1_START=79, 
	HOURS_HOUR_1_END=39,
	HOURS_H1_END=76, 
	HOURS_1_END=77, 

	//
	// Minutes
	//
	MINUTES_HHMM_AM=33,
	MINUTES_MM=34, 
	MINUTES_MINUTE_1_START=36,
	MINUTES_M1_START=82, 
	MINUTES_1_START=83,
	MINUTES_MINUTE_1_END=37, 
	MINUTES_M1_END=80,
	MINUTES_1_END=81,
};
DECLARE_ENUMTYPENAME_CONVERSION2(TimescaleFormat);

/**
* Represents the criteria used to define when a Gantt bar is displayed.
* Note that the value attribute has been chosen to allow the nrmal and
* negated types to be determined. value & 0x64 will be zero for normal types,
* and non-zero for negative types. value & 0x63 will convert a negative type
* to a normal type, the type can then be retrieved using the getInstance
* method.
*/
enum GanttBarShowForTasks
{
	eNORMAL=0, //"Normal"),
	MILESTONE=1, //"Milestone"),
	SUMMARY=2, //"Summary"),
	CRITICAL=3, //"Critical"),
	NONCRITICAL=4, //"Noncritical"),
	MARKED=5, //"Marked"),
	FINISHED=6, //"Finished"),
	INPROGRESS=7, //"In Progress"),
	NOTFINISHED=8, //"Not Finished"),
	NOTSTARTED=9, //"Not Started"),
	STARTEDLATE=10, //"Started Late"),
	FINISHEDLATE=11, //"Finished Late"),
	STARTEDEARLY=12, //"Started Early"),
	FINISHEDEARLY=13, //"Finished Early"),
	STARTEDONTIME=14, //"Started On Time"),
	FINISHEDONTIME=15, //"Finished On Time"),
	FLAG1=16, //"Flag1"),
	FLAG2=17, //"Flag2"),
	FLAG3=18, //"Flag3"),
	FLAG4=19, //"Flag4"),
	FLAG5=20, //"Flag5"),
	FLAG6=21, //"Flag6"),
	FLAG7=22, //"Flag7"),
	FLAG8=23, //"Flag8"),
	FLAG9=24, //"Flag9"),
	FLAG10=25, //"Flag10"),
	ROLLEDUP=26, //"Rolled Up"),
	PROJECTSUMMARY=27, //"Project Summary"),
	SPLIT=28, //"Split"),
	EXTERNAL=29, //"External Tasks"),
	FLAG11=30, //"Flag11"),
	FLAG12=31, //"Flag12"),
	FLAG13=32, //"Flag13"),
	FLAG14=33, //"Flag14"),
	FLAG15=34, //"Flag15"),
	FLAG16=35, //"Flag16"),
	FLAG17=36, //"Flag17"),
	FLAG18=37, //"Flag18"),
	FLAG19=38, //"Flag19"),
	FLAG20=39, //"Flag20"),
	GROUPBYSUMMARY=40, //"Group2 By Summary"),
	DELIVERABLE=41, //"Deliverable"),
	DEPENDENCY=42, //"Dependency"),
	ACTIVE=43, //"Active"),
	MANUALLYSCHEDULED=44, //"Manually Scheduled"),
	WARNING=45, //"Warning"),
	PLACEHOLDERSTART=46, //"Placeholder (Start)"),
	PLACEHOLDERFINISH=47, //"Placeholder (Finish)"),
	PLACEHOLDERDURATION=48, //"Placeholder (Duration)"),
	PLACEHOLDER=49, //"Placeholder"),
	LATE=50, //"Late"),

	NOT_NORMAL=64, //"Not Normal"),
	NOT_MILESTONE=65, //"Not Milestone"),
	NOT_SUMMARY=66, //"Not Summary"),
	NOT_CRITICAL=67, //"Not Critical"),
	//NOT_NONCRITICAL=68, //"Not NonCritical"), // Not used by MSP
	NOT_MARKED=69, //"Not Marked"),
	//NOT_FINISHED=70, //"Not Not Finished"), // Not used by MSP
	NOT_INPROGRESS=71, //"Not In Progress"),
	//NOT_NOTFINISHED=72, //"Not Not Finished"), // Not used by MSP
	//NOT_NOTSTARTED=73, //"Not Not Started"), // Not used by MSP
	NOT_STARTEDLATE=74, //"Not Started Late"),
	NOT_FINISHEDLATE=75, //"Not Finished Late"),
	NOT_STARTEDEARLY=76, //"Not Started Early"),
	NOT_FINISHEDEARLY=77, //"Not Finished Early"),
	NOT_STARTEDONTIME=78, //"Not Started On Time"),
	NOT_FINISHEDONTIME=79, //"Not Finished On Time"),
	NOT_FLAG1=80, //"Not Flag1"),
	NOT_FLAG2=81, //"Not Flag2"),
	NOT_FLAG3=82, //"Not Flag3"),
	NOT_FLAG4=83, //"Not Flag4"),
	NOT_FLAG5=84, //"Not Flag5"),
	NOT_FLAG6=85, //"Not Flag6"),
	NOT_FLAG7=86, //"Not Flag7"),
	NOT_FLAG8=87, //"Not Flag8"),
	NOT_FLAG9=88, //"Not Flag9"),
	NOT_FLAG10=89, //"Not Flag10"),
	NOT_ROLLEDUP=90, //"Not Rolled Up"),
	NOT_PROJECTSUMMARY=91, //"Not Project Summary"),
	NOT_SPLIT=92, //"Not Split"),
	NOT_EXTERNAL=93, //"Not External Tasks"),
	NOT_FLAG11=94, //"Not Flag11"),
	NOT_FLAG12=95, //"Not Flag12"),
	NOT_FLAG13=96, //"Not Flag13"),
	NOT_FLAG14=97, //"Not Flag14"),
	NOT_FLAG15=98, //"Not Flag15"),
	NOT_FLAG16=99, //"Not Flag16"),
	NOT_FLAG17=100, //"Not Flag17"),
	NOT_FLAG18=101, //"Not Flag18"),
	NOT_FLAG19=102, //"Not Flag19"),
	NOT_FLAG20=103, //"Not Flag20"),
	NOT_GROUPBYSUMMARY=104, //"Not Group2 By Summary"),
	NOT_DELIVERABLE=105, //"Not Deliverable"),
	NOT_DEPENDENCY=106, //"Not Dependency"),
	NOT_ACTIVE=107, //"Not Active"),
	NOT_MANUALLYSCHEDULED=108, //"Not Manually Scheduled"),
	NOT_WARNING=109, //"Not Warning"),
	NOT_PLACEHOLDERSTART=110, //"Not Placeholder (Start)"),
	NOT_PLACEHOLDERFINISH=111, //"Not Placeholder (Finish)"),
	NOT_PLACEHOLDERDURATION=112, //"Placeholder (Duration)"),
	NOT_PLACEHOLDER=113, //"Not Placeholder"),
	NOT_LATE=114, //"Not Late");
};
