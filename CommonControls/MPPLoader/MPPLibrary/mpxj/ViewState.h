#pragma once

class ProjectFile;
class Filter2;

/**
 * This class represents the state of a view which has been saved
 * as part of a project file->
 */
class ViewState
{
public:
   /**
    * Constructor.
    * 
    * @param file parent project file
    * @param viewName view name
    * @param uniqueIdList unique ID list
    * @param filterID filter ID
    */
	ViewState(ProjectFile* file, CString viewName, std::shared_ptr<List<int>> uniqueIdList, int filterID);

   /**
    * Retrieve the name of the view associated with this state.
    * 
    * @return view name
    */
	CString getViewName();

   /**
    * Retrieve a list of unique IDs representing the contents of this view.
    * 
    * @return unique ID list
    */
	std::shared_ptr<List<int>> getUniqueIdList();

   /**
    * Retrieve the currently applied filter. 
    * 
    * @return filter instance
    */
	std::shared_ptr<Filter2> getFilter();

private:
   ProjectFile* m_file;
   CString m_viewName;
   std::shared_ptr<List<int>> m_uniqueIdList;
   int m_filterID;
};

typedef std::shared_ptr<ViewState> ViewStatePtr;