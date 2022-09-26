#pragma once

#include "Section.h"

/**
 * <p>Represents a property set in the Horrible Property Set Format
 * (HPSF). These are usually metadata of a Microsoft Office
 * document.</p>
 *
 * <p>An application that wants to access these metadata should create
 * an instance of this class or one of its subclasses by calling the
 * factory method {@link PropertySetFactory#create} and then retrieve
 * the information its needs by calling appropriate methods.</p>
 *
 * <p>{@link PropertySetFactory#create} does its work by calling one
 * of the constructors {@link PropertySet#PropertySet(BwProj::toolkit::IDataReadStream*)} or
 * {@link PropertySet#PropertySet(ByteArrayPtr)}. If the constructor's
 * argument is not in the Horrible Property Set Format, i.e. not a
 * property set stream, or if any other error occurs, an appropriate
 * exception is thrown.</p>
 *
 * <p>A {@link PropertySet} has a list of {@link SectionPtr}s, and each
 * {@link SectionPtr} has a {@link Property} array. Use {@link
 * #getSections} to retrieve the {@link SectionPtr}s, then call {@link
 * SectionPtr#getProperties} for each {@link SectionPtr} to get hold of the
 * {@link Property} arrays.</p> Since the vast majority of {@link
 * PropertySet}s contains only a single {@link SectionPtr}, the
 * convenience method {@link #getProperties} returns the properties of
 * a {@link PropertySet}'s {@link SectionPtr} (throwing a {@link
 * NoSingleSectionException} if the {@link PropertySet} contains more
 * (or less) than exactly one {@link SectionPtr}).</p>
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 * @author Drew Varner (Drew.Varner hanginIn sc.edu)
 */
class PropertySet
{
public:
	///**
	//* <p>Returns the property set stream's low-level "byte order"
	//* field. It is always <tt>0xFFFE</tt> .</p>
	//*
	//* @return The property set stream's low-level "byte order" field.
	//*/
	//int getByteOrder();

	///**
	//* <p>Returns the property set stream's low-level "format"
	//* field. It is always <tt>0x0000</tt> .</p>
	//*
	//* @return The property set stream's low-level "format" field.
	//*/
	//int getFormat();

	///**
	//* <p>Returns the property set stream's low-level "OS version"
	//* field.</p>
	//*
	//* @return The property set stream's low-level "OS version" field.
	//*/
	//int getOSVersion();

	///**
	// * <p>Returns the property set stream's low-level "class ID"
	// * field.</p>
	// *
	// * @return The property set stream's low-level "class ID" field.
	// */
	//ClassIDPtr getClassID();

	///**
	// * <p>Returns the number of {@link SectionPtr}s in the property
	// * set.</p>
	// *
	// * @return The number of {@link SectionPtr}s in the property set.
	// */
	int getSectionCount();

	///**
	// * <p>Returns the {@link SectionPtr}s in the property set.</p>
	// *
	// * @return The {@link SectionPtr}s in the property set.
	// */
	//SectionListPtr getSections();

	///**
	// * <p>Creates a {@link PropertySet} instance from an {@link
	// * BwProj::toolkit::IDataReadStream*} in the Horrible Property Set Format.</p>
	// *
	// * <p>The constructor reads the first few bytes from the stream
	// * and determines whether it is really a property set stream. If
	// * it is, it parses the rest of the stream. If it is not, it
	// * resets the stream to its beginning in order to let other
	// * components mess around with the data and throws an
	// * exception.</p>
	// *
	// * @param stream Holds the data making out the property set
	// * stream.
	// * @throws MarkUnsupportedException if the stream does not support
	// * the {@link BwProj::toolkit::IDataReadStream*#markSupported} method.
	// * @throws IOException if the {@link BwProj::toolkit::IDataReadStream*} cannot not be
	// * accessed as needed.
	// * @exception NoPropertySetStreamException if the input stream does not
	// * contain a property set.
	// * @exception UnsupportedEncodingException if a character encoding is not
	// * supported.
	// */
	PropertySet(BwProj::toolkit::IDataReadStream* stream);


