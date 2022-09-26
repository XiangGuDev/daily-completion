#pragma once

#include "MpxjEnum.h"

/**
 * This interface represents a view of a set of project data that has been
 * instantiated within an MS Project file-> View data is instantiated when a user
 * first looks at a view in MS Project. Each "screen" in MS Project, for example
 * the Gantt Chart, the Resource2 Sheet and so on are views. If a user has not
 * looked at a view (for example the Resource2 Usage view), information about
 * that view will not be present in the MPP file->
 */
class View2
{
public:
   /**
    * This method is used to retrieve the unique view identifier. This
    * value identifies the view within the file-> It does not identify
    * the type of view represented by an instance of this class.
    *
    * @return view identifier
    */
    virtual int getID() = 0;

   /**
    * This method is used to retrieve the view name. Note that internally
    * in MS Project the view name will contain an ampersand (&) used to
    * flag the letter that can be used as a shortcut for this view. The
    * ampersand is stripped out by MPXJ.
    *
    * @return view name
    */
	virtual CString getName() = 0;

   /**
    * Retrieves the view type.
    * 
    * @return view type
    */
	virtual MppViewType getType() = 0;
};

typedef std::shared_ptr<View2> ViewPtr;