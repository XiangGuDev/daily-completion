#include "stdafx.h"
#include "ConstraintFactory.h"
#include "MPPUtility.h"
#include "..\Task.h"
#include "..\poifs\DocumentEntry.h"
#include "FixedMeta.h"
#include "FixedData.h"
#include "..\poifs\DocumentInputStream.h"
#include "EncryptedDocumentInputStream.h"

/**
* Main entry point when called to process constraint data.
* 
* @param projectDir project directory
* @param file parent project file
*/
void ConstraintFactory::process(DirectoryEntryPtr projectDir, ProjectFile* file)
{
    DirectoryEntryPtr consDir;
    try
    {
        consDir = std::dynamic_pointer_cast<DirectoryEntry> (projectDir->getEntry(L"TBkndCons"));
    }
    catch (/*BwProj::*/toolkit::Exception&)
    {
		return;
    }

	if (!consDir) return;

	FixedMeta consFixedMeta(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry>(consDir->getEntry(L"FixedMeta"))), 10);
    FixedData consFixedData (&consFixedMeta, 20, EncryptedDocumentInputStream::getInstance(file, consDir, L"FixedData").get());
    //         FixedMeta consFixed2Meta = new FixedMeta(new DocumentInputStream(((DocumentEntry) consDir.getEntry("Fixed2Meta"))), 9);
    //         FixedData consFixed2Data = new FixedData(consFixed2Meta, 48, getEncryptableInputStream(consDir, "Fixed2Data"));

    int count = consFixedMeta.getAdjustedItemCount();
    int lastConstraintID = -1;

    bool project15 = file->getMppFileType() == 14 && file->getApplicationName()== L"Microsoft.Project 15.0";
    int durationUnitsOffset = project15 ? 18 : 14;
    int durationOffset = project15 ? 14 : 16;

    for (int loop = 0; loop < count; loop++)
    {
		ByteArrayPtr metaData = consFixedMeta.getByteArray2Value(loop);

		//
		// SourceForge bug 2209477: we were reading an int here, but
		// it looks like the deleted flag is just a short.
		//
		if (MPPUtility::getShort(metaData.get(), 0) != 0)
			continue;

		int index = consFixedData.getIndexFromOffset(MPPUtility::getInt(metaData.get(), 4));
		if (index == -1)
			continue;

		//                  ByteArrayPtr metaData2 = consFixed2Meta.getByteArray2Value(loop);
		//                  int index2 = consFixed2Data.getIndexFromOffset(MPPUtility.getInt(metaData2, 4));
		//                  ByteArrayPtr data2 = consFixed2Data.getByteArray2Value(index2);

		ByteArrayPtr data = consFixedData.getByteArray2Value(index);
		int constraintID = MPPUtility::getInt(data.get(), 0);
		if (constraintID <= lastConstraintID)
			continue;

		lastConstraintID = constraintID;
		int taskID1 = MPPUtility::getInt(data.get(), 4);
		int taskID2 = MPPUtility::getInt(data.get(), 8);

		if (taskID1 != taskID2)
		{
			Task* task1 = file->getTaskByUniqueID(taskID1);
			Task* task2 = file->getTaskByUniqueID(taskID2);

			if (task1 && task2)
			{
				RelationType type = (RelationType)MPPUtility::getShort(data.get(), 12);
				TimeUnit durationUnits = MPPUtility::getDurationTimeUnits(MPPUtility::getShort(data.get(), durationUnitsOffset));
				DurationPtr lag = MPPUtility::getAdjustedDuration(file, MPPUtility::getInt(data.get(), durationOffset), durationUnits);
				RelationPtr relation = task2->addPredecessor(task1, type, lag);
				file->fireRelationReadEvent(relation);
			}
		}
    }
}