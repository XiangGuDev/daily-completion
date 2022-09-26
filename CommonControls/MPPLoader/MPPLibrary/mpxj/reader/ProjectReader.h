#pragma once

#include "..\ProjectFile.h"
#include "..\..\..\Toolkit\include\IDataStream2.h"

/**
 * This interface is implemented by all classes which can read project
 * files of any type and generate an ProjectFile instance from the contents
 * of the file.
 */
class ProjectReader
{
public:
   /**
    * Add a listener to receive events as a project is being read.
    * 
    * @param listener ProjectListener instance
    */
   virtual void addProjectListener(ProjectListener* listener) = 0;

   /**
    * Read a file where the file name is supplied.
    *
    * @param fileName file name
    * @return ProjectFile instance
    * @throws MPXJException
    */
   virtual ProjectFilePtr read(CString fileName) = 0;

   /**
    * Read a file where a File instance is supplied.
    *
    * @param file File instance
    * @return ProjectFile instance
    * @throws MPXJException
    */
   virtual ProjectFilePtr read(CAtlFile* file) = 0;

   /**
    * Read a file where the contents of the project file
    * are supplied via an input stream.
    *
    * @param inputStream InputStream instance
    * @return ProjectFile instance
    * @throws MPXJException
    */
   virtual ProjectFilePtr read(BwProj::toolkit::IDataReadStream* inputStream) = 0;
};

typedef std::shared_ptr<ProjectReader> ProjectReaderPtr;