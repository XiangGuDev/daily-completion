#include "stdafx.h"
#include "LittleEndian.h"

ByteArrayPtr LittleEndian::getByteArray2(ByteArrayPtr data, int offset, int size)
{
	if ((int)data->length() - offset < size)
		throw toolkit::Exception(L"size error.");

	ByteArrayPtr sub(new ByteArray2(size));
	memcpy_s(sub->get(), sub->length(), &(*data)[offset], size);

	return sub;
}

__int32 LittleEndian::getInt(ByteArrayPtr data, int offset)
{
	__int32 i = offset;
	int b0 = (*data)[i++] & 0xFF;
	int b1 = (*data)[i++] & 0xFF;
	int b2 = (*data)[i++] & 0xFF;
	int b3 = (*data)[i++] & 0xFF;
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + (b0 << 0);
}

void LittleEndian::putInt(ByteArrayPtr data, int offset, __int32 value)
{
	int i = offset;
	(*data)[i++] = (BYTE)((value >> 0) & 0xFF);
	(*data)[i++] = (BYTE)((value >> 8) & 0xFF);
	(*data)[i++] = (BYTE)((value >> 16) & 0xFF);
	(*data)[i++] = (BYTE)((value >> 24) & 0xFF);
}

__int64 LittleEndian::getLong(ByteArrayPtr data, int offset)
{
	__int64 result = 0xff & (*data)[offset + 7];

	for (int j = offset + LONG_SIZE - 1; j >= offset; j--)
	{
		result <<= 8;
		result |= 0xff & (*data)[j];
	}
	return result;
}

void LittleEndian::putLong(ByteArrayPtr data, int offset, __int64 value)
{
	(*data)[offset + 0] = (BYTE)((value >> 0) & 0xFF);
	(*data)[offset + 1] = (BYTE)((value >> 8) & 0xFF);
	(*data)[offset + 2] = (BYTE)((value >> 16) & 0xFF);
	(*data)[offset + 3] = (BYTE)((value >> 24) & 0xFF);
	(*data)[offset + 4] = (BYTE)((value >> 32) & 0xFF);
	(*data)[offset + 5] = (BYTE)((value >> 40) & 0xFF);
	(*data)[offset + 6] = (BYTE)((value >> 48) & 0xFF);
	(*data)[offset + 7] = (BYTE)((value >> 56) & 0xFF);
}

long LittleEndian::getUInt(ByteArrayPtr data)
{
	return getUInt(data, 0);
}
