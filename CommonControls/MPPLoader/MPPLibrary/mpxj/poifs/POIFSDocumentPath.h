#pragma once

/**
 * Class POIFSDocumentPath
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 * @version %I%, %G%
 */

class POIFSDocumentPath
{
public:
    /**
     * constructor for the path of a document that is not in the root
     * of the POIFSFileSystem
     *
     * @param components the Strings making up the path to a document.
     *                   The Strings must be ordered as they appear in
     *                   the directory hierarchy of the the document
     *                   -- the first string must be the name of a
     *                   directory in the root of the POIFSFileSystem,
     *                   and every Nth (for N > 1) string thereafter
     *                   must be the name of a directory in the
     *                   directory identified by the (N-1)th string.
     *                   <p>
     *                   If the components parameter is null or has
     *                   zero length, the POIFSDocumentPath is
     *                   appropriate for a document that is in the
     *                   root of a POIFSFileSystem
     *
     * @exception IllegalArgumentException if any of the elements in
     *                                     the components parameter
     *                                     are null or have zero
     *                                     length
     */

	POIFSDocumentPath(const std::vector<CString>& components);

    /**
     * simple constructor for the path of a document that is in the
     * root of the POIFSFileSystem. The constructor that takes an
     * array of Strings can also be used to create such a
     * POIFSDocumentPath by passing it a null or empty CString array
     */

	POIFSDocumentPath();

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

	POIFSDocumentPath(std::shared_ptr<POIFSDocumentPath> path, const std::vector<CString>& components);

 //   /**
 //    * equality. Two POIFSDocumentPath instances are equal if they
 //    * have the same number of component Strings, and if each
 //    * component CString is equal to its coresponding component CString
 //    *
 //    * @param o the object we're checking equality for
 //    *
 //    * @return true if the object is equal to this object
 //    */

	//bool equals(Object* o);

 //   /**
 //    * calculate and return the hashcode
 //    *
 //    * @return hashcode
 //    */

	//int hashCode();

    /**
     * @return the number of components
     */

	int length();

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

	CString getComponent(int n);

    /**
     * <p>Returns the path's parent or <code>null</code> if this path
     * is the root path.</p>
     *
     * @since 2002-01-24
     * @return path of parent, or null if this path is the root path
     */

	std::shared_ptr<POIFSDocumentPath> getParent();

    /**
     * <p>Returns a string representation of the path. Components are
     * separated by the platform-specific file separator.</p>
     *
     * @return string representation
     *
     * @since 2002-01-24
     */

	CString toString();

private:
	std::vector<CString> m_components;
	int      hashcode = 0;
};

typedef std::shared_ptr<POIFSDocumentPath> POIFSDocumentPathPtr;