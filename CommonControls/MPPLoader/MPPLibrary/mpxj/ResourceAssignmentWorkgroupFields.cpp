#include "stdafx.h"
#include "ResourceAssignmentWorkgroupFields.h"

/**
* Sets the Message Unique ID.
*
* @param val ID
*/
void Resource2AssignmentWorkgroupFields::setMessageUniqueID(CString val)
{
    m_messageUniqueID = val;
}

/**
* Gets the Message Unique ID.
*
* @return ID
*/
CString Resource2AssignmentWorkgroupFields::getMessageUniqueID()
{
    return (m_messageUniqueID);
}

/**
* Gets confirmed flag.
*
* @return bool value
*/
bool Resource2AssignmentWorkgroupFields::getConfirmed()
{
    return (m_confirmed);
}

/**
* Sets confirmed flag.
*
* @param val bool flag
*/
void Resource2AssignmentWorkgroupFields::setConfirmed(bool val)
{
    m_confirmed = val;
}

/**
* Sets response pending flag.
*
* @param val bool flag
*/
void Resource2AssignmentWorkgroupFields::setResponsePending(bool val)
{
    m_responsePending = val;
}

/**
* Retrieves response pending flag.
*
* @return bool flag
*/
bool Resource2AssignmentWorkgroupFields::getResponsePending()
{
    return (m_responsePending);
}

/**
* Sets the Update Start Field.
*
* @param val date to set
*/
void Resource2AssignmentWorkgroupFields::setUpdateStart(DatePtr val)
{
    m_updateStart = val;
}

/**
* Gets the Update Start Field value.
*
* @return update Start Date
*/
DatePtr Resource2AssignmentWorkgroupFields::getUpdateStart()
{
    return (m_updateStart);
}

/**
* Sets the Update Finish Field.
*
* @param val date to set
*/
void Resource2AssignmentWorkgroupFields::setUpdateFinish(DatePtr val)
{
    m_updateFinish = val;
}

/**
* Gets the Update Finish Field value.
*
* @return update Finish Date
*/
DatePtr Resource2AssignmentWorkgroupFields::getUpdateFinish()
{
    return (m_updateFinish);
}

/**
* Sets the schedule ID.
*
* @param val schedule ID
*/
void Resource2AssignmentWorkgroupFields::setScheduleID(CString val)
{
    m_scheduleID = val;
}

/**
* Retrieves the schedule ID.
*
* @return schedule ID
*/
CString Resource2AssignmentWorkgroupFields::getScheduleID()
{
    return (m_scheduleID);
}