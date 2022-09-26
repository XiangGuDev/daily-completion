#include "stdafx.h"
#include "MPP14Reader.h"
#include "MPPReader.h"
#include "MPPUtility.h"
#include "EncryptedDocumentInputStream.h"
#include "VarMeta12.h"
#include "ProjectHeaderReader.h"
#include "GraphicalIndicatorReader.h"
#include "FieldMap14.h"
#include "RecurringTaskReader.h"

#include "ConstraintFactory.h"
#include "CostRateTableFactory.h"
#include "AvailabilityFactory.h"
#include "ResourceAssignmentFactory.h"

#include "..\common\MPPTaskField14.h"
#include "..\common\MPPResourceField14.h"
#include "..\common\DateHelper.h"
#include "..\common\NumberHelper.h"
#include "..\common\RtfHelper.h"

#include<fstream>
#include <vector>

/**
* Default working week.
*/
const bool MPP14Reader::DEFAULT_WORKING_WEEK[] =
{
	false,
	true,
	true,
	true,
	true,
	true,
	false
};

MppBitFlagArrayPtr MPP14Reader::PROJECT2010_TASK_META_DATA_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2013_TASK_META_DATA_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2010_TASK_META_DATA2_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2013_TASK_META_DATA2_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS;
MppBitFlagArrayPtr MPP14Reader::PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS;

MPP14Reader::MPP14Reader()
{
	INIT_STATIC_VAR_META_DATA_BIT_FLAGS();
}

void MPP14Reader::INIT_STATIC_VAR_META_DATA_BIT_FLAGS()
{
	if (PROJECT2010_TASK_META_DATA_BIT_FLAGS)
		return;

	// PROJECT2010_TASK_META_DATA_BIT_FLAGS
	{
		PROJECT2010_TASK_META_DATA_BIT_FLAGS.reset(new MppBitFlagArray(28));

		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[0].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG1), 35, 0x0000040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[1].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG2), 35, 0x0000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[2].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG3), 35, 0x0000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[3].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG4), 35, 0x0000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[4].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG5), 35, 0x0000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[5].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG6), 35, 0x0000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[6].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG7), 35, 0x0001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[7].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG8), 35, 0x0002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[8].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG9), 35, 0x0004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[9].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG10), 35, 0x0008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[10].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG11), 35, 0x0010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[11].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG12), 35, 0x0020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[12].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG13), 35, 0x0040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[13].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG14), 35, 0x0080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[14].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG15), 35, 0x0100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[15].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG16), 35, 0x0200000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[16].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG17), 35, 0x0400000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[17].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG18), 35, 0x0800000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[18].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG19), 35, 0x1000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[19].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG20), 35, 0x2000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));

		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[20].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_MILESTONE), 8, 0x20, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[21].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_MARKED), 9, 0x40, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[22].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_IGNORE_RESOURCE_CALENDAR), 10, 0x02, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[23].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_ROLLUP), 10, 0x08, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[24].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_HIDE_BAR), 10, 0x80, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[25].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_EFFORT_DRIVEN), 11, 0x10, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[26].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_LEVEL_ASSIGNMENTS), 13, 0x04, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA_BIT_FLAGS)[27].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_LEVELING_CAN_SPLIT), 13, 0x02, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	};

	// PROJECT2013_TASK_META_DATA_BIT_FLAGS
	{
		PROJECT2013_TASK_META_DATA_BIT_FLAGS.reset(new MppBitFlagArray(28));

		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[0].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG1), 24, 0x0002, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[1].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG2), 24, 0x0004, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[2].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG3), 24, 0x0008, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[3].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG4), 24, 0x0010, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[4].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG5), 24, 0x0020, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[5].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG6), 24, 0x0040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[6].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG7), 24, 0x0080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[7].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG8), 24, 0x0100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[8].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG9), 24, 0x0200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[9].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG10), 24, 0x0400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));

		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[10].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG11), 33, 0x002, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[11].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG12), 33, 0x004, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[12].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG13), 33, 0x008, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[13].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG14), 33, 0x010, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[14].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG15), 33, 0x020, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[15].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG16), 33, 0x040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[16].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG17), 33, 0x080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[17].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG18), 33, 0x100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[18].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG19), 33, 0x200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[19].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_FLAG20), 33, 0x400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));

		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[20].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_MILESTONE), 10, 0x02, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[21].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_MARKED), 12, 0x02, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[22].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_ROLLUP), 12, 0x04, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[23].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_HIDE_BAR), 12, 0x80, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[24].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_EFFORT_DRIVEN), 13, 0x08, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[25].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_LEVEL_ASSIGNMENTS), 16, 0x04, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[26].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_LEVELING_CAN_SPLIT), 16, 0x02, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA_BIT_FLAGS)[27].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_IGNORE_RESOURCE_CALENDAR), 17, 0x20, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	};

	// PROJECT2010_TASK_META_DATA2_BIT_FLAGS
	{
		PROJECT2010_TASK_META_DATA2_BIT_FLAGS.reset(new MppBitFlagArray(2));

		(*PROJECT2010_TASK_META_DATA2_BIT_FLAGS)[0].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_ACTIVE), 8, 0x04, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_TASK_META_DATA2_BIT_FLAGS)[1].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_TASK_MODE), 8, 0x08, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	};

	//const MppBitFlag PROJECT2013_TASK_META_DATA2_BIT_FLAGS[] =
	{
		PROJECT2013_TASK_META_DATA2_BIT_FLAGS.reset(new MppBitFlagArray(2));

		(*PROJECT2013_TASK_META_DATA2_BIT_FLAGS)[0].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_ACTIVE), 8, 0x40, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_TASK_META_DATA2_BIT_FLAGS)[1].reset(new MppBitFlag(
			TaskFieldClass::getInstance(TaskField::TF_TASK_MODE), 8, 0x80, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	};

	//const MppBitFlag PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS[] =
	{
		PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS.reset(new MppBitFlagArray(2));

		(*PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS)[0].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_BUDGET), 8, 0x20, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS)[1].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_TYPE), 8, 0x10, 
			IntegerPtr(new Integer((int)Resource2Type::RT_MATERIAL)), IntegerPtr(new Integer((int)Resource2Type::RT_COST))));
	};

	//const MppBitFlag PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS[] =
	{
		PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS.reset(new MppBitFlagArray(2));

		(*PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS)[0].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_BUDGET), 8, 0x20, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS)[1].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_TYPE), 4, 0x08, 
			IntegerPtr(new Integer((int)Resource2Type::RT_COST)), IntegerPtr(new Integer((int)Resource2Type::RT_MATERIAL))));
	};

	//const MppBitFlag PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS[] =
	{
		PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS.reset(new MppBitFlagArray(20));

		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[0].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG10), 28, 0x0000020, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[1].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG1), 28, 0x0000040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[2].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG2), 28, 0x0000080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[3].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG3), 28, 0x0000100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[4].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG4), 28, 0x0000200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[5].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG5), 28, 0x0000400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[6].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG6), 28, 0x0000800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[7].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG7), 28, 0x0001000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[8].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG8), 28, 0x0002000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[9].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG9), 28, 0x0004000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[10].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG11), 28, 0x0008000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[11].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG12), 28, 0x0010000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[12].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG13), 28, 0x0020000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[13].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG14), 28, 0x0040000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[14].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG15), 28, 0x0080000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[15].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG16), 28, 0x0100000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[16].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG17), 28, 0x0200000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[17].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG18), 28, 0x0400000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[18].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG19), 28, 0x0800000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS)[19].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG20), 28, 0x1000000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	};

	//const MppBitFlag PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS[] =
	{
		PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS.reset(new MppBitFlagArray(20));

		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[0].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG10), 19, 0x0040, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[1].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG1), 19, 0x0080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[2].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG2), 19, 0x0100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[3].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG3), 19, 0x0200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[4].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG4), 19, 0x0400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[5].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG5), 19, 0x0800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[6].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG6), 19, 0x1000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[7].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG7), 19, 0x2000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[8].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG8), 19, 0x4000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[9].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG9), 19, 0x8000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));

		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[10].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG11), 24, 0x00080, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[11].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG12), 24, 0x00100, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[12].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG13), 24, 0x00200, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[13].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG14), 24, 0x00400, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[14].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG15), 24, 0x00800, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[15].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG16), 24, 0x01000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[16].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG17), 24, 0x02000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[17].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG18), 24, 0x04000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[18].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG19), 24, 0x08000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
		(*PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS)[19].reset(new MppBitFlag(
			Resource2FieldClass::getInstance(Resource2Field::RF_FLAG20), 24, 0x10000, BooleanPtr(new Boolean(false)), BooleanPtr(new Boolean(true))));
	}
}

/**
* This method is used to process an MPP14 file. This is the file format
* used by Project 14.
*
* @param reader parent file reader
* @param file parent MPP file
* @param root Root of the POI file system.
*/
void MPP14Reader::process(MPPReader* reader, ProjectFile* file, DirectoryEntryPtr root)
{
	try
    {
        populateMemberData(reader, file, root);
        processProjectHeader();

        if (!reader->getReadHeaderOnly())
        {
			processSubProjectData();
			//processGraphicalIndicators();
			processCustomValueLists();
			processCalendarData();
			processResource2Data();
			processTaskData();
			processConstraintData();
			processAssignmentData();
			postProcessTasks();
			//m_file->renumberTaskIDs();
			//m_file->debugPrintAllTasks();
			//if (reader->getReadPresentationData())
			//{
			//	processViewPropertyData();
			//	processTableData();
			//	processViewData();
			//	processFilterData();
			//	processGroup2Data();
			//	processSavedViewState();
			//}
        }
    }
	catch (toolkit::Exception& e)
	{
		clearMemberData();
		throw e;
	}

    clearMemberData();
}

// 获取不同版本参数
const MPPVersionParams* MPP14Reader::getVersionParams()
{
	static MPPVersionParams params = { 0 };
	if (params.FileVersion == 0) {
		params.FileVersion = Project2010;
		params.PropsEntry = L"Props14";
		params.ProjectDirEntry = L"   114";
	}

	return &params;
}

