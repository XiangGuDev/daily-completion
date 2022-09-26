#pragma once

#include "GroupClause.h"

/**
 * This class represents the definition of the grouping used
 * to organise data in a view.
 */
class Group2
{
public:
   /**
    * Constructor.
    * 
    * @param id group identifier
    * @param name group name
    * @param showSummaryTasks show summary tasks
    */
	Group2(int id, CString name, bool showSummaryTasks);

   /**
    * Retrieve group ID.
    * 
    * @return group ID
    */
	int getID();

   /**
    * Retrieve the group name.
    * 
    * @return group name
    */
	CString getName();

   /**
    * Retrieve the show summary tasks flag.
    * 
    * @return bool flag
    */
	bool getShowSummaryTasks();

   /**
    * Adds a clause to the group definition.
    * 
    * @param clause group clause
    */
	void addGroup2Clause(Group2ClausePtr clause);

   /**
    * Retrieve a list of all clauses which define this group.
    * 
    * @return list of clauses
    */
	List<Group2ClausePtr>* getGroup2Clauses();

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
	int m_id;
	CString m_name;
	bool m_showSummaryTasks;
	List<Group2ClausePtr> m_clauses;// = new List<Group2Clause>();
};

typedef std::shared_ptr<Group2> Group2Ptr;
