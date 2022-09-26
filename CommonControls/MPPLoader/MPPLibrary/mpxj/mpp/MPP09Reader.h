#pragma once

#include "MPPVariantReader.h"
#include "FieldMap.h"
#include "Var2Data.h"
#include "MppBitFlag.h"
#include "Props14.h"
#include "FixedMeta.h"
#include "FixedData.h"

#include "..\ProjectFile.h"

#include "..\poifs\DirectoryEntry.h"
#include "..\poifs\DocumentEntry.h"
#include "..\poifs\DocumentInputStream.h"

/**
* This class is used to represent a Microsoft Project MPP14 file. This
* implementation allows the file to be read, and the data it contains
* exported as a set of MPX objects. These objects can be interrogated
* to retrieve any required data, or stored as an MPX file.
*/
class MPP09Reader : public MPPVariantReader
{
public:
	MPP09Reader();

	/**
	* This method is used to process an MPP14 file. This is the file format
	* used by Project 14.
	*
	* @param reader parent file reader
	* @param file parent MPP file
	* @param root Root of the POI file system.
	*/
	virtual void process(MPPReader* reader, ProjectFile* file, std::shared_ptr<DirectoryEntry> root);

private:

	// 获取不同版本参数
	virtual const MPPVersionParams* getVersionParams();
	virtual VarMetaPtr newVarMetaObj(BwProj::toolkit::IDataReadStream* is);

	/**
	* Populate member data used by the rest of the reader.
	*
	* @param reader parent file reader
	* @param file parent MPP file
	* @param root Root of the POI file system.
	*/
	void populateMemberData(MPPReader* reader, ProjectFile* file, DirectoryEntryPtr root);

	/**
	* Clear transient member data.
	*/
	void clearMemberData();

	/**
	* This method extracts and collates the value list information
	* for custom column value lists.
	*/
	void processCustomValueLists();

	/**
	* Process the project header data.
	*/
	void processProjectHeader();

	///**
	// * Process the graphical indicator data.
	// */
	//void processGraphicalIndicators();

	/**
	* Read sub project data from the file, and add it to a hash map
	* indexed by task ID.
	*
	* Project stores all subprojects that have ever been inserted into this project
	* in sequence and that is what used to count unique id offsets for each of the
	* subprojects.
	*/
	void processSubProjectData();

	/**
	* Read a list of sub projects.
	*
	* @param data byte array
	* @param uniqueIDOffset offset of unique ID
	* @param filePathOffset offset of file path
	* @param fileNameOffset offset of file name
	* @param subprojectIndex index of the subproject, used to calculate unique id offset
	*/
	void readSubProjects(ByteArrayPtr data, int uniqueIDOffset, int filePathOffset, int fileNameOffset, int subprojectIndex);

	/**
	* Method used to read the sub project details from a byte array.
	*
	* @param data byte array
	* @param uniqueIDOffset offset of unique ID
	* @param filePathOffset offset of file path
	* @param fileNameOffset offset of file name
	* @param subprojectIndex index of the subproject, used to calculate unique id offset
	* @return new SubProject instance
	*/
	SubProjectPtr readSubProject(ByteArrayPtr data, int uniqueIDOffset, int filePathOffset, int fileNameOffset, int subprojectIndex);

	/**
	* This method process the data held in the props file specific to the
	* visual appearance of the project data.
	*/
	void processViewPropertyData();

	/**
	* Create an index of base font numbers and their associated base
	* font instances.
	* @param data property data
	*/
	void processBaseFonts(ByteArrayPtr data);

	/**
	* Retrieve any task field aliases defined in the MPP file.
	*
	* @param data task field name alias data
	*/
	void processTaskFieldNameAliases(ByteArrayPtr data);

	/**
	* Retrieve any resource field aliases defined in the MPP file.
	*
	* @param data resource field name alias data
	*/
	void processResource2FieldNameAliases(ByteArrayPtr data);

	/**
	* This method maps the task unique identifiers to their index number，排序了的
	* within the FixedData block.
	*
	* @param fieldMap field map
	* @param taskFixedMeta Fixed meta data for this task
	* @param taskFixedData Fixed data for this task
	* @return Mapping between task identifiers and block position
	*/
	std::shared_ptr<TreeMap<int, int>> createTaskMap(FieldMap* fieldMap, FixedMetaPtr taskFixedMeta, FixedDataPtr taskFixedData);

	/**
	* This method maps the resource unique identifiers to their index number
	* within the FixedData block.
	*
	* @param fieldMap field map
	* @param rscFixedMeta resource fixed meta data
	* @param rscFixedData resource fixed data
	* @return map of resource IDs to resource data
	*/
	std::shared_ptr<TreeMap<int, int>> createResource2Map(FieldMap* fieldMap, FixedMetaPtr rscFixedMeta, FixedDataPtr rscFixedData);

