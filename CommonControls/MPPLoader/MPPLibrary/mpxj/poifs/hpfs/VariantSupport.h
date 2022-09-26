#pragma once

#include "TypedPropertyValue.h"

/**
 * <p>Supports reading and writing of variant data.</p>
 *
 * <p><strong>FIXME (3):</strong> Reading and writing should be made more
 * uniform than it is now. The following items should be resolved:
 *
 * <ul>
 *
 * <li><p>Reading requires a length parameter that is 4 byte greater than the
 * actual data, because the variant type field is included. </p></li>
 *
 * <li><p>Reading reads from a byte array while writing writes to an byte array
 * output stream.</p></li>
 *
 * </ul>
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 */
class VariantSupport
{
public:

    ///**
    // * <p>HPSF is able to read these {@link Variant} types.</p>
    // */
    //static int SUPPORTED_TYPES[] = { VARENUM::VT_EMPTY,
    //        VARENUM::VT_I2, VARENUM::VT_I4, VARENUM::VT_I8, VARENUM::VT_R8,
    //        VARENUM::VT_FILETIME, VARENUM::VT_LPSTR, VARENUM::VT_LPWSTR,
    //        VARENUM::VT_CF, VARENUM::VT_BOOL };

    /**
     * <p>Reads a variant type from a byte array.</p>
     *
     * @param src The byte array
     * @param offset The offset in the byte array where the variant starts
     * @param length The length of the variant including the variant type field
     * @param type The variant type to read
     * @param codepage The codepage to use for non-wide strings
     * @return A Java object that corresponds best to the variant field. For
     *         example, a VT_I4 is returned as a {@link Long}, a VT_LPSTR as a
     *         {@link String}.
     * @exception ReadingNotSupportedtoolkit::Exception if a property is to be written
     *            who's variant type HPSF does not yet support
     * @exception UnsupportedEncodingtoolkit::Exception if the specified codepage is not
     *            supported.
     * @see Variant
     */
    static ObjectPtr read(ByteArrayPtr src, int offset, int length, long type, int codepage)
    {
        TypedPropertyValue typedPropertyValue ((int) type, NULL);
        int unpadded;
        //try
        //{
            unpadded = typedPropertyValue.readValue( src, offset );
        //}
        //catch (toolkit::Exception& exc )
        //{
        //    int propLength = min( length, src->length() - offset );
        //    ByteArrayPtr v (new ByteArray2(propLength];
        //    System.arraycopy( src, offset, v, 0, propLength );
        //    throw new ReadingNotSupportedtoolkit::Exception( type, v );
        //}

        switch ( (int) type )
        {
        case VARENUM::VT_EMPTY:
        case VARENUM::VT_I4:
        case VARENUM::VT_I8:
        case VARENUM::VT_R8:
            /*
             * we have more property types that can be converted into Java
             * objects, but current API need to be preserved, and it returns
             * other types as byte arrays. In future major versions it shall be
             * changed -- sergey
             */
            return typedPropertyValue.getValue();

        case VARENUM::VT_I2:
        {
			return typedPropertyValue.getValue();
            /*
             * also for backward-compatibility with prev. versions of POI
             * --sergey
             */
            //return Integer.valueOf( ( (Short) typedPropertyValue.getValue() )
            //        .intValue() );
        }
        case VARENUM::VT_FILETIME:
        {
			return typedPropertyValue.getValue();
            //Filetime filetime = (Filetime) typedPropertyValue.getValue();
            //return Util.filetimeToDate( (int) filetime.getHigh(),
            //        (int) filetime.getLow() );
        }
        case VARENUM::VT_LPSTR:
        {
			return typedPropertyValue.getValue();
            //CodePageString string = (CodePageString) typedPropertyValue
            //        .getValue();
            //return string.getJavaValue( codepage );
        }
        case VARENUM::VT_LPWSTR:
        {
			return typedPropertyValue.getValue();
            //UnicodeString string = (UnicodeString) typedPropertyValue
            //        .getValue();
            //return string.toJavaString();
        }
        //case VARENUM::VT_CF:
        //{
        //    // if(l1 < 0) {
        //    /**
        //     * YK: reading the ClipboardData packet (VT_CF) is not quite
        //     * correct. The size of the data is determined by the first four
        //     * bytes of the packet while the current implementation calculates
        //     * it in the Section constructor. Test files in Bugzilla 42726 and
        //     * 45583 clearly show that this approach does not always work. The
        //     * workaround below attempts to gracefully handle such cases instead
        //     * of throwing exceptions.
        //     * 
        //     * August 20, 2009
        //     */
        //    // l1 = LittleEndian.getInt(src, o1); o1 += LittleEndian.INT_SIZE;
        //    // }
        //    // final byte[] v = new byte[l1];
        //    // System.arraycopy(src, o1, v, 0, v.length);
        //    // value = v;
        //    // break;
        //    ClipboardData clipboardData = (ClipboardData) typedPropertyValue
        //            .getValue();
        //    return clipboardData.toByteArray2();
        //}

        //case VARENUM::VT_BOOL:
        //{
        //    VariantBool bool = (VariantBool) typedPropertyValue.getValue();
        //    return bool.valueOf( bool.getValue() );
        //}

        default:
        {
			throw toolkit::Exception(L"ReadingNotSupported");
            ///*
            // * it is not very good, but what can do without breaking current
            // * API? --sergey
            // */
            //final byte[] v = new byte[unpadded];
            //System.arraycopy( src, offset, v, 0, unpadded );
            //throw new ReadingNotSupportedtoolkit::Exception( type, v );
        }
        }
    }

    ///**
    // * <p>Turns a codepage number into the equivalent character encoding's
    // * name.</p>
    // *
    // * @param codepage The codepage number
    // *
    // * @return The character encoding's name. If the codepage number is 65001,
    // * the encoding name is "UTF-8". All other positive numbers are mapped to
    // * "cp" followed by the number, e.g. if the codepage number is 1252 the
    // * returned character encoding name will be "cp1252".
    // *
    // * @exception UnsupportedEncodingtoolkit::Exception if the specified codepage is
    // * less than zero.
    // */
    //static String codepageToEncoding(int codepage)
    //{
    //    return CodePageUtil.codepageToEncoding(codepage);
    //}
};