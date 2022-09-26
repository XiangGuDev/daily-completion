#include "stdafx.h"
#include "VarMeta12.h"

/**
* Constructor. Extracts that makes up this block from the input stream.
*
* @param is Input stream from which data is read
* @throws IOException on file read error
*/
VarMeta12::VarMeta12(BwProj::toolkit::IDataReadStream* is)
{
    if (readInt(is) != MAGIC)
    {
        throw toolkit::Exception(L"Bad magic number");
    }

    /*m_unknown1 =*/readInt(is);
    m_itemCount = readInt(is);
    /*m_unknown2 =*/readInt(is);
    /*m_unknown3 =*/readInt(is);
    m_dataSize = readInt(is);

	long uniqueID;
	int type;
	long offset;
	std::shared_ptr<Map2<int, int>> map;
    m_offsets.reset (new IntArray2(m_itemCount));

    for (int loop = 0; loop < m_itemCount; loop++)
    {
		uniqueID = readInt(is);
		offset = readInt(is);
		type = readShort(is);
        readShort(is); // unknown 2 bytes

		if (!m_table.Lookup(uniqueID, map) || map == NULL)
        {
			map.reset(new TreeMap<int, int>());
			m_table.put(uniqueID, map);
        }

        map->put(type, offset);
		(*m_offsets)[loop] = offset;// .intValue();
    }
}

VarMeta9::VarMeta9(BwProj::toolkit::IDataReadStream* is)
{
	if (readInt(is) != MAGIC)
	{
		throw toolkit::Exception(L"Bad magic number");
	}

	/*m_unknown1 =*/readInt(is);
	m_itemCount = readInt(is);
	/*m_unknown2 =*/readInt(is);
	/*m_unknown3 =*/readInt(is);
	m_dataSize = readInt(is);

	long uniqueID = 0;
	int type;
	long offset;
	std::shared_ptr<Map2<int, int>> map;
	m_offsets.reset(new IntArray2(m_itemCount));

	for (int loop = 0; loop < m_itemCount; loop++)
	{
		//uniqueID = readInt(is);
		DWORD ReadBytes = 0;
		is->Read((LPVOID)&uniqueID, 3, ReadBytes);
		type = readByte(is);
		offset = readInt(is);
		
		//readShort(is); // unknown 2 bytes

		if (!m_table.Lookup(uniqueID, map) || map == NULL)
		{
			map.reset(new TreeMap<int, int>());
			m_table.put(uniqueID, map);
		}

		map->put(type, offset);
		(*m_offsets)[loop] = offset;// .intValue();
	}
}