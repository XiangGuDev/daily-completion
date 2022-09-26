#pragma once

#include "GenericCriteriaPrompt.h"
#include "GenericCriteria.h"

class FieldContainer;


/**
 * This class represents a filter which may be applied to a
 * task or resource view.
 */
class Filter2
{
public:
	Filter2();

	/**
	 * Sets the filter's unique ID.
	 *
	 * @param id unique ID
	 */
	void setID(int id);

	/**
	 * Retrieves the filter's unique ID.
	 *
	 * @return unique ID
	 */
	int getID();

	/**
	 * Sets the filter's name.
	 *
	 * @param name filter name
	 */
	void setName(CString name);

	/**
	 * Retrieves the filter's name.
	 *
	 * @return filter name
	 */
	CString getName();

	/**
	 * Sets the "show related summary rows" flag.
	 *
	 * @param showRelatedSummaryRows bool flag
	 */
	void setShowRelatedSummaryRows(bool showRelatedSummaryRows);

	/**
	 * Retrieves the "show related summary rows" flag.
	 *
	 * @return bool flag
	 */
	bool getShowRelatedSummaryRows();

	///**
	// * Sets the criteria associted with this filter.
	// *
	// * @param criteria filter criteria
	// */
	//void setCriteria(GenericCriteriaPtr criteria);

	///**
	// * Retrieve the criteria used to define this filter.
	// *
	// * @return list of filter criteria
	// */
	//GenericCriteriaPtr getCriteria();

	/**
	 * Retrieves a flag indicating if this is a task filter.
	 *
	 * @return bool flag
	 */
	bool isTaskFilter();

	/**
	 * Sets the flag indicating if this is a task filter.
	 *
	 * @param flag task filter flag
	 */
	void setIsTaskFilter(bool flag);

	/**
	 * Retrieves a flag indicating if this is a resource filter.
	 *
	 * @return bool flag
	 */
	bool isResource2Filter();

	/**
	 * Sets the flag indicating if this is a resource filter.
	 *
	 * @param flag resource filter flag
	 */
	void setIsResource2Filter(bool flag);

	///**
	// * Evaluates the filter, returns true if the supplied Task or Resource2
	// * instance matches the filter criteria.
	// *
	// * @param container Task or Resource2 instance
	// * @param promptValues respose to prompts
	// * @return bool flag
	// */
	//bool evaluate(FieldContainer* container, Map<GenericCriteriaPrompt*, ObjectPtr>* promptValues);

	///**
	// * Sets the prompts to supply the parameters required by this filter.
	// *
	// * @param prompts filter prompts
	// */
	//void setPrompts(GenericCriteriaPromptListPtr prompts);

	///**
	// * Retrieves the prompts required to supply parameters to this filter.
	// *
	// * @return filter prompts
	// */
	//GenericCriteriaPromptListPtr getPrompts();

	/**
	 * {@inheritDoc}
	 */
	//virtual CString toString();


private:
	int m_id;
	CString m_name;
	bool m_isTaskFilter;
	bool m_isResource2Filter;
	bool m_showRelatedSummaryRows;
	//GenericCriteriaPtr m_criteria;
	//GenericCriteriaPromptListPtr m_prompts;
};

typedef std::shared_ptr<Filter2> FilterPtr;