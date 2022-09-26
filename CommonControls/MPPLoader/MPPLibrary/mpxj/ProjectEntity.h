#pragma once

class ProjectFile;

/**
 * This is the base class from which all classes representing records found
 * in an MPX file are derived. It contains common functionality and
 * attribute storage used by all of the derived classes.
 */
class ProjectEntity
{
public:
	/**
	* Constructor.
	*
	* @param mpx Parent MPX file
	*/
	ProjectEntity(ProjectFile* mpx);

   /**
    * Accessor method allowing retrieval of ProjectFile reference.
    *
    * @return reference to this the parent ProjectFile instance
    */
	ProjectFile* getParentFile();

private:
   /**
    * Reference to parent ProjectFile.
    */
    ProjectFile* m_mpx;
};