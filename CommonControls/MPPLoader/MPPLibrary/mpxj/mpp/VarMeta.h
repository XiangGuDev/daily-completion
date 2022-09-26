#pragma once

class FieldMap;

/**
 * Interface implemented by VarMeta types.
 */
class VarMeta
{
public:
   /**
    * This method retrieves the number of items in the Var2Data block.
    *
    * @return number of items
    */
   virtual int getItemCount() = 0;

   /**
    * This method retrieves the size of the Var2Data block.
    *
    * @return data size
    */
   virtual int getDataSize() = 0;

   /**
    * This method returns an array containing all of the unique identifiers
    * for which data has been stored in the Var2Data block.
    *
    * @return array of unique identifiers
    */
   virtual IntArray2Ptr getUniqueIdentifierArray() = 0;

   /**
    * This method returns an set containing all of the unique identifiers
    * for which data has been stored in the Var2Data block.
    *
    * @return set of unique identifiers
    */
   virtual std::shared_ptr<Set<int>> getUniqueIdentifierSet() = 0;

   /**
    * This method retrieves the offset of a given entry in the Var2Data block.
    * Each entry can be uniquely located by the identifier of the object to
    * which the data belongs, and the type of the data.
    *
    * @param id unique identifier of an entity
    * @param type data type identifier
    * @return offset of requested item
    */
   virtual bool getOffset(int id, int type, int& result) = 0;

   /**
    * This method retrieves the offset of the data item at
    * the position defined by the index parameter.
    *
    * @param index index of item in the block
    * @return offset of the item in the block£¬Ê§°Ü-1
    */
   virtual int getOffset(int index) = 0;

   /**
    * Retrieves a set containing the types defined
    * in the var data for a given ID.
    * 
    * @param id unique ID
    * @return set of types
    */
   virtual std::shared_ptr<Set<int>> getTypes(int id) = 0;

   /**
    * This method is used to check if a given key is present.
    * 
    * @param key key to test
    * @return Boolean flag
    */
   virtual bool containsKey(int key) = 0;

   ///**
   // * This method dumps the contents of this VarMeta block as a String.
   // * Note that this facility is provided as a debugging aid.
   // *
   // * @param fieldMap field map used to decode var data keys
   // * @return formatted contents of this block
   // */
   //virtual CString toString(std::shared_ptr<FieldMap> fieldMap) = 0;
};

typedef std::shared_ptr<VarMeta> VarMetaPtr;