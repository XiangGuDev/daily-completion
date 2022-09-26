#include "stdafx.h"
#include "POIFSFileSystem.h"
#include "DocumentNode.h"
#include "internal\LongField.h"
#include "internal\BATManaged.h"
#include "internal\BlockAllocationTableWriter.h"
#include "internal\HeaderBlockWriter.h"
#include "internal\SmallBlockTableReader.h"
#include "internal\SmallBlockTableWriter.h"

/**
* Convenience method for clients that want to avoid the auto-close behaviour of the constructor.
*/
BwProj::toolkit::IDataReadStream* POIFSFileSystem::createNonClosingInputStream(BwProj::toolkit::IDataReadStream* is)
{
	return is;
	//return new CloseIgnoringInputStream(is);
}

/**
* Constructor, intended for writing
*/
POIFSFileSystem::POIFSFileSystem()
{
	HeaderBlockPtr header_block (new HeaderBlock(bigBlockSize));
	_property_table.reset (new PropertyTable(header_block));
	_documents.reset(new POIFSDocumentList());
	//_root = null;
}

/**
* Create a POIFSFileSystem from an <tt>BwProj::toolkit::IDataReadStream*</tt>.  Normally the stream is read until
* EOF.  The stream is always closed.<p/>
*
* Some streams are usable after reaching EOF (typically those that return <code>true</code>
* for <tt>markSupported()</tt>).  In the unlikely case that the caller has such a stream
* <i>and</i> needs to use it after this constructor completes, a work around is to wrap the
* stream in order to trap the <tt>close()</tt> call.  A convenience method (
* <tt>createNonClosingInputStream()</tt>) has been provided for this purpose:
* <pre>
* BwProj::toolkit::IDataReadStream* wrappedStream = POIFSFileSystem.createNonClosingInputStream(is);
* HSSFWorkbook wb = new HSSFWorkbook(wrappedStream);
* is.reset();
* doSomethingElse(is);
* </pre>
* Note also the special case of <tt>ByteArray2InputStream</tt> for which the <tt>close()</tt>
* method does nothing.
* <pre>
* ByteArray2InputStream bais = ...
* HSSFWorkbook wb = new HSSFWorkbook(bais); // calls bais.close() !
* bais.reset(); // no problem
* doSomethingElse(bais);
* </pre>
*
* @param stream the BwProj::toolkit::IDataReadStream* from which to read the data
*
* @exception IOException on errors reading, or on invalid data
*/

POIFSFileSystem::POIFSFileSystem(BwProj::toolkit::IDataReadStream* stream)
:POIFSFileSystem()
{
	//this();
	bool success = false;

	HeaderBlockPtr header_block;
	RawDataBlockListPtr data_blocks;
	//try {
		// read the header block from the stream
		header_block.reset (new HeaderBlock(stream));
		bigBlockSize = header_block->getBigBlockSize();

		// read the rest of the stream into blocks
		data_blocks.reset (new RawDataBlockList(stream, bigBlockSize));
		success = true;
	//}
	//catch {
	//	closeInputStream(stream, success);
	//}


	// set up the block allocation table (necessary for the
	// data_blocks to be manageable
	BlockAllocationTableReaderPtr tabReader(new BlockAllocationTableReader(
		header_block->getBigBlockSize(), header_block->getBATCount(), header_block->getBATArray(),
		header_block->getXBATCount(), header_block->getXBATIndex(), data_blocks));
	data_blocks->setBAT(tabReader);

	// get property table from the document
	PropertyTablePtr properties (new PropertyTable(header_block, data_blocks));

	// init documents
	BlockListPtr blockList = SmallBlockTableReader::getSmallDocumentBlocks(bigBlockSize, 
		data_blocks, properties->getRoot(), header_block->getSBATStart());

	processProperties(blockList, data_blocks,
		properties->getRoot()->getChildren(), NULL, header_block->getPropertyStart());

	// For whatever reason CLSID of root is always 0.
	getRoot()->setStorageClsid(properties->getRoot()->getStorageClsid());
}
/**
* @param stream the stream to be closed
* @param success <code>false</code> if an exception is currently being thrown in the calling method
*/
void POIFSFileSystem::closeInputStream(BwProj::toolkit::IDataReadStream* stream, bool success)
{
	//if (stream.markSupported() && !(stream instanceof ByteArray2InputStream)) {
	//	CString msg = "POIFS is closing the supplied input stream of type ("
	//		+ stream.getClass().getName() + ") which supports mark/reset.  "
	//		+ "This will be a problem for the caller if the stream will still be used.  "
	//		+ "If that is the case the caller should wrap the input stream to avoid this close logic.  "
	//		+ "This warning is only temporary and will not be present in future versions of POI.";
	//	_logger.log(POILogger.WARN, msg);
	//}
	//try {
	//	stream.close();
	//}
	//catch (IOException e) {
	//	if (success) {
	//		throw new RuntimeException(e);
	//	}
	//	// else not success? Try block did not complete normally
	//	// just print stack trace and leave original ex to be thrown
	//	e.printStackTrace();
	//}
}

