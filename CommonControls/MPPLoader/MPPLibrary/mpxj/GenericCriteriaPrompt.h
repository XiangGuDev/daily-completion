#pragma once

#include "MpxjEnum.h"

/**
 * Represents a prompt to the user as part of filter criteria.
 */
class GenericCriteriaPrompt : public Object2
{
public:
   /**
    * Constructor.
    * 
    * @param type prompt data type
    * @param prompt text
    */
	GenericCriteriaPrompt(DataType type, CString prompt);

   /**
    * Retrieve the data type of the expected value. 
    * 
    * @return data type
    */
	DataType getType();

   /**
    * Retrieves the prompt text.
    * 
    * @return prompt text
    */
	CString getPrompt();

   /**
    * {@inheritDoc}
    */
	virtual CString toString();

private:
    DataType m_type;
   CString m_prompt;
};

typedef std::shared_ptr<GenericCriteriaPrompt> GenericCriteriaPromptPtr;

typedef List<GenericCriteriaPromptPtr> GenericCriteriaPromptList;

typedef std::shared_ptr<GenericCriteriaPromptList> GenericCriteriaPromptListPtr;