#include "stdafx.h"
#include "Relation.h"
#include "Task.h"

/**
* Default constructor.
*
* @param sourceTask source task instance
* @param targetTask target task instance
* @param type relation type
* @param lag relation lag
*/
Relation::Relation(Task* sourceTask, Task* targetTask, RelationType type, DurationPtr lag)
{
    m_sourceTask = sourceTask;
    m_targetTask = targetTask;
    m_type = type;
    m_lag = lag;

    //if (m_type == NULL)
    //{
    //    m_type = RelationType::FINISH_START;
    //}

    if (m_lag == NULL)
    {
        m_lag = Duration2::getInstance(0, TimeUnit::DAYS);
    }
}

/**
* Method used to retrieve the type of relationship being
* represented.
*
* @return relationship type
*/
RelationType Relation::getType()
{
    return (m_type);
}

/**
* This method retrieves the lag duration associated
* with this relationship.
*
* @return lag duration
*/
DurationPtr Relation::getLag()
{
    return (m_lag);
}

/**
* Retrieve the source task of this relationship.
* 
* @return source task
*/
Task* Relation::getSourceTask()
{
    return m_sourceTask;
}

/**
* Retrieve the target task of this relationship.
*
* @return target task
*/
Task* Relation::getTargetTask()
{
	return m_targetTask;
}

/**
* {@inheritDoc}
*/
CString Relation::toString()
{
	CString msg;
	msg.Format(L"%d", CStringToolkit::IntToStr (getTargetTask()->getID()));
	return msg;
    //return ("[Relation " + m_sourceTask + " -> " + m_targetTask + "]");
}