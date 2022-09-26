#pragma once

#include "internal\DirectoryProperty.h"
#include "internal\POIFSConstants.h"
#include "internal\BlockList.h"
#include "internal\PropertyTable.h"

#include "DirectoryNode.h"
#include "POIFSDocument.h"

/**
 * This is the main class of the POIFS system; it manages the entire
 * life cycle of the filesystem.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class POIFSFileSystem// : public std::enable_shared_from_this<POIFSFileSystem>
{
	friend class DirectoryNode;
public:
	/**
	* Convenience method for clients that want to avoid the auto-close behaviour of the constructor.
	*/
	static BwProj::toolkit::IDataReadStream* createNonClosingInputStream(BwProj::toolkit::IDataReadStream* is);

	/**
	 * Constructor, intended for writing
	 */
	POIFSFileSystem();

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

	POIFSFileSystem(BwProj::toolkit::IDataReadStream* stream);

	/**
	 * Checks that the supplied BwProj::toolkit::IDataReadStream* (which MUST
	 *  support mark and reset, or be a PushbackInputStream)
	 *  has a POIFS (OLE2) header at the start of it.
	 * If your BwProj::toolkit::IDataReadStream* does not support mark / reset,
	 *  then wrap it in a PushBackInputStream, then be
	 *  sure to always use that, and not the original!
	 * @param inp An BwProj::toolkit::IDataReadStream* which supports either mark/reset, or is a PushbackInputStream
	 */
	static bool hasPOIFSHeader(BwProj::toolkit::IDataReadStream* inp);

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

	DocumentEntryPtr createDocument(BwProj::toolkit::IDataReadStream* stream, CString name);

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

	DocumentEntryPtr createDocument(CString name, int size);

	/**
	 * create a new DirectoryEntry in the root directory
	 *
	 * @param name the name of the new DirectoryEntry
	 *
	 * @return the new DirectoryEntry
	 *
	 * @exception IOException on name duplication
	 */

	DirectoryEntryPtr createDirectory(CString name);

	/**
	 * Write the filesystem out
	 *
	 * @param stream the BwProj::toolkit::IDataWriteStream* to which the filesystem will be
	 *               written
	 *
	 * @exception IOException thrown on errors writing to the stream
	 */

	void writeFilesystem(BwProj::toolkit::IDataWriteStream* stream);

	/**
	 * read in a file and write it back out again
	 *
	 * @param args names of the files; arg[ 0 ] is the input file,
	 *             arg[ 1 ] is the output file
	 *
	 * @exception IOException
	 */

	static void test(CString arg1, CString arg2);

	/**
	 * get the root entry
	 *
	 * @return the root entry
	 */

	DirectoryNodePtr getRoot();

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

	DocumentInputStreamPtr createDocumentInputStream(CString documentName);

	/**
	 * @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
	 */
	int getBigBlockSize();

	/**
	 * @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
	 */
	POIFSBigBlockSize getBigBlockSizeDetails();

protected:
	/**
	* @param stream the stream to be closed
	* @param success <code>false</code> if an exception is currently being thrown in the calling method
	*/
	void closeInputStream(BwProj::toolkit::IDataReadStream* stream, bool success);

	/**
	* add a new POIFSDocument
	*
	* @param document the POIFSDocument being added
	*/

	void addDocument(POIFSDocumentPtr document);

	/**
	* add a new DirectoryProperty
	*
	* @param directory the DirectoryProperty being added
	*/

	void addDirectory(DirectoryPropertyPtr directory);

	/**
	* remove an entry
	*
	* @param entry to be removed
	*/

	void remove(EntryNodePtr entry);

	void processProperties(BlockListPtr small_blocks, BlockListPtr big_blocks,
		Iterator<PropertyPtr> properties, DirectoryNodePtr dir, int headerPropertiesStartAt);

private:
	PropertyTablePtr _property_table;
	POIFSDocumentListPtr _documents;
	DirectoryNodePtr _root;

	/**
	* What big block size the file uses. Most files
	*  use 512 bytes, but a few use 4096
	*/
	POIFSBigBlockSize bigBlockSize = POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS;
};   // end class POIFSFileSystem

//typedef std::shared_ptr<POIFSFileSystem> POIFSFileSystemPtr;