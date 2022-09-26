#pragma once

#include "Property.h"

/**
 * Directory property
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class DirectoryProperty : public Property, public Parent
{
	friend class PropertyFactory;
public:
    /**
     * Default constructor
     *
     * @param name the name of the directory
     */
	DirectoryProperty(CString name);

    /**
     * Change a Property's name
     *
     * @param property the Property whose name is being changed
     * @param newName the new name for the Property
     *
     * @return true if the name change could be made, else false
     */
	bool changeName(PropertyPtr prop, CString newName);

    /**
     * Delete a Property
     *
     * @param property the Property being deleted
     *
     * @return true if the Property could be deleted, else false
     */
	bool deleteChild(PropertyPtr prop);

    /**
     * @return true if a directory type Property
     */
	bool isDirectory();

    /**
     * Get an iterator over the children of this Parent; all elements
     * are instances of Property.
     *
     * @return Iterator of children; may refer to an empty collection
     */
	Iterator<PropertyPtr> getChildren();

    /**
     * Add a new child to the collection of children
     *
     * @param property the new child to be added; must not be null
     *
     * @exception IOException if we already have a child with the same
     *                        name
     */
	void addChild(PropertyPtr prop);

protected:

	/**
	* reader constructor
	*
	* @param index index number
	* @param array byte data
	* @param offset offset into byte data
	*/
	DirectoryProperty(int index, ByteArrayPtr array1, int offset);

	/**
	* Perform whatever activities need to be performed prior to
	* writing
	*/
	void preWrite();

private:
	/** List of Property instances */
	List<PropertyPtr> _children;

	/** set of children's names */
	std::set<CString>  _children_names;
};

typedef std::shared_ptr<DirectoryProperty> DirectoryPropertyPtr;