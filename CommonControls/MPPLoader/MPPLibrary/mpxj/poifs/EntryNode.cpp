#include "stdafx.h"
#include "EntryNode.h"
#include "DirectoryNode.h"

/**
* get the name of the Entry
*
* @return name
*/

CString EntryNode::getName()
{
	return _property->getName();
}

/**
* is this a DirectoryEntry?
*
* @return true if the Entry is a DirectoryEntry, else false
*/

bool EntryNode::isDirectoryEntry()
{
	return false;
}

/**
* is this a DocumentEntry?
*
* @return true if the Entry is a DocumentEntry, else false
*/

bool EntryNode::isDocumentEntry()
{
	return false;
}

/**
* get this Entry's parent (the DocumentEntry that owns this
* Entry). All Entry objects, except the root Entry, has a parent.
*
* @return this Entry's parent; null iff this is the root Entry
*/

std::shared_ptr<DirectoryEntry> EntryNode::getParent()
{
	return std::dynamic_pointer_cast<DirectoryEntry> (_parent.lock());
}

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

bool EntryNode::deleteThis()
{
	bool rval = false;

	if ((!isRoot()) && isDeleteOK())
	{
		rval = _parent.lock()->deleteEntry(shared_from_this());
	}
	return rval;
}

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

bool EntryNode::renameTo(CString newName)
{
	bool rval = false;

	if (!isRoot())
	{
		rval = _parent.lock()->changeName(getName(), newName);
	}
	return rval;
}

/**
* grant access to the property
*
* @return the property backing this entry
*/

PropertyPtr EntryNode::getProperty()
{
	return _property;
}

/**
* create a DocumentNode. This method is not by design; it
* is intended strictly for the internal use of extending classes
*
* @param property the Property for this Entry
* @param parent the parent of this entry
*/

	EntryNode::EntryNode(PropertyPtr prop, std::shared_ptr<DirectoryNode> parent)
{
	_property = prop;
	_parent = parent;
}

/**
* is this the root of the tree?
*
* @return true if so, else false
*/

bool EntryNode::isRoot()
{

	// only the root Entry has no parent ...
	return (!_parent.expired());
}