#pragma once

#include "GenericCriteria.h"

/**
 * This class represents the criteria used to determine if a graphical
 * indicator is displayed in place of an attribute value.
 */
class GraphicalIndicatorCriteria : public GenericCriteria
{
public:
   /**
    * Constructor.
    * 
    * @param projectFile parent project file
    */
	GraphicalIndicatorCriteria(ProjectFile* projectFile);

   /**
    * Retrieve the number of the indicator to be displayed.
    * 
    * @return indicator number
    */
	int getIndicator();

   /**
    * Set the number of the indicator to be displayed.
    * 
    * @param indicator indicator number
    */
	void setIndicator(int indicator);

   /**
    * Evaluate this criteria to determine if a graphical indicator should
    * be displayed. This method will return -1 if no indicator should
    * be displayed, or it will return a positive integer identifying the
    * required indicator.
    * 
    * @param container field container
    * @return bool flag
    */
	int evaluate(FieldContainer* container);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
	int m_indicator;
};

typedef std::shared_ptr<GraphicalIndicatorCriteria> GraphicalIndicatorCriteriaPtr;