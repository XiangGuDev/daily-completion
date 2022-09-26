#include "stdafx.h"
#include "FixedMeta.h"

/**
* Constructor. Reads the meta data from an input stream. Note that
* this version of the constructor copes with more MSP inconsistencies.
* We already know the block size, so we ignore the item count in the
* block and work it out for ourselves.
*
* @param is input stream from which the meta data is read
* @param itemSize size of each item in the block
* @throws IOException on file read failure
*/
FixedMeta::FixedMeta(BwProj::toolkit::IDataReadStream* is, int itemSize)
{
	class Argv : public FixedMetaItemSizeProvider
	{
	public:
		int _itemSize;
		Argv(int itemSize)
		{
			_itemSize = itemSize;
		}
		virtual int getItemSize(int fileSize, int itemCount)
		{
			return _itemSize;
		}
	};

	Argv argv(itemSize);

	FixedMetaProc(is, &argv);
}

/**
* Constructor. Supply an item size provider to allow different strategies to be
* used to determine the correct item size.
* 
* @param is input stream from which the meta data is read
* @param itemSizeProvider item size provider used to calculate the item size
* @throws IOException
*/
FixedMeta::FixedMeta(BwProj::toolkit::IDataReadStream* is, FixedMetaItemSizeProvider* itemSizeProvider)
{
	FixedMetaProc(is, itemSizeProvider);
}

/**
* Constructor, allowing a selection of possible block sizes to be supplied.
*
* @param is input stream
* @param itemSizes list of potential block sizes
*/
FixedMeta::FixedMeta(BwProj::toolkit::IDataReadStream* is, int itemSizes[], int length)
{
	class Argv : public FixedMetaItemSizeProvider
	{
	public:
		int* _itemSizes;
		int _length;

		Argv(int* itemSizes, int length)
		{
			_itemSizes = itemSizes;
			_length = length;
		}
		virtual int getItemSize(int fileSize, int itemCount)
		{
			int itemSize = _itemSizes[0];	// 92
			int available = fileSize - HEADER_SIZE;	//76
			long distance = MINLONG;// Integer.MIN_VALUE;	//-2147483648

			for (int index = 0; index < _length; index++)
			{
				int testItemSize = _itemSizes[index];
				if (available % testItemSize == 0)
				{
					int testDistance = (itemCount * testItemSize) - available;
					if (testDistance <= 0 && testDistance > distance)
					{
						itemSize = testItemSize;
						distance = testDistance;
					}
				}
			}

			return itemSize;
		}
	};

	Argv argv(itemSizes, length);

	FixedMetaProc(is, &argv);
}

void FixedMeta::FixedMetaProc(BwProj::toolkit::IDataReadStream* is, FixedMetaItemSizeProvider* itemSizeProvider)
{
	//
	// The POI file system guarantees that this is accurate
	//
	int fileSize = is->available();

	//
	// First 4 bytes
	//
	if (readInt(is) != MAGIC)
	{
		throw /*BwProj::*/toolkit::Exception(L"Bad magic number");
	}

	readInt(is);
	m_itemCount = readInt(is);
	readInt(is);

	int itemSize = itemSizeProvider->getItemSize(fileSize, m_itemCount);
	m_adjustedItemCount = (fileSize - HEADER_SIZE) / itemSize;

	m_array.reset (new ObjectPtrArray (m_adjustedItemCount));

	for (int loop = 0; loop < m_adjustedItemCount; loop++)
	{
		(*m_array)[loop] = readByteArray2(is, itemSize);
	}
}

///**
// * Constructor, allowing a selection of possible block sizes to be supplied.
// * 
// * @param is input stream
// * @param itemSizes list of potential block sizes
// */
//FixedMeta(InputStream is, final int... itemSizes)
//{
//   this(is, new FixedMetaItemSizeProvider()
//   {
//      @Override public int getItemSize(int fileSize, int itemCount)
//      {
//         int itemSize = itemSizes[0];
//         int available = fileSize - HEADER_SIZE;
//         int distance = Integer.MIN_VALUE;

//         for (int index = 0; index < itemSizes.length; index++)
//         {
//            int testItemSize = itemSizes[index];
//            if (available % testItemSize == 0)
//            {
//               int testDistance = (itemCount * testItemSize) - available;
//               if (testDistance <= 0 && testDistance > distance)
//               {
//                  itemSize = testItemSize;
//                  distance = testDistance;
//               }
//            }
//         }

//         return itemSize;
//      }
//   });
//}

/**
* This method retrieves the number of items in the FixedData block, as reported in the block header.
*
* @return number of items in the fixed data block
*/
int FixedMeta::getItemCount()
{
    return (m_itemCount);
}

/**
* This method retrieves the number of items in the FixedData block.
* Where we don't trust the number of items reported by the block header
* this value is adjusted based on what we know about the block size
* and the size of the individual items.
*
* @return number of items in the fixed data block
*/
int FixedMeta::getAdjustedItemCount()
{
    return (m_adjustedItemCount);
}

/**
* This method retrieves a byte array containing the data at the
* given index in the block. If no data is found at the given index
* this method returns null.
*
* @param index index of the data item to be retrieved
* @return byte array containing the requested data
*/
ByteArrayPtr FixedMeta::getByteArray2Value(int index)
{
    ByteArrayPtr result;

    if (index >= 0 && index < (int) m_array->length() && (*m_array)[index].get())
    {
        result = std::dynamic_pointer_cast<ByteArray2>((*m_array)[index]);
    }

    return (result);
}

///**
//* This method dumps the contents of this FixedMeta block as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this block
//*/
//CString FixedMeta::toString()
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.println("BEGIN: FixedMeta");
//    pw.println("   Adjusted Item count: " + m_adjustedItemCount);
//
//    for (int loop = 0; loop < m_adjustedItemCount; loop++)
//    {
//        pw.println("   Data at index: " + loop);
//        pw.println("  " + MPPUtility.hexdump((ByteArrayPtr) m_array[loop], true));
//    }
//
//    pw.println("END: FixedMeta");
//    pw.println();
//
//    pw.close();
//    return (sw.toString());
//}