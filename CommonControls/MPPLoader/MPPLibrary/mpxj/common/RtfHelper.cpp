#include "stdafx.h"
#include "RtfHelper.h"

/**
* This method removes all RTF formatting from a given piece of text.
*
* @param text Text from which the RTF formatting is to be removed.
* @return Plain text
*/
CString RtfHelper::strip(CString text)
{
	//ATLASSERT(FALSE);
	//return text;
    bool formalRTF = isFormalRTF(text);
    CString newResult;
    try
    {
        //StringTextConverter stc = new StringTextConverter();
        //stc.convert(new RtfStringSource(text));
        //newResult = stripExtraLineEnd(stc.getText(), formalRTF);
    }
    catch (/*BwProj::*/toolkit::Exception&)
    {
        newResult = L"";
    }

	return text;
}

/**
* Remove the trailing line end from an RTF block.
* 
* @param text source text
* @param formalRTF true if this is a real RTF block
* @return text with line end stripped
*/
CString RtfHelper::stripExtraLineEnd(CString text, bool formalRTF)
{
	if (formalRTF && text.GetLength() && text.Right(1) == L"\n")
    {
        text = text.Left(text.GetLength() - 1);
    }
    return text;
}

/**
* Simple heuristic to determine if the string contains formal RTF.
*
* @param text source text
* @return true if the text may contain formal RTF
*/
bool RtfHelper::isFormalRTF(CString text)
{
	return text.Left(5) == L"{\\rtf";
}