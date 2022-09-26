#pragma once

/**
 * The standard font definitions used by MS Project are split into two parts,
 * the base fonr (i.e. the name and the size) and the styles applied to that
 * font. This class represents the base font information.
 */
class FontBase
{
public:
   /**
    * Constructor.
    *
    * @param index index number for this font
    * @param name font name
    * @param size font size
    */
	FontBase(int index, CString name, int size);

   /**
    * Retrieve the font name.
    *
    * @return font name
    */
	CString getName();

   /**
    * Retrieve the font size.
    *
    * @return font size
    */
	int getSize();

   /**
    * Retrieve the index number associated with this base font.
    *
    * @return index number
    */
	int getIndex();

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
	int m_index;
   CString m_name;
   int m_size;
};

typedef std::shared_ptr<FontBase> FontBasePtr;