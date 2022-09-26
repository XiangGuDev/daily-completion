#include "stdafx.h"
#include "FontStyle.h"

/**
* Constructor.
*
* @param fontBase font base instance
* @param italic italic flag
* @param bold bold flag
* @param underline underline flag
* @param strikethrough strikethrough flag
* @param color color type
* @param backgroundColor background color
* @param backgroundPattern background pattern
*/
FontStyle::FontStyle(FontBasePtr fontBase, bool italic, bool bold, bool underline, bool strikethrough,
	COLORREF color, COLORREF backgroundColor, BackgroundPattern backgroundPattern)
{
    m_fontBase = fontBase;
    m_italic = italic;
    m_bold = bold;
    m_underline = underline;
    m_strikethrough = strikethrough;
    m_color = color;
    m_backgroundColor = backgroundColor;
    m_backgroundPattern = backgroundPattern;
}

/**
* Retrieve the font base instance.
*
* @return font base instance
*/
FontBasePtr FontStyle::getFontBase()
{
    return (m_fontBase);
}

/**
* Retrieve the bold flag.
*
* @return bold flag
*/
bool FontStyle::getBold()
{
    return (m_bold);
}

/**
* Retrieve the font color.
*
* @return font color
*/
COLORREF FontStyle::getColor()
{
    return (m_color);
}

/**
* Retrieve the italic flag.
*
* @return italic flag
*/
bool FontStyle::getItalic()
{
    return (m_italic);
}

/**
* Retrieve the underline flag.
*
* @return underline flag
*/
bool FontStyle::getUnderline()
{
    return (m_underline);
}

/**
* Retrieve the strikethrough flag.
*
* @return strikethrough flag
*/
bool FontStyle::getStrikethrough()
{
    return (m_strikethrough);
}

/**
* Retrieve the background color.
* 
* @return background color
*/
COLORREF FontStyle::getBackgroundColor()
{
    return m_backgroundColor;
}

/**
* Retrieve the background pattern.
* 
* @return background pattern
*/
BackgroundPattern FontStyle::getBackgroundPattern()
{
    return m_backgroundPattern;
}

///**
//* {@inheritDoc}
//*/
//@Override CString toString()
//{
//    return ("[FontStyle* fontBase=" + m_fontBase + " italic=" + m_italic + " bold=" + m_bold + " underline=" + m_underline + " strikethrough=" + m_strikethrough + " color=" + m_color + " backgroundColor=" + m_backgroundColor + " backgroundPattern=" + m_backgroundPattern + "]");
//}