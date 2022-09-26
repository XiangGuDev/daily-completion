#include "stdafx.h"
#include "Resource.h"
#include "ProjectFile.h"
#include "common\DateHelper.h"
#include "common\ResourceFieldLists.h"
#include "listener\FieldListener.h"

/**
* Default constructor.
* 
* @param file the parent file to which this record belongs.
*/
Resource2::Resource2(ProjectFile* file)
:ProjectEntity(file), m_array(Resource2Field::RF_FINISH + 1), 
m_availability(new AvailabilityTable()), m_bookingType(NULL_BOOKINGTYPE)
{
    setType(Resource2Type::RT_WORK);

    if (file->getAutoResource2UniqueID() == true)
    {
        setUniqueID(file->getResource2UniqueID());
    }

    if (file->getAutoResource2ID() == true)
    {
        setID(file->getResource2ID());
    }
}

Resource2::~Resource2()
{
	m_calendar.reset();
	m_assignments.RemoveAll();
	m_array.clear();

	for (int i = 0; i < 5; i++)
		m_costRateTables[i].reset();
}

/**
* Sets Name field value.
* 
* @param val value
*/
void Resource2::setName(CString val)
{
	set(Resource2Field::RF_NAME, VariantPtr(new Variant(val)));
}

/**
* Gets Resource2 Name field value.
* 
* @return value
*/
CString Resource2::getName()
{
	return NumberHelper::getString (getCachedValue(Resource2Field::RF_NAME));
}

/**
* Set the resource type. Can be TYPE_MATERIAL, or TYPE_WORK.
* 
* @param type resource type
*/
void Resource2::setType(Resource2Type type)
{
	set(Resource2Field::RF_TYPE, VariantPtr(new Variant((int)type)));
}

/**
* Retrieves the resource type. Can return TYPE_MATERIAL, or TYPE_WORK.
* 
* @return resource type
*/
Resource2Type Resource2::getType()
{
	return (Resource2Type) NumberHelper::getInt (getCachedValue(Resource2Field::RF_TYPE));
}

/**
* Set the flag indicating that this is a NULL resource.
* 
* @param isNull NULL resource flag
*/
void Resource2::setIsNull(bool isNull)
{
    m_null = isNull;
}

/**
* Retrieve a flag indicating if this is a NULL resource.
* 
* @return bool flag
*/
bool Resource2::getNull()
{
    return (m_null);
}

/**
* Sets Initials field value.
* 
* @param val value
*/
void Resource2::setInitials(CString val)
{
	set(Resource2Field::RF_INITIALS, VariantPtr(new Variant(val)));
}

/**
* Gets Initials of name field value.
* 
* @return value
*/
CString Resource2::getInitials()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_INITIALS));
}

/**
* Sets phonetic information for the Japanese version of MS Project.
* 
* @param phonetics Japanese phonetic information
*/
void Resource2::setPhonetics(CString phonetics)
{
	set(Resource2Field::RF_PHONETICS, VariantPtr(new Variant(phonetics)));
}

/**
* Retrieves phonetic information for the Japanese version of MS Project.
* 
* @return Japanese phonetic information
*/
CString Resource2::getPhonetics()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_PHONETICS));
}

/**
* Sets the Windows account name for a resource.
* 
* @param ntAccount windows account name
*/
void Resource2::setNtAccount(CString ntAccount)
{
	set(Resource2Field::RF_WINDOWS_USER_ACCOUNT, VariantPtr(new Variant(ntAccount)));
}

/**
* Retrieves the Windows account name for a resource.
* 
* @return windows account name
*/
CString Resource2::getNtAccount()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_WINDOWS_USER_ACCOUNT));
}

/**
* Set the units label for a material resource.
* 
* @param materialLabel material resource units label
*/
void Resource2::setMaterialLabel(CString materialLabel)
{
	set(Resource2Field::RF_MATERIAL_LABEL, VariantPtr(new Variant(materialLabel)));
}

/**
* Retrieves the units label for a material resource.
* 
* @return material resource units label
*/
CString Resource2::getMaterialLabel()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_MATERIAL_LABEL));
}

/**
* Sets code field value.
* 
* @param val value
*/
void Resource2::setCode(CString val)
{
	set(Resource2Field::RF_CODE, VariantPtr(new Variant(val)));
}

/**
* Gets code field value.
* 
* @return value
*/
CString Resource2::getCode()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_CODE));
}

/**
* Sets Group2 field value.
* 
* @param val value
*/
void Resource2::setGroup2(CString val)
{
	set(Resource2Field::RF_GROUP, VariantPtr(new Variant(val)));
}

/**
* Gets Group2 field value.
* 
* @return value
*/
CString Resource2::getGroup2()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_GROUP));
}

/**
* Set the messaging method used to communicate with a project team.
* 
* @param workGroup2 messaging method
*/
void Resource2::setWorkGroup2(WorkGroup2 workGroup2)
{
	set(Resource2Field::RF_WORKGROUP, VariantPtr(new Variant((int)workGroup2)));
}

/**
* Retrieve the messaging method used to communicate with a project team.
* 
* @return messaging method
*/
WorkGroup2 Resource2::getWorkGroup2()
{
    return (WorkGroup2) NumberHelper::getInt(getCachedValue(Resource2Field::RF_WORKGROUP));
}

/**
* Set the resource's email address.
* 
* @param emailAddress email address
*/
void Resource2::setEmailAddress(CString emailAddress)
{
	set(Resource2Field::RF_EMAIL_ADDRESS, VariantPtr(new Variant(emailAddress)));
}

/**
* Retrieves the resource's email address.
* 
* @return email address
*/
CString Resource2::getEmailAddress()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_EMAIL_ADDRESS));
}

/**
* Sets the hyperlink text.
* 
* @param hyperlink hyperlink text
*/
void Resource2::setHyperlink(CString hyperlink)
{
	set(Resource2Field::RF_HYPERLINK, VariantPtr(new Variant(hyperlink)));
}

/**
* Retrieves the hyperlink text.
* 
* @return hyperlink text
*/
CString Resource2::getHyperlink()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_HYPERLINK));
}

/**
* Sets the hyperlink address.
* 
* @param hyperlinkAddress hyperlink address
*/
void Resource2::setHyperlinkAddress(CString hyperlinkAddress)
{
	set(Resource2Field::RF_HYPERLINK_ADDRESS, VariantPtr(new Variant(hyperlinkAddress)));
}

