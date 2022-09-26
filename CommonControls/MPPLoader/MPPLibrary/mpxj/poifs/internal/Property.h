#pragma once

#include "Child.h"
#include "ByteField.h"
#include "IntegerField.h"
#include "ShortField.h"
#include "ClassID.h"

/**
 * This abstract base class is the ancestor of all classes
 * implementing POIFS Property behavior.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class Property : public Child
{
protected:
	Property();

    /**
     * Constructor from BYTE data
     *
     * @param index index number
     * @param array BYTE data
     * @param offset offset into BYTE data
     */
	Property(int index, ByteArrayPtr array1, int offset);

public:
    /**
     * Write the raw data to an OutputStream.
     *
     * @param stream the OutputStream to which the data should be
     *               written.
     *
     * @exception IOException on problems writing to the specified
     *            stream.
     */
	void writeData(BwProj::toolkit::IDataWriteStream* stream);

    /**
     * Set the start block for the document referred to by this
     * Property.
     *
     * @param startBlock the start block index
     */
	void setStartBlock(int startBlock);

    /**
     * @return the start block
     */
	int getStartBlock();

    /**
     * find out the document size
     *
     * @return size in bytes
     */
	int getSize();

    /**
     * Based on the currently defined size, should this property use
     * small blocks?
     *
     * @return true if the size is less than _big_block_minimum_bytes
     */
	bool shouldUseSmallBlocks();

    /**
     * does the length indicate a small document?
     *
     * @param length length in bytes
     *
     * @return true if the length is less than
     *         _big_block_minimum_bytes
     */
	static bool isSmall(int length);

    /**
     * Get the name of this property
     *
     * @return property name as CString
     */
	CString getName();

    /**
     * @return true if a directory type Property
     */
    virtual bool isDirectory() = 0;

    /**
     * Sets the storage clsid, which is the Class ID of a COM object which
     *   reads and writes this stream
     * @return storage Class ID for this property stream
     */
	ClassIDPtr getStorageClsid();

    /**
     * Sets the storage class ID for this property stream. This is the Class ID
     *   of the COM object which can read and write this property stream
     * @param clsidStorage Storage Class ID
     */
	void setStorageClsid(ClassIDPtr clsidStorage);

	/**
	* Get the next ChildPtr, if any
	*
	* @return the next ChildPtr; may return null
	*/
	virtual ChildPtr getNextChild();

	/**
	* Get the previous ChildPtr, if any
	*
	* @return the previous ChildPtr; may return null
	*/
	virtual ChildPtr getPreviousChild();

	/**
	* Set the next ChildPtr
	*
	* @param child the new 'next' child; may be null, which has the
	*              effect of saying there is no 'next' child
	*/
	virtual void setNextChild(ChildPtr child);

	/**
	* Set the previous ChildPtr
	*
	* @param child the new 'previous' child; may be null, which has
	*              the effect of saying there is no 'previous' child
	*/
	virtual void setPreviousChild(ChildPtr child);

	/**
	* Set the name; silently truncates the name if it's too long.
	*
	* @param name the new name
	*/
	void setName(CString name);

	/**
	* get the index for this Property
	*
	* @return the index of this Property within its Property Table
	*/
	int getIndex();

	/**
	* Get the child property (its index in the Property Table)
	*
	* @return child property index
	*/
	int getChildIndex();

	/**
	* determine whether the specified index is valid
	*
	* @param index value to be checked
	*
	* @return true if the index is valid
	*/
	static bool isValidIndex(int index);

	/**
	* get the previous sibling
	*
	* @return index of previous sibling
	*/
	int getPreviousChildIndex();

	/**
	* get the next sibling
	*
	* @return index of next sibling
	*/
	int getNextChildIndex();

	/**
	* Set the index for this Property
	*
	* @param index this Property's index within its containing
	*              Property Table
	*/
	void setIndex(int index);

	/**
	* Perform whatever activities need to be performed prior to
	* writing
	*/
	virtual void preWrite() = 0;

protected:

    /**
     * Set the property type. Makes no attempt to validate the value.
     *
     * @param propertyType the property type (root, file, directory)
     */
	void setPropertyType(BYTE propertyType);

    /**
     * Set the node color.
     *
     * @param nodeColor the node color (red or black)
     */
	void setNodeColor(BYTE nodeColor);

    /**
     * Set the child property.
     *
     * @param child the child property's index in the Property Table
     */
	void setChildProperty(int child);

    /**
     * Set the size of the document associated with this Property
     *
     * @param size the size of the document, in bytes
     */
	void setSize(int size);

protected:
	static const int  _NO_INDEX = -1;
	// node colors
	static const BYTE _NODE_BLACK = 1;
	static const BYTE _NODE_RED = 0;

private:
	static const BYTE   _default_fill = 0;
	static const int    _name_size_offset = 0x40;
	static const int    _max_name_length;
	

	// useful offsets
	static const int    _node_color_offset = 0x43;
	static const int    _previous_property_offset = 0x44;
	static const int    _next_property_offset = 0x48;
	static const int    _child_property_offset = 0x4C;
	static const int    _storage_clsid_offset = 0x50;
	static const int    _user_flags_offset = 0x60;
	static const int    _seconds_1_offset = 0x64;
	static const int    _days_1_offset = 0x68;
	static const int    _seconds_2_offset = 0x6C;
	static const int    _days_2_offset = 0x70;
	static const int    _start_block_offset = 0x74;
	static const int    _size_offset = 0x78;

	// documents must be at least this size to be stored in big blocks
	static const int    _big_block_minimum_bytes;
	CString              _name;
	int                 _index;
	std::weak_ptr<Child>	_next_child;
	std::weak_ptr<Child>	_previous_child;

	// row data
	ShortField          _name_size;
	ByteField           _property_type;
	ByteField           _node_color;
	IntegerField        _previous_property;
	IntegerField        _next_property;
	IntegerField        _child_property;
	ClassIDPtr             _storage_clsid;
	IntegerField        _user_flags;
	IntegerField        _seconds_1;
	IntegerField        _days_1;
	IntegerField        _seconds_2;
	IntegerField        _days_2;
	IntegerField        _start_block;
	IntegerField        _size;
	ByteArrayPtr        _raw_data;
};

typedef std::shared_ptr<Property> PropertyPtr;
typedef Array2<PropertyPtr> PropertyArray;
typedef std::shared_ptr<PropertyArray> PropertyArrayPtr;

typedef List<PropertyPtr> PropertyList;
typedef std::shared_ptr<PropertyList> PropertyListPtr;