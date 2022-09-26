#include "stdafx.h"
#include "DefaultTimephasedCostContainer.h"

/**
	* Constructor.
	*
	* @param calendar calendar to which the timephased data relates
	* @param normaliser normaliser used to process this data
	* @param data timephased data
	* @param raw flag indicating if this data is raw
	*/
DefaultTimephasedCostContainer::DefaultTimephasedCostContainer(ProjectCalendarPtr calendar,
	TimephasedCostNormaliserPtr normaliser, TimephasedCostListPtr data, bool raw)
{
	m_data = data;
	//if (data instanceof LinkedList< ? >)
	//{
	//	m_data = (LinkedList<TimephasedCost>) data;
	//}
	//else
	//{
	//	m_data = new LinkedList<TimephasedCost>(data);
	//}
	m_raw = raw;
	m_calendar = calendar;
	m_normaliser = normaliser;
}

/* (non-Javadoc)
	* @see net.sf.mpxj.TimephasedCostContainer#getData()
	*/
TimephasedCostListPtr DefaultTimephasedCostContainer::getData()
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
bool DefaultTimephasedCostContainer::hasData()
{
	return !m_data->isEmpty();
}