/**
* Retrieves the hyperlink address.
* 
* @return hyperlink address
*/
CString Resource2::getHyperlinkAddress()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_HYPERLINK_ADDRESS));
}

/**
* Sets the hyperlink sub-address.
* 
* @param hyperlinkSubAddress hyperlink sub-address
*/
void Resource2::setHyperlinkSubAddress(CString hyperlinkSubAddress)
{
	set(Resource2Field::RF_HYPERLINK_SUBADDRESS, VariantPtr(new Variant(hyperlinkSubAddress)));
}

/**
* Retrieves the hyperlink sub-address.
* 
* @return hyperlink sub-address
*/
CString Resource2::getHyperlinkSubAddress()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_HYPERLINK_SUBADDRESS));
}

/**
* Sets the maximum availability of a resource.
* 
* @param maxUnits maximum availability
*/
void Resource2::setMaxUnits(NumberPtr maxUnits)
{
    set(Resource2Field::RF_MAX_UNITS, maxUnits);
}

/**
* Retrieves the maximum availability of a resource.
* 
* @return maximum availability
*/
NumberPtr Resource2::getMaxUnits()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_MAX_UNITS));
}

/**
* Sets peak resource utilisation.
* 
* @param peakUnits peak resource utilisation
*/
void Resource2::setPeakUnits(NumberPtr peakUnits)
{
    set(Resource2Field::RF_PEAK, peakUnits);
}

/**
* Retrieves the peak resource utilisation.
* 
* @return peak resource utilisation
*/
NumberPtr Resource2::getPeakUnits()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_PEAK));
}

/**
* Set the overallocated flag.
* 
* @param overallocated overallocated flag
*/
void Resource2::setOverAllocated(bool overallocated)
{
	set(Resource2Field::RF_OVERALLOCATED, VariantPtr(new Variant(overallocated)));
}

/**
* Retrieves the overallocated flag.
* 
* @return overallocated flag
*/
bool Resource2::getOverAllocated()
{
	VariantPtr overallocated = std::dynamic_pointer_cast<Variant> (
		getCachedValue(Resource2Field::RF_OVERALLOCATED));
    if (overallocated == NULL)
    {
        NumberPtr peakUnits = getPeakUnits();
        NumberPtr maxUnits = getMaxUnits();
		overallocated = VariantPtr(new Variant(
			NumberHelper::getDouble(peakUnits) > NumberHelper::getDouble(maxUnits)));
        set(Resource2Field::RF_OVERALLOCATED, overallocated);
    }
    return NumberHelper::getBool(overallocated);
}

/**
* Retrieves the "available from" Date.
* 
* @return available from Date
*/
DatePtr Resource2::getAvailableFrom()
{
    return std::dynamic_pointer_cast<Date>( getCachedValue(Resource2Field::RF_AVAILABLE_FROM));
}

/**
* Set the "available from" Date.
* 
* @param DatePtr available from Date
*/
void Resource2::setAvailableFrom(DatePtr date)
{
    set(Resource2Field::RF_AVAILABLE_FROM, date);
}

/**
* Retrieves the "available to" Date.
* 
* @return available from Date
*/
DatePtr Resource2::getAvailableTo()
{
    return std::dynamic_pointer_cast<Date>( getCachedValue(Resource2Field::RF_AVAILABLE_TO));
}

/**
* Set the "available to" Date.
* 
* @param DatePtr available to Date
*/
void Resource2::setAvailableTo(DatePtr date)
{
    set(Resource2Field::RF_AVAILABLE_TO, date);
}

/**
* Retrieves the earliest start DatePtr for all assigned tasks.
* 
* @return start Date
*/
DatePtr Resource2::getStart()
{
    DatePtr result = NULL;
	for (size_t i = 0; i < m_assignments.GetCount(); i ++)
    {
		Resource2AssignmentPtr assignment = m_assignments[i];
        if (result == NULL || DateHelper::compare(result, assignment->getStart()) > 0)
        {
			result = assignment->getStart();
        }
    }
    return (result);
}

/**
* Retrieves the latest finish DatePtr for all assigned tasks.
* 
* @return finish Date
*/
DatePtr Resource2::getFinish()
{
    DatePtr result = NULL;
	for (size_t i = 0; i < m_assignments.GetCount(); i++)
	{
		Resource2AssignmentPtr assignment = m_assignments[i];
        if (result == NULL || DateHelper::compare(result, assignment->getFinish()) < 0)
        {
			result = assignment->getFinish();
        }
    }
    return (result);
}

/**
* Sets the flag indicating if the resource levelling can be applied to this
* resource.
* 
* @param canLevel bool flag
*/
void Resource2::setCanLevel(bool canLevel)
{
	set(Resource2Field::RF_CAN_LEVEL, VariantPtr(new Variant(canLevel)));
}

/**
* Retrieves the flag indicating if the resource levelling can be applied to
* this resource.
* 
* @return bool flag
*/
bool Resource2::getCanLevel()
{
    return NumberHelper::getBool(getCachedValue(Resource2Field::RF_CAN_LEVEL));
}

/**
* Sets the Accrue at type.The Accrue At field provides choices for how and
* when resource standard and overtime costs are to be charged, or accrued,
* to the cost of a task. The options are: Start, End and Prorated (Default)
* 
* @param type accrue type
*/
void Resource2::setAccrueAt(AccrueType type)
{
	set(Resource2Field::RF_ACCRUE_AT, VariantPtr(new Variant((int)type)));
}

/**
* Gets the Accrue at type.The Accrue At field provides choices for how and
* when resource standard and overtime costs are to be charged, or accrued,
* to the cost of a task. The options are: Start, End and Proraetd (Default)
* 
* @return accrue type
*/
AccrueType Resource2::getAccrueAt()
{
    return (AccrueType) NumberHelper::getInt(getCachedValue(Resource2Field::RF_ACCRUE_AT));
}

/**
* This field is ignored on import into MS Project.
* 
* @param val - value to be set
*/
void Resource2::setWork(DurationPtr val)
{
    set(Resource2Field::RF_WORK, val);
}

/**
* Gets Work field value.
* 
* @return value
*/
DurationPtr Resource2::getWork()
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_WORK));
}

