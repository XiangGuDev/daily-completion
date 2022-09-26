#pragma once

#include "FieldContainer.h"
#include "GraphicalIndicatorCriteria.h"

/**
 * This class represents the set of information which defines how
 * a Graphical Indicator will be presented for a single column in
 * a table within Microsoft Project.
 */
class GraphicalIndicator
{
public:
   /**
    * This method evaluates a if a graphical indicator should
    * be displayed, given a set of Task or Resource data. The
    * method will return -1 if no indicator should be displayed.
    * 
    * @param container Task or Resource instance
    * @return indicator index
    */
	int evaluate(FieldContainer* container);

   /**
    * Sets the field type to which this indicator applies.
    * 
    * @param fieldType field type
    */
	void setFieldType(FieldType* fieldType);

   /**
    * Retrieves the field type to which this indicator applies.
    * 
    * @return field type
    */
	FieldType* getFieldType();

   /**
    * Retrieves a flag indicating if graphical indicators should be displayed
    * for this column, rather than the actual values.
    * 
    * @return bool flag
    */
	bool getDisplayGraphicalIndicators();

   /**
    * Sets a flag indicating if graphical indicators should be displayed
    * for this column, rather than the actual values.
    * 
    * @param displayGraphicalIndicators bool flag
    */
	void setDisplayGraphicalIndicators(bool displayGraphicalIndicators);

   /**
    * Retrieve the criteria to be applied to non-summary rows.
    * 
    * @return list of non-summary row criteria
    */
	List<GraphicalIndicatorCriteriaPtr>* getNonSummaryRowCriteria();

   /**
    * Retrieve the criteria to be applied to the project summary.
    * 
    * @return list of project summary criteria
    */
	List<GraphicalIndicatorCriteriaPtr>* getProjectSummaryCriteria();

   /**
    * Retrieve the criteria to be applied to summary rows.
    * 
    * @return list of summary row criteria
    */
	List<GraphicalIndicatorCriteriaPtr>* getSummaryRowCriteria();

   /**
    * Retrieves a flag which indicates if the project summary row inherits
    * criteria from the summary row.
    * 
    * @return bool flag
    */
	bool getProjectSummaryInheritsFromSummaryRows();

   /**
    * Sets a flag which indicates if the project summary row inherits
    * criteria from the summary row.
    * 
    * @param projectSummaryInheritsFromSummaryRows bool flag
    */
	void setProjectSummaryInheritsFromSummaryRows(bool projectSummaryInheritsFromSummaryRows);

   /**
    * Retrieves a flag which indicates if summary rows inherit
    * criteria from non-summary rows.
    * 
    * @return bool flag
    */
	bool getSummaryRowsInheritFromNonSummaryRows();

   /**
    * Sets a flag which indicates if summary rows inherit
    * criteria from non-summary rows.
    * 
    * @param summaryRowsInheritFromNonSummaryRows bool flag
    */
	void setSummaryRowsInheritFromNonSummaryRows(bool summaryRowsInheritFromNonSummaryRows);

   /**
    * Retrieve the flag which indicates that data values should be shown
    * as tool tips.
    * 
    * @return bool flag
    */
	bool getShowDataValuesInToolTips();

   /**
    * Set the flag which indicates that data values should be shown
    * as tool tips.
    * 
    * @param showDataValuesInToolTips bool flag
    */
	void setShowDataValuesInToolTips(bool showDataValuesInToolTips);

   /**
    * Add criteria relating to non summary rows.
    * 
    * @param criteria indicator criteria
    */
	void addNonSummaryRowCriteria(GraphicalIndicatorCriteriaPtr criteria);

   /**
    * Add criteria relating to summary rows.
    * 
    * @param criteria indicator criteria
    */
	void addSummaryRowCriteria(GraphicalIndicatorCriteriaPtr criteria);

   /**
    * Add criteria relating to project summary.
    * 
    * @param criteria indicator criteria
    */
	void addProjectSummaryCriteria(GraphicalIndicatorCriteriaPtr criteria);

   ///**
   // * {@inheritDoc}
   // */
   //virtual CString toString();
  
private:
	FieldType* m_fieldType;
   bool m_displayGraphicalIndicators;
   bool m_summaryRowsInheritFromNonSummaryRows;
   bool m_projectSummaryInheritsFromSummaryRows;
   bool m_showDataValuesInToolTips;
   List<GraphicalIndicatorCriteriaPtr> m_nonSummaryRowCriteria;// = new List<GraphicalIndicatorCriteria>();
   List<GraphicalIndicatorCriteriaPtr> m_summaryRowCriteria;// = new List<GraphicalIndicatorCriteria>();
   List<GraphicalIndicatorCriteriaPtr> m_projectSummaryCriteria;// = new List<GraphicalIndicatorCriteria>();
};

typedef std::shared_ptr<GraphicalIndicator> GraphicalIndicatorPtr;