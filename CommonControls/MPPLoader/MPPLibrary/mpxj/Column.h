#pragma once

#include "FieldType.h"

class ProjectFile;

/**
 * This class represents a column in an MS Project table. The attributes held
 * here describe the layout of the column, along with the title text that has
 * been associated with the column. The title text will either be the default
 * value supplied by MS Project, or it will be a user defined value.
 */
class Column
{
public:
   /**
    * Constructor.
    * 
    * @param project reference to the parent project
    */
	Column(ProjectFile* project);

   /**
    * Retrieves a value representing the alignment of data displayed in
    * the column.
    *
    * @return alignment type
    */
	int getAlignData();

   /**
    * Retrieves a value representing the alignment of the column title text.
    *
    * @return alignment type
    */
	int getAlignTitle();

   /**
    * Retrieves the type data displayed in the column. This identifier indicates
    * what data will appear in the column, and the default column title
    * that will appear if the user has not provided a user defined column title.
    *
    * @return field type
    */
	FieldType* getFieldType();

   /**
    * Retrieves the column title.
    *
    * @return column title
    */
	CString getTitle();

   /**
    * Retrieves the column title for the given locale.
    *
    * @param locale required locale for the default column title
    * @return column title
    */
	CString getTitle(Locale* locale);

   /**
    * Retrieves the width of the column represented as a number of
    * characters.
    *
    * @return column width
    */
	int getWidth();

   /**
    * Sets the alignment of the data in the column.
    *
    * @param alignment data alignment
    */
	void setAlignData(int alignment);

   /**
    * Sets the alignment of the column title.
    *
    * @param alignment column title alignment
    */
	void setAlignTitle(int alignment);

   /**
    * Sets the type data displayed in the column. This identifier indicates
    * what data will appear in the column, and the default column title
    * that will appear if the user has not provided a user defined column title.
    *
    * @param type field type
    */
	void setFieldType(FieldType* type);

   /**
    * Sets the user defined column title.
    *
    * @param title user defined column title
    */
	void setTitle(CString title);

   /**
    * Sets the width of the column in characters.
    *
    * @param width column width
    */
	void setWidth(int width);

   /**
    * This method dumps the contents of this column as a CString.
    * Note that this facility is provided as a debugging aid.
    *
    * @return formatted contents of this column
    */
	CString toString();

   /**
    * Column alignment constants.
    */
   static const int ALIGN_LEFT = 1;
   static const int ALIGN_CENTER = 2;
   static const int ALIGN_RIGHT = 3;

private:
	FieldType* m_fieldType;
   int m_width;
   int m_alignTitle;
   int m_alignData;
   CString m_title;
   ProjectFile* m_project;
};

typedef std::shared_ptr<Column> ColumnPtr;