/**
* Retrieve the value of the regular work field. Note that this value is an
* extension to the MPX specification.
* 
* @return Regular work value
*/
DurationPtr Resource2::getRegularWork()
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_REGULAR_WORK));
}

/**
* Set the value of the regular work field. Note that this value is an
* extension to the MPX specification.
* 
* @param duration Regular work value
*/
void Resource2::setRegularWork(DurationPtr duration)
{
    set(Resource2Field::RF_REGULAR_WORK, duration);
}

/**
* Sets the Actual Work field contains the amount of work that has already
* been done for all assignments assigned to a resource.
* 
* @param val duration value
*/
void Resource2::setActualWork(DurationPtr val)
{
    set(Resource2Field::RF_ACTUAL_WORK, val);
}

/**
* Retrieves the Actual Work field contains the amount of work that has
* already been done for all assignments assigned to a resource.
* 
* @return Actual work value
*/
DurationPtr Resource2::getActualWork()
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_ACTUAL_WORK));
}

/**
* Sets the amount of overtime work->
* 
* @param overtimeWork overtime work
*/
void Resource2::setOvertimeWork(DurationPtr overtimeWork)
{
    set(Resource2Field::RF_OVERTIME_WORK, overtimeWork);
}

/**
* Retrieves the amount of overtime work->
* 
* @return overtime work
*/
DurationPtr Resource2::getOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_OVERTIME_WORK));
}

/**
* This field is ignored on import into MS Project.
* 
* @param val - value to be set
*/
void Resource2::setRemainingWork(DurationPtr val)
{
    set(Resource2Field::RF_REMAINING_WORK, val);
}

/**
* Gets Remaining Work field value.
* 
* @return value
*/
DurationPtr Resource2::getRemainingWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_REMAINING_WORK));
}

/**
* Retrieve the value of the actual overtime work field.
* 
* @return actual overtime work value
*/
DurationPtr Resource2::getActualOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_ACTUAL_OVERTIME_WORK));
}

/**
* Sets the value of the actual overtime work field.
* 
* @param duration actual overtime work value
*/
void Resource2::setActualOvertimeWork(DurationPtr duration)
{
    set(Resource2Field::RF_ACTUAL_OVERTIME_WORK, duration);
}

/**
* Retrieve the value of the remaining overtime work field.
* 
* @return remaining overtime work value
*/
DurationPtr Resource2::getRemainingOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_REMAINING_OVERTIME_WORK));
}

/**
* Sets the value of the remaining overtime work field.
* 
* @param duration remaining overtime work value
*/
void Resource2::setRemainingOvertimeWork(DurationPtr duration)
{
    set(Resource2Field::RF_REMAINING_OVERTIME_WORK, duration);
}

/**
* Sets the value of the percent work complete field.
* 
* @param percentWorkComplete percent work complete
*/
void Resource2::setPercentWorkComplete(NumberPtr percentWorkComplete)
{
    set(Resource2Field::RF_PERCENT_WORK_COMPLETE, percentWorkComplete);
}

/**
* Retrieves the value of the percent work complete field.
* 
* @return percent work complete
*/
NumberPtr Resource2::getPercentWorkComplete()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_PERCENT_WORK_COMPLETE));
}

/**
* Sets standard rate for this resource.
* 
* @param val value
*/
void Resource2::setStandardRate(RatePtr val)
{
	set(Resource2Field::RF_STANDARD_RATE, val);
}

/**
* Gets Standard RatePtr field value.
* 
* @return Rate
*/
RatePtr Resource2::getStandardRate()
{
	return std::dynamic_pointer_cast<Rate>(getCachedValue(Resource2Field::RF_STANDARD_RATE));
}

/**
* Sets the format of the standard rate.
* 
* @param units standard rate format
*/
void Resource2::setStandardRateUnits(TimeUnit units)
{
	set(Resource2Field::RF_STANDARD_RATE_UNITS, VariantPtr(new Variant((int)units)));
}

/**
* Retrieves the format of the standard rate.
* 
* @return standard rate format
*/
TimeUnit Resource2::getStandardRateUnits()
{
    return (TimeUnit) NumberHelper::getInt(getCachedValue(Resource2Field::RF_STANDARD_RATE_UNITS));
}

/**
* Sets the cost field value.
* 
* @param cost cost field value
*/
void Resource2::setCost(NumberPtr cost)
{
    set(Resource2Field::RF_COST, cost);
}

/**
* Retrieves the cost field value.
* 
* @return cost field value
*/
NumberPtr Resource2::getCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_COST));
}

/**
* Sets the overtime rate for this resource.
* 
* @param overtimeRate overtime rate value
*/
void Resource2::setOvertimeRate(RatePtr overtimeRate)
{
    set(Resource2Field::RF_OVERTIME_RATE, overtimeRate);
}

/**
* Retrieves the overtime rate for this resource.
* 
* @return overtime rate
*/
RatePtr Resource2::getOvertimeRate()
{
    return std::dynamic_pointer_cast<Rate>(getCachedValue(Resource2Field::RF_OVERTIME_RATE));
}

/**
* Sets the format of the overtime rate.
* 
* @param units overtime rate format
*/
void Resource2::setOvertimeRateUnits(TimeUnit units)
{
	set(Resource2Field::RF_OVERTIME_RATE_UNITS, VariantPtr(new Variant((int)units)));
}

/**
* Retrieves the format of the overtime rate.
* 
* @return overtime rate format
*/
TimeUnit Resource2::getOvertimeRateUnits()
{
    return (TimeUnit) NumberHelper::getInt(getCachedValue(Resource2Field::RF_OVERTIME_RATE_UNITS));
}

/**
* Retrieve the value of the overtime cost field.
* 
* @return Overtime cost value
*/
NumberPtr Resource2::getOvertimeCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_OVERTIME_COST));
}

/**
* Set the value of the overtime cost field.
* 
* @param currency Overtime cost
*/
void Resource2::setOvertimeCost(NumberPtr currency)
{
    set(Resource2Field::RF_OVERTIME_COST, currency);
}

/**
* Set the cost per use.
* 
* @param costPerUse cost per use
*/
void Resource2::setCostPerUse(NumberPtr costPerUse)
{
    set(Resource2Field::RF_COST_PER_USE, costPerUse);
}

/**
* Retrieve the cost per use.
* 
* @return cost per use
*/
NumberPtr Resource2::getCostPerUse()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_COST_PER_USE));
}

