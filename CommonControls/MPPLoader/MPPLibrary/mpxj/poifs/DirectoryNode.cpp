#include "stdafx.h"

#include "POIFSFileSystem.h"
#include "DirectoryNode.h"
#include "internal\DirectoryProperty.h"
#include "DocumentNode.h"

/**
* create a DirectoryNode. This method is not by design; it
* is intended strictly for the internal use of this package
*
* @param property the DirectoryProperty for this DirectoryEntry
* @param filesystem the POIFSFileSystem we belong to
* @param parent the parent of this entry
*/
DirectoryNode::DirectoryNode(DirectoryPropertyPtr prop, POIFSFileSystem* ofilesystem, DirectoryNodePtr parent)
:DirectoryEntry(prop, parent)
{
	_ofilesystem = ofilesystem;
	//this._nfilesystem = nfilesystem;

	if (!parent)
	{
		_path.reset (new POIFSDocumentPath());
	}
	else
	{
		std::vector<CString> names;
		names.push_back(prop->getName());
		_path.reset(new POIFSDocumentPath(parent->_path, names));
	}

	//_byname = new HashMap<CString, Entry>();
	//_entries = new ArrayList<Entry>();
	Iterator<PropertyPtr> iter = prop->getChildren();

	while (iter.hasNext())
	{
		PropertyPtr child = iter.next();
		EntryPtr    childNode;

		DirectoryNodePtr ThisPtr = std::dynamic_pointer_cast<DirectoryNode>(shared_from_this());

		if (child->isDirectory())
		{
			DirectoryPropertyPtr childDir = std::dynamic_pointer_cast<DirectoryProperty>(child);
			DirectoryNodePtr ptr(new DirectoryNode(childDir, _ofilesystem, ThisPtr));
			childNode = std::dynamic_pointer_cast<Entry>(ptr);
		}
		else 
		{
			DocumentNodePtr ptr(new DocumentNode(std::dynamic_pointer_cast<DocumentProperty>(child), ThisPtr));
			childNode = std::dynamic_pointer_cast<Entry>(ptr);
		}
		_entries.Add(childNode);
		//_byname.put(childNode->getName(), childNode);
		_byname[childNode->getName()] = childNode;
	}
}

/**
* @return this directory's path representation
*/

POIFSDocumentPathPtr DirectoryNode::getPath()
{
	return _path;
}

/**
* @return the filesystem that this belongs to
*/
POIFSFileSystem* DirectoryNode::getFileSystem()
{
	return _ofilesystem;
}

/**
* open a document in the directory's entry's list of entries
*
* @param documentName the name of the document to be opened
*
* @return a newly opened DocumentInputStream
*
* @exception IOtoolkit::Exception if the document does not exist or the
*            name is that of a DirectoryEntry
*/
DocumentInputStreamPtr DirectoryNode::createDocumentInputStream(CString documentName)
{
	return createDocumentInputStream(getEntry(documentName));
}

/**
* open a document in the directory's entry's list of entries
*
* @param document the document to be opened
*
* @return a newly opened DocumentInputStream or NDocumentInputStream
*
* @exception IOtoolkit::Exception if the document does not exist or the
*            name is that of a DirectoryEntry
*/
DocumentInputStreamPtr DirectoryNode::createDocumentInputStream(EntryPtr document)
{
	if (!document->isDocumentEntry()) {
		throw toolkit::Exception(L"Entry is not a DocumentEntry");
	}

	DocumentEntryPtr entry = std::dynamic_pointer_cast<DocumentEntry>(document);
	return DocumentInputStreamPtr (new DocumentInputStream(entry));
}

/**
* create a new DocumentEntry
*
* @param document the new document
*
* @return the new DocumentEntry
*
* @exception IOtoolkit::Exception
*/
DocumentEntryPtr DirectoryNode::createDocument(POIFSDocumentPtr document)
{
	DocumentPropertyPtr prop = document->getDocumentProperty();

	DirectoryNodePtr ThisPtr = std::dynamic_pointer_cast<DirectoryNode>(shared_from_this());

	DocumentNodePtr rval(new DocumentNode(prop, ThisPtr));

	(std::dynamic_pointer_cast<DirectoryProperty>(getProperty()))->addChild(prop);
	_ofilesystem->addDocument(document);

	_entries.Add(std::dynamic_pointer_cast<Entry>(rval));
	CString name = prop->getName();
	//_byname.put(name, std::dynamic_pointer_cast<Entry>(rval));
	_byname[name] = std::dynamic_pointer_cast<Entry>(rval);
	return rval;
}

/**
* Change a contained Entry's name
*
* @param oldName the original name
* @param newName the new name
*
* @return true if the operation succeeded, else false
*/
bool DirectoryNode::changeName(CString oldName, CString newName)
{
	EntryPtr entryByName;
	auto itMap = _byname.find(oldName);
	if(itMap == _byname.end())
	//if (!_byname.Lookup(oldName, entryByName))
		return false;

	entryByName = itMap->second;
	EntryNodePtr child = std::dynamic_pointer_cast<EntryNode>(entryByName);

	bool rval = (std::dynamic_pointer_cast<DirectoryProperty>(getProperty()))->changeName(child->getProperty(), newName);
	if (rval)
	{
// 		_byname.remove(oldName);
// 		_byname.put(child->getProperty()->getName(), child);
		_byname.erase(itMap);
		_byname[child->getProperty()->getName()] = child;
	}

	return rval;
}

