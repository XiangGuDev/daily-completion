#include "stdafx.h"
#include "ProjectEntity.h"
#include "ProjectFile.h"

/**
* Constructor.
*
* @param mpx Parent MPX file
*/
ProjectEntity::ProjectEntity(ProjectFile* mpx)
{
	m_mpx = mpx;
}

/**
* Accessor method allowing retrieval of ProjectFile reference.
*
* @return reference to this the parent ProjectFile instance
*/
ProjectFile* ProjectEntity::getParentFile()
{
    return (m_mpx);
}