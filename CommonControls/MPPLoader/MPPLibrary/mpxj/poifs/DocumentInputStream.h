#pragma once

#include "DocumentEntry.h"

class DataInputBlock;
class POIFSDocument;

///////////////////////////////////////////////////////////////////////////////////////////////
// DocumentInputStream
//
class DocumentInputStream : public BwProj::toolkit::IDataReadStream
{
public:
	DocumentInputStream(DocumentEntryPtr document);

	virtual BOOL Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

	virtual void reset();

	DWORD available();

protected:
	bool checkAvaliable(int requestedSize);
	void readFully(BYTE* buf, int off, int len);

private:
	std::shared_ptr<POIFSDocument> _document;
	std::shared_ptr<DataInputBlock> _currentBlock;
	long _current_offset;
	long _document_size;
};

typedef std::shared_ptr<DocumentInputStream> DocumentInputStreamPtr;