/**
* Set the actual cost for the work already performed by this resource.
* 
* @param actualCost actual cost
*/
void Resource2::setActualCost(NumberPtr actualCost)
{
    set(Resource2Field::RF_ACTUAL_COST, actualCost);
}

/**
* Retrieves the actual cost for the work already performed by this resource.
* 
* @return actual cost
*/
NumberPtr Resource2::getActualCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_ACTUAL_COST));
}

/**
* Retrieve actual overtime cost.
* 
* @return actual overtime cost
*/
NumberPtr Resource2::getActualOvertimeCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_ACTUAL_OVERTIME_COST));
}

/**
* Sets the actual overtime cost.
* 
* @param actualOvertimeCost actual overtime cost
*/
void Resource2::setActualOvertimeCost(NumberPtr actualOvertimeCost)
{
    set(Resource2Field::RF_ACTUAL_OVERTIME_COST, actualOvertimeCost);
}

/**
* Sets the remaining cost for this resource.
* 
* @param remainingCost remaining cost
*/
void Resource2::setRemainingCost(NumberPtr remainingCost)
{
    set(Resource2Field::RF_REMAINING_COST, remainingCost);
}

/**
* Retrieves the remaining cost for this resource.
* 
* @return remaining cost
*/
NumberPtr Resource2::getRemainingCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_REMAINING_COST));
}

/**
* Retrieve the remaining overtime cost.
* 
* @return remaining overtime cost
*/
NumberPtr Resource2::getRemainingOvertimeCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_REMAINING_OVERTIME_COST));
}

/**
* Set the remaining overtime cost.
* 
* @param remainingOvertimeCost remaining overtime cost
*/
void Resource2::setRemainingOvertimeCost(NumberPtr remainingOvertimeCost)
{
    set(Resource2Field::RF_REMAINING_OVERTIME_COST, remainingOvertimeCost);
}

/**
* Sets the work variance.
* 
* @param workVariance work variance
*/
void Resource2::setWorkVariance(DurationPtr workVariance)
{
    set(Resource2Field::RF_WORK_VARIANCE, workVariance);
}

/**
* Retrieves the work variance.
* 
* @return work variance
*/
DurationPtr Resource2::getWorkVariance()
{
	DurationPtr variance = std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_WORK_VARIANCE));
    if (variance == NULL)
    {
        DurationPtr work = getWork();
        DurationPtr baselineWork = getBaselineWork();
        if (work && baselineWork)
        {
			variance = Duration2::getInstance(work->getDuration() - 
				baselineWork->convertUnits(work->getUnits(), getParentFile()->getProjectHeader())->getDuration(), work->getUnits());
			set(Resource2Field::RF_WORK_VARIANCE, variance);
        }
    }
    return (variance);
}

/**
* Sets the cost variance.
* 
* @param costVariance cost variance
*/
void Resource2::setCostVariance(NumberPtr costVariance)
{
    set(Resource2Field::RF_COST_VARIANCE, costVariance);
}

/**
* Retrieves the cost variance.
* 
* @return cost variance
*/
NumberPtr Resource2::getCostVariance()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_COST_VARIANCE));
    if (variance == NULL)
    {
        NumberPtr cost = getCost();
        NumberPtr baselineCost = getBaselineCost();
        if (cost != NULL && baselineCost != NULL)
        {
			variance = VariantPtr(new Variant(cost->doubleValue() - baselineCost->doubleValue()));
			set(Resource2Field::RF_COST_VARIANCE, variance);
        }
    }
    return (variance);
}

/**
* Set the schedule variance.
* 
* @param sv schedule variance
*/
void Resource2::setSV(NumberPtr sv)
{
    set(Resource2Field::RF_SV, sv);
}

/**
* Retrieve the schedule variance.
* 
* @return schedule variance
*/
NumberPtr Resource2::getSV()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_SV));
    if (variance == NULL)
    {
        NumberPtr bcwp = getBCWP();
        NumberPtr bcws = getBCWS();
        if (bcwp != NULL && bcws != NULL)
        {
			variance = VariantPtr(new Variant(bcwp->doubleValue() - bcws->doubleValue()));
			set(Resource2Field::RF_SV, variance);
        }
    }
    return (variance);
}

/**
* Set the cost variance.
* 
* @param cv cost variance
*/
void Resource2::setCV(NumberPtr cv)
{
    set(Resource2Field::RF_CV, cv);
}

/**
* Retrieve the cost variance.
* 
* @return cost variance
*/
NumberPtr Resource2::getCV()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_CV));
    if (variance == NULL)
    {
		variance = VariantPtr(new Variant(
			NumberHelper::getDouble(getBCWP()) - NumberHelper::getDouble(getACWP())));
        set(Resource2Field::RF_CV, variance);
    }
    return (variance);
}

/**
* Set the actual cost of work performed.
* 
* @param acwp actual cost of work performed
*/
void Resource2::setACWP(NumberPtr acwp)
{
    set(Resource2Field::RF_ACWP, acwp);
}

/**
* Set the actual cost of work performed.
* 
* @return actual cost of work performed
*/
NumberPtr Resource2::getACWP()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_ACWP));
}

/**
* Sets the notes text for this resource.
* 
* @param notes notes to be added
*/
void Resource2::setNotes(CString notes)
{
	set(Resource2Field::RF_NOTES, VariantPtr(new Variant(notes)));
}

/**
* Retrieves the notes text for this resource.
* 
* @return notes text
*/
CString Resource2::getNotes()
{
	CString notes = NumberHelper::getString(getCachedValue(Resource2Field::RF_NOTES));
    //return (notes == NULL ? "" : notes);
	return notes;
}

/**
* Sets the budgeted cost of work scheduled.
* 
* @param bcws budgeted cost of work scheduled
*/
void Resource2::setBCWS(NumberPtr bcws)
{
    set(Resource2Field::RF_BCWS, bcws);
}

/**
* Retrieves the budgeted cost of work scheduled.
* 
* @return budgeted cost of work scheduled
*/
NumberPtr Resource2::getBCWS()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_BCWS));
}

/**
* Sets the budgeted cost of work performed.
* 
* @param bcwp budgeted cost of work performed
*/
void Resource2::setBCWP(NumberPtr bcwp)
{
    set(Resource2Field::RF_BCWP, bcwp);
}

