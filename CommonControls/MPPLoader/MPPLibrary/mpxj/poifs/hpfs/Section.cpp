#include "stdafx.h"
#include "Section.h"
#include "..\internal\LittleEndian.h"
#include "..\..\common\NumberHelper.h"
#include "PropertyIDMap.h"

///**
//    * <p>Returns the format ID. The format ID is the "type" of the
//    * section. For example, if the format ID of the first {@link
//    * Section} contains the bytes specified by
//    * <code>org.apache.poi.hpsf.wellknown.SectionIDMap.SUMMARY_INFORMATION_ID</code>
//    * the section (and thus the property set) is a SummaryInformation.</p>
//    *
//    * @return The format ID
//    */
//ClassIDPtr Section::getFormatID()
//{
//    return _formatID;
//}
//
//
///**
//    * <p>Returns the offset of the section in the stream.</p>
//    *
//    * @return The offset of the section in the stream.
//    */
//long Section::getOffset()
//{
//    return _offset;
//}
//
///**
//    * <p>Returns the section's size in bytes.</p>
//    *
//    * @return The section's size in bytes.
//    */
//int Section::getSize()
//{
//    return _size;
//}
//
///**
//    * <p>Returns the number of _properties in this section.</p>
//    *
//    * @return The number of _properties in this section.
//    */
//int Section::getPropertyCount()
//{
//    return _properties->length();
//}
//
///**
//    * <p>Returns this section's _properties.</p>
//    *
//    * @return This section's _properties.
//    */
//Property2ArrayPtr Section::getProperties()
//{
//    return _properties;
//}
//
///**
//    * <p>Creates an empty and uninitialized {@link Section}.
//    */
//Section::Section()
//{ }

/**
* <p>Represents an entry in the property list and holds a property's ID and
* its offset from the section's beginning.</p>
*/
class PropertyListEntry : public ComparableT<PropertyListEntry>
{
public:
	/**
	* <p>Compares this {@link PropertyListEntry} with another one by their
	* offsets. A {@link PropertyListEntry} is "smaller" than another one if
	* its offset from the section's begin is smaller.</p>
	*
	* @see Comparable#compareTo(java.lang.Object)
	*/
	int compareTo(PropertyListEntry* o)
	{
		int otherOffset = o->offset;
		if (offset < otherOffset)
			return -1;
		else if (offset == otherOffset)
return 0;
		else
			return 1;
	}


	int id;
	int offset;
	int length;
};

typedef std::shared_ptr<PropertyListEntry> PropertyListEntryPtr;

