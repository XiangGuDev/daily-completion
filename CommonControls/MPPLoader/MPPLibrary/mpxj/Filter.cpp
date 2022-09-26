#include "stdafx.h"
#include "Filter.h"
#include "GenericCriteria.h"
#include "GenericCriteriaPrompt.h"
#include "Task.h"

Filter2::Filter2()
	:m_id(0),
	m_isTaskFilter(false),
	m_isResource2Filter(false),
	m_showRelatedSummaryRows(false)//,
	//m_criteria(NULL),
	//m_prompts(NULL)
{

}

/**
* Sets the filter's unique ID.
* 
* @param id unique ID
*/
void Filter2::setID(int id)
{
    m_id = id;
}

/**
* Retrieves the filter's unique ID.
* 
* @return unique ID
*/
int Filter2::getID()
{
    return (m_id);
}

/**
* Sets the filter's name.
* 
* @param name filter name
*/
void Filter2::setName(CString name)
{
    m_name = name;
}

/**
* Retrieves the filter's name.
* 
* @return filter name
*/
CString Filter2::getName()
{
    return (m_name);
}

/**
* Sets the "show related summary rows" flag.
* 
* @param showRelatedSummaryRows bool flag
*/
void Filter2::setShowRelatedSummaryRows(bool showRelatedSummaryRows)
{
    m_showRelatedSummaryRows = showRelatedSummaryRows;
}

/**
* Retrieves the "show related summary rows" flag.
* 
* @return bool flag
*/
bool Filter2::getShowRelatedSummaryRows()
{
    return (m_showRelatedSummaryRows);
}

///**
//* Sets the criteria associted with this filter.
//* 
//* @param criteria filter criteria
//*/
//void Filter::setCriteria(GenericCriteriaPtr criteria)
//{
//    m_criteria = criteria;
//}
//
///**
//* Retrieve the criteria used to define this filter.
//* 
//* @return list of filter criteria
//*/
//GenericCriteriaPtr Filter::getCriteria()
//{
//    return m_criteria;
//}

/**
* Retrieves a flag indicating if this is a task filter.
* 
* @return bool flag
*/
bool Filter2::isTaskFilter()
{
    return m_isTaskFilter;
}

/**
* Sets the flag indicating if this is a task filter.
* 
* @param flag task filter flag
*/
void Filter2::setIsTaskFilter(bool flag)
{
    m_isTaskFilter = flag;
}

/**
* Retrieves a flag indicating if this is a resource filter.
* 
* @return bool flag
*/
bool Filter2::isResource2Filter()
{
    return m_isResource2Filter;
}

/**
* Sets the flag indicating if this is a resource filter.
* 
* @param flag resource filter flag
*/
void Filter2::setIsResource2Filter(bool flag)
{
    m_isResource2Filter = flag;
}

///**
//* Evaluates the filter, returns true if the supplied Task or Resource2
//* instance matches the filter criteria.
//* 
//* @param container Task or Resource2 instance
//* @param promptValues respose to prompts
//* @return bool flag
//*/
//bool Filter::evaluate(FieldContainer* container, Map<GenericCriteriaPrompt*, ObjectPtr>* promptValues)
//{
//    bool result = true;
//    if (m_criteria != NULL)
//    {
//        result = m_criteria->evaluate(container, promptValues);
//
//        //
//        // If this row has failed, but it is a summary row, and we are
//        // including related summary rows, then we need to recursively test
//        // its children
//        //
//		Task* task = dynamic_cast<Task*>(container);
//        if (!result && m_showRelatedSummaryRows && task)
//        {
//			List<Task*>* TaskList = task->getChildTasks();
//			for (size_t i = 0; i < TaskList->GetCount(); i ++)
//			{
//				Task* task = TaskList->GetAt(i);
//				if (evaluate(task.get(), promptValues))
//				{
//					result = true;
//					break;
//				}
//			}
//        }
//    }
//
//    return (result);
//}
//
///**
//* Sets the prompts to supply the parameters required by this filter.
//* 
//* @param prompts filter prompts
//*/
//void Filter::setPrompts(GenericCriteriaPromptListPtr prompts)
//{
//    m_prompts = prompts;
//}
//
///**
//* Retrieves the prompts required to supply parameters to this filter.
//* 
//* @return filter prompts
//*/
//GenericCriteriaPromptListPtr Filter::getPrompts()
//{
//    return m_prompts;
//}

///**
//* {@inheritDoc}
//*/
//CString Filter::toString()
//{
//    StringBuilder sb = new StringBuilder();
//    sb.append("[Filter id=");
//    sb.append(m_id);
//    sb.append(" name=");
//    sb.append(m_name);
//    sb.append(" showRelatedSummaryRows=");
//    sb.append(m_showRelatedSummaryRows);
//    if (m_criteria != NULL)
//    {
//        sb.append(" criteria=");
//        sb.append(m_criteria);
//    }
//    sb.append("]");
//
//    return (sb.toString());
//}