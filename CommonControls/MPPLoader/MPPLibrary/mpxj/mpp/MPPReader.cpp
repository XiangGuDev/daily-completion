#include "stdafx.h"
#include "MPPReader.h"
#include "CompObj.h"
#include "MPP09Reader.h"
#include "MPP14Reader.h"
/**
* {@inheritDoc}
*/
void MPPReader::addProjectListener(ProjectListener* listener)
{
    m_projectListeners.Add(listener);
}

/**
* {@inheritDoc}
*/
ProjectFilePtr MPPReader::read(BwProj::toolkit::IDataReadStream* is)
{
    POIFSFileSystem fs(is);
    return read(fs);
}

/**
* Alternative entry point allowing an MPP file to be read from
* a user-supplied POI file stream. 
* 
* @param fs POI file stream
* @return ProjectFile instance
* @throws MPXJtoolkit::Exception
*/
ProjectFilePtr MPPReader::read(POIFSFileSystem& fs)
{
    ProjectFilePtr projectFile (new ProjectFile());

    projectFile->addProjectListeners(&m_projectListeners);
    projectFile->setAutoTaskID(false);
    projectFile->setAutoTaskUniqueID(false);
    projectFile->setAutoResource2ID(false);
    projectFile->setAutoResource2UniqueID(false);
    projectFile->setAutoOutlineLevel(false);
    projectFile->setAutoOutlineNumber(false);
    projectFile->setAutoWBS(false);
    projectFile->setAutoCalendarUniqueID(false);
    projectFile->setAutoAssignmentUniqueID(false);

    //
    // Open the file system and retrieve the root directory
    //
    DirectoryEntryPtr root = fs.getRoot();

    //
    // Retrieve the CompObj data, validate the file format and process
    //
	DocumentInputStream is(std::dynamic_pointer_cast<DocumentEntry> (root->getEntry(L"\1CompObj")));
	CompObj compObj(&is);
	//CString u =compObj.getApplicationName();//获取文件版本
    projectFile->setApplicationName(compObj.getApplicationName());
    CString format = compObj.getFileFormat();
    MPPVariantReaderPtr reader = FILE_CLASS_MAP(format);
	if (!reader)
    {
		//throw toolkit::Exception(L"暂不支持打开“" + FILE_CLASS_TO_VERSION(format) + "”版本的文件。");
		throw toolkit::Exception(L"暂不支持打开d版本的文件。");
    }
    reader->process(this, projectFile.get(), root);

    //
    // Update the internal structure. We'll take this opportunity to
    // generate outline numbers for the tasks as they don't appear to
    // be present in the MPP file.
    //
    projectFile->setAutoOutlineNumber(true);
    projectFile->updateStructure();
    projectFile->setAutoOutlineNumber(false);

    //
    // Perform post-processing to set the summary flag and clean
    // up any instances where a task has an empty splits list.
    //
	for (size_t i = 0; i < projectFile->getAllTasks()->GetCount(); i ++)
    {
		Task* task = projectFile->getAllTasks()->GetAt(i);
		task->setSummary(task->getChildTasks()->GetCount() != 0);
		DateRangeListPtr splits = task->getSplits();
		if (splits && splits->GetCount()==0)
		{
			task->setSplits(NULL);
		}
		validationRelations(task);
    }

    //
    // Ensure that the unique ID counters are correct
    //
    projectFile->updateUniqueCounters();

    return projectFile;
}

/**
* This method validates all relationships for a task, removing
* any which have been incorrectly read from the MPP file and 
* point to a parent task.
* 
* @param task task under test
*/
void MPPReader::validationRelations(Task* task)
{
    std::shared_ptr<List<RelationPtr>> predecessors = task->getPredecessors();
	if (!predecessors || !predecessors->GetCount())
		return;

	List<RelationPtr> invalid;// = new ArrayList<Relation>();
	for (size_t i = 0; i < predecessors->GetCount(); i ++)
    {
		RelationPtr relation = predecessors->GetAt(i);

		Task* sourceTask = relation->getSourceTask();
		Task* targetTask = relation->getTargetTask();

		CString sourceOutlineNumber = sourceTask->getOutlineNumber();
		CString targetOutlineNumber = targetTask->getOutlineNumber();

		if (sourceOutlineNumber.GetLength() && targetOutlineNumber.GetLength() &&
			sourceOutlineNumber.Find(targetOutlineNumber + L".") == 0)
		{
			invalid.Add(relation);
		}
    }

	for (size_t i = 0; i < invalid.GetCount(); i++)
    {
		RelationPtr relation = invalid.GetAt(i);
		relation->getSourceTask()->removePredecessor(relation->getTargetTask(),
										relation->getType(), relation->getLag());
    }
}

/**
* This method retrieves the state of the preserve note formatting flag.
*
* @return bool flag
*/
bool MPPReader::getPreserveNoteFormatting()
{
    return (m_preserveNoteFormatting);
}

/**
* This method sets a flag to indicate whether the RTF formatting associated
* with notes should be preserved or removed. By default the formatting
* is removed.
*
* @param preserveNoteFormatting bool flag
*/
void MPPReader::setPreserveNoteFormatting(bool preserveNoteFormatting)
{
    m_preserveNoteFormatting = preserveNoteFormatting;
}

/**
* If this flag is true, raw timephased data will be retrieved
* from MS Project: no normalisation will take place.
* 
* @return bool flag
*/
bool MPPReader::getUseRawTimephasedData()
{
    return m_useRawTimephasedData;
}

