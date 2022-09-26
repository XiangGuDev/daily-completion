#pragma once

#include "TimephasedItem.h"

/**
 * Represents timephased cost.
 */
class TimephasedCost : public TimephasedItem<NumberPtr>
{
   // Empty block
};

typedef std::shared_ptr<TimephasedCost> TimephasedCostPtr;

typedef List<TimephasedCostPtr> TimephasedCostList;
typedef std::shared_ptr<TimephasedCostList> TimephasedCostListPtr;
typedef Array2<TimephasedCostPtr> TimephasedCostArray;
typedef std::shared_ptr<TimephasedCostArray> TimephasedCostArrayPtr;