#pragma once

/**
 * <p>A property in a {@link Section} of a {@link PropertySet}.</p>
 *
 * <p>The property's <strong>ID</strong> gives the property a meaning
 * in the context of its {@link Section}. Each {@link Section} spans
 * its own name space of property IDs.</p>
 *
 * <p>The property's <strong>type</strong> determines how its
 * <strong>value </strong> is interpreted. For example, if the type is
 * {@link Variant#VT_LPSTR} (byte string), the value consists of a
 * DWord telling how many bytes the string contains. The bytes follow
 * immediately, including any null bytes that terminate the
 * string. The type {@link Variant#VT_I4} denotes a four-byte integer
 * value, {@link Variant#VT_FILETIME} some date and time (of a
 * file).</p>
 *
 * <p>Please note that not all {@link Variant} types yet. This might change
 * over time but largely depends on your feedback so that the POI team knows
 * which variant types are really needed. So please feel free to submit error
 * reports or patches for the types you need.</p>
 *
 * <p>Microsoft documentation: <a
 * href="http://msdn.microsoft.com/library/en-us/stg/stg/property_set_display_name_dictionary.asp?frame=true">
 * Property Set Display Name Dictionary</a>.
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 * @author Drew Varner (Drew.Varner InAndAround sc.edu)
 * @see Section
 * @see Variant
 */
class Property2
{
public:
    /**
     * <p>Returns the property's ID.</p>
     *
     * @return The ID value
     */
	long getID();

    /**
     * <p>Returns the property's type.</p>
     *
     * @return The type value
     */
	long getType();

    /**
     * <p>Returns the property's value.</p>
     *
     * @return The property's value
     */
	ObjectPtr getValue();

    /**
     * <p>Creates a property.</p>
     *
     * @param id the property's ID.
     * @param type the property's type, see {@link Variant}.
     * @param value the property's value. Only certain types are allowed, see
     *        {@link Variant}.
     */
	Property2(long id, long type, ObjectPtr value);

    /**
     * <p>Creates a {@link Property} instance by reading its bytes
     * from the property set stream.</p>
     *
     * @param id The property's ID.
     * @param src The bytes the property set stream consists of.
     * @param offset The property's type/value pair's offset in the
     * section.
     * @param length The property's type/value pair's length in bytes.
     * @param codepage The section's and thus the property's
     * codepage. It is needed only when reading string values.
     * @exception UnsupportedEncodingException if the specified codepage is not
     * supported.
     */
	Property2(long id, ByteArrayPtr src, long offset, int length, int codepage);


protected:
    /**
     * <p>Reads a dictionary.</p>
     *
     * @param src The byte array containing the bytes making out the dictionary.
     * @param offset At this offset within <var>src </var> the dictionary
     *        starts.
     * @param length The dictionary contains at most this many bytes.
     * @param codepage The codepage of the string values.
     * @return The dictonary
     * @throws UnsupportedEncodingException if the dictionary's codepage is not
     *         (yet) supported.
     */
	std::shared_ptr<Map2<long,CString>> readDictionary(ByteArrayPtr src, long offset, int length, int codepage);

    /**
     * <p>Returns the property's size in bytes. This is always a multiple of
     * 4.</p>
     *
     * @return the property's size in bytes
     *
     * @exception WritingNotSupportedException if HPSF does not yet support the
     * property's variant type.
     */
	//int getSize();

private:
	//bool typesAreEqual(long t1, long t2);

protected:
	/** <p>The property's ID.</p> */
	long id;

	/** <p>The property's type.</p> */
	long type;

	/** <p>The property's value.</p> */
	ObjectPtr value;
};

typedef std::shared_ptr<Property2> Property2Ptr;
typedef Array2<Property2Ptr> Property2Array;
typedef std::shared_ptr<Property2Array> Property2ArrayPtr;