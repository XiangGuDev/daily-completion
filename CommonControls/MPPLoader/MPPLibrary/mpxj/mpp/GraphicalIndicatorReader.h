#pragma once

#include "..\GraphicalIndicatorCriteria.h"

#include "Props.h"

class ProjectFile;

/**
 * This class allows graphical indicator definitions to be read from an MPP
 * file.
 */
class GraphicalIndicatorReader
{
public:
   /**
    * The main entry point for processing graphical indicator definitions.
    * 
    * @param file parent project file
    * @param props properties data
    */
	void process(ProjectFile* file, PropsPtr props);

private:
   /**
    * Processes graphical indicator definitions for each column.
    */
   void processColumns();

   /**
    * Process the graphical indicator criteria for a single column.
    * 
    * @param type field type
    * @return indicator criteria data
    */
   GraphicalIndicatorCriteriaPtr processCriteria(FieldType* type);

   /**
    * Process an operand value used in the definition of the graphical
    * indicator criteria.
    * 
    * @param index position in operand list
    * @param type field type
    * @param criteria indicator criteria
    */
   void processOperandValue(int index, FieldType* type, GraphicalIndicatorCriteriaPtr criteria);

private:
   ByteArrayPtr m_data;
   int m_headerOffset;
   int m_dataOffset;
   ProjectFile* m_file;
};