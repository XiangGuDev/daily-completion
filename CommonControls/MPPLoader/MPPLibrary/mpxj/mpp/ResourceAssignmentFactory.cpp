#include "stdafx.h"
#include "ResourceAssignmentFactory.h"
#include "..\AssignmentField.h"
#include "..\Resource.h"
#include "..\ResourceAssignment.h"
#include "MPPUtility.h"
#include "..\common\RtfHelper.h"
#include "..\common\DefaultTimephasedWorkContainer.h"
#include "..\common\TimephasedWorkNormaliser.h"
#include "..\common\SplitTaskFactory.h"
#include "MPPTimephasedWorkNormaliser.h"
#include "MPPTimephasedBaselineWorkNormaliser.h"
#include "MPPTimephasedBaselineCostNormaliser.h"
#include "TimephasedDataFactory.h"

#include "..\StaticResourceClear.h"
#include <omp.h>

class RAF_STATIC_DATA
{
public:
	RAF_STATIC_DATA();

	static RAF_STATIC_DATA* Instance();

	DurationPtr DEFAULT_NORMALIZER_WORK_PER_DAY;// = Duration.getInstance(480, TimeUnit.MINUTES);
	MppBitFlagPtr ASSIGNMENT_META_DATA_BIT_FLAGS[21];
	MppBitFlagPtr MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[21];

private:
	static std::auto_ptr<RAF_STATIC_DATA> _rsd_instance;
};
std::auto_ptr<RAF_STATIC_DATA> RAF_STATIC_DATA::_rsd_instance;

RAF_STATIC_DATA* RAF_STATIC_DATA::Instance()
{
	if (!_rsd_instance.get()) {
		_rsd_instance.reset(new RAF_STATIC_DATA());

		// 注册清理这个静态对象
		StaticResource2Clear::GetInstance()->Register(new AutoPtrClear<RAF_STATIC_DATA>(&_rsd_instance));
	}

	return _rsd_instance.get();
}

RAF_STATIC_DATA::RAF_STATIC_DATA()
{
	DEFAULT_NORMALIZER_WORK_PER_DAY = Duration2::getInstance(480, TimeUnit::MINUTES);

	// 初使化 ASSIGNMENT_META_DATA_BIT_FLAGS
	ASSIGNMENT_META_DATA_BIT_FLAGS[0].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG1),
		28, 0x0000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[1].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG2),
		28, 0x0000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[2].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG3),
		28, 0x0000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[3].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG4),
		28, 0x0000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[4].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG5),
		28, 0x0000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[5].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG6),
		28, 0x0001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[6].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG7),
		28, 0x0002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[7].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG8),
		28, 0x0004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[8].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG9),
		28, 0x0008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[9].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG10),
		28, 0x0010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[10].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG11),
		28, 0x0020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[11].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG12),
		28, 0x0040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[12].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG13),
		28, 0x0080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[13].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG14),
		28, 0x0100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[14].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG15),
		28, 0x0200000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[15].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG16),
		28, 0x0400000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[16].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG17),
		28, 0x0800000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[17].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG18),
		28, 0x1000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[18].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG19),
		28, 0x2000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	ASSIGNMENT_META_DATA_BIT_FLAGS[19].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG20),
		28, 0x4000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));

	// 初使化 MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[0].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG10),
		28, 0x000002, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[1].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG1),
		28, 0x000004, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[2].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG2),
		28, 0x000008, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[3].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG3),
		28, 0x000010, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[4].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG4),
		28, 0x000020, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[5].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG5),
		28, 0x000040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[6].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG6),
		28, 0x000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[7].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG7),
		28, 0x000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[8].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG8),
		28, 0x000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[9].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG9),
		28, 0x000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[10].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG11),
		28, 0x000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[11].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG12),
		28, 0x001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[12].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG13),
		28, 0x002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[13].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG14),
		28, 0x004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[14].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG15),
		28, 0x008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[15].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG16),
		28, 0x010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[16].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG17),
		28, 0x020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[17].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG18),
		28, 0x040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[18].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG19),
		28, 0x080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[19].reset(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG20),
		28, 0x100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
}



