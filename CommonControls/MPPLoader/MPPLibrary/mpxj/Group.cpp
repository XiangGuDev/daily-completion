#include "stdafx.h"
#include "Group.h"

/**
* Constructor.
* 
* @param id group identifier
* @param name group name
* @param showSummaryTasks show summary tasks
*/
Group2::Group2(int id, CString name, bool showSummaryTasks)
{
    m_id = id;
    m_name = name;
    m_showSummaryTasks = showSummaryTasks;
}

/**
* Retrieve group ID.
* 
* @return group ID
*/
int Group2::getID()
{
    return m_id;
}

/**
* Retrieve the group name.
* 
* @return group name
*/
CString Group2::getName()
{
    return m_name;
}

/**
* Retrieve the show summary tasks flag.
* 
* @return bool flag
*/
bool Group2::getShowSummaryTasks()
{
    return m_showSummaryTasks;
}

/**
* Adds a clause to the group definition.
* 
* @param clause group clause
*/
void Group2::addGroup2Clause(Group2ClausePtr clause)
{
    m_clauses.Add(clause);
}

/**
* Retrieve a list of all clauses which define this group.
* 
* @return list of clauses
*/
List<Group2ClausePtr>* Group2::getGroup2Clauses()
{
    return &m_clauses;
}
//
///**
//* {@inheritDoc}
//*/
//CString Group2::toString()
//{
//    ByteArray2OutputStream os = new ByteArray2OutputStream();
//    PrintWriter pw = new PrintWriter(os);
//    pw.println("[Group2 ");
//    pw.println(" id=" + m_id);
//    pw.println(" name=" + m_name);
//    pw.println(" showSummaryTasks=" + m_showSummaryTasks);
//    pw.println(" [Clauses=");
//    for (Group2Clause gc : m_clauses)
//    {
//        pw.println("  " + gc);
//    }
//    pw.println(" ]");
//    pw.println("]");
//    pw.flush();
//    return (os.toString());
//
//}
