#include "stdafx.h"
#include "GraphicalIndicator.h"
#include "Task.h"

/**
* This method evaluates a if a graphical indicator should
* be displayed, given a set of Task or Resource data. The
* method will return -1 if no indicator should be displayed.
* 
* @param container Task or Resource instance
* @return indicator index
*/
int GraphicalIndicator::evaluate(FieldContainer* container)
{
    //
    // First step - determine the list of criteria we are should use
    //
    List<GraphicalIndicatorCriteriaPtr>* criteria = NULL;
    if (dynamic_cast<Task*>(container))// instanceof Task)
    {
		Task* task = dynamic_cast<Task*>(container);
        if (task->getUniqueID() == 0)
        {
			if (m_projectSummaryInheritsFromSummaryRows == false)
			{
				criteria = &m_projectSummaryCriteria;
			}
			else
			{
				if (m_summaryRowsInheritFromNonSummaryRows == false)
				{
					criteria = &m_summaryRowCriteria;
				}
				else
				{
					criteria = &m_nonSummaryRowCriteria;
				}
			}
        }
        else
        {
			if (task->getSummary() == true)
			{
				if (m_summaryRowsInheritFromNonSummaryRows == false)
				{
					criteria = &m_summaryRowCriteria;
				}
				else
				{
					criteria = &m_nonSummaryRowCriteria;
				}
			}
			else
			{
				criteria = &m_nonSummaryRowCriteria;
			}
        }
    }
    else
    {
        // It is possible to have a resource summary row, but at the moment
        // I can't see how you can determine this.
        criteria = &m_nonSummaryRowCriteria;
    }

    //
    // Now we have the criteria, evaluate each one until we get a result
    //
    int result = -1;
	for (size_t i = 0; i < criteria->GetCount(); i ++)
    {
		GraphicalIndicatorCriteriaPtr gic = criteria->GetAt(i);
        result = gic->evaluate(container);
        if (result != -1)
        {
			break;
        }
    }

    //
    // If we still don't have a result at the end, return the
    // default value, which is 0
    //
    if (result == -1)
    {
        result = 0;
    }

    return (result);
}

/**
* Sets the field type to which this indicator applies.
* 
* @param fieldType field type
*/
void GraphicalIndicator::setFieldType(FieldType* fieldType)
{
    m_fieldType = fieldType;
}

/**
* Retrieves the field type to which this indicator applies.
* 
* @return field type
*/
FieldType* GraphicalIndicator::getFieldType()
{
    return (m_fieldType);
}

/**
* Retrieves a flag indicating if graphical indicators should be displayed
* for this column, rather than the actual values.
* 
* @return bool flag
*/
bool GraphicalIndicator::getDisplayGraphicalIndicators()
{
    return (m_displayGraphicalIndicators);
}

/**
* Sets a flag indicating if graphical indicators should be displayed
* for this column, rather than the actual values.
* 
* @param displayGraphicalIndicators bool flag
*/
void GraphicalIndicator::setDisplayGraphicalIndicators(bool displayGraphicalIndicators)
{
    m_displayGraphicalIndicators = displayGraphicalIndicators;
}

/**
* Retrieve the criteria to be applied to non-summary rows.
* 
* @return list of non-summary row criteria
*/
List<GraphicalIndicatorCriteriaPtr>* GraphicalIndicator::getNonSummaryRowCriteria()
{
    return &m_nonSummaryRowCriteria;
}

/**
* Retrieve the criteria to be applied to the project summary.
* 
* @return list of project summary criteria
*/
List<GraphicalIndicatorCriteriaPtr>* GraphicalIndicator::getProjectSummaryCriteria()
{
    return &m_projectSummaryCriteria;
}

/**
* Retrieve the criteria to be applied to summary rows.
* 
* @return list of summary row criteria
*/
List<GraphicalIndicatorCriteriaPtr>* GraphicalIndicator::getSummaryRowCriteria()
{
    return &m_summaryRowCriteria;
}

/**
* Retrieves a flag which indicates if the project summary row inherits
* criteria from the summary row.
* 
* @return bool flag
*/
bool GraphicalIndicator::getProjectSummaryInheritsFromSummaryRows()
{
    return (m_projectSummaryInheritsFromSummaryRows);
}

