#include "stdafx.h"
#include "Property.h"
#include "POIFSConstants.h"
#include "LittleEndianConsts.h"
#include "PropertyConstants.h"

const int Property::_max_name_length = (_name_size_offset / LittleEndianConsts::SHORT_SIZE) - 1;
const int Property::_big_block_minimum_bytes = POIFSConstants::BIG_BLOCK_MINIMUM_DOCUMENT_SIZE;

Property::Property()
:
	_name_size(_name_size_offset),
	_property_type(PropertyConstants::PROPERTY_TYPE_OFFSET),
	_node_color(_node_color_offset),
	_previous_property(_previous_property_offset),
	_next_property(_next_property_offset),
	_child_property(_child_property_offset),
	_user_flags(_user_flags_offset),
	_seconds_1(_seconds_1_offset),
	_days_1(_days_1_offset),
	_seconds_2(_seconds_2_offset),
	_days_2(_days_2_offset),
	_start_block(_start_block_offset),
	_size(_size_offset)
{
	_raw_data.reset(new ByteArray2(POIFSConstants::PROPERTY_SIZE));
	memset(_raw_data->get(), 0, _raw_data->length());

    _previous_property.set(_NO_INDEX, _raw_data);
	_next_property.set(_NO_INDEX, _raw_data);
	_child_property.set(_NO_INDEX, _raw_data);
	_storage_clsid.reset(new ClassID(_raw_data, _storage_clsid_offset));

	_user_flags.set(0, _raw_data);
	_seconds_1.set(0, _raw_data);
	_days_1.set(0, _raw_data);
	_seconds_2.set(0, _raw_data);
	_days_2.set(0, _raw_data);
	_size.set(0, _raw_data);

    _index	= _NO_INDEX;
    //setName("");
    //setNextChild(null);
    //setPreviousChild(null);
}

/**
    * Constructor from BYTE data
    *
    * @param index index number
    * @param array BYTE data
    * @param offset offset into BYTE data
    */
Property::Property(int index, ByteArrayPtr array1, int offset)
:	_name_size(_name_size_offset),
	_property_type(PropertyConstants::PROPERTY_TYPE_OFFSET),
	_node_color(_node_color_offset),
	_previous_property(_previous_property_offset),
	_next_property(_next_property_offset),
	_child_property(_child_property_offset),
	_user_flags(_user_flags_offset),
	_seconds_1(_seconds_1_offset),
	_days_1(_days_1_offset),
	_seconds_2(_seconds_2_offset),
	_days_2(_days_2_offset),
	_start_block(_start_block_offset),
	_size(_size_offset)
{
	_raw_data.reset(new ByteArray2(POIFSConstants::PROPERTY_SIZE));
	memcpy_s(_raw_data->get(), _raw_data->length(), &(*array1)[offset], _raw_data->length());

	_name_size.readFromBytes(_raw_data);
	_property_type.readFromBytes(_raw_data);
	_node_color.readFromBytes(_raw_data);
	_previous_property.readFromBytes(_raw_data);
	_next_property.readFromBytes(_raw_data);
	_child_property.readFromBytes(_raw_data);
	_storage_clsid.reset(new ClassID(_raw_data, _storage_clsid_offset));

	_user_flags.set(0, _raw_data);
	_seconds_1.readFromBytes(_raw_data);
	_days_1.readFromBytes(_raw_data);
	_seconds_2.readFromBytes(_raw_data);
	_days_2.readFromBytes(_raw_data);
	_start_block.readFromBytes(_raw_data);
	_size.readFromBytes(_raw_data);

    _index = index;

    int name_length = (_name_size.get() / LittleEndianConsts::SHORT_SIZE) - 1;

    if (name_length >= 1)
    {
		TCHAR* char_array = new TCHAR[name_length+1];
        int    name_offset = 0;
		memset(char_array, sizeof(TCHAR)*(name_length + 1), 0);

        for (int j = 0; j < name_length; j++)
        {
			//char_array[j] = (TCHAR) new ShortField(name_offset, _raw_data).get();
			char_array[j] = *((TCHAR*)(&(*_raw_data)[name_offset]));
            name_offset     += LittleEndianConsts::SHORT_SIZE;
        }
		char_array[name_length] = 0;
		_name = char_array;// new CString(char_array, 0, name_length);

		delete[] char_array;
    }
	else
	{
		ATLASSERT(FALSE);
	}
    //_next_child     = null;
    //_previous_child = null;
}

/**
    * Write the raw data to an OutputStream.
    *
    * @param stream the OutputStream to which the data should be
    *               written.
    *
    * @exception IOException on problems writing to the specified
    *            stream.
    */
void Property::writeData(BwProj::toolkit::IDataWriteStream* stream)
{
	stream->Write(_raw_data.get(), _raw_data->length());
    //stream.write(_raw_data);
}

/**
    * Set the start block for the document referred to by this
    * Property.
    *
    * @param startBlock the start block index
    */
void Property::setStartBlock(int startBlock)
{
    _start_block.set(startBlock, _raw_data);
}

/**
    * @return the start block
    */
int Property::getStartBlock()
{
    return _start_block.get();
}

/**
    * find out the document size
    *
    * @return size in bytes
    */
int Property::getSize()
{
    return _size.get();
}

