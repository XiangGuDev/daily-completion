#pragma once

//import com.rtfparserkit.converter.text.StringTextConverter;
//import com.rtfparserkit.parser.RtfStringSource;

/**
 * This class is used to collect together utility functions for manipulating
 * RTF encoded text.
 */
class RtfHelper
{
public:
   /**
    * This method removes all RTF formatting from a given piece of text.
    *
    * @param text Text from which the RTF formatting is to be removed.
    * @return Plain text
    */
	static CString strip(CString text);

private:
   /**
    * Remove the trailing line end from an RTF block.
    * 
    * @param text source text
    * @param formalRTF true if this is a real RTF block
    * @return text with line end stripped
    */
	static CString stripExtraLineEnd(CString text, bool formalRTF);

   /**
   * Simple heuristic to determine if the string contains formal RTF.
   *
   * @param text source text
   * @return true if the text may contain formal RTF
   */
	static bool isFormalRTF(CString text);
};