#pragma once

#include "Column.h"

/**
 * This class represents the definition of a table of data from an MPP file->
 * Much of the important information held in MS Project is represented
 * in a tabular format. This class represents the attributes associated with
 * these tables. For example, the attributes of the table of data that appears
 * as the left hand part of the standard Gantt Chart view in MS Project will
 * be defined here.
 */
class Table
{
public:
	Table();

   /**
    * This method is used to retrieve the unique table identifier. This
    * value identifies the table within the file-> It does not identify
    * the type of table represented by an instance of this class.
    *
    * @return table identifier
    */
	 int getID();

   /**
    * This method is used to to set the unique identifier associated with
    * this table.
    *
    * @param id unique table identifier
    */
	void setID(int id);

   /**
    * This method is used to retrieve the table name. Note that internally
    * in MS Project the table name will contain an ampersand (&) used to
    * flag the letter that can be used as a shortcut for this table. The
    * ampersand is stripped out by MPXJ.
    *
    * @return view name
    */
	CString getName();

   /**
    * This method is used to set the name associated with this table.
    *
    * @param name table name
    */
	void setName(CString name);

   /**
    * This method retrieves the resource flag attribute of the table.
    * This attribute represents whether the table refers to
    * task data (false) or resource data (true).
    *
    * @return bool flag
    */
	bool getResource2Flag();

   /**
    * This method sets the resource flag attribute of the table.
    * This attribute represents whether the table refers to
    * task data (false) or resource data (true).
    *
    * @param flag bool flag
    */
	void setResource2Flag(bool flag);

   /**
    * Adds a column definition to this table.
    *
    * @param column column definition
    */
	void addColumn(ColumnPtr column);

   /**
    * Retrieves the list of columns that make up this table.
    *
    * @return list of columns
    */
	List<ColumnPtr>& getColumns();

 //  /**
 //   * This method dumps the contents of this table as a CString.
 //   * Note that this facility is provided as a debugging aid.
 //   *
 //   * @return formatted contents of this table
 //   */
	//virtual CString toString();

private:
   int m_id;
   CString m_name;
   bool m_resourceFlag;
   List<ColumnPtr> m_columns;// = new ArrayList<Column>();
};

typedef std::shared_ptr<Table> TablePtr;