VarMetaPtr MPP14Reader::newVarMetaObj(BwProj::toolkit::IDataReadStream* is)
{
	return VarMetaPtr(new VarMeta12(is));
}
/**
* Populate member data used by the rest of the reader.
* 
* @param reader parent file reader
* @param file parent MPP file
* @param root Root of the POI file system.
*/
void MPP14Reader::populateMemberData(MPPReader* reader, ProjectFile* file, DirectoryEntryPtr root)
{	
    m_reader = reader;
    m_file = file;
    m_root = root;

    //
    // Retrieve the high level document properties
    //
	
	DocumentInputStreamPtr is(new DocumentInputStream(
		std::dynamic_pointer_cast<DocumentEntry> (root->getEntry(getVersionParams()->PropsEntry))));
	
	Props14 props14(is);	
    //System.out.println(props14);

    file->setProjectFilePath(props14.getUnicodeString(Props::PROJECT_FILE_PATH));
	file->setEncoded(props14.getByte(Props::PASSWORD_FLAG) != 0);
	file->setEncryptionCode(props14.getByte(Props::ENCRYPTION_CODE));

    //
    // Test for password protection. In the single BYTE retrieved here:
    //
    // 0x00 = no password
    // 0x01 = protection password has been supplied
    // 0x02 = write reservation password has been supplied
    // 0x03 = both passwords have been supplied
    //  
	if ((props14.getByte(Props::PASSWORD_FLAG) & 0x01) != 0)
    {
        // Couldn't figure out how to get the password for MPP14 files so for now we just need to block the reading
        throw toolkit::Exception(L"PASSWORD_PROTECTED");
    }

    //m_resourceMap = new HashMap<Integer, ProjectCalendar>();
    m_projectDir = std::dynamic_pointer_cast<DirectoryEntry> (root->getEntry(getVersionParams()->ProjectDirEntry));
	//m_viewDir = std::dynamic_pointer_cast<DirectoryEntry> (root->getEntry(L"   214"));

	DirectoryEntryPtr outlineCodeDir = std::dynamic_pointer_cast<DirectoryEntry>(m_projectDir->getEntry(L"TBkndOutlCode"));

	DocumentInputStream stream1(std::dynamic_pointer_cast<DocumentEntry>(outlineCodeDir->getEntry(L"VarMeta")));
	m_outlineCodeVarMeta =newVarMetaObj(&stream1);

	DocumentInputStream stream2(std::dynamic_pointer_cast<DocumentEntry>(outlineCodeDir->getEntry(L"Var2Data")));
	m_outlineCodeVarData.reset(new Var2Data(m_outlineCodeVarMeta, &stream2));

	DocumentInputStream stream3(std::dynamic_pointer_cast<DocumentEntry>(outlineCodeDir->getEntry(L"FixedMeta")));
	m_outlineCodeFixedMeta.reset(new FixedMeta(&stream3, 10));

	DocumentInputStream stream4(std::dynamic_pointer_cast<DocumentEntry>(outlineCodeDir->getEntry(L"FixedData")));
	m_outlineCodeFixedData.reset(new FixedData(m_outlineCodeFixedMeta.get(), &stream4));

    //m_outlineCodeFixedMeta2 = new FixedMeta(new DocumentInputStream(((DocumentEntry) outlineCodeDir->getEntry(L"Fixed2Meta"))), 10);
    //m_outlineCodeFixedData2 = new FixedData(m_outlineCodeFixedMeta2, new DocumentInputStream(((DocumentEntry) outlineCodeDir->getEntry(L"Fixed2Data"))));
    m_projectProps.reset (new Props14(EncryptedDocumentInputStream::getInstance(m_file, m_projectDir, L"Props")));
    //MPPUtility::fileDump("c:\\temp\\props.txt", m_projectProps->toString().getBytes());
    //FieldMap fm = new FieldMap14(m_file);
    //fm.dumpKnownFieldMaps(m_projectProps);

    //m_fontBases = new HashMap<Integer, FontBase>();
    //m_taskSubProjects = new HashMap<Integer, SubProject>();
    //m_parentTasks = new HashMap<Integer, Integer>();
    //m_taskOrder = new TreeMap<Long, Integer>();
    //m_nullTaskOrder = new TreeMap<Integer, Integer>();

    m_file->setMppFileType(getVersionParams()->FileVersion);
    m_file->setAutoFilter(props14.getBoolean(Props::AUTO_FILTER));
	
}

/**
* Clear transient member data.
*/
void MPP14Reader::clearMemberData()
{
    m_reader = NULL;
    //m_file = null;
    //m_root = null;
    //m_resourceMap = null;
    //m_projectDir = null;
    //m_viewDir = null;
    //m_outlineCodeVarData = null;
    //m_outlineCodeVarMeta = null;
    //m_projectProps = null;
    //m_fontBases = null;
    //m_taskSubProjects = null;
    //m_parentTasks = null;
    //m_taskOrder = null;
    //m_nullTaskOrder = null;
}

/**
* This method extracts and collates the value list information 
* for custom column value lists.
*/
void MPP14Reader::processCustomValueLists()
{
    IntArray2Ptr uniqueid = m_outlineCodeVarMeta->getUniqueIdentifierArray();
    int parentOffset = m_file->getApplicationName() == L"Microsoft.Project 15.0" ? 10 : 8;

    for (size_t loop = 0; loop < uniqueid->length(); loop++)
    {
        int id = (*uniqueid)[loop];

        CustomFieldValueItemPtr item (new CustomFieldValueItem(id));
        item->setValue(m_outlineCodeVarData->getByteArray2(id, VALUE_LIST_VALUE));
		item->setDescription(m_outlineCodeVarData->getUnicodeString(id, VALUE_LIST_DESCRIPTION));
		item->setUnknown(m_outlineCodeVarData->getByteArray2(id, VALUE_LIST_UNKNOWN));

        ByteArrayPtr b = m_outlineCodeFixedData->getByteArray2Value(loop + 3);
        if (b)
        {
			item->setParent(MPPUtility::getShort(b.get(), parentOffset));
        }

        //BYTE b2[] = m_outlineCodeFixedData2.getByteArray2Value(loop+3); // contains FieldGUID in first 16 bytes

        m_file->addCustomFieldValueItem(item);
    }
}

/**
* Process the project header data.
*/
void MPP14Reader::processProjectHeader()
{
	ProjectHeaderReader projectHeaderReader;// = new ProjectHeaderReader();
    projectHeaderReader.process(m_file, m_projectProps, m_root);
}

///**
//* Process the graphical indicator data.
//*/
//void MPP14Reader::processGraphicalIndicators()
//{
//	GraphicalIndicatorReader graphicalIndicatorReader;// = new GraphicalIndicatorReader();
//    graphicalIndicatorReader.process(m_file.get(), m_projectProps);
//}