/**
* Sets a flag which indicates if the project summary row inherits
* criteria from the summary row.
* 
* @param projectSummaryInheritsFromSummaryRows bool flag
*/
void GraphicalIndicator::setProjectSummaryInheritsFromSummaryRows(bool projectSummaryInheritsFromSummaryRows)
{
    m_projectSummaryInheritsFromSummaryRows = projectSummaryInheritsFromSummaryRows;
}

/**
* Retrieves a flag which indicates if summary rows inherit
* criteria from non-summary rows.
* 
* @return bool flag
*/
bool GraphicalIndicator::getSummaryRowsInheritFromNonSummaryRows()
{
    return (m_summaryRowsInheritFromNonSummaryRows);
}

/**
* Sets a flag which indicates if summary rows inherit
* criteria from non-summary rows.
* 
* @param summaryRowsInheritFromNonSummaryRows bool flag
*/
void GraphicalIndicator::setSummaryRowsInheritFromNonSummaryRows(bool summaryRowsInheritFromNonSummaryRows)
{
    m_summaryRowsInheritFromNonSummaryRows = summaryRowsInheritFromNonSummaryRows;
}

/**
* Retrieve the flag which indicates that data values should be shown
* as tool tips.
* 
* @return bool flag
*/
bool GraphicalIndicator::getShowDataValuesInToolTips()
{
    return (m_showDataValuesInToolTips);
}

/**
* Set the flag which indicates that data values should be shown
* as tool tips.
* 
* @param showDataValuesInToolTips bool flag
*/
void GraphicalIndicator::setShowDataValuesInToolTips(bool showDataValuesInToolTips)
{
    m_showDataValuesInToolTips = showDataValuesInToolTips;
}

/**
* Add criteria relating to non summary rows.
* 
* @param criteria indicator criteria
*/
void GraphicalIndicator::addNonSummaryRowCriteria(GraphicalIndicatorCriteriaPtr criteria)
{
	m_nonSummaryRowCriteria.Add(criteria);
}

/**
* Add criteria relating to summary rows.
* 
* @param criteria indicator criteria
*/
void GraphicalIndicator::addSummaryRowCriteria(GraphicalIndicatorCriteriaPtr criteria)
{
	m_summaryRowCriteria.Add(criteria);
}

/**
* Add criteria relating to project summary.
* 
* @param criteria indicator criteria
*/
void GraphicalIndicator::addProjectSummaryCriteria(GraphicalIndicatorCriteriaPtr criteria)
{
    m_projectSummaryCriteria.Add(criteria);
}

///**
//* {@inheritDoc}
//*/
//CString GraphicalIndicator::toString()
//{
//    ByteArrayOutputStream os = new ByteArrayOutputStream();
//    PrintWriter pw = new PrintWriter(os);
//    pw.println("[GraphicalIndicator");
//    pw.println(" FieldType=" + m_fieldType);
//    pw.println(" DisplayGraphicalIndicators=" + m_displayGraphicalIndicators);
//    pw.println(" SummaryRowsInheritFromNonSummaryRows=" + m_summaryRowsInheritFromNonSummaryRows);
//    pw.println(" ProjectSummaryInheritsFromSummaryRows=" + m_projectSummaryInheritsFromSummaryRows);
//    pw.println(" ShowDataValuesInToolTips=" + m_showDataValuesInToolTips);
//    pw.println(" NonSummaryRowCriteria=");
//    for (GraphicalIndicatorCriteria gi : m_nonSummaryRowCriteria)
//    {
//        pw.println("  " + gi);
//    }
//    pw.println(" SummaryRowCriteria=");
//    for (GraphicalIndicatorCriteria gi : m_summaryRowCriteria)
//    {
//        pw.println("  " + gi);
//    }
//    pw.println(" ProjectSummaryCriteria=");
//    for (GraphicalIndicatorCriteria gi : m_projectSummaryCriteria)
//    {
//        pw.println("  " + gi);
//    }
//    pw.println("]");
//    pw.flush();
//    return (os.toString());
//}