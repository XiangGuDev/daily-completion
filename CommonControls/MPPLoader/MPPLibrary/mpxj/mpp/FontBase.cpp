#include "stdafx.h"
#include "FontBase.h"

/**
* Constructor.
*
* @param index index number for this font
* @param name font name
* @param size font size
*/
FontBase::FontBase(int index, CString name, int size)
{
    m_index = index;
    m_name = name;
    m_size = size;
}

/**
* Retrieve the font name.
*
* @return font name
*/
CString FontBase::getName()
{
    return (m_name);
}

/**
* Retrieve the font size.
*
* @return font size
*/
int FontBase::getSize()
{
    return (m_size);
}

/**
* Retrieve the index number associated with this base font.
*
* @return index number
*/
int FontBase::getIndex()
{
    return (m_index);
}

///**
//* {@inheritDoc}
//*/
//CString FontBase::toString()
//{
//    return ("[FontBase* name=" + m_name + " size=" + m_size + "]");
//}
