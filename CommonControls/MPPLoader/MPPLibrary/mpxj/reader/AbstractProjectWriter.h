#pragma once

#include "ProjectWriter.h"

/**
 * Abstract implementation of the ProjectWriter interface
 * which supplies implementations of the trivial write methods.
 */
class AbstractProjectWriter : public ProjectWriter
{
public:
   /**
    * {@inheritDoc}
    */
	virtual void write(ProjectFile* projectFile, CString fileName);

   /**
    * {@inheritDoc}
    */
	virtual void write(ProjectFile* projectFile, CAtlFile* file);

	/**
	* {@inheritDoc}
	*/
	virtual void write(ProjectFile* projectFile, BwProj::toolkit::IDataWriteStream* outputStream) = 0;
};