/**
* Checks that the supplied BwProj::toolkit::IDataReadStream* (which MUST
*  support mark and reset, or be a PushbackInputStream)
*  has a POIFS (OLE2) header at the start of it.
* If your BwProj::toolkit::IDataReadStream* does not support mark / reset,
*  then wrap it in a PushBackInputStream, then be
*  sure to always use that, and not the original!
* @param inp An BwProj::toolkit::IDataReadStream* which supports either mark/reset, or is a PushbackInputStream
*/
bool POIFSFileSystem::hasPOIFSHeader(BwProj::toolkit::IDataReadStream* inp)
{
	// We want to peek at the first 8 bytes
	//inp.mark(8);

	ByteArrayPtr header(new ByteArray2(8));
	//IOUtils.readFully(inp, header);
	DWORD readBytes = 0;
	inp->Read(header->get(), header->length(), readBytes);
	if (readBytes < 8) return false;

	LongField signature (HeaderBlockConstants::_signature_offset, header);

	//// Wind back those 8 bytes
	//if (inp instanceof PushbackInputStream) {
	//	PushbackInputStream pin = (PushbackInputStream)inp;
	//	pin.unread(header);
	//}
	//else {
		inp->reset();
	//}

	// Did it match the signature?
	return (signature.get() == HeaderBlockConstants::_signature);
}

/**
* Create a new document to be added to the root directory
*
* @param stream the BwProj::toolkit::IDataReadStream* from which the document's data
*               will be obtained
* @param name the name of the new POIFSDocument
*
* @return the new DocumentEntryPtr
*
* @exception IOException on error creating the new POIFSDocument
*/

DocumentEntryPtr POIFSFileSystem::createDocument(BwProj::toolkit::IDataReadStream* stream, CString name)
{
	return getRoot()->createDocument(name, stream);
}

/**
* create a new DocumentEntryPtr in the root entry; the data will be
* provided later
*
* @param name the name of the new DocumentEntryPtr
* @param size the size of the new DocumentEntryPtr
* @param writer the writer of the new DocumentEntryPtr
*
* @return the new DocumentEntryPtr
*
* @exception IOException
*/

DocumentEntryPtr POIFSFileSystem::createDocument(CString name, int size)
{
	return getRoot()->createDocument(name, size);
}

/**
* create a new DirectoryEntry in the root directory
*
* @param name the name of the new DirectoryEntry
*
* @return the new DirectoryEntry
*
* @exception IOException on name duplication
*/

DirectoryEntryPtr POIFSFileSystem::createDirectory(CString name)
{
	return getRoot()->createDirectory(name);
}

/**
* Write the filesystem out
*
* @param stream the BwProj::toolkit::IDataWriteStream* to which the filesystem will be
*               written
*
* @exception IOException thrown on errors writing to the stream
*/

void POIFSFileSystem::writeFilesystem(BwProj::toolkit::IDataWriteStream* stream)
{

	// get the property table ready
	_property_table->preWrite();

	// create the small block store, and the SBAT
	SmallBlockTableWriterPtr sbtw(new SmallBlockTableWriter(bigBlockSize, _documents, _property_table->getRoot()));

	// create the block allocation table
	BlockAllocationTableWriterPtr bat (new BlockAllocationTableWriter(bigBlockSize));

	// create a list of BATManaged objects: the documents plus the
	// property table and the small block table
	List<ObjectPtr> bm_objects;// = new ArrayList<Object>();

	for (size_t i = 0; i < _documents->GetCount(); i++)
		bm_objects.Add(_documents->GetAt(i));
	//bm_objects.Append(*_documents);
	bm_objects.Add(_property_table);
	bm_objects.Add(sbtw);
	bm_objects.Add(sbtw->getSBAT());

	// walk the list, allocating space for each and assigning each
	// a starting block number
	Iterator<ObjectPtr> iter = bm_objects.iterator();

	while (iter.hasNext())
	{
		BATManagedPtr bmo = std::dynamic_pointer_cast<BATManaged>(iter.next());
		int        block_count = bmo->countBlocks();

		if (block_count != 0)
		{
			bmo->setStartBlock(bat->allocateSpace(block_count));
		}
		else
		{

			// Either the BATManaged object is empty or its data
			// is composed of SmallBlocks; in either case,
			// allocating space in the BAT is inappropriate
		}
	}

	// allocate space for the block allocation table and take its
	// starting block
	int batStartBlock = bat->createBlocks();

	// get the extended block allocation table blocks
	HeaderBlockWriterPtr header_block_writer (new HeaderBlockWriter(bigBlockSize));
	BATBlockArrayPtr xbat_blocks (header_block_writer->setBATBlocks(bat->countBlocks(), batStartBlock));

	// set the property table start block
	header_block_writer->setPropertyStart(_property_table->getStartBlock());

	// set the small block allocation table start block
	header_block_writer->setSBATStart(sbtw->getSBAT()->getStartBlock());

	// set the small block allocation table block count
	header_block_writer->setSBATBlockCount(sbtw->getSBATBlockCount());

	// the header is now properly initialized. Make a list of
	// writers (the header block, followed by the documents, the
	// property table, the small block store, the small block
	// allocation table, the block allocation table, and the
	// extended block allocation table blocks)
	List<ObjectPtr> writers;// = new ArrayList<Object>();

	writers.Add(header_block_writer);
	
	//writers.Append(_documents);
	for (size_t i = 0; i < _documents->GetCount(); i++)
		writers.Add(_documents->GetAt(i));

	writers.Add(_property_table);
	writers.Add(sbtw);
	writers.Add(sbtw->getSBAT());
	writers.Add(bat);
	for (size_t j = 0; j < xbat_blocks->length(); j++)
	{
		BATBlockPtr block = (*xbat_blocks)[j];
		writers.Add(block);
	}

	// now, write everything out
	iter = writers.iterator();
	while (iter.hasNext())
	{
		BlockWritablePtr writer = std::dynamic_pointer_cast<BlockWritable>(iter.next());

		writer->writeBlocks(stream);
	}
}