///**
//    * <p>Creates a {@link Section} instance from a byte array.</p>
//    *
//    * @param src Contains the complete property set stream.
//    * @param offset The position in the stream that points to the
//    * section's format ID.
//    *
//    * @exception UnsupportedEncodingException if the section's codepage is not
//    * supported.
//    */
Section::Section(ByteArrayPtr src, int offset)
{
	int o1 = offset;

	/*
		* Read the format ID.
		*/
	_formatID.reset(new ClassID(src, o1));
	o1 += ClassID::LENGTH;

	/*
		* Read the offset from the stream's start and positions to
		* the section header.
		*/
	this->_offset = LittleEndian::getUInt(src, o1);
	o1 = (int) this->_offset;

	/*
		* Read the section length.
		*/
	_size = (int)LittleEndian::getUInt(src, o1);
	o1 += LittleEndian::INT_SIZE;

	/*
		* Read the number of _properties.
		*/
	int propertyCount = (int)LittleEndian::getUInt(src, o1);
	o1 += LittleEndian::INT_SIZE;

	/*
		* Read the _properties. The offset is positioned at the first
		* entry of the property list. There are two problems:
		*
		* 1. For each property we have to find out its length. In the
		*    property list we find each property's ID and its offset relative
		*    to the section's beginning. Unfortunately the _properties in the
		*    property list need not to be in ascending order, so it is not
		*    possible to calculate the length as
		*    (offset of property(i+1) - offset of property(i)). Before we can
		*    that we first have to sort the property list by ascending offsets.
		*
		* 2. We have to read the property with ID 1 before we read other
		*    _properties, at least before other _properties containing strings.
		*    The reason is that property 1 specifies the codepage. If it is
		*    1200, all strings are in Unicode. In other words: Before we can
		*    read any strings we have to know whether they are in Unicode or
		*    not. Unfortunately property 1 is not guaranteed to be the first in
		*    a section.
		*
		*    The algorithm below reads the _properties in two passes: The first
		*    one looks for property ID 1 and extracts the codepage number. The
		*    seconds pass reads the other _properties.
		*/
	_properties.reset(new Property2Array(propertyCount));

	/* Pass 1: Read the property list. */
	int pass1Offset = o1;
	List<PropertyListEntryPtr> propertyList;// = new ArrayList<PropertyListEntry>(propertyCount);
	PropertyListEntryPtr ple;
	for (size_t i = 0; i < _properties->length(); i++)
	{
		ple.reset(new PropertyListEntry());

		/* Read the property ID. */
		ple->id = (int)LittleEndian::getUInt(src, pass1Offset);
		pass1Offset += LittleEndian::INT_SIZE;

		/* Offset from the section's start. */
		ple->offset = (int)LittleEndian::getUInt(src, pass1Offset);
		pass1Offset += LittleEndian::INT_SIZE;

		int insertIndex = propertyList.GetCount();
		// 查找到比现有id大的索引号再插入（排序）
		for (int j = propertyList.GetCount() - 1; j >= 0; j--)
		{
			PropertyListEntryPtr ple1 = propertyList.GetAt(j);
			if (ple1->id < ple->id)
			{
				insertIndex = j;
				break;
			}
		}

		propertyList.InsertAt(propertyList.GetCount() == 0 ? 0 : insertIndex + 1, ple);

        /* Add the entry to the property list. */
        //propertyList.Add(ple);
    }

    /* Sort the property list by ascending offsets: */
    Collections::sort_object_list(propertyList);

    /* Calculate the _properties' lengths. */
    for (int i = 0; i < propertyCount - 1; i++)
    {
        PropertyListEntryPtr ple1 = propertyList.GetAt(i);
		PropertyListEntryPtr ple2 = propertyList.GetAt(i + 1);
		ple1->length = ple2->offset - ple1->offset;
    }
    if (propertyCount > 0)
    {
		ple = propertyList.GetAt(propertyCount - 1);
        ple->length = _size - ple->offset;
    }

    /* Look for the codepage. */
    int codepage = -1;
    for (Iterator<PropertyListEntryPtr> i = propertyList.iterator();
            codepage == -1 && i.hasNext();)
    {
        ple = i.next();

        /* Read the codepage if the property ID is 1. */
        if (ple->id == PropertyIDMap::PID_CODEPAGE1)
        {
            /* Read the property's value type. It must be
                * VT_I2. */
            int o = (int) (this->_offset + ple->offset);
            long type = LittleEndian::getUInt(src, o);
            o += LittleEndian::INT_SIZE;

            //if (type != Variant::VT_I2)
            //    throw new HPSFRuntimeException
            //        ("Value type of property ID 1 is not VT_I2 but " +
            //            type + ".");

            /* Read the codepage number. */
            codepage = LittleEndian::getUShort(src, o);
        }
    }

    /* Pass 2: Read all _properties - including the codepage property,
        * if available. */
    int i1 = 0;
    for (Iterator<PropertyListEntryPtr> i = propertyList.iterator(); i.hasNext();)
    {
        ple = i.next();
        Property2Ptr p (new Property2(ple->id, src, this->_offset + ple->offset, ple->length, codepage));
		if (p->getID() == PropertyIDMap::PID_CODEPAGE1)
		{
			p.reset(new Property2(p->getID(), p->getType(), NumberPtr(new Number(codepage))));
		}
        (*_properties)[i1++] = p;
    }

    /*
        * Extract the dictionary (if available).
        */
	_dictionary = std::dynamic_pointer_cast<Map2<long, CString>> (getProperty(0));
}