/**
* Read sub project data from the file, and add it to a hash map
* indexed by task ID.
* 
* Project stores all subprojects that have ever been inserted into this project
* in sequence and that is what used to count unique id offsets for each of the
* subprojects.
*/
void MPP14Reader::processSubProjectData()
{
    ByteArrayPtr subProjData = m_projectProps->getByteArray2(Props::SUBPROJECT_DATA);

    //System.out.println (MPPUtility::hexdump(subProjData, true, 16, ""));
    //MPPUtility::fileHexDump("c:\\temp\\dump.txt", subProjData);

    if (subProjData)
    {
        int index = 0;
        int offset = 0;
        int itemHeaderOffset;
        int uniqueIDOffset;
        int filePathOffset;
        int fileNameOffset;

        ByteArrayPtr itemHeader(new ByteArray2(20));

        /*int blockSize = MPPUtility::getInt(subProjData.get(), offset);*/
        offset += 4;

        /*int unknown = MPPUtility::getInt(subProjData.get(), offset);*/
        offset += 4;

        int itemCountOffset = MPPUtility::getInt(subProjData.get(), offset);
        offset += 4;

        while (offset < itemCountOffset)
        {
        index++;
        itemHeaderOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
        offset += 4;

        MPPUtility::getByteArray2(subProjData.get(), itemHeaderOffset, itemHeader->length(), itemHeader.get(), 0);
        BYTE subProjectType = (*itemHeader)[16];

        //            System.out.println();
        //            System.out.println (MPPUtility::hexdump(itemHeader, false, 16, ""));
        //            System.out.println(MPPUtility::hexdump(subProjData, offset, 16, false));
        //            System.out.println("Offset1: " + (MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF));
        //            System.out.println("Offset2: " + (MPPUtility::getInt(subProjData.get(), offset+4) & 0x1FFFF));
        //            System.out.println("Offset3: " + (MPPUtility::getInt(subProjData.get(), offset+8) & 0x1FFFF));
        //            System.out.println("Offset4: " + (MPPUtility::getInt(subProjData.get(), offset+12) & 0x1FFFF));
        //            System.out.println ("Offset: " + offset);
        //            System.out.println ("Item Header Offset: " + itemHeaderOffset);
        //            System.out.println("SubProjectType: " + Integer.toHexString(subProjectType));
        switch (subProjectType)
        {
        //
        // Subproject that is no longer inserted. This is a placeholder in order to be
        // able to always guarantee unique unique ids.
        //
            case 0x00:
                //   
                // deleted entry?
                //
            case 0x10:
            {
                offset += 8;
                break;
            }

            //
            // task unique ID, 8 bytes, path, file name
            //
            case (BYTE) 0x99:
            case 0x09:
            case 0x0D:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                // sometimes offset of a task ID?
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // task unique ID, 8 bytes, path, file name
            //
            case 0x03:
            case 0x11:
            case (BYTE) 0x91:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                // Unknown offset
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // task unique ID, path, unknown, file name
            //
            case (BYTE) 0x81:
            case (BYTE) 0x83:
            case 0x41:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                // unknown offset to 2 bytes of data?
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // task unique ID, path, file name
            //
            case 0x01:
            case 0x08:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // task unique ID, path, file name
            //
            case (BYTE) 0xC0:
            {
                uniqueIDOffset = itemHeaderOffset;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                // unknown offset
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // resource, task unique ID, path, file name
            //
            case 0x05:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                m_file->setResource2SubProject(readSubProject(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index));
                break;
            }

            case 0x45:
            {
                uniqueIDOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                offset += 4;

                m_file->setResource2SubProject(readSubProject(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index));
                break;
            }

            //
            // path, file name
            //
            case 0x02:
            {
                //filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                //fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                //sp = readSubProject(subProjData, -1, filePathOffset, fileNameOffset, index);
                // 0x02 looks to be the link FROM the resource pool to a project that is using it.
                break;
            }

            case 0x04:
            {
                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                m_file->setResource2SubProject(readSubProject(subProjData, -1, filePathOffset, fileNameOffset, index));
                break;
            }

            //
            // task unique ID, 4 bytes, path, 4 bytes, file name
            //
            case (BYTE) 0x8D:
            {
                uniqueIDOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 8;

                filePathOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 8;

                fileNameOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // task unique ID, path, file name
            //
            case 0x0A:
            {
                uniqueIDOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 4;

                filePathOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 4;

                fileNameOffset = MPPUtility::getShort(subProjData.get(), offset);
                offset += 4;

                readSubProjects(subProjData, uniqueIDOffset, filePathOffset, fileNameOffset, index);
                break;
            }

            // new resource pool entry
            case (BYTE) 0x44:
            {
                filePathOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                offset += 4;

                fileNameOffset = MPPUtility::getInt(subProjData.get(), offset) & 0x1FFFF;
                offset += 4;

                readSubProjects(subProjData, -1, filePathOffset, fileNameOffset, index);
                break;
            }

            //
            // Appears when a subproject is collapsed
            //
            case (BYTE) 0x80:
            {
                offset += 12;
                break;
            }

            //
            // Any other value, assume 12 bytes to handle old/deleted data?
            //
            default:
            {
                offset += 12;
                break;
            }
        }
        }
    }
}

/**
* Read a list of sub projects.
* 
* @param data BYTE array
* @param uniqueIDOffset offset of unique ID
* @param filePathOffset offset of file path
* @param fileNameOffset offset of file name
* @param subprojectIndex index of the subproject, used to calculate unique id offset
*/
void MPP14Reader::readSubProjects(ByteArrayPtr data, int uniqueIDOffset, int filePathOffset, int fileNameOffset, int subprojectIndex)
{
    while (uniqueIDOffset < filePathOffset)
    {
        readSubProject(data, uniqueIDOffset, filePathOffset, fileNameOffset, subprojectIndex++);
        uniqueIDOffset += 4;
    }
}

/**
* Method used to read the sub project details from a BYTE array.
*
* @param data BYTE array
* @param uniqueIDOffset offset of unique ID
* @param filePathOffset offset of file path
* @param fileNameOffset offset of file name
* @param subprojectIndex index of the subproject, used to calculate unique id offset
* @return new SubProject instance
*/
SubProjectPtr MPP14Reader::readSubProject(ByteArrayPtr data, int uniqueIDOffset, int filePathOffset, int fileNameOffset, int subprojectIndex)
{
    try
    {
        SubProjectPtr sp (new SubProject());
        int type = SUBPROJECT_TASKUNIQUEID0;

        if (uniqueIDOffset != -1)
        {
			int value = MPPUtility::getInt(data.get(), uniqueIDOffset);
			type = MPPUtility::getInt(data.get(), uniqueIDOffset + 4);
			switch (type)
			{
				case SUBPROJECT_TASKUNIQUEID0:
				case SUBPROJECT_TASKUNIQUEID1:
				case SUBPROJECT_TASKUNIQUEID2:
				case SUBPROJECT_TASKUNIQUEID3:
				case SUBPROJECT_TASKUNIQUEID4:
				case SUBPROJECT_TASKUNIQUEID5:
				case SUBPROJECT_TASKUNIQUEID6:
				case SUBPROJECT_TASKUNIQUEID7:
				{
					sp->setTaskUniqueID(value);
					m_taskSubProjects.put(sp->getTaskUniqueID(), sp);
					break;
				}

				default:
				{
					if (value != 0)
					{
						sp->addExternalTaskUniqueID(value);
						m_taskSubProjects.put(value, sp);
					}
					break;
				}
			}

			// Now get the unique id offset for this subproject
			value = 0x00800000 + ((subprojectIndex - 1) * 0x00400000);
			sp->setUniqueIDOffset(value);
        }

        if (type == SUBPROJECT_TASKUNIQUEID4)
        {
			sp->setFullPath(MPPUtility::getUnicodeString(data.get(), filePathOffset));
        }
        else
        {
			//
			// First block header
			//
			filePathOffset += 18;

			//
			// CString size as a 4 BYTE int
			//
			filePathOffset += 4;

			//
			// Full DOS path
			//
			sp->setDosFullPath(MPPUtility::getString(data.get(), filePathOffset));
			filePathOffset += (sp->getDosFullPath().GetLength() + 1);

			//
			// 24 BYTE block
			//
			filePathOffset += 24;

			//
			// 4 BYTE block size
			//
			int size = MPPUtility::getInt(data.get(), filePathOffset);
			filePathOffset += 4;
			if (size == 0)
			{
				sp->setFullPath(sp->getDosFullPath());
			}
			else
			{
				//
				// 4 BYTE unicode string size in bytes
				//
				size = MPPUtility::getInt(data.get(), filePathOffset);
				filePathOffset += 4;

				//
				// 2 BYTE data
				//
				filePathOffset += 2;

				//
				// Unicode string
				//
				sp->setFullPath(MPPUtility::getUnicodeString(data.get(), filePathOffset, size));
				//filePathOffset += size;
			}

			//
			// Second block header
			//
			fileNameOffset += 18;

			//
			// CString size as a 4 BYTE int
			//
			fileNameOffset += 4;

			//
			// DOS file name
			//
			sp->setDosFileName(MPPUtility::getString(data.get(), fileNameOffset));
			fileNameOffset += (sp->getDosFileName().GetLength() + 1);

			//
			// 24 BYTE block
			//
			fileNameOffset += 24;

			//
			// 4 BYTE block size
			//
			size = MPPUtility::getInt(data.get(), fileNameOffset);
			fileNameOffset += 4;

			if (size == 0)
			{
				sp->setFileName(sp->getDosFileName());
			}
			else
			{
				//
				// 4 BYTE unicode string size in bytes
				//
				size = MPPUtility::getInt(data.get(), fileNameOffset);
				fileNameOffset += 4;

				//
				// 2 BYTE data
				//
				fileNameOffset += 2;

				//
				// Unicode string
				//
				sp->setFileName(MPPUtility::getUnicodeString(data.get(), fileNameOffset, size));
				//fileNameOffset += size;
			}
        }

        //System.out.println(sp->toString());

        // Add to the list of subprojects
        m_file->addSubProject(sp);

        return (sp);
    }

    //
    // Admit defeat at this point - we have probably stumbled
    // upon a data format we don't understand, so we'll fail
    // gracefully here. This will now be reported as a missing
    // sub project error by end users of the library, rather
    // than as an exception being thrown.
    //
    catch (...)
    {
		return SubProjectPtr();
    }
}

///**
//* This method process the data held in the props file specific to the
//* visual appearance of the project data.
//*/
//void MPP14Reader::processViewPropertyData()
//{
//    Props14 props (EncryptedDocumentInputStream::getInstance(m_file.get(), m_viewDir, L"Props"));
//    ByteArrayPtr data = props.getByteArray2(Props::FONT_BASES);
//    if (data)
//    {
//        processBaseFonts(data);
//    }
//
//    ProjectHeaderPtr header = m_file->getProjectHeader();
//    header->setShowProjectSummaryTask(props.getBoolean(Props::SHOW_PROJECT_SUMMARY_TASK));
//}

///**
//* Create an index of base font numbers and their associated base
//* font instances.
//* @param data property data
//*/
//void MPP14Reader::processBaseFonts(ByteArrayPtr data)
//{
//    int offset = 0;
//
//    int blockCount = MPPUtility::getShort(data.get(), 0);
//    offset += 2;
//
//    int size;
//    CString name;
//
//    for (int loop = 0; loop < blockCount; loop++)
//    {
//        /*unknownAttribute = MPPUtility::getShort(data.get(), offset);*/
//        offset += 2;
//
//        size = MPPUtility::getShort(data.get(), offset);
//        offset += 2;
//
//        name = MPPUtility::getUnicodeString(data.get(), offset);
//        offset += 64;
//
//        if (name.GetLength() != 0)
//        {
//			FontBasePtr fontBase (new FontBase(loop, name, size));
//			m_fontBases.SetAt(fontBase->getIndex(), fontBase);
//        }
//    }
//}

/**
* Retrieve any task field aliases defined in the MPP file.
*
* @param data task field name alias data
*/
void MPP14Reader::processTaskFieldNameAliases(ByteArrayPtr data)
{
	if (!data)
		return;

    int index = 0;
    int offset = 0;
    // First the length (repeated twice)
    int length = MPPUtility::getInt(data.get(), offset);
    offset += 8;
    // Then the number of custom columns
    int numberOfAliases = MPPUtility::getInt(data.get(), offset);
    offset += 4;

    // Then the aliases themselves
    CString alias;
    int field = -1;
    int aliasOffset = 0;
    while (index < numberOfAliases && offset < length)
    {
		// Each item consists of the Field ID (2 bytes), 40 0B marker (2 bytes), and the
		// offset to the string (4 bytes)

		// Get the Field ID
		field = MPPUtility::getShort(data.get(), offset);
		offset += 2;
		// Go past 40 0B marker
		offset += 2;
		// Get the alias offset (offset + 4 for some reason).
		aliasOffset = MPPUtility::getInt(data.get(), offset) + 4;
		offset += 4;
		// Read the alias itself 
		if (aliasOffset < (int) data->length())
		{
			alias = MPPUtility::getUnicodeString(data.get(), aliasOffset);
			m_file->setTaskFieldAlias(MPPTaskField14::getInstance(field), alias);
			//System.out.println(field + ": " + alias);
		}
		index++;
    }
    //System.out.println(file.getTaskFieldAliasMap().toString());
}

/**
* Retrieve any resource field aliases defined in the MPP file.
*
* @param data resource field name alias data
*/
void MPP14Reader::processResource2FieldNameAliases(ByteArrayPtr data)
{
	if (!data)
		return;

    int index = 0;
    int offset = 0;
    // First the length (repeated twice)
    int length = MPPUtility::getInt(data.get(), offset);
    offset += 8;
    // Then the number of custom columns
    int numberOfAliases = MPPUtility::getInt(data.get(), offset);
    offset += 4;

    // Then the aliases themselves
    CString alias;
    int field = -1;
    int aliasOffset = 0;
    while (index < numberOfAliases && offset < length)
    {
		// Each item consists of the Field ID (2 bytes), 40 0B marker (2 bytes), and the
		// offset to the string (4 bytes)

		// Get the Field ID
		field = MPPUtility::getShort(data.get(), offset);
		offset += 2;
		// Go past 40 0B marker
		offset += 2;
		// Get the alias offset (offset + 4 for some reason).
		aliasOffset = MPPUtility::getInt(data.get(), offset) + 4;
		offset += 4;
		// Read the alias itself
		if (aliasOffset < (int) data->length())
		{
			Resource2Field resField;
			if (MPPResource2Field14::getInstance(field, resField))
			{
				alias = MPPUtility::getUnicodeString(data.get(), aliasOffset);
				m_file->setResource2FieldAlias(resField, alias);
			}
			
			//System.out.println(field + ": " + alias);
		}
		index++;
    }

    //System.out.println(file.getResource2FieldAliasMap().toString());
}

/**
* This method maps the task unique identifiers to their index number
* within the FixedData block.
*
* @param fieldMap field map
* @param taskFixedMeta Fixed meta data for this task
* @param taskFixedData Fixed data for this task
* @return Mapping between task identifiers and block position
*/
std::shared_ptr<TreeMap<int, int>> MPP14Reader::createTaskMap(FieldMap* fieldMap, FixedMetaPtr taskFixedMeta, FixedDataPtr taskFixedData)
{
	std::shared_ptr<TreeMap<int, int>> taskMap (new TreeMap<int, int>());
	IntegerPtr uniqueIdOffset = fieldMap->getFixedDataOffset(TaskFieldClass::getInstance(TaskField::TF_UNIQUE_ID));
    int itemCount = taskFixedMeta->getAdjustedItemCount();
    int uniqueID;
    Integer key;

    //
    // First three items are not tasks, so let's skip them
    //
    for (int loop = 3; loop < itemCount; loop++)
    {
        ByteArrayPtr data = taskFixedData->getByteArray2Value(loop);
		if (!data) continue;

		ByteArrayPtr metaData = taskFixedMeta->getByteArray2Value(loop);

        //
        // Check for the deleted task flag
        //
        int flags = MPPUtility::getInt(metaData.get(), 0);
        if ((flags & 0x02) != 0)
        {
            // Project stores the deleted tasks unique id's into the fixed data as well
            // and at least in one case the deleted task was listed twice in the list
            // the second time with data with it causing a phantom task to be shown.
            // See CalendarErrorPhantomTasks.mpp
            //
            // So let's add the unique id for the deleted task into the map so we don't
            // accidentally include the task later.
            //
            uniqueID = MPPUtility::getShort(data.get(), TASK_UNIQUE_ID_FIXED_OFFSET); // Only a short stored for deleted tasks?
            key = uniqueID;
            if (taskMap->containsKey(key) == false)
            {
                //taskMap->put(key, null); // use null so we can easily ignore this later
				taskMap->put(key, -1);	// lch
            }
        }
        else
        {
            //
            // Do we have a null task?
            //
            if (data->length() == NULL_TASK_BLOCK_SIZE)
            {
                uniqueID = MPPUtility::getInt(data.get(), TASK_UNIQUE_ID_FIXED_OFFSET);
                key = uniqueID;
                if (taskMap->containsKey(key) == false)
                {
                    taskMap->put(key, loop);
                }
            }
            else
            {
                //
                // We apply a heuristic here - if we have more than 75% of the data, we assume 
                // the task is valid.
                //                  
                int maxOffset = fieldMap->getMaxFixedDataOffset(0);
                if (maxOffset == 0 || ((data->length() * 100) / maxOffset) > 75)
                {
                    uniqueID = MPPUtility::getInt(data.get(), uniqueIdOffset->intValue());
                    key = uniqueID;
                    if (taskMap->containsKey(key) == false)
                    {
						taskMap->put(key, loop);
                    }
                }
            }
        }
    }

    return (taskMap);
}

/**
* This method maps the resource unique identifiers to their index number
* within the FixedData block.
*
* @param fieldMap field map
* @param rscFixedMeta resource fixed meta data
* @param rscFixedData resource fixed data
* @return map of resource IDs to resource data
*/
std::shared_ptr<TreeMap<int, int>> MPP14Reader::createResource2Map(FieldMap* fieldMap, FixedMetaPtr rscFixedMeta, FixedDataPtr rscFixedData)
{
	std::shared_ptr<TreeMap<int, int>> resourceMap (new TreeMap<int, int>());
    int itemCount = rscFixedMeta->getAdjustedItemCount();
    int maxFixedDataOffset = fieldMap->getMaxFixedDataOffset(0);
	IntegerPtr uniqueIdOffset = fieldMap->getFixedDataOffset(Resource2FieldClass::getInstance(Resource2Field::RF_UNIQUE_ID));

    for (int loop = 0; loop < itemCount; loop++)
    {
        ByteArrayPtr data = rscFixedData->getByteArray2Value(loop);
        if (!data || (int)data->length() <= maxFixedDataOffset)
        {
			continue;
        }

        int uniqueID = MPPUtility::getShort(data.get(), uniqueIdOffset->intValue());
        if (resourceMap->containsKey(uniqueID) == false)
        {
			resourceMap->put(uniqueID, loop);
        }
    }

    return (resourceMap);
}

/**
* The format of the calendar data is a 4 BYTE header followed
* by 7x 60 BYTE blocks, one for each day of the week. Optionally
* following this is a set of 64 BYTE blocks representing exceptions
* to the calendar.
*/
void MPP14Reader::processCalendarData()
{
	DirectoryEntryPtr calDir = std::dynamic_pointer_cast<DirectoryEntry>(m_projectDir->getEntry(L"TBkndCal"));

    //MPPUtility::fileHexDump("c:\\temp\\varmeta.txt", new DocumentInputStream (((DocumentEntry)calDir->getEntry(L"VarMeta"))));

	DocumentInputStream is1(std::dynamic_pointer_cast<DocumentEntry> (calDir->getEntry(L"VarMeta")));
	VarMetaPtr calVarMeta(newVarMetaObj(&is1));

	DocumentInputStream is2(std::dynamic_pointer_cast<DocumentEntry> (calDir->getEntry(L"Var2Data")));
	Var2DataPtr calVarData (new Var2Data(calVarMeta, &is2));

    //System.out.println(calVarMeta);
    //System.out.println(calVarData);

	DocumentInputStream is3(std::dynamic_pointer_cast<DocumentEntry> (calDir->getEntry(L"FixedMeta")));
	FixedMeta calFixedMeta (&is3, 10);
    FixedData calFixedData (&calFixedMeta, EncryptedDocumentInputStream::getInstance(m_file, calDir, L"FixedData").get(), 12);

    //System.out.println (calFixedMeta);
    //System.out.println (calFixedData);

    //FixedMeta calFixed2Meta = new FixedMeta(new DocumentInputStream(((DocumentEntry) calDir->getEntry(L"Fixed2Meta"))), 9);
    //FixedData calFixed2Data = new FixedData(calFixed2Meta, getEncryptableInputStream(calDir, "Fixed2Data"), 48);
    //System.out.println (calFixed2Meta);
    //System.out.println (calFixed2Data);

	HashMap<int, ProjectCalendarPtr> calendarMap;// = new HashMap<Integer, ProjectCalendar>();
    int items = calFixedData.getItemCount();
	List<std::pair<ProjectCalendarPtr, int>> baseCalendars;// = new LinkedList<Pair<ProjectCalendar, Integer>>();
    ByteArrayPtr defaultCalendarData = m_projectProps->getByteArray2(Props::DEFAULT_CALENDAR_HOURS);
    ProjectCalendarPtr defaultCalendar (new ProjectCalendar(m_file));
    processCalendarHours(defaultCalendarData, NULL, defaultCalendar, true);

    for (int loop = 0; loop < items; loop++)
    {
        ByteArrayPtr fixedData = calFixedData.getByteArray2Value(loop);
		if (!(fixedData && fixedData->length() >= 8))
			continue;

        int offset = 0;

        //
        // Bug 890909, here we ensure that we have a complete 12 BYTE
        // block before attempting to process the data.
        //
        while (offset + 12 <= (int) fixedData->length())
        {
            int calendarID = MPPUtility::getInt(fixedData.get(), offset + 0);
            int baseCalendarID = MPPUtility::getInt(fixedData.get(), offset + 4);

            if (calendarID > 0 && calendarMap.containsKey(calendarID) == false)
            {
                ByteArrayPtr varData = calVarData->getByteArray2(calendarID, CALENDAR_DATA);
                ProjectCalendarPtr cal;

                if (baseCalendarID == 0 || baseCalendarID == -1 || baseCalendarID == calendarID)
                {
                    if (varData || defaultCalendarData)
                    {
						cal = m_file->addCalendar();
						if (!varData)
						{
							varData = defaultCalendarData;
						}
                    }
                    else
                    {
						cal = m_file->addDefaultBaseCalendar();
                    }

                    cal->setName(calVarData->getUnicodeString(calendarID, CALENDAR_NAME));
                }
                else
                {
                    if (varData)
                    {
						cal = m_file->addCalendar();
                    }
                    else
                    {
						cal = m_file->addDefaultDerivedCalendar();
                    }

                    baseCalendars.Add(std::pair<ProjectCalendarPtr, int>(cal, baseCalendarID));
                    int resourceID = MPPUtility::getInt(fixedData.get(), offset + 8);
                    m_resourceMap.put(resourceID, cal);
                }

                cal->setUniqueID(calendarID);

                if (varData)
                {
                    processCalendarHours(varData, defaultCalendar, cal, baseCalendarID == -1);
                    processCalendarExceptions(varData, cal);
                }

                calendarMap.put(calendarID, cal);
                m_file->fireCalendarReadEvent(cal);
            }

            offset += 12;
        }
    }

    updateBaseCalendarNames(&baseCalendars, &calendarMap);
}

/**
* For a given set of calendar data, this method sets the working
* day status for each day, and if present, sets the hours for that
* day.
*
* NOTE: MPP14 defines the concept of working weeks. MPXJ does not
* currently support this, and thus we only read the working hours
* for the default working week.
* 
* @param data calendar data block
* @param defaultCalendar calendar to use for default values 
* @param cal calendar instance
* @param isBaseCalendar true if this is a base calendar
*/
void MPP14Reader::processCalendarHours(ByteArrayPtr data, ProjectCalendarPtr defaultCalendar, ProjectCalendarPtr cal, bool isBaseCalendar)
{
    // Dump out the calendar related data and fields.
    //MPPUtility::dataDump(data, true, false, false, false, true, false, true);

    int offset;
    ProjectCalendarHoursPtr hours;
    int periodIndex;
    int index;
    int defaultFlag;
    int periodCount;
    DatePtr start;
    long duration;
    Day day;
	DateRangeList dateRanges;// = new ArrayList<DateRange>(5);

    for (index = 0; index < 7; index++)
    {
        offset = (60 * index);
        defaultFlag = (!data) ? 1 : MPPUtility::getShort(data.get(), offset);
        day = (Day)(index + 1);

        if (defaultFlag == 1)
        {
			if (isBaseCalendar)
			{
				if (!defaultCalendar)
				{
					cal->setWorkingDay(day, DEFAULT_WORKING_WEEK[index]);
					if (cal->isWorkingDay(day))
					{
						hours = cal->addCalendarHours((Day)(index + 1));
						hours->addRange(ProjectCalendarWeek::DEFAULT_WORKING_MORNING);
						hours->addRange(ProjectCalendarWeek::DEFAULT_WORKING_AFTERNOON);
					}
				}
				else
				{
					bool workingDay = defaultCalendar->isWorkingDay(day);
					cal->setWorkingDay(day, workingDay);
					if (workingDay)
					{
						hours = cal->addCalendarHours((Day)(index + 1));
						
						ProjectCalendarHoursPtr hours2 = defaultCalendar->getHours(day);
						for (int i = 0; i < hours2->getRangeCount(); i++)
						{
							DateRangePtr range = hours2->getRange(i);
							hours->addRange(range);
						}
					}
				}
			}
			else
			{
				cal->setWorkingDay(day, DayType::DEFAULT);
			}
        }
        else
        {
			dateRanges.RemoveAll();

			periodIndex = 0;
			periodCount = MPPUtility::getShort(data.get(), offset + 2);
			while (periodIndex < periodCount)
			{
				int startOffset = offset + 8 + (periodIndex * 2);
				start = MPPUtility::getTime(data.get(), startOffset);
				int durationOffset = offset + 20 + (periodIndex * 4);
				duration = MPPUtility::getDuration(data.get(), durationOffset);
				DatePtr end (new Date(start->getTime() + duration));
				dateRanges.Add(DateRangePtr (new DateRange(start, end)));
				++periodIndex;
			}

			if (dateRanges.isEmpty())
			{
				cal->setWorkingDay(day, false);
			}
			else
			{
				cal->setWorkingDay(day, true);
				hours = cal->addCalendarHours((Day)(index + 1));

				for (size_t i = 0; i < dateRanges.GetCount(); i ++)
				{
					DateRangePtr range = dateRanges.GetAt(i);
					hours->addRange(range);
				}
			}
        }
    }
}

/**
* This method extracts any exceptions associated with a calendar.
*
* @param data calendar data block
* @param cal calendar instance
*/
void MPP14Reader::processCalendarExceptions(ByteArrayPtr data, ProjectCalendarPtr cal)
{
    //
    // Handle any exceptions
    //	   	
	if (data->length() <= 420)
		return;

    int offset = 420; // The first 420 is for the working hours data

    int exceptionCount = MPPUtility::getShort(data.get(), offset);

	if (exceptionCount == 0)
		return;

    int index;
    ProjectCalendarExceptionPtr exception;
    long duration;
    int periodCount;
    DatePtr start;

    //
    // Move to the start of the first exception            
    //
    offset += 4;

    //
    // Each exception is a 92 BYTE block, followed by a 
    // variable length text block
    //
    for (index = 0; index < exceptionCount; index++)
    {
        DatePtr fromDate = MPPUtility::getDate(data.get(), offset);
		DatePtr toDate = MPPUtility::getDate(data.get(), offset + 2);
        exception = cal->addCalendarException(fromDate, toDate);

        periodCount = MPPUtility::getShort(data.get(), offset + 14);
        if (periodCount != 0)
        {
            for (int exceptionPeriodIndex = 0; exceptionPeriodIndex < periodCount; exceptionPeriodIndex++)
            {
                start = MPPUtility::getTime(data.get(), offset + 20 + (exceptionPeriodIndex * 2));
                duration = MPPUtility::getDuration(data.get(), offset + 32 + (exceptionPeriodIndex * 4));
				exception->addRange(DateRangePtr(new DateRange(start, DatePtr(new Date(start->getTime() + duration)))));
            }
        }

        //
        // Extract the name length - ensure that it is aligned to a 4 BYTE boundary
        //
        int exceptionNameLength = MPPUtility::getInt(data.get(), offset + 88);
        if (exceptionNameLength % 4 != 0)
        {
            exceptionNameLength = ((exceptionNameLength / 4) + 1) * 4;
        }

        //CString exceptionName = MPPUtility::getUnicodeString(data.get(), offset+92);
        offset += (92 + exceptionNameLength);
    }
}

/**
* The way calendars are stored in an MPP14 file means that there
* can be forward references between the base calendar unique ID for a
* derived calendar, and the base calendar itself. To get around this,
* we initially populate the base calendar name attribute with the
* base calendar unique ID, and now in this method we can convert those
* ID values into the correct names.
*
* @param baseCalendars list of calendars and base calendar IDs
* @param map map of calendar ID values and calendar objects
*/
void MPP14Reader::updateBaseCalendarNames(
											List<std::pair<ProjectCalendarPtr, int>>* baseCalendars, 
											HashMap<int, ProjectCalendarPtr>* map)
{
	for (size_t i = 0; i < baseCalendars->GetCount(); i ++)
    {
		std::pair<ProjectCalendarPtr, int> pair = baseCalendars->GetAt(i);

        ProjectCalendarPtr cal = pair.first;
		int baseCalendarID = pair.second;
		ProjectCalendarPtr baseCal;

		if (map->Lookup(baseCalendarID, baseCal) && baseCal->getName().GetLength())
        {
			cal->setParent(baseCal);
        }
        else
        {
			// Remove invalid calendar to avoid serious problems later.
			m_file->removeCalendar(cal);
        }
    }
}

//获取一个整数
int CompIntValue(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

/**
* This method extracts and collates task data. The code below
* goes through the modifier methods of the Task class in alphabetical
* order extracting the data from the MPP file. Where there is no
* mapping (e.g. the field is calculated on the fly, or we can't
* find it in the data) the line is commented out.
*
* The missing bool attributes are probably represented in the Props
* section of the task data, which we have yet to decode.
*
* @throws java.io.IOException
*/
void MPP14Reader::processTaskData()
{
	FieldMapPtr fieldMap (new FieldMap14(m_file));
    fieldMap->createTaskFieldMap(m_projectProps);

	FieldMapPtr enterpriseCustomFieldMap (new FieldMap14(m_file));
    enterpriseCustomFieldMap->createEnterpriseCustomFieldMap(m_projectProps, L"TaskField");

	DirectoryEntryPtr taskDir = std::dynamic_pointer_cast<DirectoryEntry>(m_projectDir->getEntry(L"TBkndTask"));

	DocumentInputStream s1(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"VarMeta")));
	VarMetaPtr taskVarMeta (newVarMetaObj(&s1));

	DocumentInputStream s2(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"Var2Data")));
	Var2DataPtr taskVarData(new Var2Data(taskVarMeta, &s2));

	DocumentInputStream s3(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"FixedMeta")));
	FixedMetaPtr taskFixedMeta(new FixedMeta(&s3, 47));

	FixedDataPtr taskFixedData;
	DocumentInputStream s4(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"FixedData")));
	FixedMetaPtr taskFixed2Meta;
	DocumentInputStream s5(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"Fixed2Meta")));

	if (this->getVersionParams()->FileVersion == Project2010) {
		taskFixedData.reset(new FixedData(taskFixedMeta.get(), &s4, fieldMap->getMaxFixedDataOffset(0)));
		int itemSizes[] = { 92, 93 };
		taskFixed2Meta.reset(new FixedMeta(&s5, itemSizes, 2));
	}
	else if (this->getVersionParams()->FileVersion == Project2007) {
		taskFixedData.reset(new FixedData(taskFixedMeta.get(), &s4, 768, fieldMap->getMaxFixedDataOffset(0)));
		taskFixed2Meta.reset(new FixedMeta(&s5, 86));
	}
	else
		throw toolkit::Exception(L"无效的版本类型。");
    
	DocumentInputStream s6(std::dynamic_pointer_cast<DocumentEntry>(taskDir->getEntry(L"Fixed2Data")));
	FixedDataPtr taskFixed2Data (new FixedData(taskFixed2Meta.get(), &s6));

    Props14Ptr props (new Props14(EncryptedDocumentInputStream::getInstance(m_file, taskDir, L"Props")));
    //      System.out.println(taskFixedMeta);
    //      System.out.println(taskFixedData);
    //      System.out.println(taskVarMeta);
    //      System.out.println(taskVarData);
    //      System.out.println(taskFixed2Meta);
    //      System.out.println(taskFixed2Data);
    //      System.out.println(m_outlineCodeVarData->getVarMeta());
    //      System.out.println(m_outlineCodeVarData);
    //      System.out.println(props);

    // Process aliases      
    processTaskFieldNameAliases(props->getByteArray2(TASK_FIELD_NAME_ALIASES));

	std::shared_ptr<TreeMap<int, int>> taskMap (createTaskMap(fieldMap.get(), taskFixedMeta, taskFixedData));
    // The var data may not contain all the tasks as tasks with no var data assigned will
    // not be saved in there. Most notably these are tasks with no name. So use the task map
    // which contains all the tasks.
    IntArray2Ptr uniqueIdArray = taskMap->keySet()->toArray(); //taskVarMeta.getUniqueIdentifierArray();
	// 排序
	std::qsort(uniqueIdArray->get(), uniqueIdArray->length(), sizeof(int), CompIntValue);

    int offset = 0;
    ByteArrayPtr data;
    ByteArrayPtr metaData;
    ByteArrayPtr metaData2;
    Task* task;
    bool autoWBS = true;
	List<Task*> externalTasks;// = new LinkedList<Task>();
    RecurringTaskReader recurringTaskReader(m_file);
    CString notes;

    //
    // Select the correct meta data locations depending on
    // which version of Microsoft project generated this file
    //
    MppBitFlagArrayPtr metaDataBitFlags;
    MppBitFlagArrayPtr metaData2BitFlags;
    if (m_file->getApplicationName() == L"Microsoft.Project 15.0")
    {
        metaDataBitFlags = PROJECT2013_TASK_META_DATA_BIT_FLAGS;
        metaData2BitFlags = PROJECT2013_TASK_META_DATA2_BIT_FLAGS;
    }
    else
    {
        metaDataBitFlags = PROJECT2010_TASK_META_DATA_BIT_FLAGS;
        metaData2BitFlags = PROJECT2010_TASK_META_DATA2_BIT_FLAGS;
    }
	size_t arrayCount = uniqueIdArray->length();
    for (size_t loop = 0; loop < uniqueIdArray->length(); loop++)
    {
        int uniqueID = (*uniqueIdArray)[loop];

		//taskMap->Lookup(uniqueID, offset);
		if (!taskMap->Lookup(uniqueID, offset) || taskFixedData->isValidOffset(offset) == false)
        {
			continue;
        }

        data = taskFixedData->getByteArray2Value(offset);
        metaData = taskFixedMeta->getByteArray2Value(offset);
		int id = MPPUtility::getInt(data.get(), fieldMap->getFixedDataOffset(TaskFieldClass::getInstance(TaskField::TF_ID))->intValue());

        if (data->length() == NULL_TASK_BLOCK_SIZE)
        {
			task = m_file->addTask();
			task->setNull(true);
			task->setUniqueID(MPPUtility::getShort(data.get(), TASK_UNIQUE_ID_FIXED_OFFSET));
			task->setID(MPPUtility::getShort(data.get(), TASK_ID_FIXED_OFFSET));
			m_nullTaskOrder.put(task->getID(), task->getUniqueID());
			continue;
        }

        if ((int)data->length() < fieldMap->getMaxFixedDataOffset(0))
        {
			ByteArrayPtr newData (new ByteArray2(fieldMap->getMaxFixedDataOffset(0) + 8));
			//System.arraycopy(data, 0, newData, 0, data->length());

			memcpy(newData->get(), data->get(), data->length());

			data = newData;
        }

        //System.out.println (MPPUtility::hexdump(data, false, 16, ""));
        //System.out.println (MPPUtility::hexdump(data,false));
        //System.out.println (MPPUtility::hexdump(metaData, false, 16, ""));
        //MPPUtility::dataDump(m_file, data, false, false, false, true, false, false, false, false);
        //MPPUtility::dataDump(metaData, true, true, true, true, true, true, true);
        //MPPUtility::varDataDump(taskVarData, id, true, true, true, true, true, true);

		metaData2 = taskFixed2Meta->getByteArray2Value(offset);
		 ByteArrayPtr data2 = taskFixed2Data->getByteArray2Value(offset);

        //System.out.println (MPPUtility::hexdump(metaData2, false, 16, ""));         
        //System.out.println(MPPUtility::hexdump(data2, false, 16, ""));
        //System.out.println (MPPUtility::hexdump(metaData2,false));

        ByteArrayPtr recurringData = taskVarData->getByteArray2(uniqueID, fieldMap->getVarDataKey(
			TaskFieldClass::getInstance(TaskField::TF_RECURRING_DATA))->intValue());

        Task* temp = m_file->getTaskByID(id);
        if (temp)
        {
			// Task with this id already exists... determine if this is the 'real' task by seeing
			// if this task has some var data. This is sort of hokey, but it's the best method i have
			// been able to see.
			std::shared_ptr<Set<int>> result = taskVarMeta->getUniqueIdentifierSet();
			if (result && !result->contains(uniqueID))
			{
				// Sometimes Project contains phantom tasks that coexist on the same id as a valid
				// task-> In this case don't want to include the phantom task-> Seems to be a very rare case.
				continue;
			}
			else if (temp->getName().GetLength() == 0)
			{
				// Ok, this looks valid. Remove the previous instance since it is most likely not a valid task->
				// At worst case this removes a task with an empty name.
				m_file->removeTask(temp);
			}
        }
        task = m_file->addTask();

        task->disableEvents();

		ByteArrayPtr populateArr[] = { data, data2 };

		fieldMap->populateContainer(L"TaskField", task, uniqueID, populateArr, 2, taskVarData);
        enterpriseCustomFieldMap->populateContainer(L"TaskField", task, uniqueID, NULL, 0, taskVarData);
		
        task->enableEvents();

        task->setEstimated(getDurationEstimated(MPPUtility::getShort(data.get(), 
			fieldMap->getFixedDataOffset(TaskFieldClass::getInstance(TaskField::TF_ACTUAL_DURATION_UNITS))->intValue())));

        int externalTaskID = task->getSubprojectTaskID();
		if (externalTaskID != 0)
        {
			task->setExternalTask(true);
			externalTasks.Add(task);
        }

        processHyperlinkData(task, taskVarData->getByteArray2(uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_HYPERLINK_DATA))->intValue()));

        task->setID(id);

        task->setOutlineCode(1, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE1_INDEX))->intValue()));
        task->setOutlineCode(2, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE2_INDEX))->intValue()));
        task->setOutlineCode(3, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE3_INDEX))->intValue()));
        task->setOutlineCode(4, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE4_INDEX))->intValue()));
        task->setOutlineCode(5, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE5_INDEX))->intValue()));
        task->setOutlineCode(6, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE6_INDEX))->intValue()));
        task->setOutlineCode(7, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE7_INDEX))->intValue()));
        task->setOutlineCode(8, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE8_INDEX))->intValue()));
        task->setOutlineCode(9, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE9_INDEX))->intValue()));
        task->setOutlineCode(10, getCustomFieldOutlineCodeValue(taskVarData, m_outlineCodeVarData, uniqueID, 
			fieldMap->getVarDataKey(TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE10_INDEX))->intValue()));

        task->setRecurring(MPPUtility::getShort(data.get(), 40) == 2);

        task->setUniqueID(uniqueID);

        task->setExpanded((((*metaData)[12] & 0x02) == 0));
        readBitFields(metaDataBitFlags, task, metaData);
		readBitFields(metaData2BitFlags, task, metaData2);

		m_parentTasks.SetAt(task->getUniqueID(), NumberHelper::getInt(task->getCachedValue(TaskField::TF_PARENT_TASK_UNIQUE_ID)));
		
        if (!task->getStart() || 
			(task->getCachedValue(TaskField::TF_SCHEDULED_START) && 
			task->getTaskMode() == TaskMode::AUTO_SCHEDULED))
        {
			task->setStart(std::dynamic_pointer_cast<Date>(task->getCachedValue(TaskField::TF_SCHEDULED_START)));
        }

        if (!task->getFinish() || (task->getCachedValue(TaskField::TF_SCHEDULED_FINISH) && 
			task->getTaskMode() == TaskMode::AUTO_SCHEDULED))
        {
			task->setFinish(std::dynamic_pointer_cast<Date>(task->getCachedValue(TaskField::TF_SCHEDULED_FINISH)));
        }

        if (!task->getDuration() || (task->getCachedValue(TaskField::TF_SCHEDULED_DURATION) && 
			task->getTaskMode() == TaskMode::AUTO_SCHEDULED))
        {
			task->setDuration(std::dynamic_pointer_cast<Duration2>(task->getCachedValue(TaskField::TF_SCHEDULED_DURATION)));
        }

        switch (task->getConstraintType())
        {
			//
			// Adjust the start and finish dates if the task
			// is constrained to start as late as possible.
			//            
			case AS_LATE_AS_POSSIBLE:
			{
				if (DateHelper::compare(task->getStart(), task->getLateStart()) < 0)
				{
					task->setStart(task->getLateStart());
				}
				if (DateHelper::compare(task->getFinish(), task->getLateFinish()) < 0)
				{
					task->setFinish(task->getLateFinish());
				}
				break;
			}

			case START_NO_LATER_THAN:
			case FINISH_NO_LATER_THAN:
			{
				if (DateHelper::compare(task->getFinish(), task->getStart()) < 0)
				{
					task->setFinish(task->getLateFinish());
				}
				break;
			}

			default:
			{
				break;
			}
        }

        //
        // Retrieve task recurring data
        //
        if (recurringData)
        {
			//if (!recurringTaskReader)
			//{
			//	recurringTaskReader = new RecurringTaskReader(m_file);
			//}
			recurringTaskReader.processRecurringTask(task, recurringData);
			task->setRecurring(true);
        }

        //
        // Retrieve the task notes.
        //
        notes = task->getNotes();
        if (m_reader->getPreserveNoteFormatting() == false)
        {
			notes = RtfHelper::strip(notes);
        }

        task->setNotes(notes);

        //
        // Set the calendar name
        //
        IntegerPtr calendarID = std::dynamic_pointer_cast<Integer>(task->getCachedValue(TaskField::TF_CALENDAR_UNIQUE_ID));
        if (calendarID && calendarID->intValue() != -1)
        {
			ProjectCalendarPtr calendar = m_file->getCalendarByUniqueID(calendarID->intValue());
			if (calendar)
			{
				task->setCalendar(calendar);
			}
        }

        //
        // Set the sub project flag
        //
		SubProjectPtr sp;
		m_taskSubProjects.Lookup(task->getUniqueID(), sp);
		task->setSubProject(sp);

        //
        // Set the external flag
        //
        if (sp)
        {
			task->setExternalTask(sp->isExternalTask(task->getUniqueID()));
			if (task->getExternalTask())
			{
				task->setExternalTaskProject(sp->getFullPath());
			}
        }

        //
        // If we have a WBS value from the MPP file, don't autogenerate
        //
        if (task->getWBS().GetLength() > 0)
        {
			autoWBS = false;
        }

        //
        // If this is a split task, allocate space for the split durations
        //
        if (((*metaData)[9] & 0x80) == 0)
        {
			DateRangeListPtr list(new DateRangeList());
			task->setSplits(list);
        }

        //
        // If this is a manually scheduled task, read the manual duration
        //
        if (task->getTaskMode() != TaskMode::MANUALLY_SCHEDULED)
        {
			task->setManualDuration(NULL);
        }

        //
        // Process any enterprise columns
        //
        processTaskEnterpriseColumns(uniqueID, task, taskVarData, metaData2);

        // Unfortunately it looks like 'null' tasks sometimes make it through. So let's check for to see if we
        // need to mark this task as a null task after all.
        if (task->getName().GetLength() == 0 && ((!task->getStart() || 
			task->getStart()->getTime() == MPPUtility::getEpochDate()->getTime()) || 
			(!task->getFinish() || task->getFinish()->getTime() == MPPUtility::getEpochDate()->getTime()) || 
			(!task->getCreateDate() || task->getCreateDate()->getTime() == MPPUtility::getEpochDate()->getTime())))
        {
			// Remove this to avoid passing bad data to the client
			m_file->removeTask(task);

			task = m_file->addTask();
			task->setNull(true);
			task->setUniqueID(uniqueID);
			task->setID(id);
			m_nullTaskOrder.put(task->getID(), task->getUniqueID());
			//System.out.println(task);
			continue;
        }

       if (!data2 || data2->length() < 24)
        {
			m_nullTaskOrder.put(task->getID(), task->getUniqueID());
        }
        else
        {
			__int64 key = MPPUtility::getLong(data2.get(), 16);
			//while (m_taskOrder.containsKey(key))
			if(m_taskOrder.containsKey(key))
			{
					//key += (arrayCount+1);
				m_nullTaskOrder.put(task->getID(), task->getUniqueID());
			}
			else	
				m_taskOrder.put(key, task->getUniqueID());
        }

        //System.out.println(task + " " + MPPUtility::getShort(data2, 22)); // JPI - looks like this value determines the ID order! Validate and check other versions!
        m_file->fireTaskReadEvent(task);
        //dumpUnknownData(task->getUniqueID().toString(), UNKNOWN_TASK_DATA, data);
        //System.out.println(task);
    }//end for
	
    //
    // Enable auto WBS if necessary
    //
    m_file->setAutoWBS(autoWBS);

  
    // We have now read all of the task, so we are in a position
    // to perform post-processing to set up the relevant details
    // for each external task->
    //
    if (!externalTasks.isEmpty())
    {
        processExternalTasks(&externalTasks);
    }
}

