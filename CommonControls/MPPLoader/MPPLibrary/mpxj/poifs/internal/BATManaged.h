#pragma once

/**
 * This interface defines behaviors for objects managed by the Block
 * Allocation Table (BAT).
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class BATManaged
{
public:
    /**
     * Return the number of BigBlock's this instance uses
     *
     * @return count of BigBlock instances
     */

	virtual int countBlocks() = 0;

    /**
     * Set the start block for this instance
     *
     * @param index index into the array of BigBlock instances making
     *              up the the filesystem
     */

	virtual void setStartBlock(int index) = 0;
};   // end public interface BATManaged

typedef std::shared_ptr<BATManaged> BATManagedPtr;