#pragma once

/**
 * This class represents a sub project.
 */
class SubProject
{
public:
   /**
    * Retrieves the DOS file name.
    *
    * @return DOS file name
    */
	CString getDosFileName();

   /**
    * Sets the DOS file name.
    *
    * @param dosFileName DOS file name
    */
	void setDosFileName(CString dosFileName);

   /**
    * Retrieves the DOS full path.
    *
    * @return DOS full path
    */
	CString getDosFullPath();

   /**
    * Sets the DOS full path.
    *
    * @param dosFullPath DOS full path
    */
	void setDosFullPath(CString dosFullPath);

   /**
    * Retrieve the file name.
    *
    * @return file name
    */
	CString getFileName();

   /**
    * Sets the file name.
    *
    * @param fileName file name
    */
	void setFileName(CString fileName);

   /**
    * Retrieve the full path.
    *
    * @return full path
    */
	CString getFullPath();

   /**
    * Sets the full path.
    *
    * @param fullPath full path
    */
	void setFullPath(CString fullPath);

   /**
    * Retrieves the offset applied to task unique IDs
    * from the sub project.
    *
    * @return unique ID offset
    */
	int getUniqueIDOffset();

   /**
    * Set the the offset applied to task unique IDs
    * from the sub project.
    *
    * @param uniqueIDOffset unique ID offset
    */
	void setUniqueIDOffset(int uniqueIDOffset);

   /**
    * Retrieve the unique ID of the task to which this subproject
    * relates.
    *
    * @return task Unique ID
    */
	int getTaskUniqueID();

   /**
    * Set the unique ID of the task to which this subproject relates.
    *
    * @param taskUniqueID task unique ID
    */
	void setTaskUniqueID(int taskUniqueID);

   /**
    * Check to see if the given task is an external task from this subproject.
    *
    * @param taskUniqueID task unique ID
    * @return true if the task is external
    */
	bool isExternalTask(int taskUniqueID);

   /**
    * This package-private method is used to add external task unique id.
    *
    * @param externalTaskUniqueID external task unique id
    */
	void addExternalTaskUniqueID(int externalTaskUniqueID);

   /**
    * Retrieves all the external task unique ids for this project file->
    *
    * @return all sub project details
    */
	List<int>& getAllExternalTaskUniqueIDs();

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

   private:
	   int m_taskUniqueID;
	   int m_uniqueIDOffset;
	   List<int> m_externalTaskUniqueIDs;// = new List<int>();
	   CString m_dosFullPath;
	   CString m_fullPath;
	   CString m_dosFileName;
	   CString m_fileName;
};

typedef std::shared_ptr<SubProject> SubProjectPtr;