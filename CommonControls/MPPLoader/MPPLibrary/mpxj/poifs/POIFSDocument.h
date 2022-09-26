#pragma once

#include "internal\BATManaged.h"
#include "internal\BlockWritable.h"
#include "internal\DocumentProperty.h"
#include "internal\DocumentBlock.h"
#include "internal\SmallDocumentBlock.h"
#include "internal\RawDataBlock.h"
#include "POIFSDocumentPath.h"

/**
 * This class manages a document in the POIFS filesystem.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class POIFSDocument : public BATManaged, public BlockWritable, public std::enable_shared_from_this<POIFSDocument>
{
public:
	/**
 * Constructor from large blocks
 *
 * @param name the name of the POIFSDocument
 * @param blocks the big blocks making up the POIFSDocument
 * @param length the actual length of the POIFSDocument
 */
	POIFSDocument(CString name, RawDataBlockArrayPtr blocks, int length);

	/**
	 * Constructor from small blocks
	 *
	 * @param name the name of the POIFSDocument
	 * @param blocks the small blocks making up the POIFSDocument
	 * @param length the actual length of the POIFSDocument
	 */
	POIFSDocument(CString name, SmallDocumentBlockArrayPtr blocks, int length);

	/**
	 * Constructor from small blocks
	 *
	 * @param name the name of the POIFSDocument
	 * @param blocks the small blocks making up the POIFSDocument
	 * @param length the actual length of the POIFSDocument
	 */
	POIFSDocument(CString name, POIFSBigBlockSize bigBlockSize, ListManagedBlockArrayPtr blocks, int length);

	POIFSDocument(CString name, ListManagedBlockArrayPtr blocks, int length);

	/**
	 * Constructor
	 *
	 * @param name the name of the POIFSDocument
	 * @param stream the BwProj::toolkit::IDataReadStream* we read data from
	 */
	POIFSDocument(CString name, POIFSBigBlockSize bigBlockSize, BwProj::toolkit::IDataReadStream* stream);

	POIFSDocument(CString name, BwProj::toolkit::IDataReadStream* stream);

	/**
	 * Constructor
	 *
	 * @param name the name of the POIFSDocument
	 * @param size the length of the POIFSDocument
	 * @param path the path of the POIFSDocument
	 * @param writer the writer who will eventually write the document contents
	 */
	POIFSDocument(CString name, int size, POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path);

	POIFSDocument(CString name, int size, POIFSDocumentPathPtr path);

	/**
	 * @return array of SmallDocumentBlocks; may be empty, cannot be null
	 */
	SmallDocumentBlockArrayPtr getSmallBlocks();

	/**
	 * @return size of the document
	 */
	int getSize();

	/**
	 * Write the storage to an BwProj::toolkit::IDataWriteStream*
	 *
	 * @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should be written
	 */
	void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);

	/**
	 * Return the number of BigBlock's this instance uses
	 *
	 * @return count of BigBlock instances
	 */
	int countBlocks();

	/**
	 * Set the start block for this instance
	 *
	 * @param index index into the array of blocks making up the filesystem
	 */
	void setStartBlock(int index);

	/**
	* @return the instance's DocumentProperty
	*/

	DocumentPropertyPtr getDocumentProperty();

	/**
	* @return <code>null</code> if <tt>offset</tt> points to the end of the document stream
	*/
	DataInputBlockPtr getDataInputBlock(int offset);

	class SmallBlockStore
	{
	public:
		/**
		 * Constructor
		 *
		 * @param blocks blocks to construct the store from
		 */
		SmallBlockStore(POIFSBigBlockSize bigBlockSize, SmallDocumentBlockArrayPtr blocks);

		/**
		 * Constructor for a small block store that will be written later
		 *
		 * @param path path of the document
		 * @param name name of the document
		 * @param size length of the document
		 * @param writer the object that will eventually write the document
		 */
		SmallBlockStore(POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path, CString name, int size);

		/**
		 * @return <code>true</code> if this store is a valid source of data
		 */
		bool isValid();

		/**
		 * @return the SmallDocumentBlocks
		 */
		SmallDocumentBlockArrayPtr getBlocks();

	private:
		SmallDocumentBlockArrayPtr _smallBlocks;
		POIFSDocumentPathPtr _path;
		CString _name;
		int _size;
		POIFSBigBlockSize _bigBlockSize;
	}; // end private class SmallBlockStore

	class BigBlockStore
	{
	public:
		/**
		 * Constructor
		 *
		 * @param blocks the blocks making up the store
		 */
		BigBlockStore(POIFSBigBlockSize bigBlockSize, DocumentBlockArrayPtr blocks);

		/**
		 * Constructor for a big block store that will be written later
		 *
		 * @param path path of the document
		 * @param name name of the document
		 * @param size length of the document
		 * @param writer the object that will eventually write the document
		 */
		BigBlockStore(POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path, CString name, int size);

		/**
		 * @return <code>true</code> if this store is a valid source of data
		 */
		bool isValid();

		/**
		 * @return the DocumentBlocks
		 */
		DocumentBlockArrayPtr getBlocks();

		/**
		 * write the blocks to a stream
		 *
		 * @param stream the stream to which the data is to be written
		 */
		void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);

		/**
		 * @return number of big blocks making up this document
		 */
		int countBlocks();

	private:
		DocumentBlockArrayPtr bigBlocks;
		POIFSDocumentPathPtr _path;
		CString _name;
		int _size;
		POIFSBigBlockSize _bigBlockSize;
	}; // end private class BigBlockStore

protected:
	// TODO - awkward typing going on here
	static DocumentBlockArrayPtr convertRawBlocksToBigBlocks(ListManagedBlockArrayPtr blocks);

	static SmallDocumentBlockArrayPtr convertRawBlocksToSmallBlocks(ListManagedBlockArrayPtr blocks);

	/**
	* read data from the internal stores
	*
	* @param buffer the buffer to write to
	* @param offset the offset into our storage to read from
	* This method is currently (Oct 2008) only used by test code. Perhaps it can be deleted
	*/
	void read(ByteArrayPtr buffer, int offset);

private:
	static DocumentBlockArrayPtr EMPTY_BIG_BLOCK_ARRAY;// = {};
	static SmallDocumentBlockArrayPtr EMPTY_SMALL_BLOCK_ARRAY;// = {};
	DocumentPropertyPtr _property;
	int _size;

	POIFSBigBlockSize _bigBigBlockSize;

	// one of these stores will be valid
	std::shared_ptr<SmallBlockStore>  _small_store;
	std::shared_ptr<BigBlockStore>	 _big_store;
};

typedef std::shared_ptr<POIFSDocument> POIFSDocumentPtr;

typedef List<POIFSDocumentPtr> POIFSDocumentList;
typedef std::shared_ptr<POIFSDocumentList> POIFSDocumentListPtr;