#include "stdafx.h"
#include "SummaryInformation.h"

const CString SummaryInformation::DEFAULT_STREAM_NAME = L"\005SummaryInformation";

PropertyIDMapPtr SummaryInformation::getPropertySetIDMap()
{
	return PropertyIDMap::getSummaryInformationProperties();
}

/**
    * <p>Creates a {@link SummaryInformation} from a given {@link
    * PropertySet}.</p>
    *
    * @param ps A property set which should be created from a summary
    *        information stream.
    * @throws UnexpectedPropertySetTypeException if <var>ps</var> does not
    *         contain a summary information stream.
    */
SummaryInformation::SummaryInformation(PropertySet* ps)
	:SpecialPropertySet(ps)
{
    //if (!isSummaryInformation())
    //    throw Exception();
}



/**
    * <p>Returns the title (or <code>null</code>).</p>
    *
    * @return The title or <code>null</code>
    */
CString SummaryInformation::getTitle()
{
    return getPropertyStringValue(PropertyIDMap::PID_TITLE);
}

/**
    * <p>Returns the subject (or <code>null</code>).</p>
    *
    * @return The subject or <code>null</code>
    */
CString SummaryInformation::getSubject()
{
    return getPropertyStringValue(PropertyIDMap::PID_SUBJECT);
}

/**
    * <p>Returns the author (or <code>null</code>).</p>
    *
    * @return The author or <code>null</code>
    */
CString SummaryInformation::getAuthor()
{
    return getPropertyStringValue(PropertyIDMap::PID_AUTHOR);
}

/**
    * <p>Returns the keywords (or <code>null</code>).</p>
    *
    * @return The keywords or <code>null</code>
    */
CString SummaryInformation::getKeywords()
{
    return getPropertyStringValue(PropertyIDMap::PID_KEYWORDS);
}

/**
    * <p>Returns the comments (or <code>null</code>).</p>
    *
    * @return The comments or <code>null</code>
    */
CString SummaryInformation::getComments()
{
    return getPropertyStringValue(PropertyIDMap::PID_COMMENTS);
}


/**
    * <p>Returns the template (or <code>null</code>).</p>
    *
    * @return The template or <code>null</code>
    */
CString SummaryInformation::getTemplate()
{
    return getPropertyStringValue(PropertyIDMap::PID_TEMPLATE);
}

/**
    * <p>Returns the last author (or <code>null</code>).</p>
    *
    * @return The last author or <code>null</code>
    */
CString SummaryInformation::getLastAuthor()
{
    return getPropertyStringValue(PropertyIDMap::PID_LASTAUTHOR);
}

/**
    * <p>Returns the revision number (or <code>null</code>). </p>
    *
    * @return The revision number or <code>null</code>
    */
CString SummaryInformation::getRevNumber()
{
    return getPropertyStringValue(PropertyIDMap::PID_REVNUMBER);
}

/**
    * <p>Returns the total time spent in editing the document (or
    * <code>0</code>).</p>
    *
    * @return The total time spent in editing the document or 0 if the {@link
    *         SummaryInformation} does not contain this information.
    */
long SummaryInformation::getEditTime()
{
	DatePtr d = std::dynamic_pointer_cast<Date> (getProperty(PropertyIDMap::PID_EDITTIME));
    if (!d)
	{
        return 0;
    }
	return (long) d->getTime();
    //return Util.dateToFileTime(d);
}

/**
    * <p>Returns the last printed time (or <code>null</code>).</p>
    *
    * @return The last printed time or <code>null</code>
    */
DatePtr SummaryInformation::getLastPrinted()
{
    return std::dynamic_pointer_cast<Date> (getProperty(PropertyIDMap::PID_LASTPRINTED));
}

/**
    * <p>Returns the creation time (or <code>null</code>).</p>
    *
    * @return The creation time or <code>null</code>
    */
DatePtr SummaryInformation::getCreateDateTime()
{
	return std::dynamic_pointer_cast<Date> (getProperty(PropertyIDMap::PID_CREATE_DTM));
}


/**
    * <p>Returns the last save time (or <code>null</code>).</p>
    *
    * @return The last save time or <code>null</code>
    */
DatePtr SummaryInformation::getLastSaveDateTime()
{
	return std::dynamic_pointer_cast<Date> (getProperty(PropertyIDMap::PID_LASTSAVE_DTM));
}


/**
    * <p>Returns the page count or 0 if the {@link SummaryInformation} does
    * not contain a page count.</p>
    *
    * @return The page count or 0 if the {@link SummaryInformation} does not
    *         contain a page count.
    */
int SummaryInformation::getPageCount()
{
    return getPropertyIntValue(PropertyIDMap::PID_PAGECOUNT);
}

/**
    * <p>Returns the word count or 0 if the {@link SummaryInformation} does
    * not contain a word count.</p>
    *
    * @return The word count or <code>null</code>
    */
int SummaryInformation::getWordCount()
{
    return getPropertyIntValue(PropertyIDMap::PID_WORDCOUNT);
}

/**
    * <p>Returns the character count or 0 if the {@link SummaryInformation}
    * does not contain a char count.</p>
    *
    * @return The character count or <code>null</code>
    */
int SummaryInformation::getCharCount()
{
    return getPropertyIntValue(PropertyIDMap::PID_CHARCOUNT);
}

/**
    * <p>Returns the thumbnail (or <code>null</code>) <strong>when this
    * method is implemented. Please note that the return type is likely to
    * change!</strong></p>
    *
    * <p><strong>Hint to developers:</strong> Drew Varner &lt;Drew.Varner
    * -at- sc.edu&gt; said that this is an image in WMF or Clipboard (BMP?)
    * format. However, we won't do any conversion into any image type but
    * instead just return a byte array.</p>
    *
    * @return The thumbnail or <code>null</code>
    */
ByteArrayPtr SummaryInformation::getThumbnail()
{
	return std::dynamic_pointer_cast<ByteArray2> (getProperty(PropertyIDMap::PID_THUMBNAIL));
}

/**
    * <p>Returns the application name (or <code>null</code>).</p>
    *
    * @return The application name or <code>null</code>
    */
CString SummaryInformation::getApplicationName()
{
    return getPropertyStringValue(PropertyIDMap::PID_APPNAME);
}

/**
    * <p>Returns a security code which is one of the following values:</p>
    *
    * <ul>
    *
    * <li><p>0 if the {@link SummaryInformation} does not contain a
    * security field or if there is no security on the document. Use
    * {@link PropertySet#wasNull()} to distinguish between the two
    * cases!</p></li>
    *
    * <li><p>1 if the document is password protected</p></li>
    *
    * <li><p>2 if the document is read-only recommended</p></li>
    *
    * <li><p>4 if the document is read-only enforced</p></li>
    *
    * <li><p>8 if the document is locked for annotations</p></li>
    *
    * </ul>
    *
    * @return The security code or <code>null</code>
    */
int SummaryInformation::getSecurity()
{
    return getPropertyIntValue(PropertyIDMap::PID_SECURITY1);
}