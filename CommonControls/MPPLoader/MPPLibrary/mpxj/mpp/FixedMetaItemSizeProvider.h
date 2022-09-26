#pragma once

/**
 * This interface is used to deliver the item size to be used in a FixedMeta structure.
 */
class FixedMetaItemSizeProvider
{
public:
   /**
    * Retrieve the item size to use for a FixedMeta structure.
    * 
    * @param fileSize size of the entire file containing the FixedMeta data
    * @param itemCount number of items we are expecting in the FixedMeta structure
    * @return the item size to use
    */
   virtual int getItemSize(int fileSize, int itemCount) = 0;
};