//const DurationPtr Resource2AssignmentFactory::DEFAULT_NORMALIZER_WORK_PER_DAY = Duration::getInstance(480, TimeUnit::MINUTES);

//const MppBitFlagPtr Resource2AssignmentFactory::ASSIGNMENT_META_DATA_BIT_FLAGS[] =
//{
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG1), 
//			28, 0x0000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG2), 
//			28, 0x0000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG3), 
//			28, 0x0000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG4), 
//			28, 0x0000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG5), 
//			28, 0x0000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG6), 
//			28, 0x0001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG7), 
//			28, 0x0002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG8), 
//			28, 0x0004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG9), 
//			28, 0x0008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG10), 
//			28, 0x0010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG11), 
//			28, 0x0020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG12), 
//			28, 0x0040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG13), 
//			28, 0x0080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG14), 
//			28, 0x0100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG15), 
//			28, 0x0200000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG16), 
//			28, 0x0400000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG17), 
//			28, 0x0800000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG18), 
//			28, 0x1000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG19), 
//			28, 0x2000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG20), 
//			28, 0x4000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//			MppBitFlagPtr()
//};

//const MppBitFlagPtr Resource2AssignmentFactory::MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS[] =
//{
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG10),
//			28, 0x000002, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG1), 
//			28, 0x000004, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG2), 
//			28, 0x000008, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG3), 
//			28, 0x000010, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG4),
//			28, 0x000020, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG5),
//			28, 0x000040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG6), 
//			28, 0x000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG7), 
//			28, 0x000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG8), 
//			28, 0x000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG9), 
//			28, 0x000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG11), 
//			28, 0x000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG12), 
//			28, 0x001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG13), 
//			28, 0x002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG14), 
//			28, 0x004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG15), 
//			28, 0x008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG16), 
//			28, 0x010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG17), 
//			28, 0x020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG18), 
//			28, 0x040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG19), 
//			28, 0x080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr(new MppBitFlag(AssignmentFieldClass::getInstance(AssignmentField::AF_FLAG20), 
//			28, 0x100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
//	MppBitFlagPtr()
//};

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
void Resource2AssignmentFactory::process(ProjectFile* file, FieldMapPtr fieldMap, FieldMapPtr enterpriseCustomFieldMap,
	bool useRawTimephasedData, bool preserveNoteFormatting, VarMetaPtr assnVarMeta, Var2DataPtr assnVarData,
	FixedMetaPtr assnFixedMeta, FixedDataPtr assnFixedData, FixedDataPtr assnFixedData2, int count)
{
    std::shared_ptr<Set<int>> set = assnVarMeta->getUniqueIdentifierSet();
	TimephasedDataFactory timephasedFactory;// = new TimephasedDataFactory();
	SplitTaskFactory splitFactory;// = new SplitTaskFactory();
    TimephasedWorkNormaliserPtr normaliser ( new MPPTimephasedWorkNormaliser());
    TimephasedWorkNormaliserPtr baselineWorkNormaliser ( new MPPTimephasedBaselineWorkNormaliser());
    TimephasedCostNormaliserPtr baselineCostNormaliser ( new MPPTimephasedBaselineCostNormaliser());
    ProjectCalendarPtr baselineCalendar = file->getBaselineCalendar();

    //System.out.println(assnFixedMeta);
    //System.out.println(assnFixedData);
    //System.out.println(assnVarMeta.toString(fieldMap));
    //System.out.println(assnVarData);

    const MppBitFlagPtr* metaDataBitFlags = NULL;
    if (file->getMppFileType() == 14)
    {
        metaDataBitFlags = RAF_STATIC_DATA::Instance()->MPP14_ASSIGNMENT_META_DATA_BIT_FLAGS;
    }
    else
    {
        metaDataBitFlags = RAF_STATIC_DATA::Instance()->ASSIGNMENT_META_DATA_BIT_FLAGS;
    }

	omp_lock_t lock;
	int nProcNum = omp_get_num_procs();
	omp_init_lock(&lock);
#pragma omp parallel for num_threads(2*nProcNum-1)
//#pragma omp parallel for num_threads(4)
    for (int loop = 0; loop < count; loop++)
    {
		omp_set_lock(&lock);
        ByteArrayPtr meta = assnFixedMeta->getByteArray2Value(loop);
		omp_unset_lock(&lock);
        if ((*meta)[0] != 0)
        {
			continue;
        }

        int offset = MPPUtility::getInt(meta.get(), 4);
        ByteArrayPtr data = assnFixedData->getByteArray2Value(assnFixedData->getIndexFromOffset(offset));
        if (!data)
        {
			continue;
        }

        if ((int)data->length() <= fieldMap->getMaxFixedDataOffset(0))
        {
			ByteArrayPtr newData (new ByteArray2(fieldMap->getMaxFixedDataOffset(0) + 8));
			//System.arraycopy(data, 0, newData, 0, data->length());
			memcpy(newData->get(), data->get(), data->length());
			data = newData;
        }

		int id = MPPUtility::getInt(data.get(), fieldMap->getFixedDataOffset(
						AssignmentFieldClass::getInstance(AssignmentField::AF_UNIQUE_ID))->intValue());
        int varDataId = id;
        if (set->contains(varDataId) == false)
        {
			continue;
        }

        ByteArrayPtr data2;
        if (assnFixedData2)
        {
			data2 = assnFixedData2->getByteArray2Value(loop);
        }

        Resource2AssignmentPtr assignment (new Resource2Assignment(file));

        assignment->disableEvents();

		ByteArrayPtr temparr[2] = { data, data2 };
		fieldMap->populateContainer(L"AssignmentField", assignment.get(), varDataId, temparr, 2, assnVarData);

        if (enterpriseCustomFieldMap)
        {
			enterpriseCustomFieldMap->populateContainer(L"AssignmentField", assignment.get(), varDataId, NULL, 0, assnVarData);
        }

        assignment->enableEvents();

		for (int i = 0; i < 100; i ++)
        {
			MppBitFlagPtr flag = metaDataBitFlags[i];
			if (!flag) break;
			flag->setValue(assignment.get(), meta);
        }

        assignment->setConfirmed(((*meta)[8] & 0x80) != 0);
		assignment->setResponsePending(((*meta)[9] & 0x01) != 0);
		assignment->setTeamStatusPending(((*meta)[10] & 0x02) != 0);

        processHyperlinkData(assignment, assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(
					AssignmentFieldClass::getInstance(AssignmentField::AF_HYPERLINK_DATA))->intValue()));

        // Post processing
        //
        if (file->getMppFileType() == 9 && !assignment->getCreateDate())
        {
			ByteArrayPtr creationData = assnVarData->getByteArray2(varDataId, MPP9_CREATION_DATA);
			if (creationData && creationData->length() >= 28)
			{
				assignment->setCreateDate(MPPUtility::getTimestamp(creationData.get(), 24));
			}
        }

        CString notes = assignment->getNotes();
        if (notes.GetLength())
        {
			if (!preserveNoteFormatting)
			{
				notes = RtfHelper::strip(notes);
			}

			assignment->setNotes(notes);
        }

		omp_set_lock(&lock);
        Task* task = file->getTaskByUniqueID(assignment->getTaskUniqueID());

		if (!task) continue;

		task->addResource2Assignment(assignment);

		Resource2Ptr resource = file->getResource2ByUniqueID(assignment->getResource2UniqueID()->intValue());

		ProjectCalendarPtr calendar;
		if (resource)
		{
			calendar = resource->getResource2Calendar();
		}

		if (!calendar || task->getIgnoreResource2Calendar())
		{
			calendar = task->getCalendar();
		}

		if (!calendar)
		{
			calendar = file->getCalendar();
		}
		omp_unset_lock(&lock);

		assignment->setTimephasedBaselineWork(0, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(1, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE1_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(2, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE2_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(3, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE3_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(4, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE4_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(5, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE5_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(6, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE6_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(7, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE7_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(8, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE8_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(9, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE9_WORK))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineWork(10, timephasedFactory.getBaselineWork(assignment, baselineCalendar, baselineWorkNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE10_WORK))->intValue()), !useRawTimephasedData));

		assignment->setTimephasedBaselineCost(0, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(1, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE1_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(2, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE2_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(3, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE3_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(4, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE4_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(5, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE5_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(6, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE6_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(7, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE7_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(8, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE8_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(9, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE9_COST))->intValue()), !useRawTimephasedData));
		assignment->setTimephasedBaselineCost(10, timephasedFactory.getBaselineCost(baselineCalendar, baselineCostNormaliser, 
			assnVarData->getByteArray2(varDataId, fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(
			AssignmentField::AF_TIMEPHASED_BASELINE10_COST))->intValue()), !useRawTimephasedData));
		
		ByteArrayPtr timephasedActualWorkData = assnVarData->getByteArray2(varDataId,
			fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(AssignmentField::AF_TIMEPHASED_ACTUAL_WORK))->intValue());
		ByteArrayPtr timephasedWorkData = assnVarData->getByteArray2(varDataId,
			fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(AssignmentField::AF_TIMEPHASED_WORK))->intValue());
		ByteArrayPtr timephasedActualOvertimeWorkData = assnVarData->getByteArray2(varDataId, 
			fieldMap->getVarDataKey(AssignmentFieldClass::getInstance(AssignmentField::AF_TIMEPHASED_ACTUAL_OVERTIME_WORK))->intValue());

		TimephasedWorkListPtr timephasedActualWork = timephasedFactory.getCompleteWork(calendar, 
			assignment->getStart(), timephasedActualWorkData);
		TimephasedWorkListPtr timephasedWork = timephasedFactory.getPlannedWork(calendar, assignment->getStart(), 
			assignment->getUnits()->doubleValue(), timephasedWorkData, timephasedActualWork);
		//continue;
		TimephasedWorkListPtr timephasedActualOvertimeWork = timephasedFactory.getCompleteWork(calendar, 
			assignment->getStart(), timephasedActualOvertimeWorkData);

		assignment->setActualStart(timephasedActualWork->isEmpty() ? NULL : assignment->getStart());
		assignment->setActualFinish((assignment->getRemainingWork()->getDuration() == 0 && resource) ? assignment->getFinish() : NULL);

		if (task->getSplits() && task->getSplits()->isEmpty())
		{
			splitFactory.processSplitData(task, timephasedActualWork, timephasedWork);
		}

		createTimephasedData(file, assignment, timephasedWork, timephasedActualWork);

		assignment->setTimephasedWork(DefaultTimephasedWorkContainerPtr (
			new DefaultTimephasedWorkContainer(calendar, normaliser, timephasedWork, !useRawTimephasedData)));
		assignment->setTimephasedActualWork(DefaultTimephasedWorkContainerPtr(
			new DefaultTimephasedWorkContainer(calendar, normaliser, timephasedActualWork, !useRawTimephasedData)));
		assignment->setTimephasedActualOvertimeWork(DefaultTimephasedWorkContainerPtr (
			new DefaultTimephasedWorkContainer(calendar, normaliser, timephasedActualOvertimeWork, !useRawTimephasedData)));
		
		if (timephasedWorkData)
		{
			if (timephasedFactory.getWorkModified(timephasedWork))
			{
				assignment->setWorkContour(WorkContour::CONTOURED);
			}
			else
			{
				if (timephasedWorkData->length() >= 30)
				{
					assignment->setWorkContour((WorkContour)(MPPUtility::getShort(timephasedWorkData.get(), 28)));
				}
				else
				{
					assignment->setWorkContour(WorkContour::FLAT);
				}
			}
		}

		omp_set_lock(&lock);
		file->fireAssignmentReadEvent(assignment);
		omp_unset_lock(&lock);
    }
	omp_destroy_lock(&lock);
}

/**
* Extract assignment hyperlink data. 
* 
* @param assignment assignment instance
* @param data hyperlink data
*/
void Resource2AssignmentFactory::processHyperlinkData(Resource2AssignmentPtr assignment, ByteArrayPtr data)
{
	if (!data) return;

    int offset = 12;

    offset += 12;
    CString hyperlink = MPPUtility::getUnicodeString(data.get(), offset);
    offset += ((hyperlink.GetLength() + 1) * 2);

    offset += 12;
	CString address = MPPUtility::getUnicodeString(data.get(), offset);
	offset += ((address.GetLength() + 1) * 2);

    offset += 12;
	CString subaddress = MPPUtility::getUnicodeString(data.get(), offset);
	offset += ((subaddress.GetLength() + 1) * 2);

    offset += 12;
	CString screentip = MPPUtility::getUnicodeString(data.get(), offset);

    assignment->setHyperlink(hyperlink);
    assignment->setHyperlinkAddress(address);
    assignment->setHyperlinkSubAddress(subaddress);
    assignment->setHyperlinkScreenTip(screentip);
}

/**
* Method used to create missing timephased data.
* 
* @param file project file
* @param assignment resource assignment
* @param timephasedPlanned planned timephased data
* @param timephasedComplete complete timephased data
*/
void Resource2AssignmentFactory::createTimephasedData(ProjectFile* file, Resource2AssignmentPtr assignment,
	TimephasedWorkListPtr timephasedPlanned, TimephasedWorkListPtr timephasedComplete)
{
    if (timephasedPlanned->isEmpty() && timephasedComplete->isEmpty())
    {
        DurationPtr totalMinutes = assignment->getWork()->convertUnits(TimeUnit::MINUTES, file->getProjectHeader());

        DurationPtr workPerDay;

        if (!assignment->getResource2() || assignment->getResource2()->getType() == Resource2Type::RT_WORK)
        {
			workPerDay = totalMinutes->getDuration() == 0 ? totalMinutes : RAF_STATIC_DATA::Instance()->DEFAULT_NORMALIZER_WORK_PER_DAY;
			int units = NumberHelper::getInt(assignment->getUnits());
			if (units != 100)
			{
				workPerDay = Duration2::getInstance((workPerDay->getDuration() * units) / 100.0, workPerDay->getUnits());
			}
        }
        else if (assignment->getVariableRateUnits() == NULL)
		{
			DurationPtr workingDays = assignment->getCalendar()->getWork(assignment->getStart(), assignment->getFinish(), TimeUnit::DAYS);
			double units = NumberHelper::getDouble(assignment->getUnits());
			double unitsPerDayAsMinutes = (units * 60) / (workingDays->getDuration() * 100);
			workPerDay = Duration2::getInstance(unitsPerDayAsMinutes, TimeUnit::MINUTES);
		}
		else
		{
			double unitsPerHour = NumberHelper::getDouble(assignment->getUnits());
			workPerDay = RAF_STATIC_DATA::Instance()->DEFAULT_NORMALIZER_WORK_PER_DAY;
			DurationPtr hoursPerDay = workPerDay->convertUnits(TimeUnit::HOURS, file->getProjectHeader());
			double unitsPerDayAsHours = (unitsPerHour * hoursPerDay->getDuration()) / 100;
			double unitsPerDayAsMinutes = unitsPerDayAsHours * 60;
			workPerDay = Duration2::getInstance(unitsPerDayAsMinutes, TimeUnit::MINUTES);
		}

        DurationPtr overtimeWork = assignment->getOvertimeWork();
        if (overtimeWork && overtimeWork->getDuration() != 0)
        {
			DurationPtr totalOvertimeMinutes = overtimeWork->convertUnits(TimeUnit::MINUTES, file->getProjectHeader());
			totalMinutes = Duration2::getInstance(totalMinutes->getDuration() - totalOvertimeMinutes->getDuration(), TimeUnit::MINUTES);
        }

        TimephasedWorkPtr tra (new TimephasedWork());
        tra->setStart(assignment->getStart());
		tra->setAmountPerDay(workPerDay);
		tra->setModified(false);
		tra->setFinish(assignment->getFinish());
		tra->setTotalAmount(totalMinutes);
        timephasedPlanned->Add(tra);
    }
}

