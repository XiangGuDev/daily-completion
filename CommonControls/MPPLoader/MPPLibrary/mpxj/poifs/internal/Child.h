#pragma once

/**
 * This interface defines methods for finding and setting sibling
 * Property instances
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class Child
{
public:
    /**
     * Get the next Child, if any
     *
     * @return the next Child; may return null
     */

	virtual std::shared_ptr<Child> getNextChild() = 0;

    /**
     * Get the previous Child, if any
     *
     * @return the previous Child; may return null
     */

	virtual std::shared_ptr<Child> getPreviousChild() = 0;

    /**
     * Set the next Child
     *
     * @param child the new 'next' child; may be null, which has the
     *              effect of saying there is no 'next' child
     */

	virtual void setNextChild(std::shared_ptr<Child> child) = 0;

    /**
     * Set the previous Child
     *
     * @param child the new 'previous' child; may be null, which has
     *              the effect of saying there is no 'previous' child
     */

	virtual void setPreviousChild(std::shared_ptr<Child> child) = 0;
};   // end interface Child

typedef std::shared_ptr<Child> ChildPtr;

class Property;

class Parent// : public Child
{
public:
	///**
	//* Get an iterator over the children of this Parent; all elements
	//* are instances of Property.
	//*
	//* @return Iterator of children; may refer to an empty collection
	//*/

	//public Iterator getChildren();

	/**
	* Add a new child to the collection of children
	*
	* @param property the new child to be added; must not be null
	*
	* @exception IOException if the Parent already has a child with
	*                        the same name
	*/

	virtual void addChild(std::shared_ptr<Property> prop) = 0;

};

typedef std::shared_ptr<Parent> ParentPtr;