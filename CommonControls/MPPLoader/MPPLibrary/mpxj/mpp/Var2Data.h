#pragma once

#include "MPPComponent.h"
#include "VarMeta.h"

/**
 * This class represents a block of variable data. Each block of
 * data is represented by a 4 byte size, followed by the data itself.
 * Each Var2Data block should be associated with a MetaData block
 * which describes the layout of the data in the Var2Data block.
 */
class Var2Data : public MPPComponent
{
public:
   /**
    * Constructor. Extracts the content of the data block, with reference
    * to the meta data held in the VarMetaPtr block.
    *
    * @param meta meta data for this block
    * @param is BwProj::toolkit::IDataReadStream* from which data is read
    * @throws IOException on file read error
    */
	Var2Data(VarMetaPtr meta, BwProj::toolkit::IDataReadStream* is);

   /**
    * This method retrieves a byte array containing the data at the
    * given offset in the block. If no data is found at the given offset
    * this method returns NULL.
    *
    * @param offset offset of required data
    * @return byte array containing required data
    */
	ByteArrayPtr getByteArray2(int offset);

   /**
    * This method retrieves a byte array of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return byte array containing required data
    */
	ByteArrayPtr getByteArray2(int id, int type);

   /**
    * This method retrieves the data at the given offset and returns
    * it as a CString, assuming the underlying data is composed of
    * two byte characters.
    *
    * @param offset offset of required data
    * @return string containing required data
    */
	StringPtr getUnicodeString(int offset);

   /**
    * This method retrieves a CString of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return string containing required data
    */
	StringPtr getUnicodeString(int id, int type);

   /**
    * This method retrieves a timestamp of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required timestamp data
    */
	DatePtr getTimestamp(int id, int type);

   /**
    * This method retrieves the data at the given offset and returns
    * it as a CString, assuming the underlying data is composed of
    * single byte characters.
    *
    * @param offset offset of required data
    * @return string containing required data
    */
	CString getString(int offset);

   /**
    * This method retrieves a string of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required string data
    */
	CString getString(int id, int type);

   /**
    * This method retrieves an integer of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required integer data
    */
	int getShort(int id, int type);

   /**
    * This method retrieves an integer of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required integer data
    */
	int getByte(int id, int type);

   /**
    * This method retrieves an integer of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required integer data
    */
	int getInt(int id, int type);

   /**
    * This method retrieves an integer of the specified type,
    * belonging to the item with the specified unique ID. Note that
    * the integer value is read from an arbitrary offset within the 
    * byte array of data.
    *
    * @param id unique ID of entity to which this data belongs
    * @param offset offset into the byte array fom which to read the integer
    * @param type data type identifier
    * @return required integer data
    */
	int getInt(int id, int offset, int type);

   /**
    * This method retrieves an integer of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required integer data
    */
	__int64 getLong(int id, int type);

   /**
    * This method retrieves a double of the specified type,
    * belonging to the item with the specified unique ID.
    *
    * @param id unique ID of entity to which this data belongs
    * @param type data type identifier
    * @return required double data
    */
	double getDouble(int id, int type);

   /**
    * Retrieve the underlying meta data. This method is provided
    * mainly as a convenience for debugging.
    * 
    * @return VarMetaPtr instance
    */
	VarMetaPtr getVarMeta();

 //  /**
 //   * This method dumps the contents of this Var2Data block as a CString.
 //   * Note that this facility is provided as a debugging aid.
 //   *
 //   * @return formatted contents of this block
 //   */
	//virtual CString toString();

 //  /**
 //   * This is a specialised version of the toString method which 
 //   * outputs just the data in this structure for the given unique ID. 
 //   * 
 //   * @param id unique ID
 //   * @return string representation
 //   */
	//CString toString(int id);

private:
   /**
    * Map containing data items indexed by offset.
    */
   Map2<int, ByteArrayPtr> m_map;// = new TreeMap<int, ByteArrayPtr>();

   /**
    * Reference to the meta data associated with this block.
    */
   VarMetaPtr m_meta;
};

typedef std::shared_ptr<Var2Data> Var2DataPtr;