/**
* Retrievesthe budgeted cost of work performed.
* 
* @return budgeted cost of work performed
*/
NumberPtr Resource2::getBCWP()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_BCWP));
}

/**
* Sets the generic flag.
* 
* @param isGeneric generic flag
*/
void Resource2::setIsGeneric(bool isGeneric)
{
    m_generic = isGeneric;
}

/**
* Retrieves the generic flag.
* 
* @return generic flag
*/
bool Resource2::getGeneric()
{
    return (m_generic);
}

/**
* Sets the inactive flag.
* 
* @param isInactive inactive flag
*/
void Resource2::setIsInactive(bool isInactive)
{
    m_inactive = isInactive;
}

/**
* Retrieves the inactive flag.
* 
* @return inactive flag
*/
bool Resource2::getInactive()
{
    return (m_inactive);
}

/**
* Sets the active directory GUID2 for this resource.
* 
* @param guid active directory GUID2
*/
void Resource2::setActveDirectoryGUID(CString guid)
{
    m_activeDirectoryGUID = guid;
}

/**
* Retrieves the active directory GUID2 for this resource.
* 
* @return active directory GUID2
*/
CString Resource2::getActiveDirectoryGUID()
{
    return (m_activeDirectoryGUID);
}

/**
* Sets the actual overtime work protected duration.
* 
* @param duration actual overtime work protected
*/
void Resource2::setActualOvertimeWorkProtected(DurationPtr duration)
{
    m_actualOvertimeWorkProtected = duration;
}

/**
* Retrieves the actual overtime work protected duration.
* 
* @return actual overtime work protected
*/
DurationPtr Resource2::getActualOvertimeWorkProtected()
{
    return (m_actualOvertimeWorkProtected);
}

/**
* Sets the actual work protected duration.
* 
* @param duration actual work protected
*/
void Resource2::setActualWorkProtected(DurationPtr duration)
{
    m_actualWorkProtected = duration;
}

/**
* Retrieves the actual work protected duration.
* 
* @return actual work protected
*/
DurationPtr Resource2::getActualWorkProtected()
{
    return (m_actualWorkProtected);
}

/**
* Sets the booking type.
* 
* @param bookingType booking type
*/
void Resource2::setBookingType(BookingType bookingType)
{
    m_bookingType = bookingType;
}

/**
* Retrieves the booking type.
* 
* @return booking type
*/
BookingType Resource2::getBookingType()
{
    return (m_bookingType);
}

/**
* Sets the creation Date.
* 
* @param creationDate creation Date
*/
void Resource2::setCreationDate(DatePtr creationDate)
{
    set(Resource2Field::RF_CREATED, creationDate);
}

/**
* Retrieves the creation Date.
* 
* @return creation Date
*/
DatePtr Resource2::getCreationDate()
{
    return std::dynamic_pointer_cast<Date>( getCachedValue(Resource2Field::RF_CREATED));
}

/**
* Sets a flag indicating that a resource is an enterprise resource.
* 
* @param enterprise bool flag
*/
void Resource2::setIsEnterprise(bool enterprise)
{
    m_enterprise = enterprise;
}

/**
* Retrieves a flag indicating that a resource is an enterprise resource.
* 
* @return bool flag
*/
bool Resource2::getEnterprise()
{
    return (m_enterprise);
}

/**
* This method retrieves the calendar associated with this resource.
* 
* @return ProjectCalendar* instance
*/
ProjectCalendarPtr Resource2::getResource2Calendar()
{
    return (m_calendar);
}

/**
* This method allows a pre-existing resource calendar to be attached to a
* resource.
* 
* @param calendar resource calendar
*/
void Resource2::setResource2Calendar(ProjectCalendarPtr calendar)
{
    m_calendar = calendar;
    if (calendar)
    {
		calendar->setResource2(shared_from_this());
    }
}

/**
* This method allows a resource calendar to be added to a resource.
* 
* @return Resource2Calendar
* @throws MPXJException if more than one calendar is added
*/
ProjectCalendarPtr Resource2::addResource2Calendar()
{
    if (m_calendar != NULL)
    {
		throw std::bad_exception();// (MPXJException.MAXIMUM_RECORDS);
    }

	m_calendar = ProjectCalendarPtr (new ProjectCalendar(getParentFile()));
	m_calendar->setResource2(shared_from_this());
    return (m_calendar);
}

/**
* Sets the Base CalendarPtr field indicates which calendar is the base calendar
* for a resource calendar. The list includes the three built-in calendars,
* as well as any new base calendars you have created in the Change Working
* Time dialog box.
* 
* @param val calendar name
*/
void Resource2::setBaseCalendar(CString val)
{
	set(Resource2Field::RF_BASE_CALENDAR, 
		VariantPtr(new Variant(val.GetLength() == 0 ? L"Standard" : val)));
}

/**
* Sets the baseline cost. This field is ignored on import into MS Project
* 
* @param val - value to be set
*/
void Resource2::setBaselineCost(NumberPtr val)
{
    set(Resource2Field::RF_BASELINE_COST, val);
}

/**
* Sets the baseline work duration. This field is ignored on import into MS
* Project.
* 
* @param val - value to be set
*/
void Resource2::setBaselineWork(DurationPtr val)
{
    set(Resource2Field::RF_BASELINE_WORK, val);
}

/**
* Sets ID field value.
* 
* @param val value
*/
void Resource2::setID(int val)
{
	ProjectFile* parent = getParentFile();
    int previous = getID();
    if (previous != NULL)
    {
		parent->unmapResource2ID(previous);
    }
	parent->mapResource2ID(val, shared_from_this());

	set(Resource2Field::RF_ID, VariantPtr(new Variant(val)));
}

/**
* This field is ignored on import into MS Project.
* 
* @param val - value to be set
*/
void Resource2::setLinkedFields(bool val)
{
	set(Resource2Field::RF_LINKED_FIELDS, VariantPtr(new Variant(val)));
}

/**
* Set objects.
* 
* @param val - value to be set
*/
void Resource2::setObjects(int val)
{
	set(Resource2Field::RF_OBJECTS, VariantPtr(new Variant(val)));
}

/**
* Set a text value.
* 
* @param index text index (1-30)
* @param value text value
*/
void Resource2::setText(int index, CString value)
{
	set(selectField(Resource2FieldLists::CUSTOM_TEXT, index), VariantPtr(new Variant(value)));
}

