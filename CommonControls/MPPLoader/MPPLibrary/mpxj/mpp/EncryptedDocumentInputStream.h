#pragma once

#include "..\poifs\DirectoryEntry.h"
#include "..\poifs\DocumentInputStream.h"
#include "..\..\..\Toolkit\include\IDataStream2.h"


class ProjectFile;


/**
 * This class wraps the POI {@link DocumentInputStream} class
 * to allow data to be decrypted before passing
 * it back to the caller.
 */
class EncryptedDocumentInputStream : public BwProj::toolkit::IDataReadStream
{
public:
	/**
	 * Method used to instantiate the appropriate input stream reader,
	 * a standard one, or one which can deal with "encrypted" data.
	 *
	 * @param file parent project file
	 * @param directory directory entry
	 * @param name file name
	 * @return new input stream
	 * @throws IOException
	 */
	static BwProj::toolkit::DataReadStreamPtr getInstance(ProjectFile* file, DirectoryEntryPtr directory, CString name);

	/**
	 * {@inheritDoc}
	 */
	virtual BOOL Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

	/**
	 * {@inheritDoc}
	 */
	virtual DWORD available();

	/**
	 * {@inheritDoc}
	 */
	virtual void reset();

	/**
	 * {@inheritDoc}
	 */
	virtual long skip(long n);

protected:
	/**
	* Constructor.
	*
	* @param entry file entry
	* @param mask the mask used to decrypt the stream.
	* @throws IOException
	*/
	EncryptedDocumentInputStream(DocumentEntryPtr entry, int mask);

private:
	DocumentInputStreamPtr m_dis;
	int m_mask;
};