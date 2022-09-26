#pragma once
#include "..\internal\LittleEndian.h"
#include "..\..\common\DateHelper.h"

class TypedPropertyValue
{
public:
    //TypedPropertyValue()
    //{
    //}

    TypedPropertyValue( ByteArrayPtr data, int startOffset )
    {
        read( data, startOffset );
    }

    TypedPropertyValue( int type, ObjectPtr value )
    {
        _type = type;
        _value = value;
    }

    ObjectPtr getValue()
    {
        return _value;
    }

    int read( ByteArrayPtr data, int startOffset )
    {
        int offset = startOffset;

        _type = LittleEndian::getShort( data, offset );
        offset += LittleEndian::SHORT_SIZE;

        short padding = LittleEndian::getShort( data, offset );
        offset += LittleEndian::SHORT_SIZE;
        //if ( padding != 0 )
        //{
        //    logger.log( POILogger.WARN, "TypedPropertyValue padding at offset "
        //            + offset + " MUST be 0, but it's value is " + padding );
        //}

        offset += readValue( data, offset );

        return offset - startOffset;
    }

    int readValue( ByteArrayPtr data, int offset )
    {
        switch ( _type )
        {
        case VARENUM::VT_EMPTY:
        case VARENUM::VT_NULL:
			_value.reset();
            return 0;

        case VARENUM::VT_I2:
            _value.reset (new Number ( LittleEndian::getShort( data, offset ) ));
            return 4;

        case VARENUM::VT_I4:
			_value.reset(new Number (LittleEndian::getInt(data, offset)));
            return 4;

        case VARENUM::VT_R4:
			_value.reset(new Number(LittleEndian::getShort(data, offset)));
            return 4;

        case VARENUM::VT_R8:
			_value.reset(new Number(LittleEndian::getDouble(data, offset)));
            return 8;

		case VARENUM::VT_LPSTR:
		case VARENUM::VT_BSTR:
		{
			// 取出字符串数据
			int size = LittleEndian::getInt(data, offset);
			offset += LittleEndian::INT_SIZE;
			ByteArrayPtr buffer = LittleEndian::getByteArray2(data, offset, size);

			// 转换成带0结束的串
			ByteArrayPtr text(new ByteArray2(buffer->length() + 1));
			memcpy_s(text->get(), text->length(), buffer->get(), buffer->length());
			(*text)[buffer->length()] = 0;

			// 转换成字符串
			CString str ((LPCSTR)text->get());
			_value = VariantPtr(new Variant(str));
			return buffer->length();
		}
			//_value = new CodePageString(data, offset);
			//return ((CodePageString)_value).getSize();

		case VARENUM::VT_LPWSTR:
		{
			// 取出字符串数据
			int size = LittleEndian::getInt(data, offset);
			offset += LittleEndian::INT_SIZE;
			ByteArrayPtr buffer = LittleEndian::getByteArray2(data, offset, size * 2);

			// 转换成带0结束的串
			ByteArrayPtr text(new ByteArray2(buffer->length() + 2));
			memcpy_s(text->get(), text->length(), buffer->get(), buffer->length());
			(*text)[buffer->length()] = 0;
			(*text)[buffer->length()+1] = 0;

			// 转换成字符串
			CString str((LPCTSTR)text->get());
			_value = VariantPtr(new Variant(str));
			return buffer->length();
		}
			//_value = new UnicodeString(data, offset);
			//return ((UnicodeString)_value).getSize();

		//case VARENUM::VT_CY:
		//    _value = new Currency( data, offset );
		//    return Currency.SIZE;

		case VARENUM::VT_FILETIME:
		{
			ByteArrayPtr buffer = LittleEndian::getByteArray2(data, offset, 8);
			time_t time = *((__int64*)buffer->get());

			long rawOffset = 0;
			_get_timezone(&rawOffset);
			time -= (time_t)rawOffset * (time_t)10000000;

			SYSTEMTIME stime = { 0 };
			DateHelper::TimetToSystemTime(time, &stime);
			COleDateTime dt(stime);
			DatePtr date(new Date(dt));
			_value = date;
			return 8;
		}

		//case VARENUM::VT_DATE:
		//{
		//	ByteArrayPtr buffer = LittleEndian::getByteArray2(data, offset, 8);
		//	__int64 time = *((__int64*)buffer->get());
		//	DatePtr date(new Date(time /1000));
		//	_value = date;
		//}
		    //_value = new Date( data, offset );
		    //return Date.SIZE;

        //case VARENUM::VT_ERROR:
        //    _value = Long.valueOf( LittleEndian::getUInt( data, offset ) );
        //    return 4;

        //case VARENUM::VT_BOOL:
        //    _value = new VariantBool( data, offset );
        //    return VariantBool.SIZE;

        //case VARENUM::VT_DECIMAL:
        //    _value = new Decimal( data, offset );
        //    return Decimal.SIZE;

        //case VARENUM::VT_I1:
        //    _value = Byte.valueOf( data[offset] );
        //    return 1;

        //case VARENUM::VT_UI1:
        //    _value = Short.valueOf( LittleEndian::getUByte( data, offset ) );
        //    return 2;

        //case VARENUM::VT_UI2:
        //    _value = Integer.valueOf( LittleEndian::getUShort( data, offset ) );
        //    return 4;

        //case VARENUM::VT_UI4:
        //    _value = Long.valueOf( LittleEndian::getUInt( data, offset ) );
        //    return 4;

        //case VARENUM::VT_I8:
        //    _value = Long.valueOf( LittleEndian::getLong( data, offset ) );
        //    return 8;

        //case VARENUM::VT_UI8:
        //    _value = LittleEndian::getByteArray2( data, offset, 8 );
        //    return 8;

        //case VARENUM::VT_INT:
        //    _value = Integer.valueOf( LittleEndian::getInt( data, offset ) );
        //    return 4;

        //case VARENUM::VT_UINT:
        //    _value = Long.valueOf( LittleEndian::getUInt( data, offset ) );
        //    return 4;

        //case VARENUM::VT_BLOB:
        //    _value = new Blob( data, offset );
        //    return ( (Blob) _value ).getSize();

        //case VARENUM::VT_STREAM:
        //case VARENUM::VT_STORAGE:
        //case VARENUM::VT_STREAMED_OBJECT:
        //case VARENUM::VT_STORED_OBJECT:
        //    _value = new IndirectPropertyName( data, offset );
        //    return ( (IndirectPropertyName) _value ).getSize();

        //case VARENUM::VT_BLOB_OBJECT:
        //    _value = new Blob( data, offset );
        //    return ( (Blob) _value ).getSize();

        //case VARENUM::VT_CF:
        //    _value = new ClipboardData( data, offset );
        //    return ( (ClipboardData) _value ).getSize();

        //case VARENUM::VT_CLSID:
        //    _value = new GUID( data, offset );
        //    return GUID.SIZE;

        //case VARENUM::VT_VERSIONED_STREAM:
        //    _value = new VersionedStream( data, offset );
        //    return ( (VersionedStream) _value ).getSize();

        //case VARENUM::VT_VECTOR | VARENUM::VT_I2:
        //case VARENUM::VT_VECTOR | VARENUM::VT_I4:
        //case VARENUM::VT_VECTOR | VARENUM::VT_R4:
        //case VARENUM::VT_VECTOR | VARENUM::VT_R8:
        //case VARENUM::VT_VECTOR | VARENUM::VT_CY:
        //case VARENUM::VT_VECTOR | VARENUM::VT_DATE:
        //case VARENUM::VT_VECTOR | VARENUM::VT_BSTR:
        //case VARENUM::VT_VECTOR | VARENUM::VT_ERROR:
        //case VARENUM::VT_VECTOR | VARENUM::VT_BOOL:
        //case VARENUM::VT_VECTOR | VARENUM::VT_VARIANT:
        //case VARENUM::VT_VECTOR | VARENUM::VT_I1:
        //case VARENUM::VT_VECTOR | VARENUM::VT_UI1:
        //case VARENUM::VT_VECTOR | VARENUM::VT_UI2:
        //case VARENUM::VT_VECTOR | VARENUM::VT_UI4:
        //case VARENUM::VT_VECTOR | VARENUM::VT_I8:
        //case VARENUM::VT_VECTOR | VARENUM::VT_UI8:
        //case VARENUM::VT_VECTOR | VARENUM::VT_LPSTR:
        //case VARENUM::VT_VECTOR | VARENUM::VT_LPWSTR:
        //case VARENUM::VT_VECTOR | VARENUM::VT_FILETIME:
        //case VARENUM::VT_VECTOR | VARENUM::VT_CF:
        //case VARENUM::VT_VECTOR | VARENUM::VT_CLSID:
        //    _value = new Vector( (short) ( _type & 0x0FFF ) );
        //    return ( (Vector) _value ).read( data, offset );

        //case VARENUM::VT_ARRAY | VARENUM::VT_I2:
        //case VARENUM::VT_ARRAY | VARENUM::VT_I4:
        //case VARENUM::VT_ARRAY | VARENUM::VT_R4:
        //case VARENUM::VT_ARRAY | VARENUM::VT_R8:
        //case VARENUM::VT_ARRAY | VARENUM::VT_CY:
        //case VARENUM::VT_ARRAY | VARENUM::VT_DATE:
        //case VARENUM::VT_ARRAY | VARENUM::VT_BSTR:
        //case VARENUM::VT_ARRAY | VARENUM::VT_ERROR:
        //case VARENUM::VT_ARRAY | VARENUM::VT_BOOL:
        //case VARENUM::VT_ARRAY | VARENUM::VT_VARIANT:
        //case VARENUM::VT_ARRAY | VARENUM::VT_DECIMAL:
        //case VARENUM::VT_ARRAY | VARENUM::VT_I1:
        //case VARENUM::VT_ARRAY | VARENUM::VT_UI1:
        //case VARENUM::VT_ARRAY | VARENUM::VT_UI2:
        //case VARENUM::VT_ARRAY | VARENUM::VT_UI4:
        //case VARENUM::VT_ARRAY | VARENUM::VT_INT:
        //case VARENUM::VT_ARRAY | VARENUM::VT_UINT:
        //    _value = new Array();
        //    return ( (Array) _value ).read( data, offset );

        default:
            throw toolkit::Exception (L"Unknown (possibly, incorrect) TypedPropertyValue type: ");
        }
    }

    //int readValuePadded( ByteArrayPtr data, int offset )
    //{
    //    int nonPadded = readValue( data, offset );
    //    return ( nonPadded & 0x03 ) == 0 ? nonPadded : nonPadded
    //            + ( 4 - ( nonPadded & 0x03 ) );
    //}

private:
	int			_type;
	ObjectPtr	_value;
};