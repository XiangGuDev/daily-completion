#pragma once

#include "TestOperator.h"
#include "FieldType.h"

class ProjectFile;
class FieldContainer;
class GenericCriteriaPrompt;

class GenericCriteria;

typedef std::shared_ptr<GenericCriteria> GenericCriteriaPtr;

/**
 * This class represents the criteria used as part of an evaluation.
 */
class GenericCriteria
{
public:
   /**
    * Constructor.
    * 
    * @param projectFile parent project file
    */
	GenericCriteria(ProjectFile* projectFile);

   /**
    * Sets the LHS of the expression.
    * 
    * @param value LHS value
    */
	void setLeftValue(FieldType* value);

   /**
    * Retrieves the LHS of the expression.
    * 
    * @return LHS value
    */
	FieldType* getLeftValue();

   /**
    * Retrieve the operator used in the test.
    * 
    * @return test operator
    */
	TestOperator* getOperator();

   /**
    * Set the operator used in the test.
    * 
    * @param operator test operator
    */
	void setOperator(TestOperator* op);

   /**
    * Add the value to list of values to be used as part of the
    * evaluation of this indicator.
    * 
    * @param index position in the list
    * @param value evaluation value
    */
	void setRightValue(int index, Object2* value);

   /**
    * Retrieve the first value.
    * 
    * @param index position in the list
    * @return first value
    */
	Object2* getValue(int index);

   /**
    * Evaluate the criteria and return a bool result.
    * 
    * @param container field container
    * @param promptValues responses to prompts
    * @return bool flag
    */
	bool evaluate(FieldContainer* container, Map2<GenericCriteriaPrompt*, ObjectPtr>* promptValues);

   /**
    * Retrieves the list of child criteria associated with the current criteria.
    * 
    * @return list of criteria
    */
	List<GenericCriteriaPtr>* getCriteriaList();

   /**
    * Adds a an item to the list of child criteria.
    * 
    * @param criteria criteria item to add
    */
	void addCriteria(GenericCriteriaPtr criteria);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
	/**
	* Evalutes AND and OR operators.
	*
	* @param container data context
	* @param promptValues responses to prompts
	* @return operator result
	*/
	bool evaluateLogicalOperator(FieldContainer* container, 
				Map2<GenericCriteriaPrompt*, ObjectPtr>* promptValues);

	/**
	* This method is called to create a new list of values, converting from
	* any symbolic values (represented by FieldType instances) to actual
	* values retrieved from a Task or Resource2 instance.
	*
	* @param oldValues list of old values containing symbolic items
	* @param container Task or Resource2 instance
	* @param promptValues response to prompts
	* @return new list of actual values
	*/
	ObjectArrayPtr processSymbolicValues(ObjectPtrArray* oldValues,
		FieldContainer* container, Map2<GenericCriteriaPrompt*, ObjectPtr>* promptValues);

private:
	ProjectFile* m_projectFile;
	FieldType* m_leftValue;
	TestOperator* m_operator;
	Object2* m_definedRightValues[2];// = new Object[2];
	Object2* m_workingRightValues[2];// = new Object[2];
	bool m_symbolicValues;
	List<GenericCriteriaPtr> m_criteriaList;// = new List<GenericCriteria>();
};