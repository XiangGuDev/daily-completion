#include "stdafx.h"
#include "GenericCriteriaPrompt.h"
#include "java\StringBuilder.h"

/**
* Constructor.
* 
* @param type prompt data type
* @param prompt text
*/
GenericCriteriaPrompt::GenericCriteriaPrompt(DataType type, CString prompt)
{
    m_type = type;
    m_prompt = prompt;
}

/**
* Retrieve the data type of the expected value. 
* 
* @return data type
*/
DataType GenericCriteriaPrompt::getType()
{
    return m_type;
}

/**
* Retrieves the prompt text.
* 
* @return prompt text
*/
CString GenericCriteriaPrompt::getPrompt()
{
    return m_prompt;
}

/**
* {@inheritDoc}
*/
CString GenericCriteriaPrompt::toString()
{
	StringBuilder sb;
    sb.append("PROMPT(");
    sb.append(m_prompt);
    sb.append(")");
    return sb.toString();
}