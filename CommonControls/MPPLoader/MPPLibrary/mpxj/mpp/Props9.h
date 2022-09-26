#pragma once

#include "Props.h"

/**
 * This class represents the Props files found in Microsoft Project MPP9 files.
 */
class Props9 : public Props
{
public:
	/**
	 * Constructor, reads the property data from an input stream.
	 *
	 * @param is input stream for reading props data
	 */
	Props9(BwProj::toolkit::IDataReadStream* is);
};