/**
* MPP14 files seem to exhibit some occasional weirdness
* with duplicate ID values which leads to the task structure
* being reported incorrectly. The following method attempts to correct this. 
* The method uses ordering data embedded in the file to reconstruct
* the correct ID order of the tasks. 
*/
//struct MyStruct
//{
//	long long key;
//	int val;
//};
void MPP14Reader::postProcessTasks()
{
    //
    // Renumber ID values using a large increment to allow
    // space for later inserts.
    //

	//struct MyCom {
	//	bool operator() (MyStruct i, MyStruct j) { return (i.key < j.key); }
	//};
	//std::string str = "F:\\Desktop\\output.txt";
	//std::ofstream file(str, std::ios::out | std::ios::app);
	//if (file.is_open()) 
	//{
	//	std::stringstream stream;
	//	stream << "\nm_taskOrder\n";
	//	std::vector<MyStruct> v1;
	//	POSITION start = m_taskOrder.GetStartPosition();
	//	while (start != nullptr)
	//	{
	//		MyStruct s;
	//		long long key = 0;
	//		int val = 0;
	//		m_taskOrder.GetNextAssoc(start, key, val);
	//		s.key = key;
	//		s.val = val;
	//		v1.push_back(s);
	//	}
	//	//排序
	//	std::sort(v1.begin(), v1.end(), MyCom());
	//	for (auto &ele : v1)
	//	{
	//		std::string strId = std::to_string(ele.key);
	//		std::string strUid = std::to_string(ele.val);
	//		std::string all = "ID: " + strId + "\tUID: " + strUid + "\n";
	//		stream << all;
	//	}
	//		
	//	file << stream.rdbuf();
	//	file.close();
	//}


	TreeMap<int, int> taskMap;// = new TreeMap<Integer, Integer>();
    int nextIDIncrement = 2000;
    int nextID = (!m_file->getTaskByUniqueID(0) ? nextIDIncrement : 0);
	
	// 按key排序任务
	struct long_comp {
		bool operator() (__int64 i, __int64 j) { return (i < j); }
	} my_long_comp;

	std::shared_ptr<Array2<__int64>> orderArray = m_taskOrder.keySet()->toArray();
	std::vector<__int64> orderVector(orderArray->get(), orderArray->get() + orderArray->length());
	std::sort(orderVector.begin(), orderVector.end(), my_long_comp);

	for (__int64 key : orderVector)
	{
		int value = 0;
		if (!m_taskOrder.Lookup(key, value))
			continue;

		taskMap.put(nextID, value);
		nextID += nextIDIncrement;
	}

	struct int_comp {
		bool operator() (int i, int j) { return (i < j); }
	} my_int_comp;

    //
    // Insert any null tasks into the correct location
    //
    int insertionCount = 0;
	if (m_nullTaskOrder.GetCount())
	{
		std::shared_ptr<IntArray2> nullOrderArray = m_nullTaskOrder.keySet()->toArray();
		std::vector<int> nullOrderVector(nullOrderArray->get(), nullOrderArray->get() + nullOrderArray->length());
		std::sort(nullOrderVector.begin(), nullOrderVector.end(), my_int_comp);
		for (int key : nullOrderVector)
		{
			int value = 0;
			if (!m_nullTaskOrder.Lookup(key, value))
				continue;

			int idValue = key;
			int baseTargetIdValue = (idValue - insertionCount) * nextIDIncrement;
			int targetIDValue = baseTargetIdValue;
			int offset = 0;
			++insertionCount;

			while (taskMap.containsKey(targetIDValue))
			{
				++offset;
				if (offset == nextIDIncrement)
				{
					throw toolkit::Exception(L"Unable to fix task order");
				}
				targetIDValue = baseTargetIdValue - (nextIDIncrement - offset);
			}
			//CString strA;
			//strA.Format(_T("key:%d ,value(UId): %d\n"), targetIDValue, value);

			//OutputDebugString(strA);
			taskMap.put(targetIDValue, value);
		}
	}

    //
    // Finally, we can renumber the tasks
    //
	nextID = (!m_file->getTaskByUniqueID(0) ? 1 : 0);
	

	// 按key排序任务
	std::shared_ptr<IntArray2> keyArray = taskMap.keySet()->toArray();
	std::vector<int> keyVector(keyArray->get(), keyArray->get() + keyArray->length());
	std::sort(keyVector.begin(), keyVector.end(), my_int_comp);
	
	for (int key : keyVector)
	{
		int value = 0;
		if (!taskMap.Lookup(key, value))
			continue;

		Task* task = m_file->getTaskByUniqueID(value);
		if (task)
		{
			int uid = task->getUniqueID();
			task->setID(nextID);
		}
		nextID++;
	}
}

