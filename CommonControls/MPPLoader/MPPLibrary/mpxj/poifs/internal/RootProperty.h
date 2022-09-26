#pragma once

#include "DirectoryProperty.h"

/**
 * Root property
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class RootProperty : public DirectoryProperty
{
	friend class PropertyFactory;
public:
	RootProperty();

    /**
     * set size
     *
     * @param size size in terms of small blocks
     */
   void setSize(int size);

    /**
     * Returns the fixed name "Root Entry", as the
     *  raw property doesn't have a real name set
     */
   virtual CString getName();

protected:

   /**
   * reader constructor
   *
   * @param index index number
   * @param array byte data
   * @param offset offset into byte data
   */
   RootProperty(int index, ByteArrayPtr array1, int offset);
};

typedef std::shared_ptr<RootProperty> RootPropertyPtr;