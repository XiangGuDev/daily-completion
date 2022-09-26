#include "stdafx.h"
#include "Props12.h"
#include "MPPUtility.h"

/**
* Constructor, reads the property data from an input stream.
*
* @param is input stream for reading props data
*/
Props12::Props12(BwProj::toolkit::IDataReadStream* is)
{
    //FileOutputStream fos = new FileOutputStream ("c:\\temp\\props12." + System.currentTimeMillis() + ".txt");
    //PrintWriter pw = new PrintWriter (fos);

	ByteArrayPtr header(new ByteArray2(16));
    ByteArrayPtr data;
	DWORD readBytes = 0;
	is->Read(header->get(), header->length(), readBytes);

    int headerCount = MPPUtility::getShort(header.get(), 12);
    int foundCount = 0;
    int availableBytes = is->available();

    while (foundCount < headerCount)
    {
        int attrib1 = readInt(is);
        int attrib2 = readInt(is);
        /*int attrib3 = */readInt(is);
        availableBytes -= 12;

        if (availableBytes < attrib1 || attrib1 < 1)
        {
			break;
        }

        data.reset(new ByteArray2(attrib1));
        is->Read(data->get(), data->length(), readBytes);
        availableBytes -= attrib1;

		m_map.put(attrib2, data);
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