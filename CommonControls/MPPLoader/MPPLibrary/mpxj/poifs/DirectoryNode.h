#pragma once

#include "DirectoryEntry.h"
#include "POIFSDocumentPath.h"
#include "DocumentInputStream.h"

class POIFSFileSystem;
class POIFSDocument;
class DirectoryProperty;

/**
 * Simple implementation of DirectoryEntry
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class DirectoryNode : public DirectoryEntry//, POIFSViewable, Iterable<Entry>
{
public:
	/**
	 * create a DirectoryNode. This method is not by design; it
	 * is intended strictly for the internal use of this package
	 *
	 * @param property the DirectoryProperty for this DirectoryEntry
	 * @param filesystem the POIFSFileSystem we belong to
	 * @param parent the parent of this entry
	 */
	DirectoryNode(std::shared_ptr<DirectoryProperty> prop, 
		POIFSFileSystem* ofilesystem, std::shared_ptr<DirectoryNode> parent);

	/**
	 * @return this directory's path representation
	 */

	POIFSDocumentPathPtr getPath();

	/**
	 * @return the filesystem that this belongs to
	 */
	POIFSFileSystem* getFileSystem();

	/**
	 * open a document in the directory's entry's list of entries
	 * @param documentName the name of the document to be opened
	 * @return a newly opened DocumentInputStream
	 * @exception IOException if the document does not exist or the
	 *            name is that of a DirectoryEntry
	 */
	DocumentInputStreamPtr createDocumentInputStream(CString documentName);

	/**
	 * open a document in the directory's entry's list of entries
	 * @param document the document to be opened
	 * @return a newly opened DocumentInputStream or NDocumentInputStream
	 * @exception IOException if the document does not exist or the
	 *            name is that of a DirectoryEntry
	 */
	DocumentInputStreamPtr createDocumentInputStream(EntryPtr document);

	/**
	 * create a new DocumentEntry
	 * @param document the new document
	 * @return the new DocumentEntry
	 * @exception IOException
	 */
	DocumentEntryPtr createDocument(std::shared_ptr<POIFSDocument> document);

	/**
	 * Change a contained Entry's name
	 * @param oldName the original name
	 * @param newName the new name
	 * @return true if the operation succeeded, else false
	 */
	bool changeName(CString oldName, CString newName);

	/**
	 * Delete an entry
	 * @param entry the EntryNode to be deleted
	 * @return true if the entry was deleted, else false
	 */

	bool deleteEntry(EntryNodePtr entry);

	/**
	 * get an iterator of the Entry instances contained directly in
	 * this instance (in other words, children only; no grandchildren
	 * etc.)
	 *
	 * @return iterator; never null, but hasNext() may return false
	 *         immediately (i.e., this DirectoryEntry is empty). All
	 *         objects retrieved by next() are guaranteed to be
	 *         implementations of Entry.
	 */

	Iterator<EntryPtr> getEntries();

	/**
	 * get the names of all the Entries contained directly in this
	 * instance (in other words, names of children only; no grandchildren
	 * etc).
	 *
	 * @return the names of all the entries that may be retrieved with
	 *         getEntry(CString), which may be empty (if this
	 *         DirectoryEntry is empty)
	 */
	std::set<CString> getEntryNames();

	/**
	 * is this DirectoryEntry empty?
	 *
	 * @return true if this instance contains no Entry instances
	 */

	bool isEmpty();

	/**
	 * find out how many Entry instances are contained directly within
	 * this DirectoryEntry
	 *
	 * @return number of immediately (no grandchildren etc.) contained
	 *         Entry instances
	 */

	int getEntryCount();

	bool hasEntry(CString name);

	/**
	 * get a specified Entry by name
	 * @param name the name of the Entry to obtain.
	 * @return the specified Entry, if it is directly contained in
	 *         this DirectoryEntry
	 * @exception FileNotFoundException if no Entry with the specified
	 *            name exists in this DirectoryEntry
	 */

	EntryPtr getEntry(CString name);

	/**
	 * create a new DocumentEntry
	 * @param name the name of the new DocumentEntry
	 * @param stream the InputStream from which to create the new
	 *               DocumentEntry
	 * @return the new DocumentEntry
	 * @exception IOException
	 */

	DocumentEntryPtr createDocument(CString name, BwProj::toolkit::IDataReadStream* stream);

	   /**
	    * create a new DocumentEntry; the data will be provided later
	    * @param name the name of the new DocumentEntry
	    * @param size the size of the new DocumentEntry
	    * @param writer the writer of the new DocumentEntry
	    * @return the new DocumentEntry
	    * @exception IOException
	    */

	DocumentEntryPtr createDocument(CString name, int size);

	/**
	 * create a new DirectoryEntry
	 *
	 * @param name the name of the new DirectoryEntry
	 *
	 * @return the new DirectoryEntry
	 *
	 * @exception IOException
	 */

	std::shared_ptr<DirectoryEntry> createDirectory(CString name);

	/**
	 * Gets the storage clsid of the directory entry
	 *
	 * @return storage Class ID
	 */
	ClassIDPtr getStorageClsid();

	/**
	 * Sets the storage clsid for the directory entry
	 *
	 * @param clsidStorage storage Class ID
	 */
	void setStorageClsid(ClassIDPtr clsidStorage);

	/**
	 * is this a DirectoryEntry?
	 *
	 * @return true if the Entry is a DirectoryEntry, else false
	 */

	virtual bool isDirectoryEntry();

protected:
	/**
	 * extensions use this method to verify internal rules regarding
	 * deletion of the underlying store.
	 *
	 * @return true if it's ok to delete the underlying store, else
	 *         false
	 */

	virtual bool isDeleteOK();

private:
	// Map of Entry instances, keyed by their names
	std::map<CString, EntryPtr>				_byname;

	// Our list of entries, kept sorted to preserve order
	List<EntryPtr>						_entries;

	// the POIFSFileSystem we belong to
	POIFSFileSystem*					_ofilesystem;

	// the path described by this document
	POIFSDocumentPathPtr				_path;

};   // end class DirectoryNode

typedef std::shared_ptr<DirectoryNode> DirectoryNodePtr;