#pragma once

/**
 * This class represents a resource assignment workgroup fields record
 * from an MPX file->
 */
class Resource2AssignmentWorkgroupFields
{
public:
   /**
    * Sets the Message Unique ID.
    *
    * @param val ID
    */
	void setMessageUniqueID(CString val);

   /**
    * Gets the Message Unique ID.
    *
    * @return ID
    */
	CString getMessageUniqueID();

   /**
    * Gets confirmed flag.
    *
    * @return bool value
    */
	bool getConfirmed();

   /**
    * Sets confirmed flag.
    *
    * @param val bool flag
    */
	void setConfirmed(bool val);

   /**
    * Sets response pending flag.
    *
    * @param val bool flag
    */
	void setResponsePending(bool val);

   /**
    * Retrieves response pending flag.
    *
    * @return bool flag
    */
	bool getResponsePending();

   /**
    * Sets the Update Start Field.
    *
    * @param val date to set
    */
	void setUpdateStart(DatePtr val);

   /**
    * Gets the Update Start Field value.
    *
    * @return update Start Date
    */
	DatePtr getUpdateStart();

   /**
    * Sets the Update Finish Field.
    *
    * @param val date to set
    */
	void setUpdateFinish(DatePtr val);

   /**
    * Gets the Update Finish Field value.
    *
    * @return update Finish Date
    */
	DatePtr getUpdateFinish();

   /**
    * Sets the schedule ID.
    *
    * @param val schedule ID
    */
	void setScheduleID(CString val);

   /**
    * Retrieves the schedule ID.
    *
    * @return schedule ID
    */
	CString getScheduleID();

	static const Resource2AssignmentWorkgroupFields EMPTY;// = new Resource2AssignmentWorkgroupFields();

private:
   CString m_messageUniqueID;
   bool m_confirmed;
   bool m_responsePending;
   DatePtr m_updateStart;
   DatePtr m_updateFinish;
   CString m_scheduleID;
};