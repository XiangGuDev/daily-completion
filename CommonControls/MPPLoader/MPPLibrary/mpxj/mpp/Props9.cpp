#include "stdafx.h"
#include "Props9.h"
#include "MPPUtility.h"

/**
	* Constructor, reads the property data from an input stream.
	*
	* @param is input stream for reading props data
	*/
Props9::Props9(BwProj::toolkit::IDataReadStream* is)
{
	//FileOutputStream fos = new FileOutputStream ("c:\\temp\\props9." + System.currentTimeMillis() + ".txt");
	//PrintWriter pw = new PrintWriter (fos);

	ByteArrayPtr header(new ByteArray2(16));
	ByteArrayPtr data; DWORD readBytes = 0;
	is->Read(header->get(), header->length(), readBytes);

	int headerCount = MPPUtility::getShort(header.get(), 12);
	int foundCount = 0;
	int availableBytes = is->available();

	while (foundCount < headerCount)
	{
		int itemSize = readInt(is);
		int itemKey = readInt(is);
		/*int attrib3 = */readInt(is);
		availableBytes -= 12;

		if (availableBytes < itemSize || itemSize < 1)
		{
			break;
		}

		data.reset(new ByteArray2(itemSize));
		is->Read(data->get(), data->length(), readBytes);
		availableBytes -= itemSize;

		m_map.put(itemKey, data);
		//pw.println(foundCount + " "+ attrib2 + ": " + MPPUtility.hexdump(data, true));
		++foundCount;

		//
		// Align to two byte boundary
		//
		if (data->length() % 2 != 0)
		{
			is->skip(1);
		}
	}

	//pw.flush();
	//pw.close();
}