	///**
	// * <p>Creates a {@link PropertySet} instance from a byte array
	// * that represents a stream in the Horrible Property Set
	// * Format.</p>
	// *
	// * @param stream The byte array holding the stream data.
	// * @param offset The offset in <var>stream</var> where the stream
	// * data begin. If the stream data begin with the first byte in the
	// * array, the <var>offset</var> is 0.
	// * @param length The length of the stream data.
	// * @throws NoPropertySetStreamException if the byte array is not a
	// * property set stream.
	// *
	// * @exception UnsupportedEncodingException if the codepage is not supported.
	// */
	//PropertySet(ByteArrayPtr stream, int offset, int length);


	///**
	// * <p>Creates a {@link PropertySet} instance from a byte array
	// * that represents a stream in the Horrible Property Set
	// * Format.</p>
	// *
	// * @param stream The byte array holding the stream data. The
	// * complete byte array contents is the stream data.
	// * @throws NoPropertySetStreamException if the byte array is not a
	// * property set stream.
	// *
	// * @exception UnsupportedEncodingException if the codepage is not supported.
	// */
	//PropertySet(ByteArrayPtr stream);

	///**
	//* <p>Checks whether an {@link BwProj::toolkit::IDataReadStream*} is in the Horrible
	//* Property Set Format.</p>
	//*
	//* @param stream The {@link BwProj::toolkit::IDataReadStream*} to check. In order to
	//* perform the check, the method reads the first bytes from the
	//* stream. After reading, the stream is reset to the position it
	//* had before reading. The {@link BwProj::toolkit::IDataReadStream*} must support the
	//* {@link BwProj::toolkit::IDataReadStream*#mark} method.
	//* @return <code>true</code> if the stream is a property set
	//* stream, else <code>false</code>.
	//* @throws MarkUnsupportedException if the {@link BwProj::toolkit::IDataReadStream*}
	//* does not support the {@link BwProj::toolkit::IDataReadStream*#mark} method.
	//* @exception IOException if an I/O error occurs
	//*/
	//static bool isPropertySetStream(BwProj::toolkit::IDataReadStream* stream);

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
	//static bool isPropertySetStream(ByteArrayPtr src, int offset, int length);

	///**
	// * <p>Checks whether this {@link PropertySet} represents a Summary
	// * Information.</p>
	// *
	// * @return <code>true</code> if this {@link PropertySet}
	// * represents a Summary Information, else <code>false</code>.
	// */
	//bool isSummaryInformation();

	///**
	// * <p>Checks whether this {@link PropertySet} is a Document
	// * Summary Information.</p>
	// *
	// * @return <code>true</code> if this {@link PropertySet}
	// * represents a Document Summary Information, else <code>false</code>.
	// */
	//bool isDocumentSummaryInformation();

	///**
	// * <p>Convenience method returning the {@link Property} array
	// * contained in this property set. It is a shortcut for getting
	// * the {@link PropertySet}'s {@link SectionPtr}s list and then
	// * getting the {@link Property} array from the first {@link
	// * SectionPtr}.</p>
	// *
	// * @return The properties of the only {@link SectionPtr} of this
	// * {@link PropertySet}.
	// * @throws NoSingleSectionException if the {@link PropertySet} has
	// * more or less than one {@link SectionPtr}.
	// */
	//Property2ArrayPtr getProperties();

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
	//* @throws NoSingleSectionException if the {@link PropertySet} has
	//* more than one {@link SectionPtr}.
	//*/
	//bool wasNull();

	///**
	//* <p>Gets the {@link PropertySet}'s first section.</p>
	//*
	//* @return The {@link PropertySet}'s first section.
	//*/
	SectionPtr getFirstSection();

	///**
	// * <p>If the {@link PropertySet} has only a single section this
	// * method returns it.</p>
	// *
	// * @return The singleSection value
	// */
	//SectionPtr getSingleSection();

