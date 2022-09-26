#pragma once

#include "..\reader\AbstractProjectReader.h"
#include "MPPVariantReader.h"
#include "..\ProjectFile.h"

/**
 * This class creates a new ProjectFile instance by reading an MPP file.
 */
class MPPReader : public AbstractProjectReader
{
public:
   /**
    * {@inheritDoc}
    */
	virtual void addProjectListener(ProjectListener* listener);

   /**
    * {@inheritDoc}
    */
	virtual ProjectFilePtr read(BwProj::toolkit::IDataReadStream* is);

   /**
    * Alternative entry point allowing an MPP file to be read from
    * a user-supplied POI file stream. 
    * 
    * @param fs POI file stream
    * @return ProjectFile instance
    * @throws MPXJException
    */
	ProjectFilePtr read(POIFSFileSystem& fs);

   /**
    * This method retrieves the state of the preserve note formatting flag.
    *
    * @return bool flag
    */
	bool getPreserveNoteFormatting();

   /**
    * This method sets a flag to indicate whether the RTF formatting associated
    * with notes should be preserved or removed. By default the formatting
    * is removed.
    *
    * @param preserveNoteFormatting bool flag
    */
	void setPreserveNoteFormatting(bool preserveNoteFormatting);

   /**
    * If this flag is true, raw timephased data will be retrieved
    * from MS Project: no normalisation will take place.
    * 
    * @return bool flag
    */
	bool getUseRawTimephasedData();

   /**
    * If this flag is true, raw timephased data will be retrieved
    * from MS Project: no normalisation will take place. 
    * 
    * @param useRawTimephasedData bool flag
    */
	void setUseRawTimephasedData(bool useRawTimephasedData);

   /**
    * Retrieves a flag which indicates whether presentation data will
    * be read from the MPP file. Not reading this data saves time and memory. 
    * 
    * @return presentation data flag
    */
	bool getReadPresentationData();

   /**
    * Flag to allow time and memory to be saved by not reading
    * presentation data from the MPP file. 
    * 
    * @param readPresentationData set to false to prevent presentation data being read
    */
	void setReadPresentationData(bool readPresentationData);

   /**
    * Flag to determine if the reader should only read the project header data.
    * This allows for rapid access to the document properties, without the
    * cost of reading the entire contents of the project file.
    *    
    * @return true if the reader should only read the project header 
    */
	bool getReadHeaderOnly();

   /**
    * Flag to determine if the reader should only read the project header data.
    * This allows for rapid access to the document properties, without the
    * cost of reading the entire contents of the project file.
    * 
    * @param readHeaderOnly true if the reader should only read the project header
    */
	void setReadHeaderOnly(bool readHeaderOnly);

   /**
    * Set the read password for this Project file. This is needed in order to
    * be allowed to read a read-protected Project file.
    * 
    * Note: Set this each time before calling the read method.
    * 
    * @param password password text
    */
	void setReadPassword(CString password);

   /**
    * Internal only. Get the read password for this Project file. This is 
    * needed in order to be allowed to read a read-protected Project file.
    * 
    * @return password password text
    */
	CString getReadPassword();

   /**
    * Set the write password for this Project file. Currently not used.
    * 
    * Note: Set this each time before calling the read method.
    *
    * @param password password text
    */
	void setWritePassword(CString password);

   /**
    * Internal only. Get the write password for this Project file. 
    * Currently not used.
    * 
    * @return password
    */
	CString getWritePassword();

	/**
	* 获取打开文件的版本
	* 
	* @param   文件全路径
	* @return password
	*/
	//CString getApplicationName(CString strFileName);

private:
	/**
	* This method validates all relationships for a task, removing
	* any which have been incorrectly read from the MPP file and
	* point to a parent task.
	*
	* @param task task under test
	*/
	void validationRelations(Task* task);

	/**
	* Populate a map of file types and file processing classes.
	*/
	MPPVariantReaderPtr FILE_CLASS_MAP(CString typeName);

	/*根据文件类型转换成project版本*/
	CString FILE_CLASS_TO_VERSION(CString typeName);

private:

   /**
    * Flag used to indicate whether RTF formatting in notes should
    * be preserved. The default value for this flag is false.
    */
   bool m_preserveNoteFormatting = false;

   /**
    * Setting this flag to true allows raw timephased data to be retrieved. 
    */
   bool m_useRawTimephasedData = false;

   /**
    * Flag to allow time and memory to be saved by not reading
    * presentation data from the MPP file.
    */
   bool m_readPresentationData = true;
   bool m_readHeaderOnly = false;

   CString m_readPassword;
   CString m_writePassword;
   List<ProjectListener*> m_projectListeners;
};