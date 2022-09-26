#pragma once

#include "MpxjEnum.h"
#include "ProjectEntity.h"
#include "FieldContainer.h"
#include "CostRateTable.h"
#include "AvailabilityTable.h"
#include "Rate.h"
#include "ResourceField.h"
#include "Duration.h"
#include "ProjectCalendar.h"

class ProjectFile;
//class ProjectCalendar;
class Resource2Assignment;

/**
 * This class represents a resource used in a project.
 */
class COMMONCONTROLS_API Resource2 : public ComparableT<Resource2>, public ProjectEntity, public FieldContainer,
	public std::enable_shared_from_this<Resource2>
{
public:
   /**
    * Default constructor.
    * 
    * @param file the parent file to which this record belongs.
    */
	Resource2(ProjectFile* file);
	~Resource2();

   /**
    * Sets Name field value.
    * 
    * @param val value
    */
	void setName(CString val);

   /**
    * Gets Resource2 Name field value.
    * 
    * @return value
    */
	CString getName();

   /**
    * Set the resource type. Can be TYPE_MATERIAL, or TYPE_WORK.
    * 
    * @param type resource type
    */
	void setType(Resource2Type type);

   /**
    * Retrieves the resource type. Can return TYPE_MATERIAL, or TYPE_WORK.
    * 
    * @return resource type
    */
	Resource2Type getType();

   /**
    * Set the flag indicating that this is a NULL resource.
    * 
    * @param isNull NULL resource flag
    */
	void setIsNull(bool isNull);

   /**
    * Retrieve a flag indicating if this is a NULL resource.
    * 
    * @return bool flag
    */
	bool getNull();

   /**
    * Sets Initials field value.
    * 
    * @param val value
    */
	void setInitials(CString val);

   /**
    * Gets Initials of name field value.
    * 
    * @return value
    */
	CString getInitials();

   /**
    * Sets phonetic information for the Japanese version of MS Project.
    * 
    * @param phonetics Japanese phonetic information
    */
	void setPhonetics(CString phonetics);

   /**
    * Retrieves phonetic information for the Japanese version of MS Project.
    * 
    * @return Japanese phonetic information
    */
	CString getPhonetics();

   /**
    * Sets the Windows account name for a resource.
    * 
    * @param ntAccount windows account name
    */
	void setNtAccount(CString ntAccount);

   /**
    * Retrieves the Windows account name for a resource.
    * 
    * @return windows account name
    */
	CString getNtAccount();

   /**
    * Set the units label for a material resource.
    * 
    * @param materialLabel material resource units label
    */
	void setMaterialLabel(CString materialLabel);

   /**
    * Retrieves the units label for a material resource.
    * 
    * @return material resource units label
    */
	CString getMaterialLabel();

   /**
    * Sets code field value.
    * 
    * @param val value
    */
	void setCode(CString val);

   /**
    * Gets code field value.
    * 
    * @return value
    */
	CString getCode();

   /**
    * Sets Group2 field value.
    * 
    * @param val value
    */
	void setGroup2(CString val);

   /**
    * Gets Group2 field value.
    * 
    * @return value
    */
	CString getGroup2();

   /**
    * Set the messaging method used to communicate with a project team.
    * 
    * @param workGroup2 messaging method
    */
	void setWorkGroup2(WorkGroup2 workGroup2);

   /**
    * Retrieve the messaging method used to communicate with a project team.
    * 
    * @return messaging method
    */
	WorkGroup2 getWorkGroup2();

   /**
    * Set the resource's email address.
    * 
    * @param emailAddress email address
    */
	void setEmailAddress(CString emailAddress);

   /**
    * Retrieves the resource's email address.
    * 
    * @return email address
    */
	CString getEmailAddress();

   /**
    * Sets the hyperlink text.
    * 
    * @param hyperlink hyperlink text
    */
	void setHyperlink(CString hyperlink);

   /**
    * Retrieves the hyperlink text.
    * 
    * @return hyperlink text
    */
	CString getHyperlink();

   /**
    * Sets the hyperlink address.
    * 
    * @param hyperlinkAddress hyperlink address
    */
	void setHyperlinkAddress(CString hyperlinkAddress);

   /**
    * Retrieves the hyperlink address.
    * 
    * @return hyperlink address
    */
	CString getHyperlinkAddress();

   /**
    * Sets the hyperlink sub-address.
    * 
    * @param hyperlinkSubAddress hyperlink sub-address
    */
	void setHyperlinkSubAddress(CString hyperlinkSubAddress);

   /**
    * Retrieves the hyperlink sub-address.
    * 
    * @return hyperlink sub-address
    */
	CString getHyperlinkSubAddress();

   /**
    * Sets the maximum availability of a resource.
    * 
    * @param maxUnits maximum availability
    */
	void setMaxUnits(NumberPtr maxUnits);

   /**
    * Retrieves the maximum availability of a resource.
    * 
    * @return maximum availability
    */
	NumberPtr getMaxUnits();

   /**
    * Sets peak resource utilisation.
    * 
    * @param peakUnits peak resource utilisation
    */
	void setPeakUnits(NumberPtr peakUnits);

   /**
    * Retrieves the peak resource utilisation.
    * 
    * @return peak resource utilisation
    */
	NumberPtr getPeakUnits();

   /**
    * Set the overallocated flag.
    * 
    * @param overallocated overallocated flag
    */
	void setOverAllocated(bool overallocated);

   /**
    * Retrieves the overallocated flag.
    * 
    * @return overallocated flag
    */
	bool getOverAllocated();

   /**
    * Retrieves the "available from" Date.
    * 
    * @return available from Date
    */
	DatePtr getAvailableFrom();

   /**
    * Set the "available from" Date.
    * 
    * @param DatePtr available from Date
    */
	void setAvailableFrom(DatePtr date);

   /**
    * Retrieves the "available to" Date.
    * 
    * @return available from Date
    */
	DatePtr getAvailableTo();

   /**
    * Set the "available to" Date.
    * 
    * @param DatePtr available to Date
    */
	void setAvailableTo(DatePtr date);

   /**
    * Retrieves the earliest start DatePtr for all assigned tasks.
    * 
    * @return start Date
    */
	DatePtr getStart();

   /**
    * Retrieves the latest finish DatePtr for all assigned tasks.
    * 
    * @return finish Date
    */
	DatePtr getFinish();

   /**
    * Sets the flag indicating if the resource levelling can be applied to this
    * resource.
    * 
    * @param canLevel bool flag
    */
	void setCanLevel(bool canLevel);

   /**
    * Retrieves the flag indicating if the resource levelling can be applied to
    * this resource.
    * 
    * @return bool flag
    */
	bool getCanLevel();

   /**
    * Sets the Accrue at type.The Accrue At field provides choices for how and
    * when resource standard and overtime costs are to be charged, or accrued,
    * to the cost of a task. The options are: Start, End and Prorated (Default)
    * 
    * @param type accrue type
    */
	void setAccrueAt(AccrueType type);

   /**
    * Gets the Accrue at type.The Accrue At field provides choices for how and
    * when resource standard and overtime costs are to be charged, or accrued,
    * to the cost of a task. The options are: Start, End and Proraetd (Default)
    * 
    * @return accrue type
    */
	AccrueType getAccrueAt();

   /**
    * This field is ignored on import into MS Project.
    * 
    * @param val - value to be set
    */
	void setWork(DurationPtr val);

   /**
    * Gets Work field value.
    * 
    * @return value
    */
	DurationPtr getWork();

   /**
    * Retrieve the value of the regular work field. Note that this value is an
    * extension to the MPX specification.
    * 
    * @return Regular work value
    */
	DurationPtr getRegularWork();

   /**
    * Set the value of the regular work field. Note that this value is an
    * extension to the MPX specification.
    * 
    * @param duration Regular work value
    */
	void setRegularWork(DurationPtr duration);

   /**
    * Sets the Actual Work field contains the amount of work that has already
    * been done for all assignments assigned to a resource.
    * 
    * @param val duration value
    */
	void setActualWork(DurationPtr val);

   /**
    * Retrieves the Actual Work field contains the amount of work that has
    * already been done for all assignments assigned to a resource.
    * 
    * @return Actual work value
    */
	DurationPtr getActualWork();

   /**
    * Sets the amount of overtime work.
    * 
    * @param overtimeWork overtime work
    */
	void setOvertimeWork(DurationPtr overtimeWork);

   /**
    * Retrieves the amount of overtime work.
    * 
    * @return overtime work
    */
	DurationPtr getOvertimeWork();

   /**
    * This field is ignored on import into MS Project.
    * 
    * @param val - value to be set
    */
	void setRemainingWork(DurationPtr val);

   /**
    * Gets Remaining Work field value.
    * 
    * @return value
    */
	DurationPtr getRemainingWork();

   /**
    * Retrieve the value of the actual overtime work field.
    * 
    * @return actual overtime work value
    */
	DurationPtr getActualOvertimeWork();

   /**
    * Sets the value of the actual overtime work field.
    * 
    * @param duration actual overtime work value
    */
	void setActualOvertimeWork(DurationPtr duration);

   /**
    * Retrieve the value of the remaining overtime work field.
    * 
    * @return remaining overtime work value
    */
	DurationPtr getRemainingOvertimeWork();

   /**
    * Sets the value of the remaining overtime work field.
    * 
    * @param duration remaining overtime work value
    */
	void setRemainingOvertimeWork(DurationPtr duration);

   /**
    * Sets the value of the percent work complete field.
    * 
    * @param percentWorkComplete percent work complete
    */
	void setPercentWorkComplete(NumberPtr percentWorkComplete);

   /**
    * Retrieves the value of the percent work complete field.
    * 
    * @return percent work complete
    */
	NumberPtr getPercentWorkComplete();

   /**
    * Sets standard rate for this resource.
    * 
    * @param val value
    */
	void setStandardRate(RatePtr val);

   /**
    * Gets Standard RatePtr field value.
    * 
    * @return Rate
    */
	RatePtr getStandardRate();

   /**
    * Sets the format of the standard rate.
    * 
    * @param units standard rate format
    */
	void setStandardRateUnits(TimeUnit units);

   /**
    * Retrieves the format of the standard rate.
    * 
    * @return standard rate format
    */
	TimeUnit getStandardRateUnits();

   /**
    * Sets the cost field value.
    * 
    * @param cost cost field value
    */
	void setCost(NumberPtr cost);

   /**
    * Retrieves the cost field value.
    * 
    * @return cost field value
    */
	NumberPtr getCost();

   /**
    * Sets the overtime rate for this resource.
    * 
    * @param overtimeRate overtime rate value
    */
	void setOvertimeRate(RatePtr overtimeRate);

   /**
    * Retrieves the overtime rate for this resource.
    * 
    * @return overtime rate
    */
	RatePtr getOvertimeRate();

   /**
    * Sets the format of the overtime rate.
    * 
    * @param units overtime rate format
    */
	void setOvertimeRateUnits(TimeUnit units);

   /**
    * Retrieves the format of the overtime rate.
    * 
    * @return overtime rate format
    */
	TimeUnit getOvertimeRateUnits();

   /**
    * Retrieve the value of the overtime cost field.
    * 
    * @return Overtime cost value
    */
	NumberPtr getOvertimeCost();

   /**
    * Set the value of the overtime cost field.
    * 
    * @param currency Overtime cost
    */
	void setOvertimeCost(NumberPtr currency);

   /**
    * Set the cost per use.
    * 
    * @param costPerUse cost per use
    */
	void setCostPerUse(NumberPtr costPerUse);

   /**
    * Retrieve the cost per use.
    * 
    * @return cost per use
    */
	NumberPtr getCostPerUse();

   /**
    * Set the actual cost for the work already performed by this resource.
    * 
    * @param actualCost actual cost
    */
	void setActualCost(NumberPtr actualCost);

   /**
    * Retrieves the actual cost for the work already performed by this resource.
    * 
    * @return actual cost
    */
	NumberPtr getActualCost();

   /**
    * Retrieve actual overtime cost.
    * 
    * @return actual overtime cost
    */
	NumberPtr getActualOvertimeCost();

   /**
    * Sets the actual overtime cost.
    * 
    * @param actualOvertimeCost actual overtime cost
    */
	void setActualOvertimeCost(NumberPtr actualOvertimeCost);

   /**
    * Sets the remaining cost for this resource.
    * 
    * @param remainingCost remaining cost
    */
	void setRemainingCost(NumberPtr remainingCost);

   /**
    * Retrieves the remaining cost for this resource.
    * 
    * @return remaining cost
    */
	NumberPtr getRemainingCost();

   /**
    * Retrieve the remaining overtime cost.
    * 
    * @return remaining overtime cost
    */
	NumberPtr getRemainingOvertimeCost();

   /**
    * Set the remaining overtime cost.
    * 
    * @param remainingOvertimeCost remaining overtime cost
    */
	void setRemainingOvertimeCost(NumberPtr remainingOvertimeCost);

   /**
    * Sets the work variance.
    * 
    * @param workVariance work variance
    */
	void setWorkVariance(DurationPtr workVariance);

   /**
    * Retrieves the work variance.
    * 
    * @return work variance
    */
	DurationPtr getWorkVariance();

   /**
    * Sets the cost variance.
    * 
    * @param costVariance cost variance
    */
	void setCostVariance(NumberPtr costVariance);

   /**
    * Retrieves the cost variance.
    * 
    * @return cost variance
    */
	NumberPtr getCostVariance();

   /**
    * Set the schedule variance.
    * 
    * @param sv schedule variance
    */
	void setSV(NumberPtr sv);

   /**
    * Retrieve the schedule variance.
    * 
    * @return schedule variance
    */
	NumberPtr getSV();

   /**
    * Set the cost variance.
    * 
    * @param cv cost variance
    */
	void setCV(NumberPtr cv);

   /**
    * Retrieve the cost variance.
    * 
    * @return cost variance
    */
	NumberPtr getCV();

   /**
    * Set the actual cost of work performed.
    * 
    * @param acwp actual cost of work performed
    */
	void setACWP(NumberPtr acwp);

   /**
    * Set the actual cost of work performed.
    * 
    * @return actual cost of work performed
    */
	NumberPtr getACWP();

   /**
    * Sets the notes text for this resource.
    * 
    * @param notes notes to be added
    */
	void setNotes(CString notes);

   /**
    * Retrieves the notes text for this resource.
    * 
    * @return notes text
    */
	CString getNotes();

   /**
    * Sets the budgeted cost of work scheduled.
    * 
    * @param bcws budgeted cost of work scheduled
    */
	void setBCWS(NumberPtr bcws);

   /**
    * Retrieves the budgeted cost of work scheduled.
    * 
    * @return budgeted cost of work scheduled
    */
	NumberPtr getBCWS();

   /**
    * Sets the budgeted cost of work performed.
    * 
    * @param bcwp budgeted cost of work performed
    */
	void setBCWP(NumberPtr bcwp);

   /**
    * Retrievesthe budgeted cost of work performed.
    * 
    * @return budgeted cost of work performed
    */
	NumberPtr getBCWP();

   /**
    * Sets the generic flag.
    * 
    * @param isGeneric generic flag
    */
	void setIsGeneric(bool isGeneric);

   /**
    * Retrieves the generic flag.
    * 
    * @return generic flag
    */
	bool getGeneric();

   /**
    * Sets the inactive flag.
    * 
    * @param isInactive inactive flag
    */
	void setIsInactive(bool isInactive);

   /**
    * Retrieves the inactive flag.
    * 
    * @return inactive flag
    */
	bool getInactive();

   /**
    * Sets the active directory GUID2 for this resource.
    * 
    * @param guid active directory GUID2
    */
	void setActveDirectoryGUID(CString guid);

   /**
    * Retrieves the active directory GUID2 for this resource.
    * 
    * @return active directory GUID2
    */
	CString getActiveDirectoryGUID();

   /**
    * Sets the actual overtime work protected duration.
    * 
    * @param duration actual overtime work protected
    */
	void setActualOvertimeWorkProtected(DurationPtr duration);

   /**
    * Retrieves the actual overtime work protected duration.
    * 
    * @return actual overtime work protected
    */
	DurationPtr getActualOvertimeWorkProtected();

   /**
    * Sets the actual work protected duration.
    * 
    * @param duration actual work protected
    */
	void setActualWorkProtected(DurationPtr duration);

   /**
    * Retrieves the actual work protected duration.
    * 
    * @return actual work protected
    */
	DurationPtr getActualWorkProtected();

   /**
    * Sets the booking type.
    * 
    * @param bookingType booking type
    */
	void setBookingType(BookingType bookingType);

   /**
    * Retrieves the booking type.
    * 
    * @return booking type
    */
	BookingType getBookingType();

   /**
    * Sets the creation Date.
    * 
    * @param creationDate creation Date
    */
	void setCreationDate(DatePtr creationDate);

   /**
    * Retrieves the creation Date.
    * 
    * @return creation Date
    */
	DatePtr getCreationDate();

   /**
    * Sets a flag indicating that a resource is an enterprise resource.
    * 
    * @param enterprise bool flag
    */
	void setIsEnterprise(bool enterprise);

   /**
    * Retrieves a flag indicating that a resource is an enterprise resource.
    * 
    * @return bool flag
    */
	bool getEnterprise();

   /**
    * This method retrieves the calendar associated with this resource.
    * 
    * @return ProjectCalendar instance
    */
	ProjectCalendarPtr getResource2Calendar();

   /**
    * This method allows a pre-existing resource calendar to be attached to a
    * resource.
    * 
    * @param calendar resource calendar
    */
	void setResource2Calendar(ProjectCalendarPtr calendar);

   /**
    * This method allows a resource calendar to be added to a resource.
    * 
    * @return Resource2Calendar
    * @throws MPXJException if more than one calendar is added
    */
	ProjectCalendarPtr addResource2Calendar();

   /**
    * Sets the Base CalendarPtr field indicates which calendar is the base calendar
    * for a resource calendar. The list includes the three built-in calendars,
    * as well as any new base calendars you have created in the Change Working
    * Time dialog box.
    * 
    * @param val calendar name
    */
	void setBaseCalendar(CString val);

   /**
    * Sets the baseline cost. This field is ignored on import into MS Project
    * 
    * @param val - value to be set
    */
	void setBaselineCost(NumberPtr val);

   /**
    * Sets the baseline work duration. This field is ignored on import into MS
    * Project.
    * 
    * @param val - value to be set
    */
	void setBaselineWork(DurationPtr val);

   /**
    * Sets ID field value.
    * 
    * @param val value
    */
	void setID(int val);

   /**
    * This field is ignored on import into MS Project.
    * 
    * @param val - value to be set
    */
	void setLinkedFields(bool val);

   /**
    * Set objects.
    * 
    * @param val - value to be set
    */
	void setObjects(int val);

   /**
    * Set a text value.
    * 
    * @param index text index (1-30)
    * @param value text value
    */
	void setText(int index, CString value);

   /**
    * Retrieve a text value.
    * 
    * @param index text index (1-30)
    * @return text value
    */
	CString getText(int index);

   /**
    * Sets Unique ID of this resource.
    * 
    * @param val Unique ID
    */
	void setUniqueID(int val);

   /**
    * Retrieves Base CalendarPtr name associated with this resource. This field
    * indicates which calendar is the base calendar for a resource calendar.
    * 
    * @return Base calendar name
    */
	CString getBaseCalendar();

   /**
    * Retrieves the Baseline Cost value. This value is the total planned cost
    * for a resource for all assigned tasks. Baseline cost is also referred to
    * as budget at completion (BAC).
    * 
    * @return Baseline cost value
    */
	NumberPtr getBaselineCost();

   /**
    * Retrieves the Baseline Work value.
    * 
    * @return Baseline work value
    */
	DurationPtr getBaselineWork();

   /**
    * Gets ID field value.
    * 
    * @return value
    */
	int getID();

   /**
    * Gets Linked Fields field value.
    * 
    * @return value
    */
	bool getLinkedFields();

   /**
    * Gets objects field value.
    * 
    * @return value
    */
	int getObjects();

   /**
    * Gets Unique ID field value.
    * 
    * @return value
    */
	int getUniqueID();

   /**
    * Set a start value.
    * 
    * @param index start index (1-10)
    * @param value start value
    */
	void setStart(int index, DatePtr value);

   /**
    * Retrieve a start value.
    * 
    * @param index start index (1-10)
    * @return start value
    */
	DatePtr getStart(int index);

   /**
    * Set a finish value.
    * 
    * @param index finish index (1-10)
    * @param value finish value
    */
	void setFinish(int index, DatePtr value);

   /**
    * Retrieve a finish value.
    * 
    * @param index finish index (1-10)
    * @return finish value
    */
	DatePtr getFinish(int index);

   /**
    * Set a number value.
    * 
    * @param index number index (1-20)
    * @param value number value
    */
	void setNumber(int index, NumberPtr value);

   /**
    * Retrieve a number value.
    * 
    * @param index number index (1-20)
    * @return number value
    */
	NumberPtr getNumber(int index);

   /**
    * Set a duration value.
    * 
    * @param index duration index (1-10)
    * @param value duration value
    */
	void setDuration(int index, DurationPtr value);

   /**
    * Retrieve a duration value.
    * 
    * @param index duration index (1-10)
    * @return duration value
    */
	DurationPtr getDuration(int index);

   /**
    * Set a DatePtr value.
    * 
    * @param index DatePtr index (1-10)
    * @param value DatePtr value
    */
	void setDate(int index, DatePtr value);

   /**
    * Retrieve a DatePtr value.
    * 
    * @param index DatePtr index (1-10)
    * @return DatePtr value
    */
	DatePtr getDate(int index);

   /**
    * Set a cost value.
    * 
    * @param index cost index (1-10)
    * @param value cost value
    */
	void setCost(int index, NumberPtr value);

   /**
    * Retrieve a cost value.
    * 
    * @param index cost index (1-10)
    * @return cost value
    */
	NumberPtr getCost(int index);

   /**
    * Set a flag value.
    * 
    * @param index flag index (1-20)
    * @param value flag value
    */
	void setFlag(int index, bool value);

   /**
    * Retrieve a flag value.
    * 
    * @param index flag index (1-20)
    * @return flag value
    */
	bool getFlag(int index);

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode1(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode1();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode2(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode2();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode3(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode3();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode4(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode4();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode5(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode5();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode6(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode6();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode7(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode7();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode8(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode8();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode9(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode9();

   /**
    * Sets the value of an outline code field.
    * 
    * @param value outline code value
    */
	void setOutlineCode10(CString value);

   /**
    * Retrieves the value of an outline code field.
    * 
    * @return outline code value
    */
	CString getOutlineCode10();

   /**
    * Removes this resource from the project.
    */
	void remove();

   /**
    * Retrieve the value of a field using its alias.
    * 
    * @param alias field alias
    * @return field value
    */
	ObjectPtr getFieldByAlias(CString alias);

   /**
    * Set the value of a field using its alias.
    * 
    * @param alias field alias
    * @param value field value
    */
	void setFieldByAlias(CString alias, ObjectPtr value);

   /**
    * This method is used internally within MPXJ to track tasks which are
    * assigned to a particular resource.
    * 
    * @param assignment resource assignment instance
    */
	void addResource2Assignment(std::shared_ptr<Resource2Assignment> assignment);

   /**
    * Internal method used as part of the process of removing a resource
    * assignment.
    * 
    * @param assignment resource assignment to be removed
    */
	void removeResource2Assignment(std::shared_ptr<Resource2Assignment> assignment);

   /**
    * Retrieve a list of tasks assigned to this resource. Note that if this
    * project data has been read from an MPX file which declared some or all of
    * the resources assignments before the tasks and resources to which the
    * assignments relate, then these assignments may not appear in this list.
    * Caveat emptor!
    * 
    * @return list of tasks assigned to this resource
    */
	List<std::shared_ptr<Resource2Assignment>>* getTaskAssignments();

   /**
    * Where a resource in an MPP file represents a resource from a subproject,
    * this value will be non-zero. The value itself is the unique ID value shown
    * in the parent project. To retrieve the value of the resource unique ID in
    * the child project, remove the top two bytes:
    * 
    * resourceID = (subprojectUniqueID & 0xFFFF)
    * 
    * @return sub project unique resource ID
    */
	int getSubprojectResource2UniqueID();

   /**
    * Sets the sub project unique resource ID.
    * 
    * @param subprojectUniqueResource2ID subproject unique resource ID
    */
	void setSubprojectResource2UniqueID(int subprojectUniqueResource2ID);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	NumberPtr getEnterpriseCost(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseCost(int index, NumberPtr value);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	DatePtr getEnterpriseDate(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseDate(int index, DatePtr value);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	DurationPtr getEnterpriseDuration(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseDuration(int index, DurationPtr value);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	bool getEnterpriseFlag(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseFlag(int index, bool value);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	NumberPtr getEnterpriseNumber(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseNumber(int index, NumberPtr value);

   /**
    * Retrieve an enterprise field value.
    * 
    * @param index field index
    * @return field value
    */
	CString getEnterpriseText(int index);

   /**
    * Set an enterprise field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseText(int index, CString value);

   /**
    * Retrieve an enterprise custom field value.
    * 
    * @param index field index
    * @return field value
    */
	CString getEnterpriseCustomField(int index);

   /**
    * Set an enterprise custom field value.
    * 
    * @param index field index
    * @param value field value
    */
	void setEnterpriseCustomField(int index, CString value);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineCost(int baselineNumber, NumberPtr value);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineWork(int baselineNumber, DurationPtr value);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	NumberPtr getBaselineCost(int baselineNumber);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	DurationPtr getBaselineWork(int baselineNumber);

   /**
    * Retrieve the budget flag.
    * 
    * @return budget flag
    */
	bool getBudget();

   /**
    * Set the budget flag.
    * 
    * @param budget budget flag
    */
	void setBudget(bool budget);

   /**
    * Retrieves the resource GUID2.
    * 
    * @return resource GUID2.
    */
	LPCGUID getGUID();

   /**
    * Sets the resource GUID2.
    * 
    * @param value resource GUID2
    */
	void setGUID(LPCGUID value);

   /**
    * Associates a complete cost rate table with the
    * current resource. Note that the index corresponds with the
    * letter label used by MS Project to identify each table.
    * For example 0=Table A, 1=Table B, 2=Table C, and so on.
    * 
    * @param index table index
    * @param crt table instance
    */
	void setCostRateTable(int index, CostRateTablePtr crt);

   /**
    * Retrieves a cost rate table associated with a resource.
    * Note that the index corresponds with the
    * letter label used by MS Project to identify each table.
    * For example 0=Table A, 1=Table B, 2=Table C, and so on.
    * 
    * @param index table index
    * @return table instance
    */
	CostRateTablePtr getCostRateTable(int index);

   /**
    * Retrieve the availability table for this resource.
    * 
    * @return availability table
    */
	AvailabilityTablePtr getAvailability();

   /**
    * {@inheritDoc}
    */
	virtual ObjectPtr getCachedValue(FieldType* field);
	ObjectPtr getCachedValue(Resource2Field field);

   /**
    * {@inheritDoc}
    */
	virtual ObjectPtr getCurrentValue(FieldType* field);

   /**
    * {@inheritDoc}
    */
	virtual void set(FieldType* field, ObjectPtr value);
	void set(Resource2Field field, ObjectPtr value);

   /**
    * {@inheritDoc}
    */
	virtual void addFieldListener(FieldListener* listener);

   /**
    * {@inheritDoc}
    */
	virtual void removeFieldListener(FieldListener* listener);

   /**
    * Disable events firing when fields are upDated.
    */
	void disableEvents();

   /**
    * Enable events firing when fields are upDated. This is the default state.
    */
	void enableEvents();

	/**
	* {@inheritDoc}
	*/
	int compareTo(Resource2* rhs);

private:
	/**
	* This method is used to set the value of a field in the resource.
	*
	* @param field field to be set
	* @param value new value for field.
	*/
	void set(FieldType* field, bool value);
	/**
	* @return Resource2Field instance
	*/
	Resource2Field selectField(const Resource2Field fields[], int index);


	/**
	* Maps a field index to a Resource2Field instance.
	*
	* @param fields array of fields used as the basis for the mapping.
	* @param index required field index

	/**
	* Handle the change in a field value. Reset any cached calculated
	* values affected by this change, pass on the event to any external
	* listeners.
	*
	* @param field field changed
	* @param oldValue old field value
	* @param newValue new field value
	*/
	void fireFieldChangeEvent(Resource2Field field, ObjectPtr oldValue, ObjectPtr newValue);

 //  /**
 //   * This method implements the only method in the Comparable interface. This
 //   * allows Resource2s to be compared and sorted based on their ID value. Note
 //   * that if the MPX/MPP file has been generated by MSP, the ID value will
 //   * always be in the correct sequence. The Unique ID value will not
 //   * necessarily be in the correct sequence as task insertions and deletions
 //   * will change the order.
 //   * 
 //   * @param o object to compare this instance with
 //   * @return result of comparison
 //   */
	//virtual int compareTo(Resource2 o);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual bool equals(Object o);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual int hashCode();

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
   /**
    * Array of field values.
    */
	ObjectPtrArray m_array;// [Resource2Field::RF_FINISH];// = new Object[Resource2Field.MAX_VALUE];

   /**
    * Resource2 calendar for this resource.
    */
   ProjectCalendarPtr m_calendar;

   /**
    * List of all assignments for this resource.
    */
   List<std::shared_ptr<Resource2Assignment>> m_assignments;// = new List<Resource2Assignment>();

   bool m_eventsEnabled = true;
   bool m_null;
   bool m_generic;
   bool m_inactive;
   CString m_activeDirectoryGUID;
   DurationPtr m_actualOvertimeWorkProtected;
   DurationPtr m_actualWorkProtected;
   BookingType m_bookingType;
   bool m_enterprise;

   CostRateTablePtr m_costRateTables[5];// = new CostRateTable[5];
   AvailabilityTablePtr m_availability;// = new AvailabilityTable();
   List<FieldListener*> m_listeners;
};

typedef std::shared_ptr<Resource2> Resource2Ptr;

/*
NEW FIELDS - to be implemented in 5.0
{Resource2Field.Baseline Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(757)},
{Resource2Field.Baseline Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(756)},
{Resource2Field.Baseline1 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(761)},
{Resource2Field.Baseline1 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(760)},
{Resource2Field.Baseline10 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(797)},
{Resource2Field.Baseline10 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(796)},
{Resource2Field.Baseline2 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(765)},
{Resource2Field.Baseline2 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(764)},
{Resource2Field.Baseline3 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(769)},
{Resource2Field.Baseline3 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(768)},
{Resource2Field.Baseline4 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(773)},
{Resource2Field.Baseline4 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(772)},
{Resource2Field.Baseline5 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(777)},
{Resource2Field.Baseline5 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(776)},
{Resource2Field.Baseline6 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(781)},
{Resource2Field.Baseline6 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(780)},
{Resource2Field.Baseline7 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(785)},
{Resource2Field.Baseline7 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(784)},
{Resource2Field.Baseline8 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(789)},
{Resource2Field.Baseline8 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(788)},
{Resource2Field.Baseline9 Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(793)},
{Resource2Field.Baseline9 Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(792)},
{Resource2Field.Booking Type, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(699)},
{Resource2Field.Budget Cost, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(754)},
{Resource2Field.Budget Work, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(753)},
{Resource2Field.CalendarPtr GUID2, FieldLocation.FIXED_DATA, int.valueOf(24), int.valueOf(729)},
{Resource2Field.Cost Center, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(801)},
{Resource2Field.Enterprise Unique ID, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(443)},
{Resource2Field.Phonetics, FieldLocation.VAR_DATA, int.valueOf(65535), int.valueOf(252)},
{Resource2Field.Workgroup, FieldLocation.FIXED_DATA, int.valueOf(14), int.valueOf(272)},

   INDEX(DataType.INTEGER),
   HYPERLINK_SCREEN_TIP(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE1(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE2(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE3(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE4(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE5(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE6(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE7(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE8(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE9(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE10(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE11(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE12(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE13(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE14(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE15(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE16(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE17(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE18(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE19(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE20(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE21(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE22(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE23(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE24(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE25(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE26(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE27(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE28(DataType.STRING),
   ENTERPRISE_OUTLINE_CODE29(DataType.STRING),
   ENTERPRISE_RBS(DataType.STRING),
   ENTERPRISE_NAME_USED(DataType.STRING),
   ENTERPRISE_IS_CHECKED_OUT(DataType.BOOLEAN2),
   ENTERPRISE_CHECKED_OUT_BY(DataType.STRING),
   ENTERPRISE_LAST_MODIFIED_Date(DataType.Date),
   ENTERPRISE_MULTI_VALUE20(DataType.STRING),
   ENTERPRISE_MULTI_VALUE21(DataType.STRING),
   ENTERPRISE_MULTI_VALUE22(DataType.STRING),
   ENTERPRISE_MULTI_VALUE23(DataType.STRING),
   ENTERPRISE_MULTI_VALUE24(DataType.STRING),
   ENTERPRISE_MULTI_VALUE25(DataType.STRING),
   ENTERPRISE_MULTI_VALUE26(DataType.STRING),
   ENTERPRISE_MULTI_VALUE27(DataType.STRING),
   ENTERPRISE_MULTI_VALUE28(DataType.STRING),
   ENTERPRISE_MULTI_VALUE29(DataType.STRING),
   ACTUAL_WORK_PROTECTED(DataType.RT_WORK),
   ACTUAL_OVERTIME_WORK_PROTECTED(DataType.RT_WORK),

Actual Overtime Work Protected 65535 721
Actual Work Protected 65535 720
Availability Data 65535 276
Baseline Budget Cost 65535 757
Baseline Budget Work 65535 756
Baseline1 Budget Cost 65535 761
Baseline1 Budget Work 65535 760
Baseline10 Budget Cost 65535 797
Baseline10 Budget Work 65535 796
Baseline2 Budget Cost 65535 765
Baseline2 Budget Work 65535 764
Baseline3 Budget Cost 65535 769
Baseline3 Budget Work 65535 768
Baseline4 Budget Cost 65535 773
Baseline4 Budget Work 65535 772
Baseline5 Budget Cost 65535 777
Baseline5 Budget Work 65535 776
Baseline6 Budget Cost 65535 781
Baseline6 Budget Work 65535 780
Baseline7 Budget Cost 65535 785
Baseline7 Budget Work 65535 784
Baseline8 Budget Cost 65535 789
Baseline8 Budget Work 65535 788
Baseline9 Budget Cost 65535 793
Baseline9 Budget Work 65535 792
Booking Type 65535 699
Budget Cost 65535 754
Budget Work 65535 753
CalendarPtr GUID2 24 729
Cost Center 65535 801
Created 65535 726
Enterprise Unique ID 65535 443
Phonetics 65535 252
Remaining Overtime Work 116 40
Work 52 13
Workgroup 14 272
*/