#include "stdafx.h"
#include "PropertyTableBase.h"
#include "RootProperty.h"
#include <stack>

PropertyTableBase::PropertyTableBase(HeaderBlockPtr header_block)
{
    _header_block = header_block;
    _properties.reset (new List<PropertyPtr>());
	addProperty(RootPropertyPtr(new RootProperty()));
}

/**
    * Reading constructor (used when we've read in a file and we want
    * to extract the prop table from it). Populates the
    * properties thoroughly
    *
    * @param header_block the first block to read from
    * @param properties the list to populate
    *
    * @exception IOException if anything goes wrong (which should be
    *            a result of the input being NFG)
    */
PropertyTableBase::PropertyTableBase(HeaderBlockPtr header_block, std::shared_ptr<List<PropertyPtr>> properties)
{
    _header_block = header_block;
    _properties   = properties;
    populatePropertyTree( std::dynamic_pointer_cast<DirectoryProperty>(_properties->GetAt(0)));
}

/**
    * Add a prop to the list of properties we manage
    *
    * @param prop the new Property to manage
    */
void PropertyTableBase::addProperty(PropertyPtr prop)
{
    _properties->Add(prop);
}

/**
    * Remove a prop from the list of properties we manage
    *
    * @param prop the Property to be removed
    */
void PropertyTableBase::removeProperty(PropertyPtr prop)
{
    _properties->remove(prop);
}

/**
    * Get the root prop
    *
    * @return the root prop
    */
RootPropertyPtr PropertyTableBase::getRoot()
{
    // it's always the first element in the List
    return std::dynamic_pointer_cast<RootProperty>(_properties->GetAt(0));
}
    
void PropertyTableBase::populatePropertyTree(DirectoryPropertyPtr root)
{
    int index = root->getChildIndex();

    if (!Property::isValidIndex(index))
    {

        // prop has no children
        return;
    }
	std::stack<PropertyPtr> children;// = new Stack<Property>();

	children.push(_properties->GetAt(index));
    while (!children.empty())
    {
		PropertyPtr prop = children.top();
		children.pop();

        root->addChild(prop);
        if (prop->isDirectory())
        {
            populatePropertyTree(std::dynamic_pointer_cast<DirectoryProperty>(prop));
        }
        index = prop->getPreviousChildIndex();
        if (Property::isValidIndex(index))
        {
			children.push(_properties->GetAt(index));
        }
        index = prop->getNextChildIndex();
        if (Property::isValidIndex(index))
        {
			children.push(_properties->GetAt(index));
        }
    }
}

/**
    * Get the start block for the prop table
    *
    * @return start block index
    */
int PropertyTableBase::getStartBlock()
{
    return _header_block->getPropertyStart();
}

/**
    * Set the start block for this instance
    *
    * @param index index into the array of BigBlock instances making
    *              up the the filesystem
    */
void PropertyTableBase::setStartBlock(int index)
{
    _header_block->setPropertyStart(index);
}