#pragma once

#include "MpxjEnum.h"
#include "Duration.h"

class Task;

/**
 * This class represents the relationship between two tasks.
 */
class COMMONCONTROLS_API Relation : public Object2
{
public:
   /**
    * Default constructor.
    *
    * @param sourceTask source task instance
    * @param targetTask target task instance
    * @param type relation type
    * @param lag relation lag
    */
	Relation(Task* sourceTask, Task* targetTask, RelationType type, DurationPtr lag);

   /**
    * Method used to retrieve the type of relationship being
    * represented.
    *
    * @return relationship type
    */
	RelationType getType();

   /**
    * This method retrieves the lag duration associated
    * with this relationship.
    *
    * @return lag duration
    */
	DurationPtr getLag();

   /**
    * Retrieve the source task of this relationship.
    * 
    * @return source task
    */
	Task* getSourceTask();

   /**
    * Retrieve the target task of this relationship.
    *
    * @return target task
    */
	Task* getTargetTask();

   /**
    * {@inheritDoc}
    */
	virtual CString toString();

private:
   /**
    * Parent task file-> 
    */
	Task* m_sourceTask;

   /**
    * Identifier of task with which this relationship is held.
    */
	Task* m_targetTask;

   /**
    * Type of relationship.
    */
   RelationType m_type;

   /**
    * Lag between the two tasks.
    */
   DurationPtr m_lag;
};

typedef std::shared_ptr<Relation> RelationPtr;