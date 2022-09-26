#include "stdafx.h"
#include "AbstractVarMeta.h"


/**
* This method retrieves the number of items in the Var2Data block.
*
* @return number of items
*/
int AbstractVarMeta::getItemCount()
{
    return (m_itemCount);
}

/**
* This method retrieves the size of the Var2Data block.
*
* @return data size
*/
int AbstractVarMeta::getDataSize()
{
    return (m_dataSize);
}

/**
* This method returns an array containing all of the unique identifiers
* for which data has been stored in the Var2Data block.
*
* @return array of unique identifiers
*/
IntArray2Ptr AbstractVarMeta::getUniqueIdentifierArray()
{
	return m_table.keySet()->toArray();
 //   IntArray2Ptr result (new IntArray2(m_table.GetCount()));
 //   int index = 0;
	//IntArray2Ptr keyset = m_table.keySet()->toArray();
	//for (size_t i = 0; i < keyset->length(); i ++)
 //   {
	//	(*result)[index] = (*keyset)[i];
 //       ++index;
 //   }
 //   return (result);
}

/**
* This method returns an set containing all of the unique identifiers
* for which data has been stored in the Var2Data block.
*
* @return set of unique identifiers
*/
std::shared_ptr<Set<int>> AbstractVarMeta::getUniqueIdentifierSet()
{
    return (m_table.keySet());
}

/**
* This method retrieves the offset of a given entry in the Var2Data block.
* Each entry can be uniquely located by the identifier of the object to
* which the data belongs, and the type of the data.
*
* @param id unique identifier of an entity
* @param type data type identifier
* @return offset of requested item
*/
bool AbstractVarMeta::getOffset(int id, int type, int& result)
{
    //Integer result = null;

	std::shared_ptr<Map2<int, int>> map;
	if (m_table.Lookup(id, map) && type != NULL)
    {
		return map->Lookup(type, result);
    }

	return false;
}

/**
* This method retrieves the offset of the data item at
* the position defined by the index parameter.
*
* @param index index of item in the block
* @return offset of the item in the block
*/
int AbstractVarMeta::getOffset(int index)
{
    return (*m_offsets)[index];
}

/**
* {@inheritDoc}
*/
std::shared_ptr<Set<int>> AbstractVarMeta::getTypes(int id)
{
	std::shared_ptr<Set<int>> result;

	std::shared_ptr<Map2<int, int>> map;
	if (m_table.Lookup(id, map))
    {
        result = map->keySet();
    }
    else
    {
        result.reset (new Set<int>());
    }

    return result;
}

/**
* {@inheritDoc}
*/
bool AbstractVarMeta::containsKey(int key)
{
    return m_table.containsKey(key);
}

///**
//* This method dumps the contents of this VarMeta block as a String.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this block
//*/
//CString AbstractVarMeta::toString()
//{
//    return toString(null);
//}
//
///**
//* This method dumps the contents of this VarMeta block as a String.
//* Note that this facility is provided as a debugging aid.
//*
//* @param fieldMap field map used to decode var data keys
//* @return formatted contents of this block
//*/
//CString AbstractVarMeta::toString(FieldMap* fieldMap)
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.println("BEGIN: VarMeta");
//    pw.println("   Item count: " + m_itemCount);
//    pw.println("   Data size: " + m_dataSize);
//
//    for (Integer uniqueID : m_table.keySet())
//    {
//        pw.println("   Entries for Unique ID: " + uniqueID);
//        Map<Integer, Integer> map = m_table.get(uniqueID);
//        for (Map.Entry<Integer, Integer> entry : map.entrySet())
//        {
//        FieldType fieldType = fieldMap == null ? null : fieldMap.getFieldTypeFromVarDataKey(entry.getKey());
//        pw.println("      Type=" + (fieldType == null ? entry.getKey() : fieldType) + " Offset=" + entry.getValue());
//        }
//    }
//
//    pw.println("END: VarMeta");
//    pw.println();
//
//    pw.close();
//    return (sw.toString());
//}