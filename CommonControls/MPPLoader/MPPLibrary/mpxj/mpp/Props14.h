#pragma once

#include "Props.h"

/**
 * This class represents the Props files found in Microsoft Project MPP14 files.
 */
class Props14 : public Props
{
public:
   /**
    * Constructor, reads the property data from an input stream.
    *
    * @param is input stream for reading props data
    */
	Props14(BwProj::toolkit::DataReadStreamPtr is);
};

typedef std::shared_ptr<Props14> Props14Ptr;