	///**
	// * <p>Returns <code>true</code> if the <code>PropertySet</code> is equal
	// * to the specified parameter, else <code>false</code>.</p>
	// *
	// * @param o the object to compare this <code>PropertySet</code> with
	// *
	// * @return <code>true</code> if the objects are equal, <code>false</code>
	// * if not
	// */
	//bool equals(Object* o);

	///**
	// * @see Object#hashCode()
	// */
	//int hashCode();

	///**
	// * @see Object#toCString()
	// */
	//CString toCString();

public:
	/**
	* <p>If the OS version field holds this value the property set stream was
	* created on a 16-bit Windows system.</p>
	*/
	static const int OS_WIN16 = 0x0000;

	/**
	* <p>If the OS version field holds this value the property set stream was
	* created on a Macintosh system.</p>
	*/
	static const int OS_MACINTOSH = 0x0001;

	/**
	* <p>If the OS version field holds this value the property set stream was
	* created on a 32-bit Windows system.</p>
	*/
	static const int OS_WIN32 = 0x0002;

public:
	///**
	//* <p>Creates an empty (uninitialized) {@link PropertySet}.</p>
	//*
	//* <p><strong>Please note:</strong> For the time being this
	//* constructor is since it is used for internal purposes
	//* only, but expect it to become once the property set's
	//* writing functionality is implemented.</p>
	//*/
	//PropertySet();

	///**
	//* <p>Convenience method returning the value of the property with
	//* the specified ID. If the property is not available,
	//* <code>null</code> is returned and a subsequent call to {@link
	//* #wasNull} will return <code>true</code> .</p>
	//*
	//* @param id The property ID
	//* @return The property value
	//* @throws NoSingleSectionException if the {@link PropertySet} has
	//* more or less than one {@link SectionPtr}.
	//*/
	ObjectPtr getProperty(int id);

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
	//* @throws NoSingleSectionException if the {@link PropertySet} has
	//* more or less than one {@link SectionPtr}.
	//*/
	//bool getPropertyboolValue(int id);

	/**
	* <p>Convenience method returning the value of the numeric
	* property with the specified ID. If the property is not
	* available, 0 is returned. A subsequent call to {@link #wasNull}
	* will return <code>true</code> to let the caller distinguish
	* that case from a real property value of 0.</p>
	*
	* @param id The property ID
	* @return The propertyIntValue value
	* @throws NoSingleSectionException if the {@link PropertySet} has
	* more or less than one {@link SectionPtr}.
	*/
	int getPropertyIntValue(int id);

private:
	/**
	* <p>Initializes this {@link PropertySet} instance from a byte
	* array. The method assumes that it has been checked already that
	* the byte array indeed represents a property set stream. It does
	* no more checks on its own.</p>
	*
	* @param src Byte array containing the property set stream
	* @param offset The property set stream starts at this offset
	* from the beginning of <var>src</var>
	* @param length Length of the property set stream.
	* @throws UnsupportedEncodingException if HPSF does not (yet) support the
	* property set's character encoding.
	*/
	void init(ByteArrayPtr src, int offset, int length);

protected:

	/**
	* <p>Specifies this {@link PropertySet}'s byte order. See the
	* HPFS documentation for details!</p>
	*/
	int byteOrder;

	/**
	* <p>Specifies this {@link PropertySet}'s format. See the HPFS
	* documentation for details!</p>
	*/
	int format;


	/**
	* <p>Specifies the version of the operating system that created
	* this {@link PropertySet}. See the HPFS documentation for
	* details!</p>
	*/
	int osVersion;


	/**
	* <p>Specifies this {@link PropertySet}'s "classID" field. See
	* the HPFS documentation for details!</p>
	*/
	ClassIDPtr classID;


	/**
	* <p>The sections in this {@link PropertySet}.</p>
	*/
	SectionListPtr sections;

protected:
	/**
	* <p>The "byteOrder" field must equal this value.</p>
	*/
	static const BYTE BYTE_ORDER_ASSERTION[];

	/**
	* <p>The "format" field must equal this value.</p>
	*/
	static const BYTE FORMAT_ASSERTION[];
};

typedef std::shared_ptr<PropertySet> PropertySetPtr;