/**
* Delete an entry
*
* @param entry the EntryNode to be deleted
*
* @return true if the entry was deleted, else false
*/

bool DirectoryNode::deleteEntry(EntryNodePtr entry)
{
	bool rval = (std::dynamic_pointer_cast<DirectoryProperty>(getProperty()))->deleteChild(entry->getProperty());

	if (rval)
	{
// 		_entries.remove(entry);
// 		_byname.remove(entry->getName());
		auto it = _byname.find(entry->getName());
		if (it != _byname.end())
			_byname.erase(it);

		_ofilesystem->remove(entry);
	}
	return rval;
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

Iterator<EntryPtr> DirectoryNode::getEntries()
{
    return _entries.iterator();
}

/**
* get the names of all the Entries contained directly in this
* instance (in other words, names of children only; no grandchildren
* etc).
*
* @return the names of all the entries that may be retrieved with
*         getEntry(CString), which may be empty (if this
*         DirectoryEntry is empty)
*/
std::set<CString> DirectoryNode::getEntryNames()
{
	std::set<CString> setNames;
	for (auto it = _byname.begin(); it != _byname.end(); ++it)
		setNames.insert(it->first);
	return setNames;
	//return _byname.keySet();
}

/**
* is this DirectoryEntry empty?
*
* @return true if this instance contains no Entry instances
*/

bool DirectoryNode::isEmpty()
{
	return _entries.isEmpty();
}

/**
* find out how many Entry instances are contained directly within
* this DirectoryEntry
*
* @return number of immediately (no grandchildren etc.) contained
*         Entry instances
*/

int DirectoryNode::getEntryCount()
{
	return _entries.GetCount();
}

bool DirectoryNode::hasEntry(CString name)
{
	return name.GetLength() && /*_byname.containsKey(name)*/_byname.find(name) != _byname.end();
}

/**
* get a specified Entry by name
*
* @param name the name of the Entry to obtain.
*
* @return the specified Entry, if it is directly contained in
*         this DirectoryEntry
*
* @exception FileNotFoundtoolkit::Exception if no Entry with the specified
*            name exists in this DirectoryEntry
*/

EntryPtr DirectoryNode::getEntry(CString name)
{
	EntryPtr rval;

	if (name.GetLength() > 0)
	{
		//_byname.Lookup(name, rval);
		auto it = _byname.find(name);
		if (it != _byname.end())
			rval = it->second;
	}
	if (!rval)
	{
		// either a null name was given, or there is no such name
		throw toolkit::Exception(L"no such entry");
	}
	return rval;
}

/**
* create a new DocumentEntry
*
* @param name the name of the new DocumentEntry
* @param stream the InputStream from which to create the new
*               DocumentEntry
*
* @return the new DocumentEntry
*
* @exception IOtoolkit::Exception
*/

DocumentEntryPtr DirectoryNode::createDocument(CString name, BwProj::toolkit::IDataReadStream* stream)
{
	return createDocument(POIFSDocumentPtr(new POIFSDocument(name, stream)));
}

   /**
    * create a new DocumentEntry; the data will be provided later
    *
    * @param name the name of the new DocumentEntry
    * @param size the size of the new DocumentEntry
    * @param writer the writer of the new DocumentEntry
    *
    * @return the new DocumentEntry
    *
    * @exception IOtoolkit::Exception
    */

DocumentEntryPtr DirectoryNode::createDocument(CString name, int size)
{
	return createDocument(POIFSDocumentPtr(new POIFSDocument(name, size, _path)));
}

/**
* create a new DirectoryEntry
*
* @param name the name of the new DirectoryEntry
*
* @return the new DirectoryEntry
*
* @exception IOtoolkit::Exception
*/

DirectoryEntryPtr DirectoryNode::createDirectory(CString name)
{
	DirectoryNodePtr rval;
	DirectoryPropertyPtr property (new DirectoryProperty(name));

	DirectoryNodePtr ThisPtr = std::dynamic_pointer_cast<DirectoryNode>(shared_from_this());
	rval.reset(new DirectoryNode(property, _ofilesystem, ThisPtr));
	_ofilesystem->addDirectory(property);

	(std::dynamic_pointer_cast<DirectoryProperty>(getProperty()))->addChild(property);
	_entries.Add(std::dynamic_pointer_cast<Entry>(rval));
	//_byname.put(name, std::dynamic_pointer_cast<Entry>(rval));
	_byname[name] = std::dynamic_pointer_cast<Entry>(rval);
	return rval;
}

/**
* Gets the storage clsid of the directory entry
*
* @return storage Class ID
*/
ClassIDPtr DirectoryNode::getStorageClsid()
{
	return getProperty()->getStorageClsid();
}

/**
* Sets the storage clsid for the directory entry
*
* @param clsidStorage storage Class ID
*/
void DirectoryNode::setStorageClsid(ClassIDPtr clsidStorage)
{
	getProperty()->setStorageClsid(clsidStorage);
}

/**
* is this a DirectoryEntry?
*
* @return true if the Entry is a DirectoryEntry, else false
*/

bool DirectoryNode::isDirectoryEntry()
{
	return true;
}

/**
* extensions use this method to verify internal rules regarding
* deletion of the underlying store.
*
* @return true if it's ok to delete the underlying store, else
*         false
*/

bool DirectoryNode::isDeleteOK()
{
	// if this directory is empty, we can delete it
	return isEmpty();
}