#include "stdafx.h"
#include "PropsBlock.h"
#include "MPPUtility.h"

//获取一个整数
int CompIntValue3(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

/**
* Constructor.
*
* @param data block of property data
*/
PropsBlock::PropsBlock(ByteArrayPtr data)
{
	int dataSize = MPPUtility::getInt(data.get(), 0);
	int itemCount = MPPUtility::getInt(data.get(), 4);

	int offset = 8;
	//Map<Integer, Integer> offsetMap = new TreeMap<Integer, Integer>();
	TreeMap<int, int> offsetMap;
	for (int loop = 0; loop < itemCount; loop++)
	{
		int itemKey = MPPUtility::getInt(data.get(), offset);
		offset += 4;

		int itemOffset = MPPUtility::getInt(data.get(), offset);
		offset += 4;

		offsetMap.put(itemOffset, itemKey);
	}

	int previousItemOffset = -1;
	int previousItemKey = -1;

	std::shared_ptr<Array2<int>> offsetArray = offsetMap.keySet()->toArray();
	std::qsort(offsetArray->get(), offsetArray->length(), sizeof(int), CompIntValue3);

	for (int i = 0; i < (int) offsetArray->length(); i++)
	{
		int itemOffset = (*offsetArray)[i];
		populateMap(data, previousItemOffset, previousItemKey, itemOffset);
		previousItemOffset = itemOffset;
		
		if (!offsetMap.Lookup(previousItemOffset, previousItemKey))
			previousItemKey = -1;
		//previousItemKey = offsetMap.get(previousItemOffset);
	}

	//for (int itemOffset : offsetMap.keySet())
	//{
	//	populateMap(data, previousItemOffset, previousItemKey, itemOffset);
	//	previousItemOffset = itemOffset;
	//	previousItemKey = offsetMap.get(previousItemOffset);
	//}

	if (previousItemOffset != -1)
	{
		populateMap(data, previousItemOffset, previousItemKey, dataSize);
	}
}

/**
* Method used to extract data from the block of properties and
* insert the key value pair into a map.
*
* @param data block of property data
* @param previousItemOffset previous offset
* @param previousItemKey item key
* @param itemOffset current item offset
*/
void PropsBlock::populateMap(ByteArrayPtr data, int previousItemOffset, int previousItemKey, int itemOffset)
{
	if (previousItemOffset == -1)
		return;

	int itemSize = itemOffset - previousItemOffset;
	ByteArrayPtr itemData (new ByteArray2(itemSize));
	//System.arraycopy(data, previousItemOffset, itemData, 0, itemSize);
	memcpy(&(*itemData)[0], &(*data)[previousItemOffset], itemSize);
	m_map.put(previousItemKey, itemData);
}