///**
//    * <p>Returns the value of the property with the specified ID. If
//    * the property is not available, <code>NULL</code> is returned
//    * and a subsequent call to {@link #wasNull} will return
//    * <code>true</code>.</p>
//    *
//    * @param id The property's ID
//    *
//    * @return The property's value
//    */
ObjectPtr Section::getProperty(long id)
{
    _wasNull = false;
    for (size_t i = 0; i < _properties->length(); i++)
        if (id == (*_properties)[i]->getID())
            return (*_properties)[i]->getValue();
    _wasNull = true;
    return NULL;
}
//
///**
//    * <p>Returns the value of the numeric property with the specified
//    * ID. If the property is not available, 0 is returned. A
//    * subsequent call to {@link #wasNull} will return
//    * <code>true</code> to let the caller distinguish that case from
//    * a real property value of 0.</p>
//    *
//    * @param id The property's ID
//    *
//    * @return The property's value
//    */
int Section::getPropertyIntValue(long id)
{
    NumberPtr i;
    ObjectPtr o = getProperty(id);
    if (!o)
        return 0;

	return NumberHelper::getInt(o);

    //if (!(o instanceof Long || o instanceof Integer))
    //    throw new HPSFRuntimeException
    //        ("This property is not an integer type, but " +
    //            o.getClass().getName() + ".");
    //i = (Number) o;
    //return i.intValue();
}
//
//
//
///**
//    * <p>Returns the value of the bool property with the specified
//    * ID. If the property is not available, <code>false</code> is
//    * returned. A subsequent call to {@link #wasNull} will return
//    * <code>true</code> to let the caller distinguish that case from
//    * a real property value of <code>false</code>.</p>
//    *
//    * @param id The property's ID
//    *
//    * @return The property's value
//    */
//bool Section::getPropertyBooleanValue(int id)
//{
//	return NumberHelper::getBool(getProperty(id));
//    //Boolean b = (Boolean) getProperty(id);
//    //if (b == NULL) {
//    //    return false;
//    //}
//    //return b.booleanValue();
//}
//
//
///**
//    * <p>Checks whether the property which the last call to {@link
//    * #getPropertyIntValue} or {@link #getProperty} tried to access
//    * was available or not. This information might be important for
//    * callers of {@link #getPropertyIntValue} since the latter
//    * returns 0 if the property does not exist. Using {@link
//    * #wasNull} the caller can distiguish this case from a property's
//    * real value of 0.</p>
//    *
//    * @return <code>true</code> if the last call to {@link
//    * #getPropertyIntValue} or {@link #getProperty} tried to access a
//    * property that was not available, else <code>false</code>.
//    */
//bool Section::wasNull()
//{
//    return _wasNull;
//}
//
//
//
///**
//    * <p>Returns the PID string associated with a property ID. The ID
//    * is first looked up in the {@link Section}'s private
//    * dictionary. If it is not found there, the method calls {@link
//    * SectionIDMap#getPIDString}.</p>
//    *
//    * @param pid The property ID
//    *
//    * @return The property ID's string value
//    */
//CString Section::getPIDString(long pid)
//{
//    CString s;
//	if (_dictionary && _dictionary->Lookup(pid, s))
//		return s;
//
//	ATLASSERT(FALSE);
//	//s = SectionIDMap::getPIDString(getFormatID().getBytes(), pid);
//
// //   if (s == NULL)
// //       s = SectionIDMap::UNDEFINED;
//
//    return s;
//}
//
//
//
///**
//    * <p>Checks whether this section is equal to another object. The result is
//    * <code>false</code> if one of the the following conditions holds:</p>
//    *
//    * <ul>
//    *
//    * <li><p>The other object is not a {@link Section}.</p></li>
//    *
//    * <li><p>The format IDs of the two sections are not equal.</p></li>
//    *
//    * <li><p>The sections have a different number of _properties. However,
//    * _properties with ID 1 (codepage) are not counted.</p></li>
//    *
//    * <li><p>The other object is not a {@link Section}.</p></li>
//    *
//    * <li><p>The _properties have different values. The order of the _properties
//    * is irrelevant.</p></li>
//    *
//    * </ul>
//    *
//    * @param o The object to compare this section with
//    * @return <code>true</code> if the objects are equal, <code>false</code> if
//    * not
//    */
////bool Section::equals(Object* o)
////{
////	ATLASSERT(FALSE);
////	return false;
////    //if (o == NULL || !(o instanceof Section))
////    //    return false;
////    //Section s = (Section) o;
////    //if (!s.getFormatID().equals(getFormatID()))
////    //    return false;
////
////    ///* Compare all _properties except 0 and 1 as they must be handled
////    //    * specially. */
////    //Property2ArrayPtr pa1 = new Property[getProperties().length];
////    //Property2ArrayPtr pa2 = new Property[s.getProperties().length];
////    //System.arraycopy(getProperties(), 0, pa1, 0, pa1.length);
////    //System.arraycopy(s.getProperties(), 0, pa2, 0, pa2.length);
////
////    ///* Extract _properties 0 and 1 and remove them from the copy of the
////    //    * arrays. */
////    //Property p10 = NULL;
////    //Property p20 = NULL;
////    //for (int i = 0; i < pa1.length; i++)
////    //{
////    //    long id = pa1[i].getID();
////    //    if (id == 0)
////    //    {
////    //        p10 = pa1[i];
////    //        pa1 = remove(pa1, i);
////    //        i--;
////    //    }
////    //    if (id == 1)
////    //    {
////    //        // p11 = pa1[i];
////    //        pa1 = remove(pa1, i);
////    //        i--;
////    //    }
////    //}
////    //for (int i = 0; i < pa2.length; i++)
////    //{
////    //    long id = pa2[i].getID();
////    //    if (id == 0)
////    //    {
////    //        p20 = pa2[i];
////    //        pa2 = remove(pa2, i);
////    //        i--;
////    //    }
////    //    if (id == 1)
////    //    {
////    //        // p21 = pa2[i];
////    //        pa2 = remove(pa2, i);
////    //        i--;
////    //    }
////    //}
////
////    ///* If the number of _properties (not counting property 1) is unequal the
////    //    * sections are unequal. */
////    //if (pa1.length != pa2.length)
////    //    return false;
////
////    ///* If the dictionaries are unequal the sections are unequal. */
////    //bool dictionaryEqual = true;
////    //if (p10 != NULL && p20 != NULL)
////    //    dictionaryEqual = p10.getValue().equals(p20.getValue());
////    //else if (p10 != NULL || p20 != NULL)
////    //    dictionaryEqual = false;
////    //if (dictionaryEqual) {
////    //    return Util.equals(pa1, pa2);
////    //}
////    //return false;
////}
//
//
////
/////**
////    * <p>Removes a field from a property array. The resulting array is
////    * compactified and returned.</p>
////    *
////    * @param pa The property array.
////    * @param i The index of the field to be removed.
////    * @return the compactified array.
////    */
////Property2ArrayPtr Section::remove(Property2ArrayPtr pa, int i)
////{
////    Property2ArrayPtr h = new Property[pa.length - 1];
////    if (i > 0)
////        System.arraycopy(pa, 0, h, 0, i);
////    System.arraycopy(pa, i + 1, h, i, h.length - i);
////    return h;
////}
//
//
//
/////**
////    * @see Object#hashCode()
////    */
////int Section::hashCode()
////{
////    long hashCode = 0;
////    hashCode += getFormatID().hashCode();
////    Property2ArrayPtr pa = getProperties();
////    for (int i = 0; i < pa.length; i++)
////        hashCode += pa[i].hashCode();
////    int returnHashCode = (int) (hashCode & 0x0ffffffffL);
////    return returnHashCode;
////}
////
////
////
/////**
////    * @see Object#toString()
////    */
////CString Section::toString()
////{
////    StringBuffer b = new StringBuffer();
////    Property2ArrayPtr pa = getProperties();
////    b.append(getClass().getName());
////    b.append('[');
////    b.append("formatID: ");
////    b.append(getFormatID());
////    b.append(", offset: ");
////    b.append(getOffset());
////    b.append(", propertyCount: ");
////    b.append(getPropertyCount());
////    b.append(", size: ");
////    b.append(getSize());
////    b.append(", _properties: [\n");
////    for (int i = 0; i < pa.length; i++)
////    {
////        b.append(pa[i].toString());
////        b.append(",\n");
////    }
////    b.append(']');
////    b.append(']');
////    return b.toString();
////}
//
//
//
///**
//    * <p>Gets the section's dictionary. A dictionary allows an application to
//    * use human-readable property names instead of numeric property IDs. It
//    * contains mappings from property IDs to their associated string
//    * values. The dictionary is stored as the property with ID 0. The codepage
//    * for the strings in the dictionary is defined by property with ID 1.</p>
//    *
//    * @return the dictionary or <code>NULL</code> if the section does not have
//    * a dictionary.
//    */
//std::shared_ptr<Map<long,CString>> Section::getDictionary()
//{
//    return _dictionary;
//}
//
///**
//    * <p>Gets the section's codepage, if any.</p>
//    *
//    * @return The section's codepage if one is defined, else -1.
//    */
//int Section::getCodepage()
//{
//    ObjectPtr codepage = getProperty(PropertyIDMap::PID_CODEPAGE1);
//    if (codepage == NULL)
//        return -1;
//	int cp = NumberHelper::getInt(codepage);// .intValue();
//    return cp;
//}