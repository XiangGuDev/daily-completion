#pragma once

#include "..\FieldType.h"

class FieldContainer;

/**
 * Represents the type and location of a bit flag within a block of data.
 */
class MppBitFlag
{
public:
   /**
    * Constructor.
    * 
    * @param type field type
    * @param offset offset in buffer
    * @param mask bit mask 
    * @param zeroValue value to return if expression is zero
    * @param nonZeroValue value to return if expression is non-zero
    */
	MppBitFlag(FieldType* type, int offset, int mask, ObjectPtr zeroValue, ObjectPtr nonZeroValue);

   /**
    * Extracts the value of this bit flag from the supplied byte array
    * and sets the value in the supplied container.
    * 
    * @param container container
    * @param data byte array
    */
	void setValue(FieldContainer* container, ByteArrayPtr data);

private:
	FieldType* m_type;
	int m_offset;
	int m_mask;
	ObjectPtr m_zeroValue;
	ObjectPtr m_nonZeroValue;
};

typedef std::shared_ptr<MppBitFlag> MppBitFlagPtr;
typedef Array2<MppBitFlagPtr> MppBitFlagArray;
typedef std::shared_ptr<MppBitFlagArray> MppBitFlagArrayPtr;