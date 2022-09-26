#pragma once

#include "ProjectReader.h"

/**
 * Abstract implementation of the ProjectReader interface
 * which supplies implementations of the trivial read methods.
 */
class AbstractProjectReader : public ProjectReader
{
public:
   /**
    * {@inheritDoc}
    */
	virtual ProjectFilePtr read(CString fileName);

   /**
    * {@inheritDoc}
    */
	virtual ProjectFilePtr read(CAtlFile* file);

	/**
	* {@inheritDoc}
	*/
	virtual ProjectFilePtr read(BwProj::toolkit::IDataReadStream* inputStream) = 0;
};