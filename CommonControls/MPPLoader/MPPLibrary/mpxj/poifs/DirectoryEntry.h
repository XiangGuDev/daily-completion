#pragma once

#include "EntryNode.h"
#include "DocumentEntry.h"
#include "internal\ClassID.h"

/**
 * This interface defines methods specific to Directory objects
 * managed by a Filesystem instance.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class DirectoryEntry : public EntryNode
{
public:
	DirectoryEntry(PropertyPtr prop, std::shared_ptr<DirectoryNode> parent)
		: EntryNode(prop, parent)
	{

	}

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

    virtual Iterator<EntryPtr> getEntries() = 0;
    
    /**
     * get the names of all the Entries contained directly in this
     * instance (in other words, names of children only; no grandchildren
     * etc).
     *
     * @return the names of all the entries that may be retrieved with
     *         getEntry(CString), which may be empty (if this 
     *         DirectoryEntry is empty)
     */
    virtual std::set<CString> getEntryNames() = 0;

    /**
     * is this DirectoryEntry empty?
     *
     * @return true if this instance contains no Entry instances
     */

	virtual bool isEmpty() = 0;

    /**
     * find out how many Entry instances are contained directly within
     * this DirectoryEntry
     *
     * @return number of immediately (no grandchildren etc.) contained
     *         Entry instances
     */

	virtual int getEntryCount() = 0;

    /**
     * Checks if entry with specified name present
     */

	virtual bool hasEntry(CString name) = 0;

    /**
     * get a specified Entry by name
     *
     * @param name the name of the Entry to obtain.
     *
     * @return the specified Entry, if it is directly contained in
     *         this DirectoryEntry
     *
     * @exception FileNotFoundException if no Entry with the specified
     *            name exists in this DirectoryEntry
     */

	virtual EntryPtr getEntry(CString name) = 0;

    /**
     * create a new DocumentEntry
     *
     * @param name the name of the new DocumentEntry
     * @param stream the InputStream from which to create the new
     *               DocumentEntry
     *
     * @return the new DocumentEntry
     *
     * @exception IOException
     */

	virtual DocumentEntryPtr createDocument(CString name, BwProj::toolkit::IDataReadStream* stream) = 0;

    /**
     * create a new DocumentEntry; the data will be provided later
     *
     * @param name the name of the new DocumentEntry
     * @param size the size of the new DocumentEntry
     * @param writer the writer of the new DocumentEntry
     *
     * @return the new DocumentEntry
     *
     * @exception IOException
     */

	virtual DocumentEntryPtr createDocument(CString name, int size) = 0;// , POIFSWriterListener writer);

    /**
     * create a new DirectoryEntry
     *
     * @param name the name of the new DirectoryEntry
     *
     * @return the new DirectoryEntry
     *
     * @exception IOException
     */

	virtual std::shared_ptr<DirectoryEntry> createDirectory(CString name) = 0;

    /**
     * Gets the storage clsid of the directory entry
     *
     * @return storage Class ID
     */
	virtual ClassIDPtr getStorageClsid() = 0;

    /**
     * Sets the storage clsid for the directory entry
     *
     * @param clsidStorage storage Class ID
     */
	virtual void setStorageClsid(ClassIDPtr clsidStorage) = 0;

};   // end public interface DirectoryEntry

typedef std::shared_ptr<DirectoryEntry> DirectoryEntryPtr;