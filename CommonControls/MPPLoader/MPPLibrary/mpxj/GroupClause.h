#pragma once

#include "FieldType.h"
#include "mpp\FontStyle.h"

/**
 * This class represents a clause from a definition of a group.
 */
class Group2Clause
{
public:
	Group2Clause();
   /**
    * Retrieve a flag indicating that values are grouped
    * in ascending order.
    * 
    * @return bool flag
    */
	bool getAscending();

   /**
    * Sets a flag indicating that values are grouped
    * in ascending order.
    * 
    * @param ascending bool flag
    */
	void setAscending(bool ascending);

   /**
    * Retrieves the background color.
    * 
    * @return background color
    */
	COLORREF getCellBackgroundColor();

   /**
    * Sets the background color.
    * 
    * @param color background color.
    */
	void setCellBackgroundColor(COLORREF color);

   /**
    * Retrieve the grouping field.
    * 
    * @return grouping field
    */
	FieldType* getField();

   /**
    * Set the grouping field.
    * 
    * @param field grouping field
    */
	void setField(FieldType* field);

   /**
    * Retrieve the font.
    * 
    * @return font
    */
	FontStylePtr getFont();

   /**
    * Retrieve the font.
    * 
    * @param font font
    */
	void setFont(FontStylePtr font);

   /**
    * Retrieve the group interval.
    * 
    * @return group interval
    */
	ObjectPtr getGroup2Interval();

   /**
    * Sets the group interval.
    * 
    * @param groupInterval group interval
    */
	void setGroup2Interval(ObjectPtr groupInterval);

   /**
    * Retrieves the group on value.
    * 
    * @return group on value
    */
	int getGroup2On();

   /**
    * Sets the group on value.
    * 
    * @param groupOn group on value
    */
	void setGroup2On(int groupOn);

   /**
    * Retrieves the pattern.
    * 
    * @return pattern
    */
	BackgroundPattern getPattern();

   /**
    * Sets the pattern.
    * 
    * @param pattern pattern
    */
	void setPattern(BackgroundPattern pattern);

   /**
    * Retrieves the "start at" value.
    * 
    * @return "start at" value
    */
	ObjectPtr getStartAt();

   /**
    * Sets the "start at" value.
    * 
    * @param startAt "start at" value
    */
	void setStartAt(ObjectPtr startAt);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
	FieldType* m_field;
    bool m_ascending;
    FontStylePtr m_font;
    COLORREF m_cellBackgroundColor;
    BackgroundPattern m_pattern;
    int m_groupOn; // TODO can we do this as an enumeration?
    ObjectPtr m_startAt;
    ObjectPtr m_groupInterval;
};

typedef std::shared_ptr<Group2Clause> Group2ClausePtr;
