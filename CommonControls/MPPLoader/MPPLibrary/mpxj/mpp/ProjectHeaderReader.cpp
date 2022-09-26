#include "stdafx.h"
#include "..\ProjectFile.h"
#include "ProjectHeaderReader.h"
#include "MPPUtility.h"
#include "..\Rate.h"
#include "..\poifs\hpfs\SummaryInformation.h"
#include "..\poifs\DocumentInputStream.h"

/**
* The main entry point for processing project header data.
* 
* @param file parent project file
* @param props properties data
* @param rootDir Root of the POI file system.
*/
void ProjectHeaderReader::process(ProjectFile* file, PropsPtr props, DirectoryEntryPtr rootDir)
{
    //try
    //{
        //MPPUtility.fileDump("c:\\temp\\props->txt", props->toString().getBytes());
        ProjectHeader* ph = file->getProjectHeader();
        ph->setStartDate(props->getTimestamp(Props::PROJECT_START_DATE));
        ph->setFinishDate(props->getTimestamp(Props::PROJECT_FINISH_DATE));
		ph->setScheduleFrom((ScheduleFrom)(1 - props->getShort(Props::SCHEDULE_FROM)));
        ph->setCalendarName(props->getUnicodeString(Props::DEFAULT_CALENDAR_NAME));
        ph->setDefaultStartTime(props->getTime(Props::START_TIME));
        ph->setDefaultEndTime(props->getTime(Props::END_TIME));
        ph->setStatusDate(props->getTimestamp(Props::STATUS_DATE));
        ph->setHyperlinkBase(props->getUnicodeString(Props::HYPERLINK_BASE));

        //ph->setDefaultDurationIsFixed();
        ph->setDefaultDurationUnits(MPPUtility::getDurationTimeUnits(props->getShort(Props::DURATION_UNITS)));
        ph->setMinutesPerDay(NumberPtr(new Number(props->getInt(Props::MINUTES_PER_DAY))));
		ph->setMinutesPerWeek(NumberPtr(new Number(props->getInt(Props::MINUTES_PER_WEEK))));
		ph->setDefaultOvertimeRate(RatePtr(new Rate(props->getDouble(Props::OVERTIME_RATE), TimeUnit::HOURS)));
		ph->setDefaultStandardRate(RatePtr(new Rate(props->getDouble(Props::STANDARD_RATE), TimeUnit::HOURS)));
		ph->setDefaultWorkUnits(MPPUtility::getWorkTimeUnits(props->getShort(Props::WORK_UNITS)));
        ph->setSplitInProgressTasks(props->getBoolean(Props::SPLIT_TASKS));
        ph->setUpdatingTaskStatusUpdatesResource2Status(props->getBoolean(Props::TASK_UPDATES_RESOURCE));

		ph->setCurrencyDigits(NumberPtr(new Number(props->getShort(Props::CURRENCY_DIGITS))));
        ph->setCurrencySymbol(props->getUnicodeString(Props::CURRENCY_SYMBOL));
        ph->setCurrencyCode(props->getUnicodeString(Props::CURRENCY_CODE));
        //ph->setDecimalSeparator();
		ph->setSymbolPosition(MPPUtility::getSymbolPosition(props->getShort(Props::CURRENCY_PLACEMENT)));
        //ph->setThousandsSeparator();
		ph->setWeekStartDay((Day)(props->getShort(Props::WEEK_START_DAY) + 1));
        ph->setFiscalYearStartMonth(props->getShort(Props::FISCAL_YEAR_START_MONTH));
        ph->setFiscalYearStart(props->getShort(Props::FISCAL_YEAR_START) == 1);
        ph->setDaysPerMonth(NumberPtr(new Number(props->getShort(Props::DAYS_PER_MONTH))));
        ph->setEditableActualCosts(props->getBoolean(Props::EDITABLE_ACTUAL_COSTS));
        ph->setHonorConstraints(!props->getBoolean(Props::HONOR_CONSTRAINTS));

		DocumentInputStream is (std::dynamic_pointer_cast<DocumentEntry>(rootDir->getEntry(SummaryInformation::DEFAULT_STREAM_NAME)));
		PropertySet ps(&is);
        SummaryInformation summaryInformation (&ps);
        ph->setProjectTitle(summaryInformation.getTitle());
        ph->setSubject(summaryInformation.getSubject());
        ph->setAuthor(summaryInformation.getAuthor());
        ph->setKeywords(summaryInformation.getKeywords());
        ph->setComments(summaryInformation.getComments());
        ph->setTemplate(summaryInformation.getTemplate());
        ph->setLastAuthor(summaryInformation.getLastAuthor());
		ph->setRevision(_ttol(summaryInformation.getRevNumber()));
        ph->setCreationDate(summaryInformation.getCreateDateTime());
        ph->setLastSaved(summaryInformation.getLastSaveDateTime());
        ph->setApplicationName(summaryInformation.getApplicationName());
        ph->setEditingTime((int) summaryInformation.getEditTime());
        ph->setLastPrinted(summaryInformation.getLastPrinted());

   //     ps = new PropertySet(new DocumentInputStream(((DocumentEntry) rootDir.getEntry(DocumentSummaryInformation.DEFAULT_STREAM_NAME))));
   //     ExtendedDocumentSummaryInformation documentSummaryInformation = new ExtendedDocumentSummaryInformation(ps);
   //     ph->setCategory(documentSummaryInformation.getCategory());
   //     ph->setPresentationFormat(documentSummaryInformation.getPresentationFormat());
   //     ph->setManager(documentSummaryInformation.getManager());
   //     ph->setCompany(documentSummaryInformation.getCompany());
   //     ph->setContentType(documentSummaryInformation.getContentType());
   //     ph->setContentStatus(documentSummaryInformation.getContentStatus());
   //     ph->setLanguage(documentSummaryInformation.getLanguage());
   //     ph->setDocumentVersion(documentSummaryInformation.getDocumentVersion());

   //     Map<String, Object> customPropertiesMap = new HashMap<String, Object>();
   //     CustomProperties customProperties = documentSummaryInformation.getCustomProperties();
   //     if (customProperties)
   //     {
			//for (CustomProperty property : customProperties.values())
			//{
			//	customPropertiesMap.put(property.getName(), property.getValue());
			//}
   //     }
   //     ph->setCustomProperties(customPropertiesMap);

        ph->setCalculateMultipleCriticalPaths(props->getBoolean(Props::CALCULATE_MULTIPLE_CRITICAL_PATHS));

        ph->setBaselineDate(0, props->getTimestamp(Props::BASELINE_DATE));
        ph->setBaselineDate(1, props->getTimestamp(Props::BASELINE1_DATE));
        ph->setBaselineDate(2, props->getTimestamp(Props::BASELINE2_DATE));
        ph->setBaselineDate(3, props->getTimestamp(Props::BASELINE3_DATE));
        ph->setBaselineDate(4, props->getTimestamp(Props::BASELINE4_DATE));
        ph->setBaselineDate(5, props->getTimestamp(Props::BASELINE5_DATE));
        ph->setBaselineDate(6, props->getTimestamp(Props::BASELINE6_DATE));
        ph->setBaselineDate(7, props->getTimestamp(Props::BASELINE7_DATE));
        ph->setBaselineDate(8, props->getTimestamp(Props::BASELINE8_DATE));
        ph->setBaselineDate(9, props->getTimestamp(Props::BASELINE9_DATE));
        ph->setBaselineDate(10, props->getTimestamp(Props::BASELINE10_DATE));
    //}

    //catch (Exception& ex)
    //{
    //    throw new MPXJException(MPXJException.READ_ERROR, ex);
    //}
}