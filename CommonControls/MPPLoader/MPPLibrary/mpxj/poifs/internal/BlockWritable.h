#pragma once

/**
 * An interface for persisting block storage of POIFS components.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class BlockWritable : public Object2
{
public:
    /**
     * Write the storage to an OutputStream
     *
     * @param stream the OutputStream to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */

    virtual void writeBlocks(BwProj::toolkit::IDataWriteStream* stream) = 0;
};   // end public interface BlockWritable

typedef std::shared_ptr<BlockWritable> BlockWritablePtr;
typedef Array2<BlockWritablePtr> BlockWritableArray;
typedef std::shared_ptr<BlockWritableArray> BlockWritableArrayPtr;