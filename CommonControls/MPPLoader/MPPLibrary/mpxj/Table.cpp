#include "stdafx.h"
#include "Table.h"

Table::Table()
	:m_id(0),
	m_resourceFlag(false)
{

}

/**
* This method is used to retrieve the unique table identifier. This
* value identifies the table within the file-> It does not identify
* the type of table represented by an instance of this class.
*
* @return table identifier
*/
int Table::getID()
{
    return (m_id);
}

/**
* This method is used to to set the unique identifier associated with
* this table.
*
* @param id unique table identifier
*/
void Table::setID(int id)
{
    m_id = id;
}

/**
* This method is used to retrieve the table name. Note that internally
* in MS Project the table name will contain an ampersand (&) used to
* flag the letter that can be used as a shortcut for this table. The
* ampersand is stripped out by MPXJ.
*
* @return view name
*/
CString Table::getName()
{
    return (m_name);
}

/**
* This method is used to set the name associated with this table.
*
* @param name table name
*/
void Table::setName(CString name)
{
    m_name = name;
}

/**
* This method retrieves the resource flag attribute of the table.
* This attribute represents whether the table refers to
* task data (false) or resource data (true).
*
* @return bool flag
*/
bool Table::getResource2Flag()
{
    return (m_resourceFlag);
}

/**
* This method sets the resource flag attribute of the table.
* This attribute represents whether the table refers to
* task data (false) or resource data (true).
*
* @param flag bool flag
*/
void Table::setResource2Flag(bool flag)
{
    m_resourceFlag = flag;
}

/**
* Adds a column definition to this table.
*
* @param column column definition
*/
void Table::addColumn(ColumnPtr column)
{
    m_columns.Add(column);
}

/**
* Retrieves the list of columns that make up this table.
*
* @return list of columns
*/
List<ColumnPtr>& Table::getColumns()
{
    return (m_columns);
}

///**
//* This method dumps the contents of this table as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this table
//*/
//CString Table::toString()
//{
//	StringWriter sw = new StringWriter();
//	PrintWriter pw = new PrintWriter(sw);
//
//	pw.print("[TABLE id=");
//	pw.print(m_id);
//	pw.print(" name=");
//	pw.print(m_name);
//	pw.print(" resourceFlag=");
//	pw.println(m_resourceFlag);
//
//	for (Column c : m_columns)
//	{
//		pw.print("   ");
//		pw.print(c);
//	}
//
//	pw.println("]");
//	pw.close();
//
//	return (sw.toString());
//}