/**
* Retrieve a text value.
* 
* @param index text index (1-30)
* @return text value
*/
CString Resource2::getText(int index)
{
    return NumberHelper::getString(getCachedValue(selectField(Resource2FieldLists::CUSTOM_TEXT, index)));
}

/**
* Sets Unique ID of this resource.
* 
* @param val Unique ID
*/
void Resource2::setUniqueID(int val)
{
	ProjectFile* parent = getParentFile();
    int previous = getUniqueID();
    if (previous != NULL)
    {
        parent->unmapResource2UniqueID(previous);
    }
    parent->mapResource2UniqueID(val, shared_from_this());

	set(Resource2Field::RF_UNIQUE_ID, VariantPtr(new Variant(val)));

    if (m_assignments.isEmpty() == false)
    {
		for (size_t i = 0; i < m_assignments.GetCount(); i++)
		{
			Resource2AssignmentPtr assignment = m_assignments[i];
			assignment->setResource2UniqueID(val);
        }
    }
}

/**
* Retrieves Base CalendarPtr name associated with this resource. This field
* indicates which calendar is the base calendar for a resource calendar.
* 
* @return Base calendar name
*/
CString Resource2::getBaseCalendar()
{
	return NumberHelper::getString(getCachedValue(Resource2Field::RF_BASE_CALENDAR));
}

/**
* Retrieves the Baseline Cost value. This value is the total planned cost
* for a resource for all assigned tasks. Baseline cost is also referred to
* as budget at completion (BAC).
* 
* @return Baseline cost value
*/
NumberPtr Resource2::getBaselineCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(Resource2Field::RF_BASELINE_COST));
}

/**
* Retrieves the Baseline Work value.
* 
* @return Baseline work value
*/
DurationPtr Resource2::getBaselineWork()
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(Resource2Field::RF_BASELINE_WORK));
}

/**
* Gets ID field value.
* 
* @return value
*/
int Resource2::getID()
{
	return NumberHelper::getInt(getCachedValue(Resource2Field::RF_ID));
}

/**
* Gets Linked Fields field value.
* 
* @return value
*/
bool Resource2::getLinkedFields()
{
    return NumberHelper::getBool(getCachedValue(Resource2Field::RF_LINKED_FIELDS));
}

/**
* Gets objects field value.
* 
* @return value
*/
int Resource2::getObjects()
{
	return NumberHelper::getInt(getCachedValue(Resource2Field::RF_OBJECTS));
}

/**
* Gets Unique ID field value.
* 
* @return value
*/
int Resource2::getUniqueID()
{
	return NumberHelper::getInt(getCachedValue(Resource2Field::RF_UNIQUE_ID));
}

/**
* Set a start value.
* 
* @param index start index (1-10)
* @param value start value
*/
void Resource2::setStart(int index, DatePtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_START, index), value);
}

/**
* Retrieve a start value.
* 
* @param index start index (1-10)
* @return start value
*/
DatePtr Resource2::getStart(int index)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_START, index)));
}

/**
* Set a finish value.
* 
* @param index finish index (1-10)
* @param value finish value
*/
void Resource2::setFinish(int index, DatePtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_FINISH, index), value);
}

/**
* Retrieve a finish value.
* 
* @param index finish index (1-10)
* @return finish value
*/
DatePtr Resource2::getFinish(int index)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_FINISH, index)));
}

/**
* Set a number value.
* 
* @param index number index (1-20)
* @param value number value
*/
void Resource2::setNumber(int index, NumberPtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_NUMBER, index), value);
}

/**
* Retrieve a number value.
* 
* @param index number index (1-20)
* @return number value
*/
NumberPtr Resource2::getNumber(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_NUMBER, index)));
}

/**
* Set a duration value.
* 
* @param index duration index (1-10)
* @param value duration value
*/
void Resource2::setDuration(int index, DurationPtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_DURATION, index), value);
}

/**
* Retrieve a duration value.
* 
* @param index duration index (1-10)
* @return duration value
*/
DurationPtr Resource2::getDuration(int index)
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_DURATION, index)));
}

/**
* Set a DatePtr value.
* 
* @param index DatePtr index (1-10)
* @param value DatePtr value
*/
void Resource2::setDate(int index, DatePtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_DATE, index), value);
}

/**
* Retrieve a DatePtr value.
* 
* @param index DatePtr index (1-10)
* @return DatePtr value
*/
DatePtr Resource2::getDate(int index)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_DATE, index)));
}

/**
* Set a cost value.
* 
* @param index cost index (1-10)
* @param value cost value
*/
void Resource2::setCost(int index, NumberPtr value)
{
    set(selectField(Resource2FieldLists::CUSTOM_COST, index), value);
}

/**
* Retrieve a cost value.
* 
* @param index cost index (1-10)
* @return cost value
*/
NumberPtr Resource2::getCost(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(Resource2FieldLists::CUSTOM_COST, index)));
}

/**
* Set a flag value.
* 
* @param index flag index (1-20)
* @param value flag value
*/
void Resource2::setFlag(int index, bool value)
{
	set(selectField(Resource2FieldLists::CUSTOM_FLAG, index), VariantPtr(new Variant(value)));
}

/**
* Retrieve a flag value.
* 
* @param index flag index (1-20)
* @return flag value
*/
bool Resource2::getFlag(int index)
{
    return NumberHelper::getBool(getCachedValue(selectField(Resource2FieldLists::CUSTOM_FLAG, index)));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode1(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE1, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode1()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE1));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode2(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE2, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode2()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE2));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode3(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE3, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode3()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE3));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode4(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE4, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode4()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE4));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode5(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE5, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode5()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE5));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode6(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE6, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode6()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE6));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode7(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE7, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode7()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE7));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode8(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE8, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode8()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE8));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode9(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE9, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode9()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE9));
}

/**
* Sets the value of an outline code field.
* 
* @param value outline code value
*/
void Resource2::setOutlineCode10(CString value)
{
	set(Resource2Field::RF_OUTLINE_CODE10, VariantPtr(new Variant(value)));
}

/**
* Retrieves the value of an outline code field.
* 
* @return outline code value
*/
CString Resource2::getOutlineCode10()
{
    return NumberHelper::getString(getCachedValue(Resource2Field::RF_OUTLINE_CODE10));
}

/**
* Removes this resource from the project.
*/
void Resource2::remove()
{
    getParentFile()->removeResource2(shared_from_this());
}

