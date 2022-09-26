#pragma once

#include "AbstractVarMeta.h"

/**
 * This class reads in the data from a VarMeta block. This block contains
 * meta data about variable length data items stored in a Var2Data block.
 * The meta data allows the size of the Var2Data block to be determined,
 * along with the number of data items it contains, identifiers for each item,
 * and finally the offset of each item within the block.
 */
class VarMeta12 : public AbstractVarMeta
{
public:
   /**
    * Constructor. Extracts that makes up this block from the input stream.
    *
    * @param is Input stream from which data is read
    * @throws IOException on file read error
    */
	VarMeta12(BwProj::toolkit::IDataReadStream* is);

private:
	static const int MAGIC = 0xFADFADBA;
};

class VarMeta9 : public AbstractVarMeta
{
public:
	/**
	* Constructor. Extracts that makes up this block from the input stream.
	*
	* @param is Input stream from which data is read
	* @throws IOException on file read error
	*/
	VarMeta9(BwProj::toolkit::IDataReadStream* is);

private:
	static const int MAGIC = 0xFADFADBA;
};