	/**
	* The format of the calendar data is a 4 byte header followed
	* by 7x 60 byte blocks, one for each day of the week. Optionally
	* following this is a set of 64 byte blocks representing exceptions
	* to the calendar.
	*/
	virtual void processCalendarData();

	/**
	* For a given set of calendar data, this method sets the working
	* day status for each day, and if present, sets the hours for that
	* day.
	*
	* NOTE: MPP14 defines the concept of working weeks. MPXJ does not
	* currently support this, and thus we only read the working hours
	* for the default working week.
	*
	* @param data calendar data block
	* @param defaultCalendar calendar to use for default values
	* @param cal calendar instance
	* @param isBaseCalendar true if this is a base calendar
	*/
	void processCalendarHours(ByteArrayPtr data, ProjectCalendarPtr defaultCalendar, ProjectCalendarPtr cal, bool isBaseCalendar);

	/**
	* This method extracts any exceptions associated with a calendar.
	*
	* @param data calendar data block
	* @param cal calendar instance
	*/
	void processCalendarExceptions(ByteArrayPtr data, ProjectCalendarPtr cal);

	/**
	* The way calendars are stored in an MPP14 file means that there
	* can be forward references between the base calendar unique ID for a
	* derived calendar, and the base calendar itself. To get around this,
	* we initially populate the base calendar name attribute with the
	* base calendar unique ID, and now in this method we can convert those
	* ID values into the correct names.
	*
	* @param baseCalendars list of calendars and base calendar IDs
	* @param map map of calendar ID values and calendar objects
	*/
	void updateBaseCalendarNames(List<std::pair<ProjectCalendarPtr, int>>* baseCalendars, HashMap<int, ProjectCalendarPtr>* map);

	/**
	* This method extracts and collates task data. The code below
	* goes through the modifier methods of the Task class in alphabetical
	* order extracting the data from the MPP file. Where there is no
	* mapping (e.g. the field is calculated on the fly, or we can't
	* find it in the data) the line is commented out.
	*
	* The missing bool attributes are probably represented in the Props
	* section of the task data, which we have yet to decode.
	*
	* @throws java.io.IOException
	*/
	void processTaskData();

	/**
	* MPP14 files seem to exhibit some occasional weirdness
	* with duplicate ID values which leads to the task structure
	* being reported incorrectly. The following method attempts to correct this.
	* The method uses ordering data embedded in the file to reconstruct
	* the correct ID order of the tasks.
	*/
	void postProcessTasks();

	/**
	* Extracts task enterprise column values.
	*
	* @param id task unique ID
	* @param task task instance
	* @param taskVarData task var data
	* @param metaData2 task meta data
	*/
	void processTaskEnterpriseColumns(int id, Task* task, Var2DataPtr taskVarData, ByteArrayPtr metaData2);

	/**
	* Extracts resource enterprise column data.
	*
	* @param id resource unique ID
	* @param resource resource instance
	* @param resourceVarData resource var data
	* @param metaData2 resource meta data
	*/
	void processResource2EnterpriseColumns(FieldMapPtr fieldMap, Resource2* resource, Var2DataPtr resourceVarData);

	/**
	* The project files to which external tasks relate appear not to be
	* held against each task, instead there appears to be the concept
	* of the "current" external task file, i.e. the last one used.
	* This method iterates through the list of tasks marked as external
	* and attempts to ensure that the correct external project data (in the
	* form of a SubProject object) is linked to the task.
	*
	* @param externalTasks list of tasks marked as external
	*/
	void processExternalTasks(List<Task*>* externalTasks);

	/**
	* This method is used to extract the task hyperlink attributes
	* from a block of data and call the appropriate modifier methods
	* to configure the specified task object.
	*
	* @param task task instance
	* @param data hyperlink data block
	*/
	void processHyperlinkData(Task* task, ByteArrayPtr data);

	/**
	* This method is used to extract the resource hyperlink attributes
	* from a block of data and call the appropriate modifier methods
	* to configure the specified task object.
	*
	* @param resource resource instance
	* @param data hyperlink data block
	*/
	void processHyperlinkData(Resource2Ptr resource, ByteArrayPtr data);

	/**
	* This method extracts and collates constraint data.
	*
	* @throws java.io.IOException
	*/
	void processConstraintData();

	/**
	* This method extracts and collates resource data.
	*
	* @throws java.io.IOException
	*/
	void processResource2Data();

	/**
	* This method extracts and collates resource assignment data.
	*
	* @throws IOException
	*/
	void processAssignmentData();