/**
* Retrieve the value of a field using its alias.
* 
* @param alias field alias
* @return field value
*/
ObjectPtr Resource2::getFieldByAlias(CString alias)
{
    return (getCachedValue(getParentFile()->getAliasResource2Field(alias)));
}

/**
* Set the value of a field using its alias.
* 
* @param alias field alias
* @param value field value
*/
void Resource2::setFieldByAlias(CString alias, ObjectPtr value)
{
    set(getParentFile()->getAliasResource2Field(alias), value);
}

/**
* This method is used internally within MPXJ to track tasks which are
* assigned to a particular resource.
* 
* @param assignment resource assignment instance
*/
void Resource2::addResource2Assignment(Resource2AssignmentPtr assignment)
{
    m_assignments.Add(assignment);
}

/**
* Internal method used as part of the process of removing a resource
* assignment.
* 
* @param assignment resource assignment to be removed
*/
void Resource2::removeResource2Assignment(Resource2AssignmentPtr assignment)
{
    m_assignments.remove(assignment);
}

/**
* Retrieve a list of tasks assigned to this resource. Note that if this
* project data has been read from an MPX file which declared some or all of
* the resources assignments before the tasks and resources to which the
* assignments relate, then these assignments may not appear in this list.
* Caveat emptor!
* 
* @return list of tasks assigned to this resource
*/
List<Resource2AssignmentPtr>* Resource2::getTaskAssignments()
{
    return &m_assignments;
}

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
int Resource2::getSubprojectResource2UniqueID()
{
	return NumberHelper::getInt(getCachedValue(Resource2Field::RF_SUBPROJECT_RESOURCE_UNIQUE_ID));
}

/**
* Sets the sub project unique resource ID.
* 
* @param subprojectUniqueResource2ID subproject unique resource ID
*/
void Resource2::setSubprojectResource2UniqueID(int subprojectUniqueResource2ID)
{
	set(Resource2Field::RF_SUBPROJECT_RESOURCE_UNIQUE_ID, 
		VariantPtr(new Variant(subprojectUniqueResource2ID)));
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
NumberPtr Resource2::getEnterpriseCost(int index)
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_COST, index)));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseCost(int index, NumberPtr value)
{
    set(selectField(Resource2FieldLists::ENTERPRISE_COST, index), value);
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
DatePtr Resource2::getEnterpriseDate(int index)
{
    return std::dynamic_pointer_cast<Date>( getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_DATE, index)));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseDate(int index, DatePtr value)
{
    set(selectField(Resource2FieldLists::ENTERPRISE_DATE, index), value);
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
DurationPtr Resource2::getEnterpriseDuration(int index)
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_DURATION, index)));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseDuration(int index, DurationPtr value)
{
    set(selectField(Resource2FieldLists::ENTERPRISE_DURATION, index), value);
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
bool Resource2::getEnterpriseFlag(int index)
{
    return (NumberHelper::getBool(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_FLAG, index))));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseFlag(int index, bool value)
{
	set(selectField(Resource2FieldLists::ENTERPRISE_FLAG, index), VariantPtr(new Variant(value)));
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
NumberPtr Resource2::getEnterpriseNumber(int index)
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_NUMBER, index)));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseNumber(int index, NumberPtr value)
{
    set(selectField(Resource2FieldLists::ENTERPRISE_NUMBER, index), value);
}

/**
* Retrieve an enterprise field value.
* 
* @param index field index
* @return field value
*/
CString Resource2::getEnterpriseText(int index)
{
    return NumberHelper::getString(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_TEXT, index)));
}

/**
* Set an enterprise field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseText(int index, CString value)
{
	set(selectField(Resource2FieldLists::ENTERPRISE_TEXT, index), VariantPtr(new Variant(value)));
}

/**
* Retrieve an enterprise custom field value.
* 
* @param index field index
* @return field value
*/
CString Resource2::getEnterpriseCustomField(int index)
{
    return NumberHelper::getString(getCachedValue(selectField(Resource2FieldLists::ENTERPRISE_CUSTOM_FIELD, index)));
}

/**
* Set an enterprise custom field value.
* 
* @param index field index
* @param value field value
*/
void Resource2::setEnterpriseCustomField(int index, CString value)
{
	set(selectField(Resource2FieldLists::ENTERPRISE_CUSTOM_FIELD, index), VariantPtr(new Variant(value)));
}

/**
* Set a baseline value.
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2::setBaselineCost(int baselineNumber, NumberPtr value)
{
    set(selectField(Resource2FieldLists::BASELINE_COSTS, baselineNumber), value);
}

/**
* Set a baseline value.
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2::setBaselineWork(int baselineNumber, DurationPtr value)
{
    set(selectField(Resource2FieldLists::BASELINE_WORKS, baselineNumber), value);
}

/**
* Retrieve a baseline value.
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
NumberPtr Resource2::getBaselineCost(int baselineNumber)
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(
		selectField(Resource2FieldLists::BASELINE_COSTS, baselineNumber)));
}

/**
* Retrieve a baseline value.
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
DurationPtr Resource2::getBaselineWork(int baselineNumber)
{
    return std::dynamic_pointer_cast<Duration2>(getCachedValue(
		selectField(Resource2FieldLists::BASELINE_WORKS, baselineNumber)));
}

/**
* Retrieve the budget flag.
* 
* @return budget flag
*/
bool Resource2::getBudget()
{
    return (NumberHelper::getBool(getCachedValue(Resource2Field::RF_BUDGET)));
}

/**
* Set the budget flag.
* 
* @param budget budget flag
*/
void Resource2::setBudget(bool budget)
{
	set(Resource2Field::RF_BUDGET, VariantPtr(new Variant(budget)));
}

/**
* Retrieves the resource GUID2.
* 
* @return resource GUID2.
*/
LPCGUID Resource2::getGUID()
{
	return NumberHelper::getGuid (getCachedValue(Resource2Field::RF_GUID));
}

/**
* Sets the resource GUID2.
* 
* @param value resource GUID2
*/
void Resource2::setGUID(LPCGUID value)
{
	set(Resource2Field::RF_GUID, VariantPtr(new Variant(value)));
}

