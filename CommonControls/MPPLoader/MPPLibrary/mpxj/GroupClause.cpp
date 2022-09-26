#include "stdafx.h"
#include "GroupClause.h"

Group2Clause::Group2Clause()
	:m_field(NULL)
{

}

/**
* Retrieve a flag indicating that values are grouped
* in ascending order.
* 
* @return bool flag
*/
bool Group2Clause::getAscending()
{
    return m_ascending;
}

/**
* Sets a flag indicating that values are grouped
* in ascending order.
* 
* @param ascending bool flag
*/
void Group2Clause::setAscending(bool ascending)
{
    m_ascending = ascending;
}

/**
* Retrieves the background color.
* 
* @return background color
*/
COLORREF Group2Clause::getCellBackgroundColor()
{
    return m_cellBackgroundColor;
}

/**
* Sets the background color.
* 
* @param color background color.
*/
void Group2Clause::setCellBackgroundColor(COLORREF color)
{
    m_cellBackgroundColor = color;
}

/**
* Retrieve the grouping field.
* 
* @return grouping field
*/
FieldType* Group2Clause::getField()
{
    return m_field;
}

/**
* Set the grouping field.
* 
* @param field grouping field
*/
void Group2Clause::setField(FieldType* field)
{
    m_field = field;
}

/**
* Retrieve the font.
* 
* @return font
*/
FontStylePtr Group2Clause::getFont()
{
    return m_font;
}

/**
* Retrieve the font.
* 
* @param font font
*/
void Group2Clause::setFont(FontStylePtr font)
{
    m_font = font;
}

/**
* Retrieve the group interval.
* 
* @return group interval
*/
ObjectPtr Group2Clause::getGroup2Interval()
{
    return m_groupInterval;
}

/**
* Sets the group interval.
* 
* @param groupInterval group interval
*/
void Group2Clause::setGroup2Interval(ObjectPtr groupInterval)
{
    m_groupInterval = groupInterval;
}

/**
* Retrieves the group on value.
* 
* @return group on value
*/
int Group2Clause::getGroup2On()
{
    return m_groupOn;
}

/**
* Sets the group on value.
* 
* @param groupOn group on value
*/
void Group2Clause::setGroup2On(int groupOn)
{
    m_groupOn = groupOn;
}

/**
* Retrieves the pattern.
* 
* @return pattern
*/
BackgroundPattern Group2Clause::getPattern()
{
    return m_pattern;
}

/**
* Sets the pattern.
* 
* @param pattern pattern
*/
void Group2Clause::setPattern(BackgroundPattern pattern)
{
    m_pattern = pattern;
}

/**
* Retrieves the "start at" value.
* 
* @return "start at" value
*/
ObjectPtr Group2Clause::getStartAt()
{
    return m_startAt;
}

/**
* Sets the "start at" value.
* 
* @param startAt "start at" value
*/
void Group2Clause::setStartAt(ObjectPtr startAt)
{
    m_startAt = startAt;
}
//
///**
//* {@inheritDoc}
//*/
//CString Group2Clause::toString()
//{
//    StringBuilder sb = new StringBuilder();
//    sb.append("[Group2Clause field=");
//    sb.append(m_field);
//    sb.append(" ascending=");
//    sb.append(m_ascending);
//    sb.append(" font=");
//    sb.append(m_font);
//    sb.append(" color=");
//    sb.append(m_cellBackgroundColor);
//    sb.append(" pattern=");
//    sb.append(m_pattern);
//    sb.append(" groupOn=");
//    sb.append(m_groupOn);
//    sb.append(" startAt=");
//    sb.append(m_startAt);
//    sb.append(" groupInterval=");
//    sb.append(m_groupInterval);
//    return (sb.toString());
//}
