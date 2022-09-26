#pragma once

#include "..\ProjectFile.h"

#include "FieldMap.h"
#include "VarMeta.h"
#include "Var2Data.h"
#include "FixedMeta.h"
#include "FixedData.h"
#include "MppBitFlag.h"

/**
 * Common implementation detail to extract resource assignment data from 
 * MPP9, MPP12, and MPP14 files.
 */
class Resource2AssignmentFactory
{
public:
   /**
    * Main entry point when called to process assignment data.
    * 
    * @param file parent project file
    * @param fieldMap assignment field map
    * @param enterpriseCustomFieldMap enterprise custom field map
    * @param useRawTimephasedData use raw timephased data flag
    * @param preserveNoteFormatting preserve note formatting flag
    * @param assnVarMeta var meta
    * @param assnVarData var data
    * @param assnFixedMeta fixed meta
    * @param assnFixedData fixed data
    * @param assnFixedData2 fixed data
    * @param count expected number of assignments
    */
	void process(ProjectFile* file, FieldMapPtr fieldMap, FieldMapPtr enterpriseCustomFieldMap,
		bool useRawTimephasedData, bool preserveNoteFormatting, VarMetaPtr assnVarMeta, Var2DataPtr assnVarData,
		FixedMetaPtr assnFixedMeta, FixedDataPtr assnFixedData, FixedDataPtr assnFixedData2, int count);

private:
   /**
    * Extract assignment hyperlink data. 
    * 
    * @param assignment assignment instance
    * @param data hyperlink data
    */
	void processHyperlinkData(Resource2AssignmentPtr assignment, ByteArrayPtr data);

   /**
    * Method used to create missing timephased data.
    * 
    * @param file project file
    * @param assignment resource assignment
    * @param timephasedPlanned planned timephased data
    * @param timephasedComplete complete timephased data
    */
   void createTimephasedData(ProjectFile* file, Resource2AssignmentPtr assignment,
	   TimephasedWorkListPtr timephasedPlanned, TimephasedWorkListPtr timephasedComplete);

private:
	static const int MPP9_CREATION_DATA = 138;
};