#pragma once

#include "MPPComponent.h"
#include "VarMeta.h"

#include "..\FieldType.h"

/**
 * This class reads in the data from a VarMeta block. This block contains
 * meta data about variable length data items stored in a Var2Data block.
 * The meta data allows the size of the Var2Data block to be determined,
 * along with the number of data items it contains, identifiers for each item,
 * and finally the offset of each item within the block.
 */
class AbstractVarMeta : public MPPComponent, public VarMeta
{
public:
   /**
    * This method retrieves the number of items in the Var2Data block.
    *
    * @return number of items
    */
	virtual int getItemCount();

   /**
    * This method retrieves the size of the Var2Data block.
    *
    * @return data size
    */
	virtual int getDataSize();

   /**
    * This method returns an array containing all of the unique identifiers
    * for which data has been stored in the Var2Data block.
    *
    * @return array of unique identifiers
    */
	virtual IntArray2Ptr getUniqueIdentifierArray();

   /**
    * This method returns an set containing all of the unique identifiers
    * for which data has been stored in the Var2Data block.
    *
    * @return set of unique identifiers
    */
	virtual std::shared_ptr<Set<int>> getUniqueIdentifierSet();

   /**
    * This method retrieves the offset of a given entry in the Var2Data block.
    * Each entry can be uniquely located by the identifier of the object to
    * which the data belongs, and the type of the data.
    *
    * @param id unique identifier of an entity
    * @param type data type identifier
    * @return offset of requested item
    */
	virtual bool getOffset(int id, int type, int& result);

   /**
    * This method retrieves the offset of the data item at
    * the position defined by the index parameter.
    *
    * @param index index of item in the block
    * @return offset of the item in the block
    */
	virtual int getOffset(int index);

   /**
    * {@inheritDoc}
    */
	virtual std::shared_ptr<Set<int>> getTypes(int id);

   /**
    * {@inheritDoc}
    */
	virtual bool containsKey(int key);

 //  /**
 //   * This method dumps the contents of this VarMeta block as a String.
 //   * Note that this facility is provided as a debugging aid.
 //   *
 //   * @return formatted contents of this block
 //   */
	//virtual CString toString();

 //  /**
 //   * This method dumps the contents of this VarMeta block as a String.
 //   * Note that this facility is provided as a debugging aid.
 //   *
 //   * @param fieldMap field map used to decode var data keys
 //   * @return formatted contents of this block
 //   */
	//virtual CString toString(FieldMap* fieldMap);

protected:
   //protected int m_unknown1;
   int m_itemCount;
   //protected int m_unknown2;
   //protected int m_unknown3;
   int m_dataSize;
   IntArray2Ptr m_offsets;
   TreeMap<int, std::shared_ptr<Map2<int, int>>> m_table;// = new TreeMap<Integer, Map<Integer, Integer>>();
};