#include "stdafx.h"
#include "DefaultTimephasedWorkContainer.h"
#include "..\ProjectCalendar.h"

/**
* Constructor.
* 
* @param calendar calendar to which the timephased data relates
* @param normaliser normaliser used to process this data
* @param data timephased data
* @param raw flag indicating if this data is raw
*/
DefaultTimephasedWorkContainer::DefaultTimephasedWorkContainer(ProjectCalendarPtr calendar,
	TimephasedWorkNormaliserPtr normaliser, TimephasedWorkListPtr data, bool raw)
{
    //if (data instanceof List<?>)
    //{
	m_data = data;// (List<TimephasedWork>) data;
    //}
    //else
    //{
    //    m_data = new List<TimephasedWork>(data);
    //}
    m_raw = raw;
    m_calendar = calendar;
    m_normaliser = normaliser;
}

/**
* Copy constructor which can be used to scale the data it is copying
* by a given factor.
* 
* @param source source data
* @param perDayFactor per day scaling factor
* @param totalFactor total scaling factor
*/
DefaultTimephasedWorkContainer::DefaultTimephasedWorkContainer(
	DefaultTimephasedWorkContainer* source,
	double perDayFactor, double totalFactor)
{
	m_data.reset (new TimephasedWorkList());
    m_raw = source->m_raw;
    m_calendar = source->m_calendar;
    m_normaliser = source->m_normaliser;

	for (size_t i = 0; i < source->m_data->GetCount(); i ++)
    {
		TimephasedWorkPtr sourceItem = source->m_data->GetAt(i);
		m_data->Add(TimephasedWorkPtr (new TimephasedWork(sourceItem, totalFactor, perDayFactor)));
    }
}

/* (non-Javadoc)
* @see net.sf.mpxj.TimephasedWorkContainer#getData()
*/
TimephasedWorkListPtr DefaultTimephasedWorkContainer::getData()
{
    if (m_raw)
    {
        m_normaliser->normalise(m_calendar, m_data);
        m_raw = false;
    }
    return m_data;
}

/**
* Indicates if any timephased data is present.
* 
* @return bool flag
*/
bool DefaultTimephasedWorkContainer::hasData()
{
	//return !m_data.isEmpty();
	return m_data->GetCount() != 0;
}