/**
    * Based on the currently defined size, should this property use
    * small blocks?
    *
    * @return true if the size is less than _big_block_minimum_bytes
    */
bool Property::shouldUseSmallBlocks()
{
    return Property::isSmall(_size.get());
}

/**
    * does the length indicate a small document?
    *
    * @param length length in bytes
    *
    * @return true if the length is less than
    *         _big_block_minimum_bytes
    */
bool Property::isSmall(int length)
{
    return length < _big_block_minimum_bytes;
}

/**
    * Get the name of this property
    *
    * @return property name as CString
    */
CString Property::getName()
{
    return _name;
}

/**
    * Sets the storage clsid, which is the Class ID of a COM object which
    *   reads and writes this stream
    * @return storage Class ID for this property stream
    */
ClassIDPtr Property::getStorageClsid()
{
    return _storage_clsid;
}

/**
    * Set the name; silently truncates the name if it's too long.
    *
    * @param name the new name
    */
void Property::setName(CString name)
{
    //char[] char_array = name.toCharArray();
	int    limit = min(name.GetLength(), _max_name_length);
	_name = name.Left(limit);

    short offset = 0;
    int   j      = 0;

    for (; j < limit; j++)
    {
		ShortField field(offset, (short)name[j], _raw_data);
        offset += LittleEndianConsts::SHORT_SIZE;
    }
    for (; j < _max_name_length + 1; j++)
    {
        ShortField field(offset, ( short ) 0, _raw_data);
        offset += LittleEndianConsts::SHORT_SIZE;
    }

    // double the count, and include the null at the end
    _name_size.set(( short ) ((limit + 1) * LittleEndianConsts::SHORT_SIZE), _raw_data);
}

/**
    * Sets the storage class ID for this property stream. This is the Class ID
    *   of the COM object which can read and write this property stream
    * @param clsidStorage Storage Class ID
    */
void Property::setStorageClsid(ClassIDPtr clsidStorage)
{
    _storage_clsid = clsidStorage;
    if(!clsidStorage)
	{
		memset(&(*_raw_data)[_storage_clsid_offset], 0, ClassID::LENGTH);
        //Arrays.fill( _raw_data, _storage_clsid_offset, _storage_clsid_offset + ClassID::LENGTH, (BYTE) 0);
    }
	else 
	{
        clsidStorage->write( _raw_data, _storage_clsid_offset);
    }
}
/**
    * Set the property type. Makes no attempt to validate the value.
    *
    * @param propertyType the property type (root, file, directory)
    */
void Property::setPropertyType(BYTE propertyType)
{
    _property_type.set(propertyType, _raw_data);
}

/**
    * Set the node color.
    *
    * @param nodeColor the node color (red or black)
    */
void Property::setNodeColor(BYTE nodeColor)
{
    _node_color.set(nodeColor, _raw_data);
}

/**
    * Set the child property.
    *
    * @param child the child property's index in the Property Table
    */
void Property::setChildProperty(int child)
{
    _child_property.set(child, _raw_data);
}

/**
    * Get the child property (its index in the Property Table)
    *
    * @return child property index
    */
int Property::getChildIndex()
{
    return _child_property.get();
}

/**
    * Set the size of the document associated with this Property
    *
    * @param size the size of the document, in bytes
    */
void Property::setSize(int size)
{
    _size.set(size, _raw_data);
}

/**
    * Set the index for this Property
    *
    * @param index this Property's index within its containing
    *              Property Table
    */
void Property::setIndex(int index)
{
    _index = index;
}

/**
    * get the index for this Property
    *
    * @return the index of this Property within its Property Table
    */
int Property::getIndex()
{
    return _index;
}

/**
    * get the next sibling
    *
    * @return index of next sibling
    */
int Property::getNextChildIndex()
{
    return _next_property.get();
}

/**
    * get the previous sibling
    *
    * @return index of previous sibling
    */
int Property::getPreviousChildIndex()
{
    return _previous_property.get();
}

/**
    * determine whether the specified index is valid
    *
    * @param index value to be checked
    *
    * @return true if the index is valid
    */
bool Property::isValidIndex(int index)
{
    return index != _NO_INDEX;
}

/**
    * Get the next ChildPtr, if any
    *
    * @return the next ChildPtr; may return null
    */
ChildPtr Property::getNextChild()
{
    return _next_child.lock();
}

/**
    * Get the previous ChildPtr, if any
    *
    * @return the previous ChildPtr; may return null
    */
ChildPtr Property::getPreviousChild()
{
	return _previous_child.lock();
}

/**
    * Set the next ChildPtr
    *
    * @param child the new 'next' child; may be null, which has the
    *              effect of saying there is no 'next' child
    */
void Property::setNextChild(ChildPtr child)
{
    _next_child = child;
    _next_property.set((!child) ? _NO_INDEX : (std::dynamic_pointer_cast<Property>(child))->getIndex(), _raw_data);
}

/**
    * Set the previous ChildPtr
    *
    * @param child the new 'previous' child; may be null, which has
    *              the effect of saying there is no 'previous' child
    */
void Property::setPreviousChild(ChildPtr child)
{
    _previous_child = child;
	_previous_property.set((!child) ? _NO_INDEX : (std::dynamic_pointer_cast<Property>(child))->getIndex(), _raw_data);
}