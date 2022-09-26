#include "stdafx.h"
#include "Column.h"
#include "TaskField.h"
#include "ProjectFile.h"

/**
* Constructor.
* 
* @param project reference to the parent project
*/
Column::Column(ProjectFile* project)
{
    m_project = project;
}

/**
* Retrieves a value representing the alignment of data displayed in
* the column.
*
* @return alignment type
*/
int Column::getAlignData()
{
    return (m_alignData);
}

/**
* Retrieves a value representing the alignment of the column title text.
*
* @return alignment type
*/
int Column::getAlignTitle()
{
    return (m_alignTitle);
}

/**
* Retrieves the type data displayed in the column. This identifier indicates
* what data will appear in the column, and the default column title
* that will appear if the user has not provided a user defined column title.
*
* @return field type
*/
FieldType* Column::getFieldType()
{
    return (m_fieldType);
}

/**
* Retrieves the column title.
*
* @return column title
*/
CString Column::getTitle()
{
	return (getTitle(Locale::getDefault()));
}

/**
* Retrieves the column title for the given locale.
*
* @param locale required locale for the default column title
* @return column title
*/
CString Column::getTitle(Locale* locale)
{
    CString result;

	if (m_title.GetLength())
    {
        result = m_title;
    }
    else
    {
		if (dynamic_cast<TaskFieldClass*> (m_fieldType))
        {
			result = m_project->getTaskFieldAlias((TaskField)m_fieldType->getValue());
        }
		else if (dynamic_cast<Resource2FieldClass*> (m_fieldType))
        {
			result = m_project->getResource2FieldAlias((Resource2Field)m_fieldType->getValue());
        }

		if (result.GetLength() == 0 && m_fieldType != NULL)
        {
			result = m_fieldType->getName(locale);
        }
    }

    return (result);
}

/**
* Retrieves the width of the column represented as a number of
* characters.
*
* @return column width
*/
int Column::getWidth()
{
    return m_width;
}

/**
* Sets the alignment of the data in the column.
*
* @param alignment data alignment
*/
void Column::setAlignData(int alignment)
{
    m_alignData = alignment;
}

/**
* Sets the alignment of the column title.
*
* @param alignment column title alignment
*/
void Column::setAlignTitle(int alignment)
{
    m_alignTitle = alignment;
}

/**
* Sets the type data displayed in the column. This identifier indicates
* what data will appear in the column, and the default column title
* that will appear if the user has not provided a user defined column title.
*
* @param type field type
*/
void Column::setFieldType(FieldType* type)
{
    m_fieldType = type;
}

/**
* Sets the user defined column title.
*
* @param title user defined column title
*/
void Column::setTitle(CString title)
{
    m_title = title;
}

/**
* Sets the width of the column in characters.
*
* @param width column width
*/
void Column::setWidth(int width)
{
    m_width = width;
}
//
///**
//* This method dumps the contents of this column as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this column
//*/
//CString Column::toString()
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.print("[Column type=");
//    pw.print(m_fieldType);
//
//    pw.print(" width=");
//    pw.print(m_width);
//
//    pw.print(" titleAlignment=");
//    if (m_alignTitle == ALIGN_LEFT)
//    {
//        pw.print("LEFT");
//    }
//    else
//    {
//        if (m_alignTitle == ALIGN_CENTER)
//        {
//        pw.print("CENTER");
//        }
//        else
//        {
//        pw.print("RIGHT");
//        }
//    }
//
//    pw.print(" dataAlignment=");
//    if (m_alignData == ALIGN_LEFT)
//    {
//        pw.print("LEFT");
//    }
//    else
//    {
//        if (m_alignData == ALIGN_CENTER)
//        {
//        pw.print("CENTER");
//        }
//        else
//        {
//        pw.print("RIGHT");
//        }
//    }
//
//    pw.print(" title=");
//    pw.print(getTitle());
//    pw.println("]");
//    pw.close();
//
//    return (sw.toString());
//}