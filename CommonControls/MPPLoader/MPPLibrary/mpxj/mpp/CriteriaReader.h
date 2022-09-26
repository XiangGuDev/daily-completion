#pragma once

#include "..\ProjectFile.h"
//#include "..\GenericCriteria.h"
#include "..\GenericCriteriaPrompt.h"

/**
 * This class allows criteria definitions to be read from an MPP file.
 */
class CriteriaReader
{
protected:
   /**
    * Retrieves the offset of the start of the criteria data.
    * 
    * @return criteria start offset
    */
   virtual int getCriteriaStartOffset() = 0;

   /**
    * Retrieves the criteria block size.
    * 
    * @return criteria block size
    */
   virtual int getCriteriaBlockSize() = 0;

   /**
    * Retrieves the child of the current block.
    * 
    * @param block parent block
    * @return child block
    */
   virtual ByteArrayPtr getChildBlock(ByteArrayPtr block) = 0;

   /**
    * Retrieves the next list sibling of this block.
    * 
    * @param block current block
    * @return next sibling list block
    */
   virtual ByteArrayPtr getListNextBlock(ByteArrayPtr block) = 0;

   /**
    * Retrieves the offset of the start of the text block. 
    * 
    * @param block current block
    * @return text block start offset
    */
   virtual int getTextOffset(ByteArrayPtr block) = 0;

   /**
    * Retrieves the offset of the prompt text.
    * 
    * @param block current block
    * @return prompt text offset
    */
   virtual int getPromptOffset(ByteArrayPtr block) = 0;

   /**
    * Retrieves the offset of the field value.
    * 
    * @return field value offset
    */
   virtual int getValueOffset() = 0;

   /**
    * Retrieves the offset of the time unit field.
    * 
    * @return time unit field offset
    */
   virtual int getTimeUnitsOffset() = 0;

   /**
    * Retrieves offset of value which determines the start of the text block.
    * 
    * @return criteria text start offset
    */
   virtual int getCriteriaTextStartOffset() = 0;

   /**
    * Retrieves a field type value.
    * 
    * @param block criteria block
    * @return field type value
    */
   virtual FieldType* getFieldType(ByteArrayPtr block) = 0;

public:
   /**
    * Main entry point to read criteria data.
    * 
    * @param file parent project file
    * @param data criteria data block
    * @param dataOffset offset of the data start within the larger data block
    * @param entryOffset offset of start node for walking the tree
    * @param prompts optional list to hold prompts
    * @param fields optional list of hold fields
    * @param criteriaType optional array representing criteria types
    * @return first node of the criteria
    */
	GenericCriteriaPtr process(ProjectFile* file, ByteArrayPtr data, int dataOffset, int entryOffset,
	   List<GenericCriteriaPromptPtr>* prompts, List<FieldType*>* fields, BoolArrayPtr criteriaType);

private:
   /**
    * Process a single criteria block.
    * 
    * @param list parent criteria list
    * @param block current block 
    */
	void processBlock(List<GenericCriteriaPtr>* list, ByteArrayPtr block);

   /**
    * Adds a basic LHS OPERATOR RHS block.
    * 
    * @param list parent criteria list
    * @param block current block
    */
   void addCriteria(List<GenericCriteriaPtr>* list, ByteArrayPtr block);

   /**
    * Adds a logical operator block.
    * 
    * @param list parent criteria list
    * @param block current block
    * @param operator logical operator represented by this block
    */
   void addBlock(List<GenericCriteriaPtr>* list, ByteArrayPtr block, TestOperator* oper);

   /**
    * Retrieves the value component of a criteria expression.
    * 
    * @param field field type
    * @param block block data
    * @return field value
    */
   ObjectPtr getValue(FieldType* field, ByteArrayPtr block);

   /**
    * Retrieves a constant value.
    * 
    * @param type field type
    * @param block criteria data block
    * @return constant value
    */
   ObjectPtr getConstantValue(FieldType* type, ByteArrayPtr block);

   /**
    * Retrieves a prompt value.
    * 
    * @param field field type
    * @param block criteria data block
    * @return prompt value
    */
   GenericCriteriaPromptPtr getPromptValue(FieldType* field, ByteArrayPtr block);

private:
   ProjectFile* m_file;
   ByteArrayPtr m_criteriaData;
   BoolArrayPtr m_criteriaType;
   int m_criteriaTextStart;
   int m_dataOffset;
   List<GenericCriteriaPromptPtr>* m_prompts;
   List<FieldType*>* m_fields;

protected:
	Map<int, ByteArrayPtr> m_criteriaBlockMap;// = new TreeMap<Integer, ByteArrayPtr>();
};

typedef std::shared_ptr<CriteriaReader> CriteriaReaderPtr;