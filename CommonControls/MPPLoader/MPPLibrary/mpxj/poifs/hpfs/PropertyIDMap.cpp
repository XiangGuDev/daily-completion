#include "stdafx.h"
#include "PropertyIDMap.h"

std::shared_ptr<PropertyIDMap> PropertyIDMap::summaryInformationProperties;

std::shared_ptr<PropertyIDMap> PropertyIDMap::documentSummaryInformationProperties;

///**
//    * <p>Puts a ID string for an ID into the {@link
//    * PropertyIDMap}.</p>
//    *
//    * @param id The ID.
//    * @param idString The ID string.
//    * @return As specified by the {@link java.util.Map} interface, this method
//    * returns the previous value associated with the specified
//    * <var>id</var>, or <code>null</code> if there was no mapping for
//    * key.
//    */
//void PropertyIDMap::put(long id, String idString)
//{
//	put(id, idString);
//}



/**
    * <p>Gets the ID string for an ID from the {@link
    * PropertyIDMap}.</p>
    *
    * @param id The ID.
    * @return The ID string associated with <var>id</var>.
    */
bool PropertyIDMap::get(long id, CString& idString)
{
	return Lookup(id, idString);
    //return get(Long.valueOf(id));
}


/**
    * @return the Summary Information properties singleton
    */
std::shared_ptr<PropertyIDMap> PropertyIDMap::getSummaryInformationProperties()
{
    if (!summaryInformationProperties)
    {
		summaryInformationProperties.reset(new PropertyIDMap());
		summaryInformationProperties->put(PID_TITLE, L"PID_TITLE");
		summaryInformationProperties->put(PID_SUBJECT, L"PID_SUBJECT");
		summaryInformationProperties->put(PID_AUTHOR, L"PID_AUTHOR");
		summaryInformationProperties->put(PID_KEYWORDS, L"PID_KEYWORDS");
		summaryInformationProperties->put(PID_COMMENTS, L"PID_COMMENTS");
		summaryInformationProperties->put(PID_TEMPLATE, L"PID_TEMPLATE");
		summaryInformationProperties->put(PID_LASTAUTHOR, L"PID_LASTAUTHOR");
		summaryInformationProperties->put(PID_REVNUMBER, L"PID_REVNUMBER");
		summaryInformationProperties->put(PID_EDITTIME, L"PID_EDITTIME");
		summaryInformationProperties->put(PID_LASTPRINTED, L"PID_LASTPRINTED");
		summaryInformationProperties->put(PID_CREATE_DTM, L"PID_CREATE_DTM");
		summaryInformationProperties->put(PID_LASTSAVE_DTM, L"PID_LASTSAVE_DTM");
		summaryInformationProperties->put(PID_PAGECOUNT, L"PID_PAGECOUNT");
		summaryInformationProperties->put(PID_WORDCOUNT, L"PID_WORDCOUNT");
		summaryInformationProperties->put(PID_CHARCOUNT, L"PID_CHARCOUNT");
		summaryInformationProperties->put(PID_THUMBNAIL, L"PID_THUMBNAIL");
		summaryInformationProperties->put(PID_APPNAME, L"PID_APPNAME");
		summaryInformationProperties->put(PID_SECURITY, L"PID_SECURITY");
    }
    return summaryInformationProperties;
}



/**
    * <p>Returns the Document Summary Information properties
    * singleton.</p>
    *
    * @return The Document Summary Information properties singleton.
    */
std::shared_ptr<PropertyIDMap> PropertyIDMap::getDocumentSummaryInformationProperties()
{
    if (!documentSummaryInformationProperties)
    {
		documentSummaryInformationProperties.reset (new PropertyIDMap());
        documentSummaryInformationProperties->put(PID_DICTIONARY, L"PID_DICTIONARY");
        documentSummaryInformationProperties->put(PID_CODEPAGE, L"PID_CODEPAGE");
        documentSummaryInformationProperties->put(PID_CATEGORY, L"PID_CATEGORY");
        documentSummaryInformationProperties->put(PID_PRESFORMAT, L"PID_PRESFORMAT");
        documentSummaryInformationProperties->put(PID_BYTECOUNT, L"PID_BYTECOUNT");
        documentSummaryInformationProperties->put(PID_LINECOUNT, L"PID_LINECOUNT");
        documentSummaryInformationProperties->put(PID_PARCOUNT, L"PID_PARCOUNT");
        documentSummaryInformationProperties->put(PID_SLIDECOUNT, L"PID_SLIDECOUNT");
        documentSummaryInformationProperties->put(PID_NOTECOUNT, L"PID_NOTECOUNT");
        documentSummaryInformationProperties->put(PID_HIDDENCOUNT, L"PID_HIDDENCOUNT");
        documentSummaryInformationProperties->put(PID_MMCLIPCOUNT, L"PID_MMCLIPCOUNT");
        documentSummaryInformationProperties->put(PID_SCALE, L"PID_SCALE");
        documentSummaryInformationProperties->put(PID_HEADINGPAIR, L"PID_HEADINGPAIR");
        documentSummaryInformationProperties->put(PID_DOCPARTS, L"PID_DOCPARTS");
        documentSummaryInformationProperties->put(PID_MANAGER, L"PID_MANAGER");
        documentSummaryInformationProperties->put(PID_COMPANY, L"PID_COMPANY");
        documentSummaryInformationProperties->put(PID_LINKSDIRTY, L"PID_LINKSDIRTY");
    }
    return documentSummaryInformationProperties;
}