/**
* Extracts task enterprise column values. 
* 
* @param id task unique ID
* @param task task instance
* @param taskVarData task var data
* @param metaData2 task meta data
*/
void MPP14Reader::processTaskEnterpriseColumns(int id, Task* task, Var2DataPtr taskVarData, ByteArrayPtr metaData2)
{
	if (!metaData2)
		return;
 
    int bits = MPPUtility::getInt(metaData2.get(), 29);
    task->set(TaskField::TF_ENTERPRISE_FLAG1, BooleanPtr(new Boolean((bits & 0x0000800) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG2, BooleanPtr(new Boolean((bits & 0x0001000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG3, BooleanPtr(new Boolean((bits & 0x0002000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG4, BooleanPtr(new Boolean((bits & 0x0004000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG5, BooleanPtr(new Boolean((bits & 0x0008000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG6, BooleanPtr(new Boolean((bits & 0x0001000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG7, BooleanPtr(new Boolean((bits & 0x0002000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG8, BooleanPtr(new Boolean((bits & 0x0004000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG9, BooleanPtr(new Boolean((bits & 0x0008000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG10, BooleanPtr(new Boolean((bits & 0x0010000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG11, BooleanPtr(new Boolean((bits & 0x0020000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG12, BooleanPtr(new Boolean((bits & 0x0040000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG13, BooleanPtr(new Boolean((bits & 0x0080000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG14, BooleanPtr(new Boolean((bits & 0x0100000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG15, BooleanPtr(new Boolean((bits & 0x0200000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG16, BooleanPtr(new Boolean((bits & 0x0400000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG17, BooleanPtr(new Boolean((bits & 0x0800000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG18, BooleanPtr(new Boolean((bits & 0x1000000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG19, BooleanPtr(new Boolean((bits & 0x2000000) != 0)));
	task->set(TaskField::TF_ENTERPRISE_FLAG20, BooleanPtr(new Boolean((bits & 0x4000000) != 0)));
}

/**
* Extracts resource enterprise column data.
* 
* @param id resource unique ID 
* @param resource resource instance
* @param resourceVarData resource var data
* @param metaData2 resource meta data 
*/
void MPP14Reader::processResource2EnterpriseColumns(int id, Resource2* resource, Var2DataPtr resourceVarData, ByteArrayPtr metaData2)
{
	if (!metaData2)
		return;

    int bits = MPPUtility::getInt(metaData2.get(), 16);
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG1, BooleanPtr(new Boolean((bits & 0x00010) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG2, BooleanPtr(new Boolean((bits & 0x00020) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG3, BooleanPtr(new Boolean((bits & 0x00040) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG4, BooleanPtr(new Boolean((bits & 0x00080) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG5, BooleanPtr(new Boolean((bits & 0x00100) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG6, BooleanPtr(new Boolean((bits & 0x00200) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG7, BooleanPtr(new Boolean((bits & 0x00400) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG8, BooleanPtr(new Boolean((bits & 0x00800) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG9, BooleanPtr(new Boolean((bits & 0x01000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG10, BooleanPtr(new Boolean((bits & 0x02000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG11, BooleanPtr(new Boolean((bits & 0x04000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG12, BooleanPtr(new Boolean((bits & 0x08000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG13, BooleanPtr(new Boolean((bits & 0x10000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG14, BooleanPtr(new Boolean((bits & 0x20000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG15, BooleanPtr(new Boolean((bits & 0x40000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG16, BooleanPtr(new Boolean((bits & 0x80000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG17, BooleanPtr(new Boolean((bits & 0x100000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG18, BooleanPtr(new Boolean((bits & 0x200000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG19, BooleanPtr(new Boolean((bits & 0x400000) != 0)));
	resource->set(Resource2Field::RF_ENTERPRISE_FLAG20, BooleanPtr(new Boolean((bits & 0x800000) != 0)));
}

/**
* The project files to which external tasks relate appear not to be
* held against each task, instead there appears to be the concept
* of the "current" external task file, i.e. the last one used.
* This method iterates through the list of tasks marked as external
* and attempts to ensure that the correct external project data (in the
* form of a SubProject object) is linked to the task->
* 
* @param externalTasks list of tasks marked as external
*/
void MPP14Reader::processExternalTasks(List<Task*>* externalTasks)
{
    //
    // Sort the list of tasks into ID order
    //
    //Collections.sort(externalTasks);

    //
    // Find any external tasks which don't have a sub project
    // object, and set this attribute using the most recent 
    // value.
    //
    SubProjectPtr currentSubProject;

	for (size_t i = 0; i < externalTasks->GetCount(); i ++)
    {
		Task* currentTask = externalTasks->GetAt(i);

        SubProjectPtr sp = currentTask->getSubProject();
        if (!sp)
        {
			currentTask->setSubProject(currentSubProject);

			//we need to set the external task project path now that we have
			//the subproject for this task (was skipped while processing the task earlier)
			if (currentSubProject)
			{
				currentTask->setExternalTaskProject(currentSubProject->getFullPath());
			}

        }
        else
        {
			currentSubProject = sp;
        }

        if (currentSubProject)
        {
			currentTask->setProject(currentSubProject->getFullPath());
        }
    }
}

/**
* This method is used to extract the task hyperlink attributes
* from a block of data and call the appropriate modifier methods
* to configure the specified task object.
*
* @param task task instance
* @param data hyperlink data block
*/
void MPP14Reader::processHyperlinkData(Task* task, ByteArrayPtr data)
{
	if (!data)
		return;

    int offset = 12;
    CString hyperlink;
    CString address;
    CString subaddress;

    offset += 12;
    hyperlink = MPPUtility::getUnicodeString(data.get(), offset);
    offset += ((hyperlink.GetLength() + 1) * 2);

    offset += 12;
    address = MPPUtility::getUnicodeString(data.get(), offset);
	offset += ((address.GetLength() + 1) * 2);

    offset += 12;
    subaddress = MPPUtility::getUnicodeString(data.get(), offset);

    task->setHyperlink(hyperlink);
    task->setHyperlinkAddress(address);
    task->setHyperlinkSubAddress(subaddress);
}

/**
* This method is used to extract the resource hyperlink attributes
* from a block of data and call the appropriate modifier methods
* to configure the specified task object.
*
* @param resource resource instance
* @param data hyperlink data block
*/
void MPP14Reader::processHyperlinkData(Resource2Ptr resource, ByteArrayPtr data)
{
	if (!data) return;
   
    int offset = 12;
    CString hyperlink;
    CString address;
    CString subaddress;

    offset += 12;
    hyperlink = MPPUtility::getUnicodeString(data.get(), offset);
	offset += ((hyperlink.GetLength() + 1) * 2);

    offset += 12;
    address = MPPUtility::getUnicodeString(data.get(), offset);
	offset += ((address.GetLength() + 1) * 2);

    offset += 12;
    subaddress = MPPUtility::getUnicodeString(data.get(), offset);

    resource->setHyperlink(hyperlink);
	resource->setHyperlinkAddress(address);
	resource->setHyperlinkSubAddress(subaddress);
   
}

/**
* This method extracts and collates constraint data.
*
* @throws java.io.IOException
*/
void MPP14Reader::processConstraintData()
{
	ConstraintFactory factory;// = new ConstraintFactory();
    factory.process(m_projectDir, m_file);
}

/**
* This method extracts and collates resource data.
*
* @throws java.io.IOException
*/
void MPP14Reader::processResource2Data()
{
    FieldMapPtr fieldMap (new FieldMap14(m_file));
    fieldMap->createResource2FieldMap(m_projectProps);

    FieldMapPtr enterpriseCustomFieldMap (new FieldMap14(m_file));
    enterpriseCustomFieldMap->createEnterpriseCustomFieldMap(m_projectProps, L"Resource2Field");

	DirectoryEntryPtr rscDir = std::dynamic_pointer_cast<DirectoryEntry>(m_projectDir->getEntry(L"TBkndRsc"));

    VarMetaPtr rscVarMeta (newVarMetaObj(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry> (rscDir->getEntry(L"VarMeta")))));

	Var2DataPtr rscVarData(new Var2Data(rscVarMeta, &DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry> (rscDir->getEntry(L"Var2Data")))));

	FixedMetaPtr rscFixedMeta(new FixedMeta(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry> (rscDir->getEntry(L"FixedMeta"))), 37));

	FixedDataPtr rscFixedData (new FixedData(rscFixedMeta.get(), EncryptedDocumentInputStream::getInstance(m_file, rscDir, L"FixedData").get()));

	FixedMetaPtr rscFixed2Meta;
	if (getVersionParams()->FileVersion == Project2010)
		rscFixed2Meta.reset(new FixedMeta(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry> (rscDir->getEntry(L"Fixed2Meta"))), 50));
	else if (getVersionParams()->FileVersion == Project2007)
		rscFixed2Meta.reset(new FixedMeta(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry> (rscDir->getEntry(L"Fixed2Meta"))), 49));
	//else
	//	throw Exception(L"不支持的文件格式。");

	
	FixedDataPtr rscFixed2Data;
	
	if (rscFixed2Meta.get()) 
	{
		rscFixed2Data.reset ((new FixedData(rscFixed2Meta.get(),
			EncryptedDocumentInputStream::getInstance(m_file, rscDir, L"Fixed2Data").get())));

		Props14 props(EncryptedDocumentInputStream::getInstance(m_file, rscDir, L"Props"));

		// Process aliases      
		processResource2FieldNameAliases(props.getByteArray2(RESOURCE_FIELD_NAME_ALIASES));
	}
	else
	{
		// Process aliases      
		processResource2FieldNameAliases(m_projectProps->getByteArray2(RESOURCE_FIELD_NAME_ALIASES));
	}

	std::shared_ptr<TreeMap<int, int>> resourceMap (createResource2Map(fieldMap.get(), rscFixedMeta, rscFixedData));
    IntArray2Ptr uniqueid = rscVarMeta->getUniqueIdentifierArray();
    int id;
	int offset;
    ByteArrayPtr data;
    ByteArrayPtr metaData;
    Resource2Ptr resource;
    CString notes;

    //
    // Select the correct meta data locations depending on
    // which version of Microsoft project generated this file
    //
    MppBitFlagArrayPtr metaDataBitFlags;
    MppBitFlagArrayPtr metaData2BitFlags;
    if (m_file->getApplicationName() == L"Microsoft.Project 15.0")
    {
        metaDataBitFlags = PROJECT2013_RESOURCE_META_DATA_BIT_FLAGS;
        metaData2BitFlags = PROJECT2013_RESOURCE_META_DATA2_BIT_FLAGS;
    }
    else
    {
        metaDataBitFlags = PROJECT2010_RESOURCE_META_DATA_BIT_FLAGS;
        metaData2BitFlags = PROJECT2010_RESOURCE_META_DATA2_BIT_FLAGS;
    }

    for (size_t loop = 0; loop < uniqueid->length(); loop++)
    {
        id = (*uniqueid)[loop];
		if (!resourceMap->Lookup(id, offset)) {
			continue;
        }

        data = rscFixedData->getByteArray2Value(offset);
		ByteArrayPtr metaData2;
		ByteArrayPtr data2;
		if (rscFixed2Meta) 
		{
			metaData2 = rscFixed2Meta->getByteArray2Value(offset);
			data2 = rscFixed2Data->getByteArray2Value(offset);
		}

        resource = m_file->addResource2();

        resource->disableEvents();

		ByteArrayPtr arr2[] = { data, data2 };
		fieldMap->populateContainer(L"Resource2Field", resource.get(), id, arr2, 2, rscVarData);

		enterpriseCustomFieldMap->populateContainer(L"Resource2Field", resource.get(), id, NULL, 0, rscVarData);

        resource->enableEvents();

        processHyperlinkData(resource, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_HYPERLINK_DATA))->intValue()));

		if (getVersionParams()->FileVersion == Project2010)
		{
			resource->setID(MPPUtility::getInt(data.get(), fieldMap->getFixedDataOffset(
				Resource2FieldClass::getInstance(Resource2Field::RF_ID))->intValue()));
		}
		else if (getVersionParams()->FileVersion == Project2007)
		{
			resource->setBudget(((*metaData2)[8] & 0x20) != 0);
			resource->setGUID(&MPPUtility::getGUID(data2.get(), 0));
			resource->setID(MPPUtility::getInt(data.get(), 4));
		}

        resource->setOutlineCode1(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE1_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode2(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE2_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode3(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE3_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode4(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE4_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode5(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE5_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode6(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE6_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode7(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE7_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode8(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE8_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode9(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE9_INDEX))->intValue()), OUTLINECODE_DATA));
        resource->setOutlineCode10(m_outlineCodeVarData->getUnicodeString(rscVarData->getInt(id, 2, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE10_INDEX))->intValue()), OUTLINECODE_DATA));

        metaData = rscFixedMeta->getByteArray2Value(offset);

		int fileVer = getVersionParams()->FileVersion;
		if (fileVer == Project2007 || fileVer == Project2003)
		{
			resource->setFlag(1, ((*metaData)[28] & 0x40) != 0);
			resource->setFlag(2, ((*metaData)[28] & 0x80) != 0);
			resource->setFlag(3, ((*metaData)[29] & 0x01) != 0);
			resource->setFlag(4, ((*metaData)[29] & 0x02) != 0);
			resource->setFlag(5, ((*metaData)[29] & 0x04) != 0);
			resource->setFlag(6, ((*metaData)[29] & 0x08) != 0);
			resource->setFlag(7, ((*metaData)[29] & 0x10) != 0);
			resource->setFlag(8, ((*metaData)[29] & 0x20) != 0);
			resource->setFlag(9, ((*metaData)[29] & 0x40) != 0);
			resource->setFlag(10, ((*metaData)[28] & 0x20) != 0);
			resource->setFlag(11, ((*metaData)[29] & 0x20) != 0);
			resource->setFlag(12, ((*metaData)[30] & 0x01) != 0);
			resource->setFlag(13, ((*metaData)[30] & 0x02) != 0);
			resource->setFlag(14, ((*metaData)[30] & 0x04) != 0);
			resource->setFlag(15, ((*metaData)[30] & 0x08) != 0);
			resource->setFlag(16, ((*metaData)[30] & 0x10) != 0);
			resource->setFlag(17, ((*metaData)[30] & 0x20) != 0);
			resource->setFlag(18, ((*metaData)[30] & 0x40) != 0);
			resource->setFlag(19, ((*metaData)[30] & 0x80) != 0);
			resource->setFlag(20, ((*metaData)[31] & 0x01) != 0);
		}
		else
		{
			readBitFields(metaDataBitFlags, resource.get(), metaData);
			readBitFields(metaData2BitFlags, resource.get(), metaData2);
		}

        Resource2Type type;
		if (resource->getWorkGroup2() == WorkGroup2::WG_DEFAULT)
        {
			type = Resource2Type::RT_WORK;
        }
        else
        {
			if (resource->getStandardRateUnits() == TimeUnit::ELAPSED_MINUTES)
			{
				type = Resource2Type::RT_MATERIAL;
			}
			else
			{
				type = Resource2Type::RT_COST;
			}
        }
        resource->setType(type);

        resource->setUniqueID(id);

        notes = resource->getNotes();
        if (m_reader->getPreserveNoteFormatting() == false)
        {
			notes = RtfHelper::strip(notes);
        }

        resource->setNotes(notes);

        //
        // Configure the resource calendar
        //
		ProjectCalendarPtr caltemp;
		if (m_resourceMap.Lookup(id, caltemp))
		{
			resource->setResource2Calendar(caltemp);
		}

        //
        // Process any enterprise columns
        //         
        processResource2EnterpriseColumns(id, resource.get(), rscVarData, metaData2);

        //
        // Process cost rate tables
        //
		CostRateTableFactory crt;// = new CostRateTableFactory();
        crt.process(resource, 0, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_A))->intValue()));
        crt.process(resource, 1, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_B))->intValue()));
        crt.process(resource, 2, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_C))->intValue()));
        crt.process(resource, 3, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_D))->intValue()));
        crt.process(resource, 4, rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_E))->intValue()));

        //
        // Process availability table
        //
		AvailabilityFactory af;// = new AvailabilityFactory();
        af.process(resource->getAvailability(), rscVarData->getByteArray2(id, fieldMap->getVarDataKey(
			Resource2FieldClass::getInstance(Resource2Field::RF_AVAILABILITY_DATA))->intValue()));

        m_file->fireResource2ReadEvent(resource);
    }
}

/**
* This method extracts and collates resource assignment data.
*
* @throws IOException
*/
void MPP14Reader::processAssignmentData()
{
    FieldMapPtr fieldMap (new FieldMap14(m_file));
    fieldMap->createAssignmentFieldMap(m_projectProps);

    FieldMapPtr enterpriseCustomFieldMap (new FieldMap14(m_file));
    enterpriseCustomFieldMap->createEnterpriseCustomFieldMap(m_projectProps, L"AssignmentField");

	DirectoryEntryPtr assnDir = std::dynamic_pointer_cast<DirectoryEntry>(m_projectDir->getEntry(L"TBkndAssn"));
    VarMetaPtr assnVarMeta (newVarMetaObj(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry>(assnDir->getEntry(L"VarMeta")))));
	Var2DataPtr assnVarData(new Var2Data(assnVarMeta, &DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry>(assnDir->getEntry(L"Var2Data")))));
	FixedMetaPtr assnFixedMeta(new FixedMeta(&DocumentInputStream(std::dynamic_pointer_cast<DocumentEntry>(assnDir->getEntry(L"FixedMeta"))), 34));

	FixedDataPtr assnFixedData;
	FixedDataPtr assnFixedData2;
	
	if (getVersionParams()->FileVersion == Project2010)
		assnFixedData.reset(new FixedData(110, EncryptedDocumentInputStream::getInstance(m_file, assnDir, L"FixedData").get()));
	else if (getVersionParams()->FileVersion == Project2007)
		assnFixedData.reset(new FixedData(assnFixedMeta.get(), EncryptedDocumentInputStream::getInstance(m_file, assnDir, L"FixedData").get()));
	else
		throw toolkit::Exception(L"无效的版本类型。");

	assnFixedData2.reset(new FixedData(48, EncryptedDocumentInputStream::getInstance(m_file, assnDir, L"Fixed2Data").get()));
    //FixedMeta assnFixedMeta2 = new FixedMeta(new DocumentInputStream(((DocumentEntry) assnDir->getEntry(L"Fixed2Meta"))), 53);
    //Props props = new Props14(new DocumentInputStream(((DocumentEntry) assnDir->getEntry(L"Props"))));

	Resource2AssignmentFactory factory;// = new Resource2AssignmentFactory();
    factory.process(m_file, fieldMap, enterpriseCustomFieldMap, m_reader->getUseRawTimephasedData(), 
		m_reader->getPreserveNoteFormatting(), assnVarMeta, assnVarData, 
		assnFixedMeta, assnFixedData, assnFixedData2, assnFixedMeta->getItemCount());
}

/**
* This method is used to determine if a duration is estimated.
*
* @param type Duration units value
* @return bool Estimated flag
*/
bool MPP14Reader::getDurationEstimated(int type)
{
    return ((type & DURATION_CONFIRMED_MASK) != 0);
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param outlineCodeVarData var data block
* @param id item ID
* @param type item type
* @return item value
*/
CString MPP14Reader::getCustomFieldOutlineCodeValue(Var2DataPtr varData, Var2DataPtr outlineCodeVarData, int id, int type)
{
    CString result;

    int mask = varData->getShort(id, type);
    if ((mask & 0xFF00) != VALUE_LIST_MASK)
    {
        result = (CString)outlineCodeVarData->getUnicodeString(varData->getInt(id, 2, type), OUTLINECODE_DATA);
    }
    else
    {
        int uniqueId = varData->getInt(id, 2, type);
		CustomFieldValueItemPtr item = m_file->getCustomFieldValueItem(uniqueId);
        if (item && item->getValue())
        {
			result = MPPUtility::getUnicodeString(item->getValue().get());

			CString result2 = getCustomFieldOutlineCodeValue(varData, outlineCodeVarData, item->getParent());
			if (result2.GetLength() > 0)
			{
				result = result2 + L"." + result;
			}
        }
    }
    return result;
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param outlineCodeVarData var data block
* @param id parent item ID
* @return item value
*/
CString MPP14Reader::getCustomFieldOutlineCodeValue(Var2DataPtr varData, Var2DataPtr outlineCodeVarData, int id)
{
    CString result;

	int uniqueId = id;// .intValue();
    if (uniqueId == 0)
    {
        return L"";
    }

	CustomFieldValueItemPtr item = m_file->getCustomFieldValueItem(uniqueId);
    if (item && item->getValue())
    {
        result = MPPUtility::getUnicodeString(item->getValue().get());
		if (id != item->getParent())
        {
			CString result2 = getCustomFieldOutlineCodeValue(varData, outlineCodeVarData, item->getParent());
			if (result2.GetLength() > 0)
			{
				result = result2 + L"." + result;
			}
        }
    }

    return result;
}

/**
* Iterate through a set of bit field flags and set the value for each one
* in the supplied container.
* 
* @param flags bit field flags
* @param container field container
* @param data source data
*/
void MPP14Reader::readBitFields(MppBitFlagArrayPtr flags, FieldContainer* container, ByteArrayPtr data)
{
	for (size_t i = 0; i < flags->length(); i ++)
    {
		MppBitFlagPtr flag = (*flags)[i];

        flag->setValue(container, data);
    }
}

// 获取不同版本参数
const MPPVersionParams* MPP12Reader::getVersionParams()
{
	static MPPVersionParams params = { 0 };
	if (params.FileVersion == 0) {
		params.FileVersion = Project2007;
		params.PropsEntry = L"Props12";
		params.ProjectDirEntry = L"   112";
	}

	return &params;
}