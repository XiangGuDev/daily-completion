#include "stdafx.h"
#include "DirectoryProperty.h"
#include "PropertyConstants.h"


/**
    * Default constructor
    *
    * @param name the name of the directory
    */
DirectoryProperty::DirectoryProperty(CString name)
:Property()
{
    //_children       = new ArrayList<Property>();
    //_children_names = new HashSet<CString>();
    setName(name);
    setSize(0);
    setPropertyType(PropertyConstants::DIRECTORY_TYPE);
    setStartBlock(0);
    setNodeColor(_NODE_BLACK);   // simplification
}

/**
    * reader constructor
    *
    * @param index index number
    * @param array byte data
    * @param offset offset into byte data
    */
DirectoryProperty::DirectoryProperty(int index, ByteArrayPtr array1, int offset)
:Property(index, array1, offset)
{
    //_children       = new ArrayList<Property>();
    //_children_names = new HashSet<CString>();
}

/**
    * Change a Property's name
    *
    * @param property the Property whose name is being changed
    * @param newName the new name for the Property
    *
    * @return true if the name change could be made, else false
    */
bool DirectoryProperty::changeName(PropertyPtr prop, CString newName)
{
    bool result;
	CString  oldName = prop->getName();

	prop->setName(newName);
	CString cleanNewName = prop->getName();

	auto itNewName = _children_names.find(cleanNewName);
    if (itNewName != _children_names.end() /*_children_names.contains(cleanNewName)*/)
    {
        // revert the change
		prop->setName(oldName);
        result = false;
    }
    else
    {
//         _children_names.add(cleanNewName);
//         _children_names.remove(oldName);
		auto itNewOld = _children_names.find(oldName);
		_children_names.erase(itNewOld);
		_children_names.insert(cleanNewName);
        result = true;
    }
    return result;
}

/**
    * Delete a Property
    *
    * @param property the Property being deleted
    *
    * @return true if the Property could be deleted, else false
    */
bool DirectoryProperty::deleteChild(PropertyPtr prop)
{
    bool result = _children.remove(prop);

    if (result)
    {
		//_children_names.remove(prop->getName());
		auto itName = _children_names.find(prop->getName());
		_children_names.erase(itName);
    }
    return result;
}
//
//static class PropertyComparator implements Comparator<Property> {
//
    /**
        * compare method. Assumes both parameters are non-null
        * instances of Property. One property is less than another if
        * its name is shorter than the other property's name. If the
        * names are the same length, the property whose name comes
        * before the other property's name, alphabetically, is less
        * than the other property.
        *
        * @param o1 first object to compare, better be a Property
        * @param o2 second object to compare, better be a Property
        *
        * @return negative value if o1 <  o2,
        *         zero           if o1 == o2,
        *         positive value if o1 >  o2.
        */
int compare(PropertyPtr o1, PropertyPtr o2)
{
    CString VBA_PROJECT = L"_VBA_PROJECT";
    CString name1  = o1->getName();
    CString name2  = o2->getName();
	int  result = name1.GetLength() - name2.GetLength();

	if (result == 0)
		return result;

    // _VBA_PROJECT, it seems, will always come last
    if (_tcscmp (name1, VBA_PROJECT) == 0)
		result = 1;
	else if (_tcscmp (name2, VBA_PROJECT) == 0)
		result = -1;
    else
    {
		if (name1.Left(2) == L"__" && name2.Left(2) == L"__")
		{
			// Betweeen __SRP_0 and __SRP_1 just sort as normal
			result = _wcsicmp(name1, name2);
		}
		else if (name1.Left(2) == L"__")
		{
			// If only name1 is __XXX then this will be placed after name2
			result = 1;
		}
		else if (name2.Left(2) == L"__")
		{
			// If only name2 is __XXX then this will be placed after name1
			result = -1;
		}
		else
		{
			// result = name1.compareTo(name2);
			// The default case is to sort names ignoring case
			result = _wcsicmp(name1, name2);
		}
    }

    return result;
}
//}

void sort(List<PropertyPtr>& list)
{
	for (size_t i = 0; i < list.GetCount(); i++)
	{
		PropertyPtr curProp = list.GetAt(i);

		for (size_t j = i + 1; j < list.GetCount(); i++)
		{
			PropertyPtr nxtProp = list.GetAt(j);

			if (compare(curProp, nxtProp) > 0)
			{
				list.SetAt(i, nxtProp);
				list.SetAt(j, curProp);
			}
		}
	}
}

/**
    * @return true if a directory type Property
    */
bool DirectoryProperty::isDirectory()
{
    return true;
}

/**
    * Perform whatever activities need to be performed prior to
    * writing
    */
void DirectoryProperty::preWrite()
{
	if (_children.GetCount() <= 0)
		return;

	List<PropertyPtr>& children = _children;// .toArray(new Property[0]);

    //Arrays.sort(children, new PropertyComparator());
	sort(children);

    int midpoint = children.GetCount() / 2;

    setChildProperty(children[ midpoint ]->getIndex());
    children[ 0 ]->setPreviousChild(NULL);
	children[0]->setNextChild(NULL);
    for (int j = 1; j < midpoint; j++)
    {
		children[j]->setPreviousChild(children[j - 1]);
		children[j]->setNextChild(NULL);
    }
    if (midpoint != 0)
    {
        children[ midpoint ]->setPreviousChild(children[midpoint - 1]);
    }
    if (midpoint != (children.GetCount() - 1))
    {
		children[midpoint]->setNextChild(children[midpoint + 1]);
        for (size_t j = midpoint + 1; j < children.GetCount() - 1; j++)
        {
			children[j]->setPreviousChild(NULL);
			children[j]->setNextChild(children[j + 1]);
        }
		children[children.GetCount() - 1]->setPreviousChild(NULL);
		children[children.GetCount() - 1]->setNextChild(NULL);
    }
    else
    {
		children[midpoint]->setNextChild(NULL);
    }
}

/**
 * Get an iterator over the children of this Parent; all elements
 * are instances of Property.
 *
 * @return Iterator of children; may refer to an empty collection
 */
Iterator<PropertyPtr> DirectoryProperty::getChildren()
{
    return _children.iterator();
}

/**
    * Add a new child to the collection of children
    *
    * @param property the new child to be added; must not be null
    *
    * @exception IOtoolkit::Exception if we already have a child with the same
    *                        name
    */
void DirectoryProperty::addChild(PropertyPtr prop)
{
	CString name = prop->getName();

    if (!_children_names.insert(name).second/*_children_names.contains(name)*/)
    {
        throw toolkit::Exception(L"Duplicate name");
    }
   // _children_names.add(name);
	_children.Add(prop);
}