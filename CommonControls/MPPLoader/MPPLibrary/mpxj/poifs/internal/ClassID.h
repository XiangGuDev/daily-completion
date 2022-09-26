#pragma once

/**
 *  <p>Represents a class ID (16 bytes). Unlike other little-endian
 *  type the {@link ClassID} is not just 16 bytes stored in the wrong
 *  order. Instead, it is a double word (4 bytes) followed by two
 *  words (2 bytes each) followed by 8 bytes.</p>
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 */
class ClassID
{
public:
    //static const ClassID OLE10_PACKAGE = new ClassID("{0003000C-0000-0000-C000-000000000046}");
    //static const ClassID PPT_SHOW = new ClassID("{64818D10-4F9B-11CF-86EA-00AA00B929E8}");
    //static const ClassID XLS_WORKBOOK = new ClassID("{00020841-0000-0000-C000-000000000046}");
    //static const ClassID TXT_ONLY = new ClassID("{5e941d80-bf96-11cd-b579-08002b30bfeb}"); // ???
    //static const ClassID EXCEL97      = new ClassID("{00020820-0000-0000-C000-000000000046}");
    //static const ClassID EXCEL95      = new ClassID("{00020810-0000-0000-C000-000000000046}");
    //static const ClassID WORD97       = new ClassID("{00020906-0000-0000-C000-000000000046}");
    //static const ClassID WORD95       = new ClassID("{00020900-0000-0000-C000-000000000046}");
    //static const ClassID POWERPOINT97 = new ClassID("{64818D10-4F9B-11CF-86EA-00AA00B929E8}");
    //static const ClassID POWERPOINT95 = new ClassID("{EA7BAE70-FB3B-11CD-A903-00AA00510EA3}");
    //static const ClassID EQUATION30   = new ClassID("{0002CE02-0000-0000-C000-000000000046}");

	/** <p>The number of bytes occupied by this object in the byte
	* stream.</p> */
	static const int LENGTH = 16;

	/**
	*  <p>Creates a {@link ClassID} and initializes its value with
	*  0x00 bytes.</p>
	*/
	ClassID();

    /**
     *  <p>Creates a {@link ClassID} and reads its value from a byte
     *  array.</p>
     *
     * @param src The byte array to read from.
     * @param offset The offset of the first byte to read.
     */
	ClassID(ByteArrayPtr src, int offset);

    /**
     * <p>Creates a {@link ClassID} from a human-readable representation of the Class ID in standard 
     * format <code>"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"</code>.</p>
     * 
     * @param externalForm representation of the Class ID represented by this object.
     */
	ClassID(CString externalForm);

    /**
     * @return The number of bytes occupied by this object in the byte
     * stream.
     */
	int length();

    /**
     * <p>Gets the bytes making out the class ID. They are returned in
     * correct order, i.e. big-endian.</p>
     *
     * @return the bytes making out the class ID.
     */
	ByteArrayPtr getBytes();

    /**
     * <p>Sets the bytes making out the class ID.</p>
     *
     * @param bytes The bytes making out the class ID in big-endian format. They
     * are copied without their order being changed.
     */
	void setBytes(ByteArrayPtr bytes);

    /**
     * <p>Reads the class ID's value from a byte array by turning
     * little-endian into big-endian.</p>
     *
     * @param src The byte array to read from
     *
     * @param offset The offset within the <var>src</var> byte array
     *
     * @return A byte array containing the class ID.
     */
	ByteArrayPtr read(ByteArrayPtr src, int offset);

    /**
     * <p>Writes the class ID to a byte array in the
     * little-endian format.</p>
     *
     * @param dst The byte array to write to.
     *
     * @param offset The offset within the <var>dst</var> byte array.
     *
     * @exception ArrayStoreException if there is not enough room for the class
     * ID 16 bytes in the byte array after the <var>offset</var> position.
     */
	void write(ByteArrayPtr dst, int offset);

protected:
	/**
	* <p>The bytes making out the class ID in correct order,
	* i.e. big-endian.</p>
	*/
	ByteArrayPtr bytes;
};

typedef std::shared_ptr<ClassID> ClassIDPtr;