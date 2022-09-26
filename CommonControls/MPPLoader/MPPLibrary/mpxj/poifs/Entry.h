#pragma once

/**
 * This interface provides access to an object managed by a Filesystem
 * instance. Entry objects are further divided into DocumentEntry and
 * DirectoryEntry instances.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class DirectoryEntry;

class Entry// : public Object
{
public:
    /**
     * get the name of the Entry
     *
     * @return name
     */

    virtual CString getName() = 0;

    /**
     * is this a DirectoryEntry?
     *
     * @return true if the Entry is a DirectoryEntry, else false
     */

	virtual bool isDirectoryEntry() = 0;

    /**
     * is this a DocumentEntry?
     *
     * @return true if the Entry is a DocumentEntry, else false
     */

	virtual bool isDocumentEntry() = 0;

    /**
     * get this Entry's parent (the DirectoryEntry that owns this
     * Entry). All Entry objects, except the root Entry, has a parent.
     *
     * @return this Entry's parent; null iff this is the root Entry
     */

	virtual std::shared_ptr<DirectoryEntry> getParent() = 0;

    /**
     * Delete this Entry. This operation should succeed, but there are
     * special circumstances when it will not:
     *
     * If this Entry is the root of the Entry tree, it cannot be
     * deleted, as there is no way to create another one.
     *
     * If this Entry is a directory, it cannot be deleted unless it is
     * empty.
     *
     * @return true if the Entry was successfully deleted, else false
     */

	virtual bool deleteThis() = 0;

    /**
     * Rename this Entry. This operation will fail if:
     *
     * There is a sibling Entry (i.e., an Entry whose parent is the
     * same as this Entry's parent) with the same name.
     *
     * This Entry is the root of the Entry tree. Its name is dictated
     * by the Filesystem and many not be changed.
     *
     * @param newName the new name for this Entry
     *
     * @return true if the operation succeeded, else false
     */

	virtual bool renameTo(CString newName) = 0;
};

typedef std::shared_ptr<Entry> EntryPtr;