#include "stdafx.h"
#include "Property2.h"
#include "..\internal\LittleEndian.h"
#include "VariantSupport.h"
#include "..\..\java\StringBuilder.h"

/**
    * <p>Returns the property's ID.</p>
    *
    * @return The ID value
    */
long Property2::getID()
{
    return id;
}


/**
    * <p>Returns the property's type.</p>
    *
    * @return The type value
    */
long Property2::getType()
{
    return type;
}


/**
    * <p>Returns the property's value.</p>
    *
    * @return The property's value
    */
ObjectPtr Property2::getValue()
{
    return value;
}



/**
    * <p>Creates a property.</p>
    *
    * @param id the property's ID.
    * @param type the property's type, see {@link Variant}.
    * @param value the property's value. Only certain types are allowed, see
    *        {@link Variant}.
    */
Property2::Property2(long id, long type, ObjectPtr value)
{
    this->id = id;
	this->type = type;
	this->value = value;
}

/**
    * <p>Creates a {@link Property2} instance by reading its bytes
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
Property2::Property2(long id, ByteArrayPtr src, long offset, int length, int codepage)
{
    this->id = id;

    /*
        * ID 0 is a special case since it specifies a dictionary of
        * property IDs and property names.
        */
    if (id == 0)
    {
        value = readDictionary(src, offset, length, codepage);
        return;
    }

    int o = (int) offset;
    type = LittleEndian::getUInt(src, o);
    o += LittleEndian::INT_SIZE;

    try
    {
        value = VariantSupport::read(src, o, length, (int) type, codepage);
    }
    catch (...)
    {
		o = o;
    }
}

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
std::shared_ptr<Map2<long, CString>> Property2::readDictionary(ByteArrayPtr src, long offset, int length, int codepage)
{
    /* Check whether "offset" points into the "src" array". */
    if (offset < 0 || offset > (int)src->length())
        throw toolkit::Exception(L"Illegal offset while HPSF stream contains");
    int o = (int) offset;

    /*
        * Read the number of dictionary entries.
        */
    long nrEntries = LittleEndian::getUInt(src, o);
    o += LittleEndian::INT_SIZE;

	std::shared_ptr<Map2<long, CString>> m (new Map2<long, CString>());

    try
    {
        for (int i = 0; i < nrEntries; i++)
        {
            /* The key. */
            long id = LittleEndian::getUInt(src, o);
            o += LittleEndian::INT_SIZE;

            /* The value (a string). The length is the either the
                * number of (two-byte) characters if the character set is Unicode
                * or the number of bytes if the character set is not Unicode.
                * The length includes terminating 0x00 bytes which we have to strip
                * off to create a Java string. */
            long sLength = LittleEndian::getUInt(src, o);
            o += LittleEndian::INT_SIZE;

            /* Read the string. */
			StringBuilder b;// = new StringBuffer();
            switch (codepage)
            {
                case -1:	// µ¥×Ö½Ú
                {
                    /* Without a codepage the length is equal to the number of
                        * bytes. */
					Array2<char> str(sLength + 1);
					memcpy_s(str.get(), sLength, src->get(), sLength);
					str[sLength] = 0;
					b.append(CString(str.get()));
                    break;
                }
				case 1200:// CodePageUtil.CP_UNICODE:	Ë«×Ö½Ú
                {
                    /* The length is the number of characters, i.e. the number
                        * of bytes is twice the number of the characters. */
                    int nrBytes = (int) (sLength * 2);
					Array2<wchar_t> str(sLength + 1);
					const wchar_t* src2 = (wchar_t*) src->get();
					memcpy_s(str.get(), sLength, src2, sLength);
					str[sLength] = 0;
					b.append(CString(str.get()));
                    break;
                }
                default:
                {
                    /* For encodings other than Unicode the length is the number
                        * of bytes. */
					int nrBytes = (int)(sLength * 2);
					Array2<wchar_t> str(sLength + 1);
					const wchar_t* src2 = (wchar_t*)src->get();
					memcpy_s(str.get(), sLength, src2, sLength);
					str[sLength] = 0;
					b.append(CString(str.get()));
                    break;
                }
            }

            ///* Strip 0x00 characters from the end of the string: */
            //while (b.length() > 0 && b.charAt(b.length() - 1) == 0x00)
            //    b.setLength(b.length() - 1);

			if (codepage == 1200)//CodePageUtil.CP_UNICODE)
            {
                if (sLength % 2 == 1)
                    sLength++;
                o += (sLength + sLength);
            }
            else
                o += sLength;
            m->put(id, b.toString());
        }
    }
    catch (toolkit::Exception&)
    {
        //POILogger l = POILogFactory.getLogger(getClass());
        //l.log(POILogger.WARN,
        //        "The property set's dictionary contains bogus data. "
        //        + "All dictionary entries starting with the one with ID "
        //        + id + " will be ignored.", ex);
    }
    return m;
}



///**
//    * <p>Returns the property's size in bytes. This is always a multiple of
//    * 4.</p>
//    *
//    * @return the property's size in bytes
//    *
//    * @exception WritingNotSupportedException if HPSF does not yet support the
//    * property's variant type.
//    */
//int Property2::getSize()
//{
//    int length = VariantSupport.getVariantLength(type);
//    if (length >= 0)
//        return length; /* Fixed length */
//    if (length == -2)
//        /* Unknown length */
//        throw new WritingNotSupportedException(type, null);
//
//    /* Variable length: */
//    int PADDING = 4; /* Pad to multiples of 4. */
//    switch ((int) type)
//    {
//        case VARENUM::VT_LPSTR:
//        {
//            int l = ((CString) value).length() + 1;
//            int r = l % PADDING;
//            if (r > 0)
//                l += PADDING - r;
//            length += l;
//            break;
//        }
//        case VARENUM::VT_EMPTY:
//            break;
//        default:
//            throw new WritingNotSupportedException(type, value);
//    }
//    return length;
//}
//
//bool Property2::typesAreEqual(long t1, long t2)
//{
//	if (t1 == t2 ||
//		(t1 == VARENUM::VT_LPSTR && t2 == VARENUM::VT_LPWSTR) ||
//		(t2 == VARENUM::VT_LPSTR && t1 == VARENUM::VT_LPWSTR)) {
//		return true;
//	}
//	return false;
//}