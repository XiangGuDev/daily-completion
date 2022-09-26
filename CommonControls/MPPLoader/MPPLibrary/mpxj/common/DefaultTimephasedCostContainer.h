#pragma once

#include "..\TimephasedCostContainer.h"
#include "TimephasedCostNormaliser.h"

/**
 * Class used to manage timephased data.
 */
class DefaultTimephasedCostContainer : public TimephasedCostContainer
{
public:
	/**
	 * Constructor.
	 *
	 * @param calendar calendar to which the timephased data relates
	 * @param normaliser normaliser used to process this data
	 * @param data timephased data
	 * @param raw flag indicating if this data is raw
	 */
	DefaultTimephasedCostContainer(ProjectCalendarPtr calendar,
		TimephasedCostNormaliserPtr normaliser, TimephasedCostListPtr data, bool raw);

	/* (non-Javadoc)
	 * @see net.sf.mpxj.TimephasedCostContainer#getData()
	 */
	virtual TimephasedCostListPtr getData();

	/**
	 * Indicates if any timephased data is present.
	 *
	 * @return bool flag
	 */
	virtual bool hasData();

private:
	TimephasedCostListPtr m_data;
	bool m_raw;
	TimephasedCostNormaliserPtr m_normaliser;
	ProjectCalendarPtr m_calendar;
};

typedef std::shared_ptr<DefaultTimephasedCostContainer> DefaultTimephasedCostContainerPtr;