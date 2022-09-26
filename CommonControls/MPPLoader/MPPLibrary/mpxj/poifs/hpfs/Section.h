#pragma once

#include "Property2.h"
#include "..\internal\ClassID.h"

/**
 * <p>Represents a section in a {@link PropertySet}.</p>
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 * @author Drew Varner (Drew.Varner allUpIn sc.edu)
 */
class Section
{
public:
//	/**
//     * <p>Returns the format ID. The format ID is the "type" of the
//     * section. For example, if the format ID of the first {@link
//     * Section} contains the bytes specified by
//     * <code>org.apache.poi.hpsf.wellknown.SectionIDMap.SUMMARY_INFORMATION_ID</code>
//     * the section (and thus the property set) is a SummaryInformation.</p>
//     *
//     * @return The format ID
//     */
//	ClassIDPtr getFormatID();
//
//
//    /**
//     * <p>Returns the offset of the section in the stream.</p>
//     *
//     * @return The offset of the section in the stream.
//     */
//	long getOffset();
//
//    /**
//     * <p>Returns the section's size in bytes.</p>
//     *
//     * @return The section's size in bytes.
//     */
//	int getSize();
//
//    /**
//     * <p>Returns the number of properties in this section.</p>
//     *
//     * @return The number of properties in this section.
//     */
//	int getPropertyCount();
//
//    /**
//     * <p>Returns this section's properties.</p>
//     *
//     * @return This section's properties.
//     */
//	Property2ArrayPtr getProperties();
//
//    /**
//     * <p>Creates a {@link Section} instance from a byte array.</p>
//     *
//     * @param src Contains the complete property set stream.
//     * @param offset The position in the stream that points to the
//     * section's format ID.
//     *
//     * @exception UnsupportedEncodingException if the section's codepage is not
//     * supported.
//     */
	Section(ByteArrayPtr src, int offset);
//
//    /**
//     * <p>Returns the value of the property with the specified ID. If
//     * the property is not available, <code>NULL</code> is returned
//     * and a subsequent call to {@link #wasNull} will return
//     * <code>true</code>.</p>
//     *
//     * @param id The property's ID
//     *
//     * @return The property's value
//     */
	ObjectPtr getProperty(long id);
//
//    /**
//     * <p>Checks whether the property which the last call to {@link
//     * #getPropertyIntValue} or {@link #getProperty} tried to access
//     * was available or not. This information might be important for
//     * callers of {@link #getPropertyIntValue} since the latter
//     * returns 0 if the property does not exist. Using {@link
//     * #wasNull} the caller can distiguish this case from a property's
//     * real value of 0.</p>
//     *
//     * @return <code>true</code> if the last call to {@link
//     * #getPropertyIntValue} or {@link #getProperty} tried to access a
//     * property that was not available, else <code>false</code>.
//     */
//	bool wasNull();
//
//
//    /**
//     * <p>Returns the PID string associated with a property ID. The ID
//     * is first looked up in the {@link Section}'s private
//     * dictionary. If it is not found there, the method calls {@link
//     * SectionIDMap#getPIDString}.</p>
//     *
//     * @param pid The property ID
//     *
//     * @return The property ID's string value
//     */
//	CString getPIDString(long pid);
//
// //   /**
// //    * <p>Checks whether this section is equal to another object. The result is
// //    * <code>false</code> if one of the the following conditions holds:</p>
// //    *
// //    * <ul>
// //    *
// //    * <li><p>The other object is not a {@link Section}.</p></li>
// //    *
// //    * <li><p>The format IDs of the two sections are not equal.</p></li>
// //    *
// //    * <li><p>The sections have a different number of properties. However,
// //    * properties with ID 1 (codepage) are not counted.</p></li>
// //    *
// //    * <li><p>The other object is not a {@link Section}.</p></li>
// //    *
// //    * <li><p>The properties have different values. The order of the properties
// //    * is irrelevant.</p></li>
// //    *
// //    * </ul>
// //    *
// //    * @param o The object to compare this section with
// //    * @return <code>true</code> if the objects are equal, <code>false</code> if
// //    * not
// //    */
//	//bool equals(Object* o);
//
// //   /**
// //    * @see Object#hashCode()
// //    */
//	//int hashCode();
//
// //   /**
// //    * @see Object#toString()
// //    */
//	//CString toString();
//
//    /**
//     * <p>Gets the section's dictionary. A dictionary allows an application to
//     * use human-readable property names instead of numeric property IDs. It
//     * contains mappings from property IDs to their associated string
//     * values. The dictionary is stored as the property with ID 0. The codepage
//     * for the strings in the dictionary is defined by property with ID 1.</p>
//     *
//     * @return the dictionary or <code>NULL</code> if the section does not have
//     * a dictionary.
//     */
//	std::shared_ptr<Map<long,CString>> getDictionary();
//
//    /**
//     * <p>Gets the section's codepage, if any.</p>
//     *
//     * @return The section's codepage if one is defined, else -1.
//     */
//	int getCodepage();
//
//public:
//	/**
//	* <p>Creates an empty and uninitialized {@link Section}.
//	*/
//	Section();
//
//	/**
//	* <p>Returns the value of the numeric property with the specified
//	* ID. If the property is not available, 0 is returned. A
//	* subsequent call to {@link #wasNull} will return
//	* <code>true</code> to let the caller distinguish that case from
//	* a real property value of 0.</p>
//	*
//	* @param id The property's ID
//	*
//	* @return The property's value
//	*/
	int getPropertyIntValue(long id);
//
//	/**
//	* <p>Returns the value of the bool property with the specified
//	* ID. If the property is not available, <code>false</code> is
//	* returned. A subsequent call to {@link #wasNull} will return
//	* <code>true</code> to let the caller distinguish that case from
//	* a real property value of <code>false</code>.</p>
//	*
//	* @param id The property's ID
//	*
//	* @return The property's value
//	*/
//	bool getPropertyBooleanValue(int id);

//private:
//	/**
//	* <p>Removes a field from a property array. The resulting array is
//	* compactified and returned.</p>
//	*
//	* @param pa The property array.
//	* @param i The index of the field to be removed.
//	* @return the compactified array.
//	*/
//	Property2ArrayPtr remove(Property2ArrayPtr pa, int i);

protected:
	/**
	* <p>Maps property IDs to section-private PID strings. These
	* strings can be found in the property with ID 0.</p>
	*/
	std::shared_ptr<Map2<long,CString>> _dictionary;

	/**
	* <p>The section's format ID, {@link #getFormatID}.</p>
	*/
	ClassIDPtr _formatID;

	/**
	* @see #getOffset
	*/
	long _offset;

	/**
	* @see #getSize
	*/
	int _size;

	/**
	* @see #getProperties
	*/
	Property2ArrayPtr _properties;

private:
	/**
	* <p>This member is <code>true</code> if the last call to {@link
	* #getPropertyIntValue} or {@link #getProperty} tried to access a
	* property that was not available, else <code>false</code>.</p>
	*/
	bool _wasNull;
};

typedef std::shared_ptr<Section> SectionPtr;
typedef std::shared_ptr<List<SectionPtr>> SectionListPtr;