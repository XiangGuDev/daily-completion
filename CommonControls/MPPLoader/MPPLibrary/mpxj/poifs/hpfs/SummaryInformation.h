#pragma once

#include "SpecialPropertySet.h"

/**
 * <p>Convenience class representing a Summary Information stream in a
 * Microsoft Office document.</p>
 *
 * @see DocumentSummaryInformation
 */
class SummaryInformation : public SpecialPropertySet
{
public:
    /**
     * <p>The document name a summary information stream usually has in a POIFS
     * filesystem.</p>
     */
    static const CString DEFAULT_STREAM_NAME;

	PropertyIDMapPtr getPropertySetIDMap();

    /**
     * <p>Creates a {@link SummaryInformation} from a given {@link
     * PropertySet}.</p>
     *
     * @param ps A property set which should be created from a summary
     *        information stream.
     * @throws UnexpectedPropertySetTypeException if <var>ps</var> does not
     *         contain a summary information stream.
     */
	SummaryInformation(PropertySet* ps);

    /**
     * <p>Returns the title (or <code>null</code>).</p>
     *
     * @return The title or <code>null</code>
     */
	CString getTitle();

    /**
     * <p>Returns the subject (or <code>null</code>).</p>
     *
     * @return The subject or <code>null</code>
     */
	CString getSubject();

    /**
     * <p>Returns the author (or <code>null</code>).</p>
     *
     * @return The author or <code>null</code>
     */
	CString getAuthor();

    /**
     * <p>Returns the keywords (or <code>null</code>).</p>
     *
     * @return The keywords or <code>null</code>
     */
	CString getKeywords();

    /**
     * <p>Returns the comments (or <code>null</code>).</p>
     *
     * @return The comments or <code>null</code>
     */
	CString getComments();

    /**
     * <p>Returns the template (or <code>null</code>).</p>
     *
     * @return The template or <code>null</code>
     */
	CString getTemplate();

    /**
     * <p>Returns the last author (or <code>null</code>).</p>
     *
     * @return The last author or <code>null</code>
     */
	CString getLastAuthor();

    /**
     * <p>Returns the revision number (or <code>null</code>). </p>
     *
     * @return The revision number or <code>null</code>
     */
	CString getRevNumber();

    /**
     * <p>Returns the total time spent in editing the document (or
     * <code>0</code>).</p>
     *
     * @return The total time spent in editing the document or 0 if the {@link
     *         SummaryInformation} does not contain this information.
     */
	long getEditTime();

    /**
     * <p>Returns the last printed time (or <code>null</code>).</p>
     *
     * @return The last printed time or <code>null</code>
     */
	DatePtr getLastPrinted();

    /**
     * <p>Returns the creation time (or <code>null</code>).</p>
     *
     * @return The creation time or <code>null</code>
     */
	DatePtr getCreateDateTime();

    /**
     * <p>Returns the last save time (or <code>null</code>).</p>
     *
     * @return The last save time or <code>null</code>
     */
	DatePtr getLastSaveDateTime();

    /**
     * <p>Returns the page count or 0 if the {@link SummaryInformation} does
     * not contain a page count.</p>
     *
     * @return The page count or 0 if the {@link SummaryInformation} does not
     *         contain a page count.
     */
	int getPageCount();

    /**
     * <p>Returns the word count or 0 if the {@link SummaryInformation} does
     * not contain a word count.</p>
     *
     * @return The word count or <code>null</code>
     */
	int getWordCount();

    /**
     * <p>Returns the character count or 0 if the {@link SummaryInformation}
     * does not contain a char count.</p>
     *
     * @return The character count or <code>null</code>
     */
	int getCharCount();

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
	ByteArrayPtr getThumbnail();

    /**
     * <p>Returns the application name (or <code>null</code>).</p>
     *
     * @return The application name or <code>null</code>
     */
	CString getApplicationName();

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
	int getSecurity();
};