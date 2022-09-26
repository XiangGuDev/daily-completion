#pragma once

#include "FieldType.h"

/**
 * Instances of this type represent constraint fields.
 */
enum ConstraintField
{
	CF_UNIQUE_ID,//(DataType.INTEGER),
	CF_TASK1,//(DataType.INTEGER),
	CF_TASK2,//(DataType.INTEGER);
	CF_FINISH,
};

// 获取枚举值对应的更多信息
class ConstraintFieldClass : public FieldType
{
public:
	static FieldType* getInstance(int type);
};