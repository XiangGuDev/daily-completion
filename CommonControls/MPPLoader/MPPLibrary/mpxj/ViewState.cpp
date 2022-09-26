#include "stdafx.h"
#include "ViewState.h"
#include "ProjectFile.h"
#include "Filter.h"

/**
* Constructor.
* 
* @param file parent project file
* @param viewName view name
* @param uniqueIdList unique ID list
* @param filterID filter ID
*/
ViewState::ViewState(ProjectFile* file, CString viewName, std::shared_ptr<List<int>> uniqueIdList, int filterID)
{
    m_file = file;
    m_viewName = viewName;
    m_uniqueIdList = uniqueIdList;
    m_filterID = filterID;
}

/**
* Retrieve the name of the view associated with this state.
* 
* @return view name
*/
CString ViewState::getViewName()
{
    return m_viewName;
}

/**
* Retrieve a list of unique IDs representing the contents of this view.
* 
* @return unique ID list
*/
std::shared_ptr<List<int>> ViewState::getUniqueIdList()
{
    return m_uniqueIdList;
}

/**
* Retrieve the currently applied filter. 
* 
* @return filter instance
*/
FilterPtr ViewState::getFilter()
{
    return m_file->getFilterByID(m_filterID);
}