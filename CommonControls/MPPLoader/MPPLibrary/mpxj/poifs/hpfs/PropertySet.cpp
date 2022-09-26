#include "stdafx.h"
#include "PropertySet.h"
#include "..\internal\LittleEndian.h"

/**
* <p>The "byteOrder" field must equal this value.</p>
*/
const BYTE PropertySet::BYTE_ORDER_ASSERTION[] = { 0xFE, 0xFF };

/**
* <p>The "format" field must equal this value.</p>
*/
const BYTE PropertySet::FORMAT_ASSERTION[] = { 0x00, 0x00 };

///**
//* <p>Returns the property set stream's low-level "byte order"
//* field. It is always <tt>0xFFFE</tt> .</p>
//*
//* @return The property set stream's low-level "byte order" field.
//*/
//int PropertySet::getByteOrder()
//{
//	return byteOrder;
//}
//
///**
//* <p>Returns the property set stream's low-level "format"
//* field. It is always <tt>0x0000</tt> .</p>
//*
//* @return The property set stream's low-level "format" field.
//*/
//int PropertySet::getFormat()
//{
//	return format;
//}
//
///**
//* <p>Returns the property set stream's low-level "OS version"
//* field.</p>
//*
//* @return The property set stream's low-level "OS version" field.
//*/
//int PropertySet::getOSVersion()
//{
//	return osVersion;
//}
//
///**
//	* <p>Returns the property set stream's low-level "class ID"
//	* field.</p>
//	*
//	* @return The property set stream's low-level "class ID" field.
//	*/
//ClassIDPtr PropertySet::getClassID()
//{
//	return classID;
//}
//
//
//
///**
//	* <p>Returns the number of {@link SectionPtr}s in the property
//	* set.</p>
//	*
//	* @return The number of {@link SectionPtr}s in the property set.
//	*/
int PropertySet::getSectionCount()
{
	return sections->GetCount();
}
//
///**
//	* <p>Returns the {@link SectionPtr}s in the property set.</p>
//	*
//	* @return The {@link SectionPtr}s in the property set.
//	*/
//SectionListPtr PropertySet::getSections()
//{
//	return sections;
//}
//
///**
//	* <p>Creates a {@link PropertySet} instance from an {@link
//	* BwProj::toolkit::IDataReadStream*} in the Horrible Property Set Format.</p>
//	*
//	* <p>The constructor reads the first few bytes from the stream
//	* and determines whether it is really a property set stream-> If
//	* it is, it parses the rest of the stream-> If it is not, it
//	* resets the stream to its beginning in order to let other
//	* components mess around with the data and throws an
//	* exception.</p>
//	*
//	* @param stream Holds the data making out the property set
//	* stream->
//	* @throws MarkUnsupportedtoolkit::Exception if the stream does not support
//	* the {@link BwProj::toolkit::IDataReadStream*#markSupported} method.
//	* @throws IOtoolkit::Exception if the {@link BwProj::toolkit::IDataReadStream*} cannot not be
//	* accessed as needed.
//	* @exception NoPropertySetStreamtoolkit::Exception if the input stream does not
//	* contain a property set.
//	* @exception UnsupportedEncodingtoolkit::Exception if a character encoding is not
//	* supported.
//	*/
PropertySet::PropertySet(BwProj::toolkit::IDataReadStream* stream)
{
	//if (isPropertySetStream(stream))
	//{
		int avail = stream->available();
		ByteArrayPtr buffer (new ByteArray2(avail));
		DWORD readBytes = 0;
		stream->Read(buffer->get(), buffer->length(), readBytes);
		init(buffer, 0, buffer->length());
	//}
	//else
	//	throw toolkit::Exception(L"NoPropertySetStream");
}
//
//
//
///**
//	* <p>Creates a {@link PropertySet} instance from a byte array
//	* that represents a stream in the Horrible Property Set
//	* Format.</p>
//	*
//	* @param stream The byte array holding the stream data.
//	* @param offset The offset in <var>stream</var> where the stream
//	* data begin. If the stream data begin with the first byte in the
//	* array, the <var>offset</var> is 0.
//	* @param length The length of the stream data.
//	* @throws NoPropertySetStreamtoolkit::Exception if the byte array is not a
//	* property set stream->
//	*
//	* @exception UnsupportedEncodingtoolkit::Exception if the codepage is not supported.
//	*/
//PropertySet::PropertySet(ByteArrayPtr stream, int offset, int length)
//{
//	if (isPropertySetStream(stream, offset, length))
//		init(stream, offset, length);
//	else
//		throw toolkit::Exception(L"NoPropertySetStream");
//}
//
//
//
///**
//	* <p>Creates a {@link PropertySet} instance from a byte array
//	* that represents a stream in the Horrible Property Set
//	* Format.</p>
//	*
//	* @param stream The byte array holding the stream data. The
//	* complete byte array contents is the stream data.
//	* @throws NoPropertySetStreamtoolkit::Exception if the byte array is not a
//	* property set stream->
//	*
//	* @exception UnsupportedEncodingtoolkit::Exception if the codepage is not supported.
//	*/
//PropertySet::PropertySet(ByteArrayPtr stream)
//	:PropertySet(stream, 0, stream->length())
//{
//}
//
///**
//* <p>Checks whether an {@link BwProj::toolkit::IDataReadStream*} is in the Horrible
//* Property Set Format.</p>
//*
//* @param stream The {@link BwProj::toolkit::IDataReadStream*} to check. In order to
//* perform the check, the method reads the first bytes from the
//* stream-> After reading, the stream is reset to the position it
//* had before reading. The {@link BwProj::toolkit::IDataReadStream*} must support the
//* {@link BwProj::toolkit::IDataReadStream*#mark} method.
//* @return <code>true</code> if the stream is a property set
//* stream, else <code>false</code>.
//* @throws MarkUnsupportedtoolkit::Exception if the {@link BwProj::toolkit::IDataReadStream*}
//* does not support the {@link BwProj::toolkit::IDataReadStream*#mark} method.
//* @exception IOtoolkit::Exception if an I/O error occurs
//*/
//bool PropertySet::isPropertySetStream(BwProj::toolkit::IDataReadStream* stream)
//{
//	/*
//		* Read at most this many bytes.
//		*/
//	int BUFFER_SIZE = 50;
//
//	/*
//		* Mark the current position in the stream so that we can
//		* reset to this position if the stream does not contain a
//		* property set.
//		*/
//	if (!stream->markSupported())
//		throw toolkit::Exception ("MarkUnsupported");
//	stream->mark(BUFFER_SIZE);
//
//	/*
//		* Read a couple of bytes from the stream->
//		*/
//	ByteArrayPtr buffer = new byte[BUFFER_SIZE];
//	int bytes =
//		stream->read(buffer, 0,
//		Math.min(buffer.length, stream->available()));
//	bool isPropertySetStream =
//		isPropertySetStream(buffer, 0, bytes);
//	stream->reset();
//	return isPropertySetStream;
//}
//
///**
//* <p>Checks whether a byte array is in the Horrible Property Set
//* Format.</p>
//*
//* @param src The byte array to check.
//* @param offset The offset in the byte array.
//* @param length The significant number of bytes in the byte
//* array. Only this number of bytes will be checked.
//* @return <code>true</code> if the byte array is a property set
//* stream, <code>false</code> if not.
//*/
//bool PropertySet::isPropertySetStream(ByteArrayPtr src, int offset, int length)
//{
//	/* FIXME (3): Ensure that at most "length" bytes are read. */
//
//	/*
//		* Read the header fields of the stream-> They must always be
//		* there.
//		*/
//	int o = offset;
//	int byteOrder = LittleEndian.getUShort(src, o);
//	o += LittleEndian.SHORT_SIZE;
//	ByteArrayPtr temp = new byte[LittleEndian.SHORT_SIZE];
//	LittleEndian.putShort(temp, 0, (short)byteOrder);
//	if (!Util.equal(temp, BYTE_ORDER_ASSERTION))
//		return false;
//	int format = LittleEndian.getUShort(src, o);
//	o += LittleEndian.SHORT_SIZE;
//	temp = new byte[LittleEndian.SHORT_SIZE];
//	LittleEndian.putShort(temp, 0, (short)format);
//	if (!Util.equal(temp, FORMAT_ASSERTION))
//		return false;
//	// long osVersion = LittleEndian.getUInt(src, offset);
//	o += LittleEndian.INT_SIZE;
//	// ClassIDPtr classID = new ClassIDPtr(src, offset);
//	o += ClassIDPtr.LENGTH;
//	long sectionCount = LittleEndian.getUInt(src, o);
//	o += LittleEndian.INT_SIZE;
//	if (sectionCount < 0)
//		return false;
//	return true;
//}
//
//
//
///**
//	* <p>Checks whether this {@link PropertySet} represents a Summary
//	* Information.</p>
//	*
//	* @return <code>true</code> if this {@link PropertySet}
//	* represents a Summary Information, else <code>false</code>.
//	*/
//bool PropertySet::isSummaryInformation()
//{
//	if (sections.size() <= 0)
//		return false;
//	return Util.equal(sections.get(0).getFormatID().getBytes(),
//		SectionIDMap.SUMMARY_INFORMATION_ID);
//}
//
//
//
///**
//	* <p>Checks whether this {@link PropertySet} is a Document
//	* Summary Information.</p>
//	*
//	* @return <code>true</code> if this {@link PropertySet}
//	* represents a Document Summary Information, else <code>false</code>.
//	*/
//bool PropertySet::isDocumentSummaryInformation()
//{
//	if (sections.size() <= 0)
//		return false;
//	return Util.equal(sections.get(0).getFormatID().getBytes(),
//		SectionIDMap.DOCUMENT_SUMMARY_INFORMATION_ID[0]);
//}
//
//
//
///**
//	* <p>Convenience method returning the {@link Property} array
//	* contained in this property set. It is a shortcut for getting
//	* the {@link PropertySet}'s {@link SectionPtr}s list and then
//	* getting the {@link Property} array from the first {@link
//	* SectionPtr}.</p>
//	*
//	* @return The properties of the only {@link SectionPtr} of this
//	* {@link PropertySet}.
//	* @throws NoSingleSectiontoolkit::Exception if the {@link PropertySet} has
//	* more or less than one {@link SectionPtr}.
//	*/
//Property2ArrayPtr PropertySet::getProperties()
//{
//	return getFirstSection().getProperties();
//}
//
//
///**
//* <p>Checks whether the property which the last call to {@link
//* #getPropertyIntValue} or {@link #getProperty} tried to access
//* was available or not. This information might be important for
//* callers of {@link #getPropertyIntValue} since the latter
//* returns 0 if the property does not exist. Using {@link
//* #wasNull}, the caller can distiguish this case from a
//* property's real value of 0.</p>
//*
//* @return <code>true</code> if the last call to {@link
//* #getPropertyIntValue} or {@link #getProperty} tried to access a
//* property that was not available, else <code>false</code>.
//* @throws NoSingleSectiontoolkit::Exception if the {@link PropertySet} has
//* more than one {@link SectionPtr}.
//*/
//bool PropertySet::wasNull()
//{
//	return getFirstSection().wasNull();
//}
//
//
//
///**
//* <p>Gets the {@link PropertySet}'s first section.</p>
//*
//* @return The {@link PropertySet}'s first section.
//*/
SectionPtr PropertySet::getFirstSection()
{
	if (getSectionCount() < 1)
		throw toolkit::Exception(L"Property set does not contain any sections.");
	return sections->GetAt(0);
}
//
//
//
///**
//	* <p>If the {@link PropertySet} has only a single section this
//	* method returns it.</p>
//	*
//	* @return The singleSection value
//	*/
//SectionPtr PropertySet::getSingleSection()
//{
//	int sectionCount = getSectionCount();
//	if (sectionCount != 1)
//		throw new NoSingleSectiontoolkit::Exception
//		("Property set contains " + sectionCount + " sections.");
//	return sections.get(0);
//}
//
//
//
///**
//	* <p>Returns <code>true</code> if the <code>PropertySet</code> is equal
//	* to the specified parameter, else <code>false</code>.</p>
//	*
//	* @param o the object to compare this <code>PropertySet</code> with
//	*
//	* @return <code>true</code> if the objects are equal, <code>false</code>
//	* if not
//	*/
//bool PropertySet::equals(Object* o)
//{
//	if (o == null || !(o instanceof PropertySet))
//		return false;
//	PropertySet ps = (PropertySet)o;
//	int byteOrder1 = ps.getByteOrder();
//	int byteOrder2 = getByteOrder();
//	ClassIDPtr classID1 = ps.getClassID();
//	ClassIDPtr classID2 = getClassID();
//	int format1 = ps.getFormat();
//	int format2 = getFormat();
//	int osVersion1 = ps.getOSVersion();
//	int osVersion2 = getOSVersion();
//	int sectionCount1 = ps.getSectionCount();
//	int sectionCount2 = getSectionCount();
//	if (byteOrder1 != byteOrder2 ||
//		!classID1.equals(classID2) ||
//		format1 != format2 ||
//		osVersion1 != osVersion2 ||
//		sectionCount1 != sectionCount2)
//		return false;
//
//	/* Compare the sections: */
//	return Util.equals(getSections(), ps.getSections());
//}
//
//
//
///**
//	* @see Object#hashCode()
//	*/
//int PropertySet::hashCode()
//{
//	throw new UnsupportedOperationtoolkit::Exception("FIXME: Not yet implemented.");
//}
//
//
//
///**
//	* @see Object#toCString()
//	*/
//CString PropertySet::toCString()
//{
//	CStringBuffer b = new CStringBuffer();
//	int sectionCount = getSectionCount();
//	b.append(getClass().getName());
//	b.append('[');
//	b.append("byteOrder: ");
//	b.append(getByteOrder());
//	b.append(", classID: ");
//	b.append(getClassID());
//	b.append(", format: ");
//	b.append(getFormat());
//	b.append(", OSVersion: ");
//	b.append(getOSVersion());
//	b.append(", sectionCount: ");
//	b.append(sectionCount);
//	b.append(", sections: [\n");
//	for (SectionPtr section : getSections())
//		b.append(section);
//	b.append(']');
//	b.append(']');
//	return b.toCString();
//}
//
///**
//* <p>Creates an empty (uninitialized) {@link PropertySet}.</p>
//*
//* <p><strong>Please note:</strong> For the time being this
//* constructor is since it is used for internal purposes
//* only, but expect it to become once the property set's
//* writing functionality is implemented.</p>
//*/
//PropertySet::PropertySet()
//{ }
//
///**
//* <p>Convenience method returning the value of the property with
//* the specified ID. If the property is not available,
//* <code>null</code> is returned and a subsequent call to {@link
//* #wasNull} will return <code>true</code> .</p>
//*
//* @param id The property ID
//* @return The property value
//* @throws NoSingleSectiontoolkit::Exception if the {@link PropertySet} has
//* more or less than one {@link SectionPtr}.
//*/
ObjectPtr PropertySet::getProperty(int id)
{
	return getFirstSection()->getProperty(id);
}
//
///**
//* <p>Convenience method returning the value of a bool property
//* with the specified ID. If the property is not available,
//* <code>false</code> is returned. A subsequent call to {@link
//* #wasNull} will return <code>true</code> to let the caller
//* distinguish that case from a real property value of
//* <code>false</code>.</p>
//*
//* @param id The property ID
//* @return The property value
//* @throws NoSingleSectiontoolkit::Exception if the {@link PropertySet} has
//* more or less than one {@link SectionPtr}.
//*/
//bool PropertySet::getPropertyboolValue(int id)
//{
//	return getFirstSection().getPropertyboolValue(id);
//}
//
///**
//* <p>Convenience method returning the value of the numeric
//* property with the specified ID. If the property is not
//* available, 0 is returned. A subsequent call to {@link #wasNull}
//* will return <code>true</code> to let the caller distinguish
//* that case from a real property value of 0.</p>
//*
//* @param id The property ID
//* @return The propertyIntValue value
//* @throws NoSingleSectiontoolkit::Exception if the {@link PropertySet} has
//* more or less than one {@link SectionPtr}.
//*/
int PropertySet::getPropertyIntValue(int id)
{
	return getFirstSection()->getPropertyIntValue(id);
}
//
///**
//* <p>Initializes this {@link PropertySet} instance from a byte
//* array. The method assumes that it has been checked already that
//* the byte array indeed represents a property set stream-> It does
//* no more checks on its own.</p>
//*
//* @param src Byte array containing the property set stream
//* @param offset The property set stream starts at this offset
//* from the beginning of <var>src</var>
//* @param length Length of the property set stream->
//* @throws UnsupportedEncodingtoolkit::Exception if HPSF does not (yet) support the
//* property set's character encoding.
//*/
void PropertySet::init(ByteArrayPtr src, int offset, int length)
{
	/* FIXME (3): Ensure that at most "length" bytes are read. */

	/*
	* Read the stream's header fields.
	*/
	int o = offset;
	byteOrder = LittleEndian::getUShort(src, o);
	o += LittleEndian::SHORT_SIZE;
	format = LittleEndian::getUShort(src, o);
	o += LittleEndian::SHORT_SIZE;
	osVersion = (int)LittleEndian::getUInt(src, o);
	o += LittleEndian::INT_SIZE;
	classID.reset (new ClassID(src, o));
	o += ClassID::LENGTH;
	int sectionCount = LittleEndian::getInt(src, o);
	o += LittleEndian::INT_SIZE;
	if (sectionCount < 0)
		throw toolkit::Exception(L"HPSFRuntime toolkit::Exception, Section count is negative.");

	/*
	* Read the sections, which are following the header. They
	* start with an array of section descriptions. Each one
	* consists of a format ID telling what the section contains
	* and an offset telling how many bytes from the start of the
	* stream the section begins.
	*/
	/*
	* Most property sets have only one section. The Document
	* Summary Information stream has 2. Everything else is a rare
	* exception and is no longer fostered by Microsoft.
	*/
	sections.reset(new List<SectionPtr>());// (sectionCount);

	/*
	* Loop over the section descriptor array. Each descriptor
	* consists of a ClassIDPtr and a DWord, and we have to increment
	* "offset" accordingly.
	*/
	for (int i = 0; i < sectionCount; i++)
	{
		SectionPtr s (new Section(src, o));
		o += ClassID::LENGTH + LittleEndian::INT_SIZE;
		sections->Add(s);
	}
}