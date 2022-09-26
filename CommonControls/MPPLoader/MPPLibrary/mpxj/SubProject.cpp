#pragma once
#include "stdafx.h"
#include "SubProject.h"

/**
* Retrieves the DOS file name.
*
* @return DOS file name
*/
CString SubProject::getDosFileName()
{
    return (m_dosFileName);
}

/**
* Sets the DOS file name.
*
* @param dosFileName DOS file name
*/
void SubProject::setDosFileName(CString dosFileName)
{
    m_dosFileName = dosFileName;
}

/**
* Retrieves the DOS full path.
*
* @return DOS full path
*/
CString SubProject::getDosFullPath()
{
    return (m_dosFullPath);
}

/**
* Sets the DOS full path.
*
* @param dosFullPath DOS full path
*/
void SubProject::setDosFullPath(CString dosFullPath)
{
    m_dosFullPath = dosFullPath;
}

/**
* Retrieve the file name.
*
* @return file name
*/
CString SubProject::getFileName()
{
    return (m_fileName);
}

/**
* Sets the file name.
*
* @param fileName file name
*/
void SubProject::setFileName(CString fileName)
{
    m_fileName = fileName;
}

/**
* Retrieve the full path.
*
* @return full path
*/
CString SubProject::getFullPath()
{
    return (m_fullPath);
}

/**
* Sets the full path.
*
* @param fullPath full path
*/
void SubProject::setFullPath(CString fullPath)
{
    m_fullPath = fullPath;
}

/**
* Retrieves the offset applied to task unique IDs
* from the sub project.
*
* @return unique ID offset
*/
int SubProject::getUniqueIDOffset()
{
    return (m_uniqueIDOffset);
}

/**
* Set the the offset applied to task unique IDs
* from the sub project.
*
* @param uniqueIDOffset unique ID offset
*/
void SubProject::setUniqueIDOffset(int uniqueIDOffset)
{
    m_uniqueIDOffset = uniqueIDOffset;
}

/**
* Retrieve the unique ID of the task to which this subproject
* relates.
*
* @return task Unique ID
*/
int SubProject::getTaskUniqueID()
{
    return (m_taskUniqueID);
}

/**
* Set the unique ID of the task to which this subproject relates.
*
* @param taskUniqueID task unique ID
*/
void SubProject::setTaskUniqueID(int taskUniqueID)
{
    m_taskUniqueID = taskUniqueID;
}

/**
* Check to see if the given task is an external task from this subproject.
*
* @param taskUniqueID task unique ID
* @return true if the task is external
*/
bool SubProject::isExternalTask(int taskUniqueID)
{
	for (size_t i = 0; i < m_externalTaskUniqueIDs.GetCount(); i++)
		if (m_externalTaskUniqueIDs[i] == taskUniqueID)
			return true;
	return false;
    //return m_externalTaskUniqueIDs.contains(taskUniqueID);
}

/**
* This package-private method is used to add external task unique id.
*
* @param externalTaskUniqueID external task unique id
*/
void SubProject::addExternalTaskUniqueID(int externalTaskUniqueID)
{
    m_externalTaskUniqueIDs.Add(externalTaskUniqueID);
}

/**
* Retrieves all the external task unique ids for this project file->
*
* @return all sub project details
*/
List<int>& SubProject::getAllExternalTaskUniqueIDs()
{
    return (m_externalTaskUniqueIDs);
}
//
///**
//* {@inheritDoc}
//*/
//CString SubProject::toString()
//{
//    return ("[SubProject taskUniqueID=" + m_taskUniqueID + " uniqueIDOffset=" + m_uniqueIDOffset + " path=" + m_fullPath + "]");
//}