#pragma once

//#include "MutablePropertySet.h"
#include "PropertySet.h"
#include "PropertyIDMap.h"

/**
 * <p>Abstract superclass for the convenience classes {@link
 * SummaryInformation} and {@link DocumentSummaryInformation}.</p>
 *
 * <p>The motivation behind this class is quite nasty if you look
 * behind the scenes, but it serves the application programmer well by
 * providing him with the easy-to-use {@link SummaryInformation} and
 * {@link DocumentSummaryInformation} classes. When parsing the data a
 * property set stream consists of (possibly coming from an {@link
 * java.io.InputStream}) we want to read and process each byte only
 * once. Since we don't know in advance which kind of property set we
 * have, we can expect only the most general {@link
 * PropertySet}. Creating a special subclass should be as easy as
 * calling the special subclass' constructor and pass the general
 * {@link PropertySet} in. To make things easy internally, the special
 * class just holds a reference to the general {@link PropertySet} and
 * delegates all method calls to it.</p>
 *
 * <p>A cleaner implementation would have been like this: The {@link
 * PropertySetFactory} parses the stream data into some internal
 * object first.  Then it finds out whether the stream is a {@link
 * SummaryInformation}, a {@link DocumentSummaryInformation} or a
 * general {@link PropertySet}.  However, the current implementation
 * went the other way round historically: the convenience classes came
 * only late to my mind.</p>
 */
class SpecialPropertySet// : public MutablePropertySet
{
public:
    /**
     * The id to name mapping of the properties
     *  in this set.
     */
	virtual PropertyIDMapPtr getPropertySetIDMap() = 0;

    /**
     * <p>Creates a <code>SpecialPropertySet</code>.
     *
     * @param ps The property set to be encapsulated by the
     * <code>SpecialPropertySet</code>
     */
	SpecialPropertySet(PropertySet* ps);

//    /**
//     * <p>Creates a <code>SpecialPropertySet</code>.
//     *
//     * @param ps The mutable property set to be encapsulated by the
//     * <code>SpecialPropertySet</code>
//     */
//	SpecialPropertySet(MutablePropertySetPtr ps);
//
//    /**
//     * @see PropertySet#getByteOrder
//     */
//	int getByteOrder();
//
//    /**
//     * @see PropertySet#getFormat
//     */
//	int getFormat();
//
//    /**
//     * @see PropertySet#getOSVersion
//     */
//	int getOSVersion();
//
//    /**
//     * @see PropertySet#getClassID
//     */
//	ClassIDPtr getClassID();
//
//    /**
//     * @see PropertySet#getSectionCount
//     */
//	int getSectionCount();
//
//    /**
//     * @see PropertySet#getSections
//     */
//	SectionListPtr getSections();
//
//    /**
//     * @see PropertySet#isSummaryInformation
//     */
//	bool isSummaryInformation();
//
//    /**
//     * @see PropertySet#isDocumentSummaryInformation
//     */
//	bool isDocumentSummaryInformation();
//
//    /**
//     * @see PropertySet#getSingleSection
//     */
//	SectionPtr getFirstSection();
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#addSection(org.apache.poi.hpsf.SectionPtr)
//     */
//	void addSection(SectionPtr section);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#clearSections()
//     */
//	void clearSections();
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#setByteOrder(int)
//     */
//	void setByteOrder(int byteOrder);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#setClassID(org.apache.poi.hpsf.ClassIDPtr)
//     */
//	void setClassID(ClassIDPtr classID);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#setFormat(int)
//     */
//	void setFormat(int format);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#setOSVersion(int)
//     */
//	void setOSVersion(int osVersion);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#toInputStream()
//     */
//	BwProj::toolkit::DataReadStreamPtr toInputStream();
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#write(org.apache.poi.poifs.filesystem.DirectoryEntry, java.lang.CString)
//     */
//	void write(DirectoryEntryPtr dir, CString name);
//
//    /**
//     * @see org.apache.poi.hpsf.MutablePropertySet#write(java.io.OutputStream)
//     */
//	void write(IDataWriteStream* out);
//
//    /**
//     * @see org.apache.poi.hpsf.PropertySet#equals(java.lang.Object)
//     */
//	bool equals(Object* o);
//
//    /**
//     * @see org.apache.poi.hpsf.PropertySet#getProperties()
//     */
//	Property2ArrayPtr getProperties();
//
//    /**
//     * @see org.apache.poi.hpsf.PropertySet#hashCode()
//     */
//	int hashCode();
//
//    /**
//     * @see org.apache.poi.hpsf.PropertySet#toString()
//     */
//	CString toString();
//
//    /**
//     * @see org.apache.poi.hpsf.PropertySet#wasNull()
//     */
//	bool wasNull();
//
protected:
	/**
	* @see org.apache.poi.hpsf.PropertySet#getProperty(int)
	*/
	ObjectPtr getProperty(int id);

//	/**
//	* @see org.apache.poi.hpsf.PropertySet#getPropertyBooleanValue(int)
//	*/
//	bool getPropertyBooleanValue(int id);

	/**
	* @see org.apache.poi.hpsf.PropertySet#getPropertyIntValue(int)
	*/
	int getPropertyIntValue(int id);

	/**
	* Fetches the property with the given ID, then does its
	*  best to return it as a CString
	* @return The property as a CString, or null if unavailable
	*/
	CString getPropertyStringValue(int propertyId);

	static CString getPropertyStringValue(ObjectPtr propertyValue);

private:
	/**
	* <p>The "real" property set <code>SpecialPropertySet</code>
	* delegates to.</p>
	*/
	//MutablePropertySetPtr deleg;
	PropertySet* deleg;
};