/**
* If this flag is true, raw timephased data will be retrieved
* from MS Project: no normalisation will take place. 
* 
* @param useRawTimephasedData bool flag
*/
void MPPReader::setUseRawTimephasedData(bool useRawTimephasedData)
{
    m_useRawTimephasedData = useRawTimephasedData;
}

/**
* Retrieves a flag which indicates whether presentation data will
* be read from the MPP file. Not reading this data saves time and memory. 
* 
* @return presentation data flag
*/
bool MPPReader::getReadPresentationData()
{
    return m_readPresentationData;
}

/**
* Flag to allow time and memory to be saved by not reading
* presentation data from the MPP file. 
* 
* @param readPresentationData set to false to prevent presentation data being read
*/
void MPPReader::setReadPresentationData(bool readPresentationData)
{
    m_readPresentationData = readPresentationData;
}

/**
* Flag to determine if the reader should only read the project header data.
* This allows for rapid access to the document properties, without the
* cost of reading the entire contents of the project file.
*    
* @return true if the reader should only read the project header 
*/
bool MPPReader::getReadHeaderOnly()
{
    return m_readHeaderOnly;
}

/**
* Flag to determine if the reader should only read the project header data.
* This allows for rapid access to the document properties, without the
* cost of reading the entire contents of the project file.
* 
* @param readHeaderOnly true if the reader should only read the project header
*/
void MPPReader::setReadHeaderOnly(bool readHeaderOnly)
{
    m_readHeaderOnly = readHeaderOnly;
}

/**
* Set the read password for this Project file. This is needed in order to
* be allowed to read a read-protected Project file.
* 
* Note: Set this each time before calling the read method.
* 
* @param password password text
*/
void MPPReader::setReadPassword(CString password)
{
    m_readPassword = password;
}

/**
* Internal only. Get the read password for this Project file. This is 
* needed in order to be allowed to read a read-protected Project file.
* 
* @return password password text
*/
CString MPPReader::getReadPassword()
{
    return m_readPassword;
}

/**
* Set the write password for this Project file. Currently not used.
* 
* Note: Set this each time before calling the read method.
*
* @param password password text
*/
void MPPReader::setWritePassword(CString password)
{
    m_writePassword = password;
}

/**
* Internal only. Get the write password for this Project file. 
* Currently not used.
* 
* @return password
*/
CString MPPReader::getWritePassword()
{
    return m_writePassword;
}

//CString MPPReader::getApplicationName(CString strFileName)
//{
//	CFileDataReadStream fis(NULL);
//	if (!fis.OpenFile(strFileName)) {
//		CString tipMsg = CFileToolkit::GetFileLogicName(strFileName) + L"：文件打开失败。";
//		throw toolkit::Exception(tipMsg);
//	}
//
//	POIFSFileSystem fs(&fis);
//	DirectoryEntryPtr root = fs.getRoot();
//	DocumentInputStream is(std::dynamic_pointer_cast<DocumentEntry> (root->getEntry(L"\1CompObj")));
//	CompObj compObj(&is);
//
//	fis.CloseFile();
//	return compObj.getApplicationName();
//}

/**
* Populate a map of file types and file processing classes.
*/
MPPVariantReaderPtr MPPReader::FILE_CLASS_MAP(CString typeName)
{
	//FILE_CLASS_MAP.put("MSProject.MPP8", MPP8Reader.class);
	//FILE_CLASS_MAP.put("MSProject.MPT8", MPP8Reader.class);

 //   FILE_CLASS_MAP.put("MSProject.MPP9", MPP9Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.MPT9", MPP9Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.GLOBAL9", MPP9Reader.class);
	if (typeName == L"MSProject.MPP9" || typeName == L"MSProject.MPT9" || typeName == L"MSProject.GLOBAL9")
		return MPPVariantReaderPtr(new MPP09Reader());

 //   FILE_CLASS_MAP.put("MSProject.MPP12", MPP12Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.MPT12", MPP12Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.GLOBAL12", MPP12Reader.class);
	if (typeName == L"MSProject.MPP12" || typeName == L"MSProject.MPT12" || typeName == L"MSProject.GLOBAL12")
		return MPPVariantReaderPtr(new MPP12Reader());

	if (typeName == L"MSProject.MPP14" || typeName == L"MSProject.MPT14" || typeName == L"MSProject.GLOBAL14")
		return MPPVariantReaderPtr(new MPP14Reader());
 //   FILE_CLASS_MAP.put("MSProject.MPP14", MPP14Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.MPT14", MPP14Reader.class);
 //   FILE_CLASS_MAP.put("MSProject.GLOBAL14", MPP14Reader.class);

	return MPPVariantReaderPtr(NULL);
}

/*根据文件类型转换成project版本*/
CString MPPReader::FILE_CLASS_TO_VERSION(CString typeName)
{
	if (typeName == L"MSProject.MPP8" || typeName == L"MSProject.MPT8")
		return L"Project 98";

	if (typeName == L"MSProject.MPP9" || typeName == L"MSProject.MPT9" || typeName == L"MSProject.GLOBAL9")
		return L"Project 2000-2003";

	if (typeName == L"MSProject.MPP12" || typeName == L"MSProject.MPT12" || typeName == L"MSProject.GLOBAL12")
		return L"Project 2007";

	if (typeName == L"MSProject.MPP14" || typeName == L"MSProject.MPT14" || typeName == L"MSProject.GLOBAL14")
		return L"Project 2010-2013";

	return typeName;
}