/**
* Associates a complete cost rate table with the
* current resource. Note that the index corresponds with the
* letter label used by MS Project to identify each table.
* For example 0=Table A, 1=Table B, 2=Table C, and so on.
* 
* @param index table index
* @param crt table instance
*/
void Resource2::setCostRateTable(int index, CostRateTablePtr crt)
{
	m_costRateTables[index] = crt;
}

/**
* Retrieves a cost rate table associated with a resource.
* Note that the index corresponds with the
* letter label used by MS Project to identify each table.
* For example 0=Table A, 1=Table B, 2=Table C, and so on.
* 
* @param index table index
* @return table instance
*/
CostRateTablePtr Resource2::getCostRateTable(int index)
{
    return m_costRateTables[index];
}

/**
* Retrieve the availability table for this resource.
* 
* @return availability table
*/
AvailabilityTablePtr Resource2::getAvailability()
{
    return m_availability;
}

/**
* Maps a field index to a Resource2Field instance.
* 
* @param fields array of fields used as the basis for the mapping.
* @param index required field index
* @return Resource2Field instance
*/
Resource2Field Resource2::selectField(const Resource2Field fields[], int index)
{
    if (index < 1)// || index > fields.length)
    {
		throw std::bad_exception();// new IllegalArgumentException(index + " is not a valid field index");
    }
    return (fields[index - 1]);
}

/**
* {@inheritDoc}
*/
ObjectPtr Resource2::getCachedValue(FieldType* field)
{
    return (field == NULL ? NULL : m_array[field->getValue()]);
}

ObjectPtr Resource2::getCachedValue(Resource2Field field)
{
	return getCachedValue(Resource2FieldClass::getInstance(field));
}

/**
* {@inheritDoc}
*/
ObjectPtr Resource2::getCurrentValue(FieldType* field)
{
    ObjectPtr result = NULL;

    if (field != NULL)
    {
        Resource2Field resourceField = (Resource2Field) field->getValue();

        switch (resourceField)
        {
			case RF_COST_VARIANCE:
			{
				result = getCostVariance();
				break;
			}

			case RF_WORK_VARIANCE:
			{
				result = getWorkVariance();
				break;
			}

			case RF_CV:
			{
				result = getCV();
				break;
			}

			case RF_SV:
			{
				result = getSV();
				break;
			}

			case RF_OVERALLOCATED:
			{
				result = VariantPtr(new Variant(getOverAllocated()));
				break;
			}

			default:
			{
				result = m_array[field->getValue()];
				break;
			}
        }
    }

    return (result);
}

/**
* {@inheritDoc}
*/
void Resource2::set(FieldType* field, ObjectPtr value)
{
    if (field != NULL)
    {
        int index = field->getValue();
        if (m_eventsEnabled)
        {
			fireFieldChangeEvent((Resource2Field) field->getValue(), m_array[index], value);
        }

        m_array[index] = value;
    }
}

void Resource2::set(Resource2Field field, ObjectPtr value)
{
	set(Resource2FieldClass::getInstance(field), value);
}

/**
* Handle the change in a field value. Reset any cached calculated
* values affected by this change, pass on the event to any external
* listeners.
* 
* @param field field changed
* @param oldValue old field value
* @param newValue new field value
*/
void Resource2::fireFieldChangeEvent(Resource2Field field, ObjectPtr oldValue, ObjectPtr newValue)
{
    //
    // Internal event handling
    //
    switch (field)
    {
        case RF_COST:
        case RF_BASELINE_COST:
        {
			m_array[Resource2Field::RF_COST_VARIANCE] = NULL;
			break;
        }

		case RF_WORK:
		case RF_BASELINE_WORK:
        {
			m_array[Resource2Field::RF_WORK_VARIANCE] = NULL;
			break;
        }

		case RF_BCWP:
		case RF_ACWP:
        {
			m_array[Resource2Field::RF_CV] = NULL;
			m_array[Resource2Field::RF_SV] = NULL;
			break;
        }

		case RF_BCWS:
        {
			m_array[Resource2Field::RF_SV] = NULL;
			break;
        }

		case RF_PEAK:
		case RF_MAX_UNITS:
        {
			m_array[Resource2Field::RF_OVERALLOCATED] = NULL;
			break;
        }

        default:
        {
			break;
        }
    }

    //
    // External event handling
    //
	for (size_t i = 0; i < m_listeners.GetCount(); i ++)
    {
		FieldListener* listener = m_listeners[i];
		listener->fieldChange(this, Resource2FieldClass::getInstance(field), oldValue, newValue);
    }
}

/**
* {@inheritDoc}
*/
void Resource2::addFieldListener(FieldListener* listener)
{
    m_listeners.Add(listener);
}

/**
* {@inheritDoc}
*/
void Resource2::removeFieldListener(FieldListener* listener)
{
	m_listeners.remove(listener);
}

/**
* This method is used to set the value of a field in the resource.
* 
* @param field field to be set
* @param value new value for field.
*/
void Resource2::set(FieldType* field, bool value)
{
	set(field, VariantPtr(new Variant(value)));
}

/**
* Disable events firing when fields are upDated.
*/
void Resource2::disableEvents()
{
    m_eventsEnabled = false;
}

/**
* Enable events firing when fields are upDated. This is the default state.
*/
void Resource2::enableEvents()
{
    m_eventsEnabled = true;
}

/**
* This method implements the only method in the Comparable interface. This
* allows Resource2s to be compared and sorted based on their ID value. Note
* that if the MPX/MPP file has been generated by MSP, the ID value will
* always be in the correct sequence. The Unique ID value will not
* necessarily be in the correct sequence as task insertions and deletions
* will change the order.
* 
* @param o object to compare this instance with
* @return result of comparison
*/
int Resource2::compareTo(Resource2* o)
{
    int id1 = getID();
    int id2 = o->getID();
    return ((id1 < id2) ? (-1) : ((id1 == id2) ? 0 : 1));
}

///**
// * {@inheritDoc}
// */
//bool equals(Object o)
//{
//   bool result = false;
//   if (o instanceof Resource2)
//   {
//      result = (compareTo((Resource2) o) == 0);
//   }
//   return result;
//}

///**
// * {@inheritDoc}
// */
//int hashCode()
//{
//   return (NumberHelper::getInt(getID()));
//}

///**
// * {@inheritDoc}
// */
//CString toString()
//{
//   return ("[Resource2 id=" + getID() + " uniqueID=" + getUniqueID() + " name=" + getName() + "]");
//}
