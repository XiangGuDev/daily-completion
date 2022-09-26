#pragma once

#include "..\TimephasedWorkContainer.h"
#include "TimephasedWorkNormaliser.h"

/**
 * Class used to manage timephased data.
 */
class DefaultTimephasedWorkContainer : public TimephasedWorkContainer
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
	DefaultTimephasedWorkContainer(ProjectCalendarPtr calendar,
		TimephasedWorkNormaliserPtr normaliser, TimephasedWorkListPtr data, bool raw);

   /**
    * Copy constructor which can be used to scale the data it is copying
    * by a given factor.
    * 
    * @param source source data
    * @param perDayFactor per day scaling factor
    * @param totalFactor total scaling factor
    */
	DefaultTimephasedWorkContainer(DefaultTimephasedWorkContainer* source,
		double perDayFactor, double totalFactor);

   /* (non-Javadoc)
    * @see net.sf.mpxj.TimephasedWorkContainer#getData()
    */
	virtual TimephasedWorkListPtr getData();

   /**
    * Indicates if any timephased data is present.
    * 
    * @return bool flag
    */
	virtual bool hasData();

private:
	TimephasedWorkListPtr m_data;
	bool m_raw;
	TimephasedWorkNormaliserPtr m_normaliser;
	ProjectCalendarPtr m_calendar;
};

typedef std::shared_ptr<DefaultTimephasedWorkContainer> DefaultTimephasedWorkContainerPtr;