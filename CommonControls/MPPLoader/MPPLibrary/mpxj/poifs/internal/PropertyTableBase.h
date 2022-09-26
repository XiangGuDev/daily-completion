#pragma once

#include "BATManaged.h"
#include "HeaderBlock.h"
#include "RootProperty.h"

/**
 * This class embodies the Property Table for the filesystem,
 *  which looks up entries in the filesystem to their
 *  chain of blocks.
 * This is the core support, there are implementations
 *  for the different block schemes as needed.
 */
class PropertyTableBase : public BATManaged
{
public:
	PropertyTableBase(HeaderBlockPtr header_block);

    /**
     * Reading constructor (used when we've read in a file and we want
     * to extract the prop table from it). Populates the
     * properties thoroughly
     *
     * @param header_block the first block to read from
     * @param properties the list to populate
     *
     * @exception IOException if anything goes wrong (which should be
     *            a result of the input being NFG)
     */
	PropertyTableBase(HeaderBlockPtr header_block, std::shared_ptr<List<PropertyPtr>> properties);

    /**
     * Add a prop to the list of properties we manage
     *
     * @param prop the new Property to manage
     */
	void addProperty(PropertyPtr prop);

    /**
     * Remove a prop from the list of properties we manage
     *
     * @param prop the Property to be removed
     */
	void removeProperty(PropertyPtr prop);

    /**
     * Get the root prop
     *
     * @return the root prop
     */
	RootPropertyPtr getRoot();
   

    /**
     * Get the start block for the prop table
     *
     * @return start block index
     */
	int getStartBlock();

    /**
     * Set the start block for this instance
     *
     * @param index index into the array of BigBlock instances making
     *              up the the filesystem
     */
	void setStartBlock(int index);

protected:
	void populatePropertyTree(DirectoryPropertyPtr root);

protected:
	HeaderBlockPtr						_header_block;
	std::shared_ptr<List<PropertyPtr>>	_properties;
};