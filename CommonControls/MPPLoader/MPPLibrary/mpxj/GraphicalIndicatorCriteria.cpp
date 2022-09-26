#include "stdafx.h"
#include "GraphicalIndicatorCriteria.h"
#include "ProjectFile.h"
/**
* Constructor.
* 
* @param projectFile parent project file
*/
GraphicalIndicatorCriteria::GraphicalIndicatorCriteria(ProjectFile* projectFile)
:GenericCriteria(projectFile)
{

}

/**
* Retrieve the number of the indicator to be displayed.
* 
* @return indicator number
*/
int GraphicalIndicatorCriteria::getIndicator()
{
    return m_indicator;
}

/**
* Set the number of the indicator to be displayed.
* 
* @param indicator indicator number
*/
void GraphicalIndicatorCriteria::setIndicator(int indicator)
{
    m_indicator = indicator;
}

/**
* Evaluate this criteria to determine if a graphical indicator should
* be displayed. This method will return -1 if no indicator should
* be displayed, or it will return a positive integer identifying the
* required indicator.
* 
* @param container field container
* @return bool flag
*/
int GraphicalIndicatorCriteria::evaluate(FieldContainer* container)
{
	return GenericCriteria::evaluate(container, NULL) ? m_indicator : -1;
}

///**
//* {@inheritDoc}
//*/
//CString GraphicalIndicatorCriteria::toString()
//{
//    StringBuilder sb = new StringBuilder();
//    sb.append("[GraphicalIndicatorCriteria indicator=");
//    sb.append(m_indicator);
//    sb.append(" criteria=");
//    sb.append(super.toString());
//    sb.append("]");
//    return (sb.toString());
//}
