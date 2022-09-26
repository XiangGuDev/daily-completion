#include "stdafx.h"
#include "DocumentInputStream.h"
#include "POIFSDocument.h"
#include "DocumentNode.h"

DocumentInputStream::DocumentInputStream(DocumentEntryPtr document)
{
	DocumentNodePtr node = std::dynamic_pointer_cast<DocumentNode>(document);
	_document = node->getDocument();
	if (!_document)
	{
		throw toolkit::Exception(L"Cannot open internal document storage");
	}
	reset();
}

void DocumentInputStream::reset()
{
	_currentBlock = _document->getDataInputBlock(0);
	_document_size = _document->getSize();
	_current_offset = 0;
}

BOOL DocumentInputStream::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
{
	nBytesRead = 0;
	if (_document_size == 0 || nBufSize == 0)
		return false;

	int limit = min(available(), (int) nBufSize);
	readFully((BYTE*)pBuffer, 0, nBufSize);
	nBytesRead = limit;

	return nBytesRead  != 0;
}

DWORD DocumentInputStream::available()
{
	return _document_size - _current_offset;
}

bool DocumentInputStream::checkAvaliable(int requestedSize)
{
	return (requestedSize <= _document_size - _current_offset);
}

void DocumentInputStream::readFully(BYTE* buf, int off, int len)
{
	if (!checkAvaliable(len)) return;

	int blockAvailable = _currentBlock->available();
	if (blockAvailable > len)
	{
		_currentBlock->readFully(buf, off, len);
		_current_offset += len;
		return;
	}

	// else read big amount in chunks
	int remaining = len;
	int writePos = off;
	while (remaining > 0)
	{
		bool blockIsExpiring = remaining >= blockAvailable;
		int reqSize = remaining;
		if (blockIsExpiring)
		{
			reqSize = blockAvailable;
		}

		_currentBlock->readFully(buf, writePos, reqSize);
		remaining -= reqSize;
		writePos += reqSize;
		_current_offset += reqSize;
		if (blockIsExpiring) {
			if (_current_offset == _document_size) {
				if (remaining > 0) {
					throw toolkit::Exception(L"reached end of document stream unexpectedly");
				}
				_currentBlock.reset();
				break;
			}
			_currentBlock = _document->getDataInputBlock(_current_offset);
			blockAvailable = _currentBlock->available();
		}
	}
}