#pragma once

#include "Entry.h"
#include "internal\Property.h"

class DirectoryEntry;
class DirectoryNode;

/**
 * Abstract implementation of Entry
 *
 * Extending classes should override isDocument() or isDirectory(), as
 * appropriate
 *
 * Extending classes must override isDeleteOK()
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

//template <class T>
class EntryNode : public Entry, public std::enable_shared_from_this<EntryNode>
{
public:
	/**
	* get the name of the Entry
	*
	* @return name
	*/

	CString getName();

	/**
	* is this a DirectoryEntry?
	*
	* @return true if the Entry is a DirectoryEntry, else false
	*/

	bool isDirectoryEntry();

	/**
	* is this a DocumentEntry?
	*
	* @return true if the Entry is a DocumentEntry, else false
	*/

	bool isDocumentEntry();

	/**
	* get this Entry's parent (the DocumentEntry that owns this
	* Entry). All Entry objects, except the root Entry, has a parent.
	*
	* @return this Entry's parent; null iff this is the root Entry
	*/

	std::shared_ptr<DirectoryEntry> getParent();

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

	bool deleteThis();

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

	bool renameTo(CString newName);

	/**
	* grant access to the property
	*
	* @return the property backing this entry
	*/

	PropertyPtr getProperty();

protected:
	/**
	* create a DocumentNode. This method is not by design; it
	* is intended strictly for the internal use of extending classes
	*
	* @param property the Property for this Entry
	* @param parent the parent of this entry
	*/

	EntryNode(PropertyPtr prop, std::shared_ptr<DirectoryNode> parent);

	/**
	* is this the root of the tree?
	*
	* @return true if so, else false
	*/

	bool isRoot();

	/**
	* extensions use this method to verify internal rules regarding
	* deletion of the underlying store.
	*
	* @return true if it's ok to delete the underlying store, else
	*         false
	*/

	virtual bool isDeleteOK() = 0;

private:
	// the DocumentProperty backing this object
	PropertyPtr						_property;

	// this object's parent Entry
	std::weak_ptr<DirectoryNode>	_parent;

};   // end class EntryNode

typedef std::shared_ptr<EntryNode> EntryNodePtr;