/**
* read in a file and write it back out again
*
* @param args names of the files; arg[ 0 ] is the input file,
*             arg[ 1 ] is the output file
*
* @exception IOException
*/

void POIFSFileSystem::test(CString arg1, CString arg2)
{
	//FileInputStream  istream = new FileInputStream(arg1);
	//FileOutputStream ostream = new FileOutputStream(arg2);

	//new POIFSFileSystem(istream).writeFilesystem(ostream);
	//istream.close();
	//ostream.close();
}

/**
* get the root entry
*
* @return the root entry
*/

DirectoryNodePtr POIFSFileSystem::getRoot()
{
	if (!_root)
	{
		_root.reset(new DirectoryNode(_property_table->getRoot(), this, NULL));
	}
	return _root;
}

/**
* open a document in the root entry's list of entries
*
* @param documentName the name of the document to be opened
*
* @return a newly opened DocumentInputStream
*
* @exception IOException if the document does not exist or the
*            name is that of a DirectoryEntry
*/

DocumentInputStreamPtr POIFSFileSystem::createDocumentInputStream(CString documentName)
{
	return getRoot()->createDocumentInputStream(documentName);
}

/**
* add a new POIFSDocument
*
* @param document the POIFSDocument being added
*/

void POIFSFileSystem::addDocument(POIFSDocumentPtr document)
{
	_documents->Add(document);
	_property_table->addProperty(document->getDocumentProperty());
}

/**
* add a new DirectoryProperty
*
* @param directory the DirectoryProperty being added
*/

void POIFSFileSystem::addDirectory(DirectoryPropertyPtr directory)
{
	_property_table->addProperty(directory);
}

/**
* remove an entry
*
* @param entry to be removed
*/

void POIFSFileSystem::remove(EntryNodePtr entry)
{
	_property_table->removeProperty(entry->getProperty());
	if (entry->isDocumentEntry())
	{
		_documents->remove((std::dynamic_pointer_cast<DocumentNode>(entry))->getDocument());
	}
}

void POIFSFileSystem::processProperties(BlockListPtr small_blocks, BlockListPtr big_blocks,
	Iterator<PropertyPtr> properties, DirectoryNodePtr dir, int headerPropertiesStartAt)
{
	while (properties.hasNext())
	{
		PropertyPtr      prop = properties.next();
		CString        name = prop->getName();
		DirectoryNodePtr parent = (!dir)
			? std::dynamic_pointer_cast<DirectoryNode>(getRoot())
			: dir;

		if (prop->isDirectory())
		{
			DirectoryNodePtr new_dir =
				std::dynamic_pointer_cast<DirectoryNode>(parent->createDirectory(name));

			new_dir->setStorageClsid(prop->getStorageClsid());

			processProperties(
				small_blocks, big_blocks,
				(std::dynamic_pointer_cast<DirectoryProperty>(prop))->getChildren(),
				new_dir, headerPropertiesStartAt);
		}
		else
		{
			int           startBlock = prop->getStartBlock();
			int           size = prop->getSize();
			POIFSDocumentPtr document;

			if (prop->shouldUseSmallBlocks())
			{
				document.reset(
					new POIFSDocument(name,
					small_blocks->fetchBlocks(startBlock, headerPropertiesStartAt),
					size));
				document->getDocumentProperty()->setDocument(document);
				//_property->setDocument(shared_from_this());
			}
			else
			{
				document.reset(
					new POIFSDocument(name,
					big_blocks->fetchBlocks(startBlock, headerPropertiesStartAt),
					size));
				document->getDocumentProperty()->setDocument(document);
			}
			parent->createDocument(document);
		}
	}
}

/**
* @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
*/
int POIFSFileSystem::getBigBlockSize()
{
	return bigBlockSize.getBigBlockSize();
}
/**
* @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
*/
POIFSBigBlockSize POIFSFileSystem::getBigBlockSizeDetails()
{
	return bigBlockSize;
}