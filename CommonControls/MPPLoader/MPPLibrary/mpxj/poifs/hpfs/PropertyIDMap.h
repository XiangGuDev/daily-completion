#pragma once

/**
 * <p>This is a dictionary which maps property ID values to property
 * ID strings.</p>
 *
 * <p>The methods {@link #getSummaryInformationProperties} and {@link
 * #getDocumentSummaryInformationProperties} return singleton {@link
 * PropertyIDMap}s. An application that wants to extend these maps
 * should treat them as unmodifiable, copy them and modifiy the
 * copies.</p>
 *
 * @author Rainer Klute <a
 * href="mailto:klute@rainer-klute.de">&lt;klute@rainer-klute.de&gt;</a>
 */
class PropertyIDMap : public Map2<int, CString>
{
public:
    static const int PID_TITLE = 2;
    static const int PID_SUBJECT = 3;
    static const int PID_AUTHOR = 4;
    static const int PID_KEYWORDS = 5;
    static const int PID_COMMENTS = 6;
    static const int PID_TEMPLATE = 7;
    static const int PID_LASTAUTHOR = 8;
    static const int PID_REVNUMBER = 9;
    static const int PID_EDITTIME = 10;
    static const int PID_LASTPRINTED = 11;
    static const int PID_CREATE_DTM = 12;
    static const int PID_LASTSAVE_DTM = 13;
    static const int PID_PAGECOUNT = 14;
    static const int PID_WORDCOUNT = 15;
    static const int PID_CHARCOUNT = 16;
    static const int PID_THUMBNAIL = 17;
    static const int PID_APPNAME = 18;
    static const int PID_SECURITY1 = 19;
    static const int PID_DICTIONARY1 = 0;
    static const int PID_CODEPAGE1 = 1;
    static const int PID_CATEGORY = 2;
    static const int PID_PRESFORMAT = 3;
    static const int PID_BYTECOUNT = 4;
    static const int PID_LINECOUNT = 5;
    static const int PID_PARCOUNT = 6;
    static const int PID_SLIDECOUNT = 7;
    static const int PID_NOTECOUNT = 8;
    static const int PID_HIDDENCOUNT = 9;
    static const int PID_MMCLIPCOUNT = 10;
    static const int PID_SCALE = 11;
    static const int PID_HEADINGPAIR = 12;
    static const int PID_DOCPARTS = 13;
    static const int PID_MANAGER = 14;
    static const int PID_COMPANY = 15;
    static const int PID_LINKSDIRTY = 16;
    static const int PID_MAX = PID_LINKSDIRTY;

 //   /**
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
	//void put(long id, CString idString);

    /**
     * <p>Gets the ID string for an ID from the {@link
     * PropertyIDMap}.</p>
     *
     * @param id The ID.
     * @return The ID string associated with <var>id</var>.
     */
	bool get(long id, CString& idString);


    /**
     * @return the Summary Information properties singleton
     */
	static std::shared_ptr<PropertyIDMap> getSummaryInformationProperties();


    /**
     * <p>Returns the Document Summary Information properties
     * singleton.</p>
     *
     * @return The Document Summary Information properties singleton.
     */
	static std::shared_ptr<PropertyIDMap> getDocumentSummaryInformationProperties();

private:
	/**
	* <p>Contains the summary information property ID values and
	* associated strings. See the overall HPSF documentation for
	* details!</p>
	*/
	static std::shared_ptr<PropertyIDMap> summaryInformationProperties;

	/**
	* <p>Contains the summary information property ID values and
	* associated strings. See the overall HPSF documentation for
	* details!</p>
	*/
	static std::shared_ptr<PropertyIDMap> documentSummaryInformationProperties;
};

typedef std::shared_ptr<PropertyIDMap> PropertyIDMapPtr;