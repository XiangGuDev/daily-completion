#pragma once

#include "FontBase.h"
#include "..\MpxjEnum.h"

/**
 * This class builds on the font described by a FontBase* instance
 * and add attributes for color, bold, italic and underline.
 */
class FontStyle
{
public:
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
	FontStyle(FontBasePtr fontBase, bool italic, bool bold, bool underline, bool strikethrough,
		COLORREF color, COLORREF backgroundColor, BackgroundPattern backgroundPattern);

   /**
    * Retrieve the font base instance.
    *
    * @return font base instance
    */
	FontBasePtr getFontBase();

   /**
    * Retrieve the bold flag.
    *
    * @return bold flag
    */
   bool getBold();

   /**
    * Retrieve the font color.
    *
    * @return font color
    */
   COLORREF getColor();

   /**
    * Retrieve the italic flag.
    *
    * @return italic flag
    */
   bool getItalic();

   /**
    * Retrieve the underline flag.
    *
    * @return underline flag
    */
   bool getUnderline();

   /**
    * Retrieve the strikethrough flag.
    *
    * @return strikethrough flag
    */
   bool getStrikethrough();

   /**
    * Retrieve the background color.
    * 
    * @return background color
    */
   COLORREF getBackgroundColor();

   /**
    * Retrieve the background pattern.
    * 
    * @return background pattern
    */
   BackgroundPattern getBackgroundPattern();

   ///**
   // * {@inheritDoc}
   // */
   //virtual CString toString();

private:
	FontBasePtr m_fontBase;
    bool m_italic;
    bool m_bold;
    bool m_underline;
    bool m_strikethrough;
    COLORREF m_color;
    COLORREF m_backgroundColor;
    BackgroundPattern m_backgroundPattern;
};

typedef std::shared_ptr<FontStyle> FontStylePtr;