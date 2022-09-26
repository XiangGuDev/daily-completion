#include "stdafx.h"
#include "POIFSDocumentPath.h"
#include "..\java\StringBuilder.h"

POIFSDocumentPath::POIFSDocumentPath(const std::vector<CString>& components)
{
    //if (components == null)
    //{
    //    this.components = new CString[ 0 ];
    //}
    //else
    //{
        //this.components = new CString[ components.length ];
        for (size_t j = 0; j < components.size(); j++)
        {
			if (components[j].GetLength() == 0)
            {
                throw toolkit::Exception(L"components cannot contain null or empty strings");
            }
			m_components[j] = components[j];
        }
    //}
}

/**
    * simple constructor for the path of a document that is in the
    * root of the POIFSFileSystem. The constructor that takes an
    * array of Strings can also be used to create such a
    * POIFSDocumentPath by passing it a null or empty CString array
    */

POIFSDocumentPath::POIFSDocumentPath()
{
    //this.components = new CString[ 0 ];
}

/**
    * constructor that adds additional subdirectories to an existing
    * path
    *
    * @param path the existing path
    * @param components the additional subdirectory names to be added
    *
    * @exception IllegalArgumentException if any of the Strings in
    *                                     components is null or zero
    *                                     length
    */

POIFSDocumentPath::POIFSDocumentPath(POIFSDocumentPathPtr path, const  std::vector<CString>& components)
{
    //if (components == null)
    //{
    //    this.components = new CString[ path.components.length ];
    //}
    //else
    //{
    //    this.components =
    //        new CString[ path.components.length + components.length ];
    //}
    //for (int j = 0; j < path.components.length; j++)
    //{
    //    this.components[ j ] = path.components[ j ];
    //}

	//m_components.Append(path->m_components);
	for (auto &strComp : path->m_components)
		m_components.push_back(strComp);

    //if (components != null)
    //{
        for (size_t j = 0; j < components.size(); j++)
        {
			if (components[j].GetLength() == 0)
			{
				throw toolkit::Exception(L"components cannot contain null or empty strings");
			}
                
			m_components.push_back(components[ j ]);
        }
    //}
}

///**
//    * equality. Two POIFSDocumentPath instances are equal if they
//    * have the same number of component Strings, and if each
//    * component CString is equal to its coresponding component CString
//    *
//    * @param o the object we're checking equality for
//    *
//    * @return true if the object is equal to this object
//    */
//
//bool POIFSDocumentPath::equals(Object* o)
//{
//    boolean rval = false;
//
//    if ((o != null) && (o.getClass() == this.getClass()))
//    {
//        if (this == o)
//        {
//            rval = true;
//        }
//        else
//        {
//            POIFSDocumentPath path = ( POIFSDocumentPath ) o;
//
//            if (path.components.length == this.components.length)
//            {
//                rval = true;
//                for (int j = 0; j < this.components.length; j++)
//                {
//                    if (!path.components[ j ]
//                            .equals(this.components[ j ]))
//                    {
//                        rval = false;
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    return rval;
//}
//
///**
//    * calculate and return the hashcode
//    *
//    * @return hashcode
//    */
//
//int POIFSDocumentPath::hashCode()
//{
//    if (hashcode == 0)
//    {
//        for (int j = 0; j < components.length; j++)
//        {
//            hashcode += components[ j ].hashCode();
//        }
//    }
//    return hashcode;
//}

/**
    * @return the number of components
    */

int POIFSDocumentPath::length()
{
	return m_components.size();
}

/**
    * get the specified component
    *
    * @param n which component (0 ... length() - 1)
    *
    * @return the nth component;
    *
    * @exception ArrayIndexOutOfBoundsException if n < 0 or n >=
    *                                           length()
    */

CString POIFSDocumentPath::getComponent(int n)
{
	return m_components[n];
}

/**
    * <p>Returns the path's parent or <code>null</code> if this path
    * is the root path.</p>
    *
    * @since 2002-01-24
    * @return path of parent, or null if this path is the root path
    */

POIFSDocumentPathPtr POIFSDocumentPath::getParent()
{
	int len = length() - 1;

	POIFSDocumentPathPtr parent;
	if (len < 0)
    {
		return parent;
    }
    parent.reset (new POIFSDocumentPath());

	// 复制当前内容并删除最后一个
	//parent->m_components.Append(m_components);// = new CString[length];
	//parent->m_components.RemoveAt(len);
	for (size_t s = 0; s < m_components.size() - 1; ++s)
		parent->m_components.push_back(m_components[s]);

    //System.arraycopy(components, 0, parent.components, 0, length);
    return parent;
}

/**
    * <p>Returns a string representation of the path. Components are
    * separated by the platform-specific file separator.</p>
    *
    * @return string representation
    *
    * @since 2002-01-24
    */

CString POIFSDocumentPath::toString()
{
    StringBuilder b;
    int          l = length();

    b.append(L"\\");
    for (int i = 0; i < l; i++)
    {
        b.append(getComponent(i));
        if (i < l - 1)
        {
			b.append(L"\\");
        }
    }
    return b.toString();
}