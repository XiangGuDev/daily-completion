#pragma once

#include "..\ProjectFile.h"
//#include "..\Toolkit\include\IDataStream.h"
class OutputStream;

/**
 * This interface is implemented by classes which can create project files
 * from the contents of an ProjectFile instance.
 */
class ProjectWriter
{
public:
   /**
    * Create a project file using the supplied file name.
    *
    * @param projectFile project file instance
    * @param fileName file name
    * @throws IOException
    */
   virtual void write(ProjectFile* projectFile, CString fileName) = 0;

   /**
    * Create a project file using the supplied File instance.
    *
    * @param projectFile project file instance
    * @param file File instance
    * @throws IOException
    */
   virtual void write(ProjectFile* projectFile, CAtlFile* file) = 0;

   /**
    * Write a project file's content to the supplied OutputStream instance.
    *
    * @param projectFile project file instance
    * @param outputStream OutputStream instance
    * @throws IOException
    */
   virtual void write(ProjectFile* projectFile, BwProj::toolkit::IDataWriteStream* outputStream) = 0;
};

typedef std::shared_ptr<ProjectWriter> ProjectWriterPtr;