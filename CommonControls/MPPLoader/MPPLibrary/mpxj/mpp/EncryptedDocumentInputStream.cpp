#include "stdafx.h"
#include "EncryptedDocumentInputStream.h"
#include "..\ProjectFile.h"
#include "..\..\..\Toolkit\include\IDataStream2.h"


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
BwProj::toolkit::DataReadStreamPtr EncryptedDocumentInputStream::getInstance(ProjectFile* file, DirectoryEntryPtr directory, CString name)
{
	DocumentEntryPtr entry = std::dynamic_pointer_cast<DocumentEntry>(directory->getEntry(name));
	BwProj::toolkit::DataReadStreamPtr stream;
	if (file->getEncoded())
	{
		stream.reset (new EncryptedDocumentInputStream(entry, file->getEncryptionCode()));
	}
	else
	{
		stream.reset(new DocumentInputStream(entry));
	}

	return stream;
}

/**
	* {@inheritDoc}
	*/
BOOL EncryptedDocumentInputStream::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
{
	BOOL result = m_dis->Read(pBuffer, nBufSize, nBytesRead);
	for (size_t loop = 0; loop < nBytesRead; loop++)
	{
		((BYTE*)pBuffer)[loop] ^= m_mask;
	}
	return result;
}

/**
	* {@inheritDoc}
	*/
DWORD EncryptedDocumentInputStream::available()
{
	return m_dis->available();
}

/**
	* {@inheritDoc}
	*/
void EncryptedDocumentInputStream::reset()
{
	m_dis->reset();
}

/**
	* {@inheritDoc}
	*/
long EncryptedDocumentInputStream::skip(long n)
{
	return m_dis->skip(n);
}

/**
* Constructor.
*
* @param entry file entry
* @param mask the mask used to decrypt the stream.
* @throws IOException
*/
EncryptedDocumentInputStream::EncryptedDocumentInputStream(DocumentEntryPtr entry, int mask)
{
	m_dis.reset(new DocumentInputStream(entry));
	m_mask = mask;
}