	/**
	* This method is used to determine if a duration is estimated.
	*
	* @param type Duration units value
	* @return bool Estimated flag
	*/
	bool getDurationEstimated(int type);

	///**
	// * This method extracts view data from the MPP file.
	// *
	// * @throws java.io.IOException
	// */
	//void processViewData();

	///**
	// * This method extracts table data from the MPP file.
	// *
	// * @throws java.io.IOException
	// */
	//void processTableData();

	///**
	// * Read filter definitions.
	// *
	// * @throws IOException
	// */
	//void processFilterData();

	///**
	// * Read saved view state from an MPP file.
	// *
	// * @throws IOException
	// */
	//void processSavedViewState();

	///**
	// * Read group definitions.
	// *
	// * @throws IOException
	// */
	//void processGroup2Data();

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param outlineCodeVarData var data block
	* @param id item ID
	* @param type item type
	* @return item value
	*/
	CString getCustomFieldOutlineCodeValue(Var2DataPtr varData, Var2DataPtr outlineCodeVarData, int id, int type);

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param outlineCodeVarData var data block
	* @param id parent item ID
	* @return item value
	*/
	CString getCustomFieldOutlineCodeValue(Var2DataPtr varData, Var2DataPtr outlineCodeVarData, int id);

	/**
	* Iterate through a set of bit field flags and set the value for each one
	* in the supplied container.
	*
	* @param flags bit field flags
	* @param container field container
	* @param data source data
	*/
	void readBitFields(MppBitFlagArrayPtr flags, FieldContainer* container, ByteArrayPtr data);

protected:
	//void INIT_STATIC_VAR_META_DATA_BIT_FLAGS();

protected:
	MPPReader* m_reader;
	ProjectFile* m_file;
	DirectoryEntryPtr m_root;
	HashMap<int, ProjectCalendarPtr> m_resourceMap;
	Var2DataPtr m_outlineCodeVarData;
	VarMetaPtr m_outlineCodeVarMeta;
	FixedDataPtr m_outlineCodeFixedData;
	FixedMetaPtr m_outlineCodeFixedMeta;
	//FixedData m_outlineCodeFixedData2;
	//FixedMeta m_outlineCodeFixedMeta2;
	Props14Ptr m_projectProps;
	Map2<int, FontBasePtr> m_fontBases;
	Map2<int, SubProjectPtr> m_taskSubProjects;
	DirectoryEntryPtr m_projectDir;
	//DirectoryEntryPtr m_viewDir;
	Map2<int, int> m_parentTasks;
	Map2<__int64, int> m_taskOrder;
	//Map<int, int> m_nullTaskOrder;


	// Signals the end of the list of subproject task unique ids
	//static const int SUBPROJECT_LISTEND = 0x00000303;

	// Signals that the previous value was for the subproject task unique id
	static const int SUBPROJECT_TASKUNIQUEID0 = 0x00000000;
	static const int SUBPROJECT_TASKUNIQUEID1 = 0x0B340000;
	static const int SUBPROJECT_TASKUNIQUEID2 = 0x0ABB0000;
	static const int SUBPROJECT_TASKUNIQUEID3 = 0x05A10000;
	static const int SUBPROJECT_TASKUNIQUEID4 = 0x02F70000;
	static const int SUBPROJECT_TASKUNIQUEID5 = 0x07010000;

	/**
	* Calendar data types.
	*/
	static const int CALENDAR_NAME = 1;
	static const int CALENDAR_DATA = 3;

	/**
	* Resource2 data types.
	*/
	static const int TABLE_COLUMN_DATA_STANDARD = 1;
	static const int TABLE_COLUMN_DATA_ENTERPRISE = 2;
	static const int TABLE_COLUMN_DATA_BASELINE = 0;
	static const int OUTLINECODE_DATA = 1;

	/**
	* Custom value list types.
	*/
	static const int VALUE_LIST_VALUE = 22;
	static const int VALUE_LIST_DESCRIPTION = 8;
	static const int VALUE_LIST_UNKNOWN = 23;
	static const int VALUE_LIST_MASK = 0x0700;

	/**
	* Mask used to isolate confirmed flag from the duration units field.
	*/
	static const int DURATION_CONFIRMED_MASK = 0x20;

	/**
	* Deleted and null tasks have their ID and UniqueID attributes at fixed offsets.
	*/
	static const int TASK_UNIQUE_ID_FIXED_OFFSET = 0;
	static const int TASK_ID_FIXED_OFFSET = 4;
	static const int NULL_TASK_BLOCK_SIZE = 8;

	/**
	* Default working week.
	*/
	static const bool DEFAULT_WORKING_WEEK[];
};