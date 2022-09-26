#include "stdafx.h"
#include "CompoundFile.h"
#include <atlconv.h>

namespace CompoundFiles
{
	/********************************** Start of Class Block *************************************/
	// PURPOSE: Manage a file by treating it as blocks of data of a certain size.
	Block::Block() :
		blockSize_(512), fileSize_(0), indexEnd_(0)
	{}

	bool Block::Create(const wchar_t* filename)
		// PURPOSE: Create a new block file and open it.
		// PURPOSE: If file is present, truncate it and then open it.
		// PROMISE: Return true if file is successfully created and opened, false if otherwise.
	{
		// Create new file
		string name = CW2A(filename);

		file_.open(name.c_str(), ios_base::out | ios_base::trunc);
		file_.close();
		file_.clear();

		// Open the file
		bool ret = this->Open(filename);
		return ret;
	}

	bool Block::Open(const wchar_t* filename, ios_base::openmode mode)
		// PURPOSE: Open an existing block file.
		// PROMISE: Return true if file is successfully opened, false if otherwise.
	{
		// Open existing file for reading or writing or both
		filename_ = filename;

		file_.open(filename_.c_str(), mode | ios_base::binary);
		if (!file_.is_open()) return false;

		mode_ = mode;

		// Calculate filesize
		if (mode & ios_base::in)
		{
			file_.seekg(0, ios_base::end);
			fileSize_ = (size_t) file_.tellg();
		}
		else if (mode & ios_base::out)
		{
			file_.seekp(0, ios_base::end);
			fileSize_ = (size_t)file_.tellp();
		}
		else
		{
			this->Close();
			return false;
		}

		// Calculate last index + 1
		indexEnd_ = fileSize_ / blockSize_ + (fileSize_ % blockSize_ ? 1 : 0);
		return true;
	}

	bool Block::Close()
		// PURPOSE: Close the opened block file.
		// PROMISE: Return true if file is successfully closed, false if otherwise.
	{
		file_.close();
		file_.clear();
		filename_.clear();
		fileSize_ = 0;
		indexEnd_ = 0;
		blockSize_ = 512;
		return !file_.is_open();
	}

	bool Block::IsOpen()
		// PURPOSE: Check if the block file is still opened.
		// PROMISE: Return true if file is still opened, false if otherwise.
	{
		return file_.is_open();
	}

	bool Block::Read(size_t index, char* block)
		// PURPOSE: Read a block of data from the opened file at the index position.
		// EXPLAIN: index is from [0..].
		// PROMISE: Return true if data are successfully read, false if otherwise.
	{
		if (!(mode_ & ios_base::in)) return false;
		if (index < indexEnd_)
		{
			file_.seekg(index * blockSize_);
			file_.read(block, blockSize_);
			return !file_.fail();
		}
		else return false;
	}

	bool Block::Write(size_t index, const char* block)
		// PURPOSE: Write a block of data to the opened file at the index position.
		// EXPLAIN: index is from [0..].
		// PROMISE: Return true if data are successfully written, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;
		file_.seekp(index * blockSize_);
		file_.write(block, blockSize_);
		if (indexEnd_ <= index)
		{
			indexEnd_ = index + 1;
			fileSize_ += blockSize_;
		}
		file_.close();
		file_.clear();
		file_.open(filename_.c_str(), mode_ | ios_base::binary);
		return file_.is_open();
	}

	bool Block::Swap(size_t index1, size_t index2)
		// PURPOSE: Swap two blocks of data in the opened file at the index positions.
		// EXPLAIN: index1 and index2 are from [0..].
		// PROMISE: Return true if data are successfully swapped, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;
		if (index1 < indexEnd_ && index2 < indexEnd_)
		{
			if (index1 == index2) return true;

			char* block1 = new char[blockSize_];
			if (!this->Read(index1, block1)) return false;

			char* block2 = new char[blockSize_];
			if (!this->Read(index2, block2)) return false;

			if (!this->Write(index1, block2)) return false;
			if (!this->Write(index2, block1)) return false;

			delete[] block1;
			delete[] block2;
			return true;
		}
		else return false;
	}

	bool Block::Move(size_t from, size_t to)
		// PURPOSE: Move a block of data in the opened file from an index position to another index position.
		// EXPLAIN: from and to are from [0..].
		// PROMISE: Return true if data are successfully moved, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;
		if (from < indexEnd_ && to < indexEnd_)
		{
			if (to > from)
			{
				for (size_t i = from; i != to; ++i)
				{
					if (!this->Swap(i, i + 1)) return false;
				}
			}
			else
			{
				for (size_t i = from; i != to; --i)
				{
					if (!this->Swap(i, i - 1)) return false;
				}
			}
			return true;
		}
		else return false;
	}

	bool Block::Insert(size_t index, const char* block)
		// PURPOSE: Insert a new block of data in the opened file at the index position.
		// EXPLAIN: index is from [0..].
		// PROMISE: Return true if data are successfully inserted, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;
		if (index <= indexEnd_)
		{
			// Write block to end of file
			if (!this->Write(indexEnd_, block)) return false;

			// Move block to index if necessary
			if (index < indexEnd_ - 1) return this->Move(indexEnd_ - 1, index);
			else return true;
		}
		else
		{
			// Write block to index after end of file
			return this->Write(index, block);
		}
	}

	bool Block::Erase(size_t index)
		// PURPOSE: Erase a block of data in the opened file at the index position.
		// EXPLAIN: index is from [0..].
		// PROMISE: Return true if data are successfully erased, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;
		if (index < indexEnd_)
		{
			fileSize_ -= blockSize_;
			indexEnd_ -= 1;

			// Read entire file except the block to be deleted into memory.
			char* buffer = new char[fileSize_];
			for (size_t i = 0, j = 0; i != indexEnd_ + 1; ++i)
			{
				file_.seekg(i*blockSize_);
				if (i != index)
				{
					file_.read(buffer + j*blockSize_, blockSize_);
					++j;
				}
			}
			file_.close();
			file_.open(filename_.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
			file_.write(buffer, fileSize_);	// Write the new file.
			file_.close();
			file_.open(filename_.c_str(), mode_ | ios_base::binary);
			delete[] buffer;
			return true;
		}
		else return false;
	}

	bool Block::Erase(vector<size_t>& indices)
		// PURPOSE: Erase blocks of data in the opened file at the index positions.
		// EXPLAIN: Each index in indices is from [0..].
		// PROMISE: Return true if data are successfully erased, false if otherwise.
	{
		if (!(mode_ & ios_base::out)) return false;

		// Read entire file except the blocks to be deleted into memory.
		size_t maxIndices = indices.size();
		fileSize_ -= maxIndices*blockSize_;
		char* buffer = new char[fileSize_];
		for (size_t i = 0, k = 0; i != indexEnd_; ++i)
		{
			file_.seekg(i*blockSize_);
			bool toDelete = false;
			for (size_t j = 0; j < maxIndices; ++j)
			{
				if (i == indices[j])
				{
					toDelete = true;
					break;
				}
			}
			if (!toDelete)
			{
				file_.read(buffer + k*blockSize_, blockSize_);
				++k;
			}
		}
		indexEnd_ -= maxIndices;

		file_.close();
		file_.open(filename_.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
		file_.write(buffer, fileSize_);	// Write the new file.
		file_.close();
		file_.open(filename_.c_str(), mode_ | ios_base::binary);
		delete[] buffer;
		return true;
	}
	/********************************** End of Class Block ***************************************/

	/********************************** Start of Class Header ************************************/
	// PURPOSE: Read and write data to a compound file header.
	CompoundFile::Header::Header() :
		fileType_(0xE11AB1A1E011CFD0LL),
		uk1_(0), uk2_(0), uk3_(0), uk4_(0), uk5_(0x003B), uk6_(0x0003), uk7_(-2),
		log2BigBlockSize_(9), log2SmallBlockSize_(6),
		uk8_(0), uk9_(0), uk10_(0), uk11_(0x00001000),
		SBATStart_(-2), SBATCount_(0),
		XBATStart_(-2), XBATCount_(0),
		BATCount_(1), propertiesStart_(1)
	{
		BATArray_[0] = 0;	// Initial BAT indices at block 0 (=block 1 in Block)
		fill(BATArray_ + 1, BATArray_ + 109, -1);	// Rest of the BATArray is empty
		Initialize();
	}

	void CompoundFile::Header::Write(char* block)
		// PURPOSE: Write header information into a block of data.
		// REQUIRE: Block of data must be at least 512 bytes in size.
	{
		LittleEndian::Write(block, fileType_, 0x0000, 8);
		LittleEndian::Write(block, uk1_, 0x0008, 4);
		LittleEndian::Write(block, uk2_, 0x000C, 4);
		LittleEndian::Write(block, uk3_, 0x0010, 4);
		LittleEndian::Write(block, uk4_, 0x0014, 4);
		LittleEndian::Write(block, uk5_, 0x0018, 2);
		LittleEndian::Write(block, uk6_, 0x001A, 2);
		LittleEndian::Write(block, uk7_, 0x001C, 2);
		LittleEndian::Write(block, log2BigBlockSize_, 0x001E, 2);
		LittleEndian::Write(block, log2SmallBlockSize_, 0x0020, 4);
		LittleEndian::Write(block, uk8_, 0x0024, 4);
		LittleEndian::Write(block, uk9_, 0x0028, 4);
		LittleEndian::Write(block, BATCount_, 0x002C, 4);
		LittleEndian::Write(block, propertiesStart_, 0x0030, 4);
		LittleEndian::Write(block, uk10_, 0x0034, 4);
		LittleEndian::Write(block, uk11_, 0x0038, 4);
		LittleEndian::Write(block, SBATStart_, 0x003C, 4);
		LittleEndian::Write(block, SBATCount_, 0x0040, 4);
		LittleEndian::Write(block, XBATStart_, 0x0044, 4);
		LittleEndian::Write(block, XBATCount_, 0x0048, 4);
		for (size_t i = 0; i < 109; ++i) LittleEndian::Write(block, BATArray_[i], 0x004C + i * 4, 4);
	}

	void CompoundFile::Header::Read(char* block)
		// PURPOSE: Read header information from a block of data.
		// REQUIRE: Block of data must be at least 512 bytes in size.
	{
		LittleEndian::Read(block, fileType_, 0x0000, 8);
		LittleEndian::Read(block, uk1_, 0x0008, 4);
		LittleEndian::Read(block, uk2_, 0x000C, 4);
		LittleEndian::Read(block, uk3_, 0x0010, 4);
		LittleEndian::Read(block, uk4_, 0x0014, 4);
		LittleEndian::Read(block, uk5_, 0x0018, 2);
		LittleEndian::Read(block, uk6_, 0x001A, 2);
		LittleEndian::Read(block, uk7_, 0x001C, 2);
		LittleEndian::Read(block, log2BigBlockSize_, 0x001E, 2);
		LittleEndian::Read(block, log2SmallBlockSize_, 0x0020, 4);
		LittleEndian::Read(block, uk8_, 0x0024, 4);
		LittleEndian::Read(block, uk9_, 0x0028, 4);
		LittleEndian::Read(block, BATCount_, 0x002C, 4);
		LittleEndian::Read(block, propertiesStart_, 0x0030, 4);
		LittleEndian::Read(block, uk10_, 0x0034, 4);
		LittleEndian::Read(block, uk11_, 0x0038, 4);
		LittleEndian::Read(block, SBATStart_, 0x003C, 4);
		LittleEndian::Read(block, SBATCount_, 0x0040, 4);
		LittleEndian::Read(block, XBATStart_, 0x0044, 4);
		LittleEndian::Read(block, XBATCount_, 0x0048, 4);
		for (size_t i = 0; i < 109; ++i)
		{
			LittleEndian::Read(block, BATArray_[i], 0x004C + i * 4, 4);
		}

		Initialize();
	}

	void CompoundFile::Header::Initialize()
	{
		bigBlockSize_ = (size_t)pow(2.0, log2BigBlockSize_);		// Calculate each big block size.
		smallBlockSize_ = (size_t)pow(2.0, log2SmallBlockSize_);	// Calculate each small block size.
	}
	/********************************** End of Class Header **************************************/

	/********************************** Start of Class Property **********************************/
	// PURPOSE: Read and write data to a compound file property.
	CompoundFile::Property::Property() :
		nameSize_(0),
		propertyType_(1), nodeColor_(1),
		previousProp_(-1), nextProp_(-1), childProp_(-1),
		uk1_(0), uk2_(0), uk3_(0), uk4_(0), uk5_(0),
		seconds1_(0), days1_(0), seconds2_(0), days2_(0),
		startBlock_(-2), size_(0)
	{
		fill(name_, name_ + 32, 0);
	}

	void CompoundFile::Property::Write(char* block)
		// PURPOSE: Write property information from a block of data.
		// REQUIRE: Block of data must be at least 128 bytes in size.
	{
		LittleEndian::WriteString(block, name_, 0x00, 32);
		LittleEndian::Write(block, nameSize_, 0x40, 2);
		LittleEndian::Write(block, propertyType_, 0x42, 1);
		LittleEndian::Write(block, nodeColor_, 0x43, 1);
		LittleEndian::Write(block, previousProp_, 0x44, 4);
		LittleEndian::Write(block, nextProp_, 0x48, 4);
		LittleEndian::Write(block, childProp_, 0x4C, 4);
		LittleEndian::Write(block, uk1_, 0x50, 4);
		LittleEndian::Write(block, uk2_, 0x54, 4);
		LittleEndian::Write(block, uk3_, 0x58, 4);
		LittleEndian::Write(block, uk4_, 0x5C, 4);
		LittleEndian::Write(block, uk5_, 0x60, 4);
		LittleEndian::Write(block, seconds1_, 0x64, 4);
		LittleEndian::Write(block, days1_, 0x68, 4);
		LittleEndian::Write(block, seconds2_, 0x6C, 4);
		LittleEndian::Write(block, days2_, 0x70, 4);
		LittleEndian::Write(block, startBlock_, 0x74, 4);
		LittleEndian::Write(block, size_, 0x78, 4);
	}

	void CompoundFile::Property::Read(char* block)
		// PURPOSE: Read property information from a block of data.
		// REQUIRE: Block of data must be at least 128 bytes in size.
	{
		LittleEndian::ReadString(block, name_, 0x00, 32);
		LittleEndian::Read(block, nameSize_, 0x40, 2);
		LittleEndian::Read(block, propertyType_, 0x42, 1);
		LittleEndian::Read(block, nodeColor_, 0x43, 1);
		LittleEndian::Read(block, previousProp_, 0x44, 4);
		LittleEndian::Read(block, nextProp_, 0x48, 4);
		LittleEndian::Read(block, childProp_, 0x4C, 4);
		LittleEndian::Read(block, uk1_, 0x50, 4);
		LittleEndian::Read(block, uk2_, 0x54, 4);
		LittleEndian::Read(block, uk3_, 0x58, 4);
		LittleEndian::Read(block, uk4_, 0x5C, 4);
		LittleEndian::Read(block, uk5_, 0x60, 4);
		LittleEndian::Read(block, seconds1_, 0x64, 4);
		LittleEndian::Read(block, days1_, 0x68, 4);
		LittleEndian::Read(block, seconds2_, 0x6C, 4);
		LittleEndian::Read(block, days2_, 0x70, 4);
		LittleEndian::Read(block, startBlock_, 0x74, 4);
		LittleEndian::Read(block, size_, 0x78, 4);
	}
	/********************************** End of Class Property ************************************/

	/********************************** Start of Class PropertyTree **********************************/
	CompoundFile::PropertyTree::PropertyTree() {};

	CompoundFile::PropertyTree::~PropertyTree()
	{
		size_t maxChildren = children_.size();
		for (size_t i = 0; i < maxChildren; ++i) delete children_[i];
	}
	/********************************** End of Class PropertyTree ************************************/

	/********************************** Start of Class CompoundFile ******************************/
	// PURPOSE: Manage a compound file.
	CompoundFile::CompoundFile() :
		block_(512), properties_(0), propertyTrees_(0),
		blocksIndices_(0), sblocksIndices_(0) {};

	CompoundFile::~CompoundFile() { this->Close(); }

	/************************* Compound File Functions ***************************/
	bool CompoundFile::Create(const wchar_t* filename)
		// PURPOSE: Create a new compound file and open it.
		// PURPOSE: If file is present, truncate it and then open it.
		// PROMISE: Return true if file is successfully created and opened, false if otherwise.
	{
		Close();
		file_.Create(filename);

		// Write compound file header
		header_ = Header();
		SaveHeader();

		// Save BAT
		blocksIndices_.clear();
		blocksIndices_.resize(128, -1);
		blocksIndices_[0] = -3;
		blocksIndices_[1] = -2;
		SaveBAT();

		// Save properties
		Property* root = new Property;
		wcscpy_s(root->name_, 32, L"Root Entry");
		root->propertyType_ = 5;
		properties_.push_back(root);
		SaveProperties();

		// Set property tree
		propertyTrees_ = new PropertyTree;
		propertyTrees_->parent_ = 0;
		propertyTrees_->self_ = properties_[0];
		propertyTrees_->index_ = 0;
		currentDirectory_ = propertyTrees_;

		return true;
	}

	bool CompoundFile::Open(const wchar_t* filename, ios_base::openmode mode)
		// PURPOSE: Open an existing compound file.
		// PROMISE: Return true if file is successfully opened, false if otherwise.
	{
		Close();
		if (!file_.Open(filename, mode)) return false;

		// Load header
		if (!LoadHeader()) return false;

		// Load BAT information
		LoadBAT();

		// Load properties
		propertyTrees_ = new PropertyTree;
		LoadProperties();
		currentDirectory_ = propertyTrees_;

		return true;
	}

	bool CompoundFile::Close()
		// PURPOSE: Close the opened compound file.
		// PURPOSE: Reset BAT indices, SBAT indices, properties and properties tree information.
		// PROMISE: Return true if file is successfully closed, false if otherwise.
	{
		blocksIndices_.clear();
		sblocksIndices_.clear();

		size_t maxProperties = properties_.size();
		for (size_t i = 0; i < maxProperties; ++i)
		{
			if (properties_[i]) delete properties_[i];
		}
		properties_.clear();

		if (propertyTrees_)
		{
			delete propertyTrees_;
			propertyTrees_ = 0;
		}

		previousDirectories_.clear();
		currentDirectory_ = 0;

		return file_.Close();
	}

	bool CompoundFile::IsOpen()
		// PURPOSE: Check if the compound file is still opened.
		// PROMISE: Return true if file is still opened, false if otherwise.
	{
		return file_.IsOpen();
	}

	/************************* Directory Functions ***************************/
	int CompoundFile::ChangeDirectory(const wchar_t* path)
		// PURPOSE: Change to a different directory in the compound file.
		// PROMISE: Current directory will not be changed if directory is not present.
	{
		previousDirectories_.push_back(currentDirectory_);

		// Handle special cases
		if (wcscmp(path, L".") == 0)
		{
			// Current directory
			previousDirectories_.pop_back();
			return SUCCESS;
		}
		if (wcscmp(path, L"..") == 0)
		{
			// Go up 1 directory
			if (currentDirectory_->parent_ != 0)
			{
				currentDirectory_ = currentDirectory_->parent_;
			}
			previousDirectories_.pop_back();
			return SUCCESS;
		}
		if (wcscmp(path, L"\\") == 0)
		{
			// Go to root directory
			currentDirectory_ = propertyTrees_;
			previousDirectories_.pop_back();
			return SUCCESS;
		}

		// Handle normal cases
		size_t ipos = 0;
		size_t npos = 0;
		size_t pathLength = wcslen(path);
		if (pathLength > 0 && path[0] == L'\\')
		{
			// Start from root directory
			currentDirectory_ = propertyTrees_;
			++ipos;
			++npos;
		}
		do
		{
			for (; npos < pathLength; ++npos)
			{
				if (path[npos] == L'\\') break;
			}

			wchar_t* directory = new wchar_t[npos - ipos + 1];
			copy(path + ipos, path + npos, directory);
			directory[npos - ipos] = 0;
			currentDirectory_ = FindProperty(currentDirectory_, directory);
			delete[] directory;
			ipos = npos + 1;
			npos = ipos;
			if (currentDirectory_ == 0)
			{
				// Directory not found
				currentDirectory_ = previousDirectories_.back();
				previousDirectories_.pop_back();
				return DIRECTORY_NOT_FOUND;
			}
		} while (npos < pathLength);
		previousDirectories_.pop_back();
		return SUCCESS;
	}

	int CompoundFile::MakeDirectory(const wchar_t* path)
		// PURPOSE: Create a new directory in the compound file.
		// PROMISE: Directory will not be created if it is already present or
		// PROMISE: a file with the same name is present.
	{
		previousDirectories_.push_back(currentDirectory_);
		Property* property = new Property;
		property->propertyType_ = 1;
		int ret = MakeProperty(path, property);
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		SaveHeader();
		SaveBAT();
		SaveProperties();
		return ret;
	}

	int CompoundFile::PresentWorkingDirectory(wchar_t* path)
		// PURPOSE: Get the full path of the current directory in the compound file.
		// REQUIRE: path must be large enough to receive the full path information.
	{
		previousDirectories_.push_back(currentDirectory_);
		vector<wchar_t> fullpath;
		do
		{
			size_t directoryLength = wcslen(currentDirectory_->self_->name_);
			vector<wchar_t> directory(directoryLength + 1);
			directory[0] = L'\\';
			copy(currentDirectory_->self_->name_,
				currentDirectory_->self_->name_ + directoryLength,
				directory.begin() + 1);
			fullpath.insert(fullpath.begin(), directory.begin(), directory.end());
		} while (currentDirectory_ = currentDirectory_->parent_);

		fullpath.erase(fullpath.begin(), fullpath.begin() + 11);
		if (fullpath.empty()) fullpath.push_back(L'\\');
		copy(fullpath.begin(), fullpath.end(), path);
		path[fullpath.size()] = 0;
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		return SUCCESS;
	}

	int CompoundFile::PresentWorkingDirectory(vector<wchar_t>& path)
		// PURPOSE: Get the full path of the current directory in the compound file.
	{
		previousDirectories_.push_back(currentDirectory_);
		path.clear();
		do
		{
			size_t directoryLength = wcslen(currentDirectory_->self_->name_);
			vector<wchar_t> directory(directoryLength + 1);
			directory[0] = L'\\';
			copy(currentDirectory_->self_->name_,
				currentDirectory_->self_->name_ + directoryLength,
				directory.begin() + 1);
			path.insert(path.begin(), directory.begin(), directory.end());
		} while (currentDirectory_ = currentDirectory_->parent_);

		path.erase(path.begin(), path.begin() + 11);
		if (path.empty()) path.push_back(L'\\');
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		return SUCCESS;
	}

	int CompoundFile::RemoveDirectory(const wchar_t* path)
		// PURPOSE: Remove a directory in the compound file.
		// PROMISE: Directory will not be removed if it has subdirectories or files under it.
	{
		PropertyTree* directory = FindProperty(path);
		if (directory == 0) return DIRECTORY_NOT_FOUND;
		if (directory->self_->childProp_ != -1) return DIRECTORY_NOT_EMPTY;
		DeletePropertyTree(directory);
		SaveHeader();
		SaveBAT();
		SaveProperties();
		return SUCCESS;
	}

	int CompoundFile::DelTree(const wchar_t* path)
		// PURPOSE: Remove everything in the path in the compound file, including
		// PURPOSE: any files and subdirectories.
	{
		previousDirectories_.push_back(currentDirectory_);
		PropertyTree* directory = FindProperty(path);
		if (directory == 0) return DIRECTORY_NOT_FOUND;
		if (directory->self_->childProp_ != -1)
		{
			size_t maxChildren = directory->children_.size();
			wchar_t* curpath = new wchar_t[65535];
			for (size_t i = 0; i < maxChildren; ++i)
			{
				currentDirectory_ = directory->children_[i];
				PresentWorkingDirectory(curpath);
				if (directory->children_[i]->self_->propertyType_ == 1)
				{
					// Directory
					DelTree(curpath);
				}
				else if (directory->children_[i]->self_->propertyType_ == 2)
				{
					// File
					RemoveFile(curpath);
				}
			}
			directory->self_->childProp_ = -1;
			delete[] curpath;
		}

		if (directory->self_->propertyType_ == 1)
		{
			// Directory
			RemoveDirectory(path);
		}
		else if (directory->self_->propertyType_ == 2)
		{
			// File
			RemoveFile(path);
		}

		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		return SUCCESS;
	}

	int CompoundFile::DirectoryList(vector<vector<wchar_t> >& list, const wchar_t* path)
	{
		previousDirectories_.push_back(currentDirectory_);
		if (path != 0)
		{
			int ret = ChangeDirectory(path);
			if (ret != SUCCESS) return ret;
		}
		list.clear();
		size_t maxChildren = currentDirectory_->children_.size();
		vector<wchar_t> name(32);
		for (size_t i = 0; i < maxChildren; ++i)
		{
			wcscpy_s(&*(name.begin()), 32, currentDirectory_->children_[i]->self_->name_);
			list.push_back(name);
		}
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		return SUCCESS;
	}


	/************************* File Functions ***************************/
	int CompoundFile::MakeFile(const wchar_t* path)
		// PURPOSE: Create a new file in the compound file.
		// PROMISE: File will not be created if it is already present or
		// PROMISE: a directory with the same name is present.
	{
		previousDirectories_.push_back(currentDirectory_);
		Property* property = new Property;
		property->propertyType_ = 2;
		int ret = MakeProperty(path, property);
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		SaveHeader();
		SaveBAT();
		SaveProperties();
		return ret;
	}

	int CompoundFile::RemoveFile(const wchar_t* path)
		// PURPOSE: Remove a file in the compound file.
	{
		int ret = WriteFile(path, 0, 0);
		if (ret == SUCCESS)
		{
			DeletePropertyTree(FindProperty(path));
			SaveHeader();
			SaveBAT();
			SaveProperties();
			return SUCCESS;
		}
		else return ret;
	}

	int CompoundFile::FileSize(const wchar_t* path, size_t& size)
		// PURPOSE: Get the size of a file in the compound file.
		// PROMISE: Return the data size stored in the Root Entry if path = "\".
		// PROMISE: size will not be set if file is not present in the compound file.
	{
		// Special case of reading root entry
		if (wcscmp(path, L"\\") == 0)
		{
			size = propertyTrees_->self_->size_;
			return SUCCESS;
		}

		// Check to see if file is present in the specified directory.
		PropertyTree* property = FindProperty(path);
		if (property == 0)
			return FILE_NOT_FOUND;
		else
		{
			size = property->self_->size_;
			return SUCCESS;
		}
	}

	int CompoundFile::ReadFile(const wchar_t* path, char* data)
		// PURPOSE: Read a file's data in the compound file.
		// REQUIRE: data must be large enough to receive the file's data.
		// REQUIRE: The required data size can be obtained by using FileSize().
		// PROMISE: Returns the small blocks of data stored by the Root Entry if path = "\".
		// PROMISE: data will not be set if file is not present in the compound file.
	{
		// Special case of reading root entry
		char* buffer;
		if (wcscmp(path, L"\\") == 0)
		{
			buffer = new char[DataSize(propertyTrees_->self_->startBlock_, true)];
			ReadData(propertyTrees_->self_->startBlock_, buffer, true);
			copy(buffer, buffer + propertyTrees_->self_->size_, data);
			delete[] buffer;
			return SUCCESS;
		}

		// Check to see if file is present in the specified directory.
		PropertyTree* property = FindProperty(path);
		if (property == 0) return FILE_NOT_FOUND;

		if (property->self_->size_ >= 4096)
		{
			// Data stored in normal big blocks
			buffer = new char[DataSize(property->self_->startBlock_, true)];
			ReadData(property->self_->startBlock_, buffer, true);
		}
		else
		{
			// Data stored in small blocks
			buffer = new char[DataSize(property->self_->startBlock_, false)];
			ReadData(property->self_->startBlock_, buffer, false);
		}
		// Truncated the retrieved data to the actual file size.
		copy(buffer, buffer + property->self_->size_, data);
		delete[] buffer;
		return SUCCESS;
	}

	int CompoundFile::ReadFile(const wchar_t* path, vector<char>& data)
		// PURPOSE: Read a file's data in the compound file.
		// PROMISE: Returns the small blocks of data stored by the Root Entry if path = "\".
		// PROMISE: data will not be set if file is not present in the compound file.
	{
		data.clear();
		size_t dataSize;
		int ret = FileSize(path, dataSize);
		if (ret != SUCCESS) return ret;

		data.resize(dataSize);
		return ReadFile(path, &*(data.begin()));
	}

	int CompoundFile::WriteFile(const wchar_t* path, const char* data, size_t size)
		// PURPOSE: Write data to a file in the compound file.
		// PROMISE: The file's original data will be replaced by the new data.
	{
		PropertyTree* property = FindProperty(path);
		if (property == 0) return FILE_NOT_FOUND;

		if (property->self_->size_ >= 4096)
		{
			if (size >= 4096) property->self_->startBlock_ = WriteData(data, size, property->self_->startBlock_, true);
			else
			{
				property->self_->startBlock_ = WriteData(0, 0, property->self_->startBlock_, true);
				property->self_->startBlock_ = WriteData(data, size, property->self_->startBlock_, false);
			}
		}
		else
		{
			if (size < 4096) property->self_->startBlock_ = WriteData(data, size, property->self_->startBlock_, false);
			else
			{
				property->self_->startBlock_ = WriteData(0, 0, property->self_->startBlock_, false);
				property->self_->startBlock_ = WriteData(data, size, property->self_->startBlock_, true);
			}
		}
		property->self_->size_ = size;
		SaveHeader();
		SaveBAT();
		SaveProperties();
		return SUCCESS;
	}

	int CompoundFile::WriteFile(const wchar_t* path, const vector<char>& data, size_t size)
		// PURPOSE: Write data to a file in the compound file.
		// PROMISE: The file's original data will be replaced by the new data.
	{
		return WriteFile(path, &*(data.begin()), size);
	}

	/*************ANSI char compound file, directory and file functions******************/
	//bool CompoundFile::Create(const char* filename)
	//{
	//	size_t filenameLength = strlen(filename);
	//	wchar_t* wname = new wchar_t[filenameLength+1];
	//	mbstowcs(wname, filename, filenameLength);
	//	wname[filenameLength] = 0;
	//	bool ret = Create(wname);
	//	delete[] wname;
	//	return ret;
	//}
	//
	//bool CompoundFile::Open(const char* filename, ios_base::openmode mode)
	//{
	//	size_t filenameLength = strlen(filename);
	//	wchar_t* wname = new wchar_t[filenameLength+1];
	//	mbstowcs(wname, filename, filenameLength);
	//	wname[filenameLength] = 0;
	//	bool ret = Open(wname, mode);
	//	delete[] wname;
	//	return ret;
	//}
	//
	//int CompoundFile::ChangeDirectory(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = ChangeDirectory(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::MakeDirectory(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = MakeDirectory(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::PresentWorkingDirectory(char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	int ret = PresentWorkingDirectory(wpath);
	//	if (ret == SUCCESS)
	//	{
	//		pathLength = wcslen(wpath);
	//		wcstombs(path, wpath, pathLength);
	//		path[pathLength] = 0;
	//	}
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::PresentWorkingDirectory(vector<char>& path)
	//{
	//	vector<wchar_t> wpath;
	//	int ret = PresentWorkingDirectory(wpath);
	//	if (ret == SUCCESS)
	//	{
	//		size_t pathLength = wpath.size();
	//		path.resize(pathLength);
	//		wcstombs(&*(path.begin()), &*(wpath.begin()), pathLength);
	//		path[pathLength] = 0;
	//	}
	//	return ret;
	//}
	//
	//int CompoundFile::RemoveDirectory(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = RemoveDirectory(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::DelTree(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = DelTree(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::MakeFile(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = MakeFile(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::RemoveFile(const char* path)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = RemoveFile(wpath);
	//	delete[] wpath;
	//	return ret;
	//}
	//
	//int CompoundFile::FileSize(const char* path, size_t& size)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = FileSize(wpath, size);
	//	delete[] wpath;
	//	return ret;
	//}
	//int CompoundFile::ReadFile(const char* path, char* data)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = ReadFile(wpath, data);
	//	delete[] wpath;
	//	return ret;
	//}
	//int CompoundFile::ReadFile(const char* path, vector<char>& data)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = ReadFile(wpath, data);
	//	delete[] wpath;
	//	return ret;
	//}
	//int CompoundFile::WriteFile(const char* path, char* data, size_t size)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = WriteFile(wpath, data, size);
	//	delete[] wpath;
	//	return ret;
	//}
	//int CompoundFile::WriteFile(const char* path, vector<char>& data, size_t size)
	//{
	//	size_t pathLength = strlen(path);
	//	wchar_t* wpath = new wchar_t[pathLength+1];
	//	mbstowcs(wpath, path, pathLength);
	//	wpath[pathLength] = 0;
	//	int ret = WriteFile(wpath, data, size);
	//	delete[] wpath;
	//	return ret;
	//}

	/*********************** Inaccessible General Functions ***************************/
	void CompoundFile::IncreaseLocationReferences(vector<size_t> indices)
		// PURPOSE: Increase block location references in header, BAT indices and properties,
		// PURPOSE: which will be affected by the insertion of new indices contained in indices.
		// PROMISE: Block location references which are smaller than all the new indices
		// PROMISE: will not be affected.
		// PROMISE: SBAT location references will not be affected.
		// PROMISE: Changes will not be written to compound file.
	{
		size_t maxIndices = indices.size();

		// Change BAT Array references
		{for (size_t i = 0; i < 109 && header_.BATArray_[i] != -1; ++i)
		{
			size_t count = 0;
			for (size_t j = 0; j < maxIndices; ++j)
			{
				if (header_.BATArray_[i] >= (int)indices[j] &&
					header_.BATArray_[i] != -1) ++count;
			}
			header_.BATArray_[i] += count;
		}}

		// Change XBAT start block if any
		if (header_.XBATCount_)
		{
			size_t count = 0;
			for (size_t j = 0; j < maxIndices; ++j)
			{
				if (header_.XBATStart_ >= (int)indices[j] &&
					header_.XBATStart_ != -2) ++count;
			}
			header_.XBATStart_ += count;
		}

		// Change SBAT start block if any
		if (header_.SBATCount_)
		{
			size_t count = 0;
			for (size_t j = 0; j < maxIndices; ++j)
			{
				if (header_.SBATStart_ >= (int)indices[j] &&
					header_.SBATStart_ != -2) ++count;
			}
			header_.SBATStart_ += count;
		}

		// Change BAT block indices
		size_t maxBATindices = blocksIndices_.size();
		{for (size_t i = 0; i < maxBATindices && blocksIndices_[i] != -1; ++i)
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (blocksIndices_[i] >(int)indices[j] &&
					blocksIndices_[i] != -2 &&
					blocksIndices_[i] != -3) ++count;
			}
			blocksIndices_[i] += count;
		}}

		// Change properties start block
		size_t count = 0;
		{for (size_t i = 0; i < maxIndices; ++i)
		{
			if (header_.propertiesStart_ >= (int)indices[i] &&
				header_.propertiesStart_ != -2) ++count;
		}}
		header_.propertiesStart_ += count;

		// Change individual properties start block if their size is more than 4096
		size_t maxProperties = properties_.size();
		if (!properties_.empty())
		{
			size_t count = 0;
			for (size_t j = 0; j < maxIndices; ++j)
			{
				if (properties_[0]->startBlock_ >= (int)indices[j] &&
					properties_[0]->startBlock_ != -2) ++count;
			}
			properties_[0]->startBlock_ += count;
		}
		{for (size_t i = 1; i < maxProperties; ++i)
		{
			if (properties_[i]->size_ >= 4096)
			{
				size_t count = 0;
				for (size_t j = 0; j < maxIndices; ++j)
				{
					if (properties_[i]->startBlock_ >= (int)indices[j] &&
						properties_[i]->startBlock_ != -2) ++count;
				}
				properties_[i]->startBlock_ += count;
			}
		}}
	}

	void CompoundFile::DecreaseLocationReferences(vector<size_t> indices)
		// PURPOSE: Decrease block location references in header, BAT indices and properties,
		// PURPOSE: which will be affected by the deletion of indices contained in indices.
		// PROMISE: BAT indices pointing to a deleted index will be redirected to point to 
		// PROMISE: the location where the deleted index original points to.
		// PROMISE: Block location references which are smaller than all the new indices
		// PROMISE: will not be affected.
		// PROMISE: SBAT location references will not be affected.
		// PROMISE: Changes will not be written to compound file.
	{
		size_t maxIndices = indices.size();

		// Change BAT Array references
		{for (size_t i = 0; i < 109 && header_.BATArray_[i] != -1; ++i)
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (header_.BATArray_[i] >(int)indices[j] &&
					header_.BATArray_[i] != -1) ++count;
			}
			header_.BATArray_[i] -= count;
		}}

		// Change XBAT start block if any
		if (header_.XBATCount_)
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (header_.XBATStart_ >(int)indices[j] &&
					header_.XBATStart_ != -2) ++count;
			}
			header_.XBATStart_ -= count;
		}

		// Change SBAT start block if any
		if (header_.SBATCount_)
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (header_.SBATStart_ >(int)indices[j] &&
					header_.SBATStart_ != -2) ++count;
			}
			header_.SBATStart_ -= count;
		}

		// Change BAT block indices
		// Redirect BAT indices pointing to a deleted index to point to
		// the location where the deleted index original points to.
		size_t maxBATindices = blocksIndices_.size();
		{for (size_t i = 0; i < maxBATindices && blocksIndices_[i] != -1; ++i)
		{
			bool end;
			do
			{
				end = true;
				for (size_t j = 0; j < maxIndices; ++j)
				{
					if (blocksIndices_[i] == indices[j])
					{
						blocksIndices_[i] = blocksIndices_[indices[j]];
						end = false;
						break;
					}
				}
			} while (!end);
		}}
		// Erase indices to be deleted from the block indices
		sort(indices.begin(), indices.end(), greater<size_t>());
		{for (size_t i = 0; i < maxIndices; ++i)
		{
			blocksIndices_.erase(blocksIndices_.begin() + indices[i]);
			blocksIndices_.push_back(-1);
		}}

		// Decrease block location references for affected block indices.
		{for (size_t i = 0; i < maxBATindices && blocksIndices_[i] != -1; ++i)
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (blocksIndices_[i] >(int)indices[j] &&
					blocksIndices_[i] != -2 &&
					blocksIndices_[i] != -3) ++count;
			}
			blocksIndices_[i] -= count;
		}}

		// Change properties start block
		size_t count = 0;
		{for (size_t i = 0; i<maxIndices; ++i)
		{
			if (header_.propertiesStart_ >(int)indices[i] &&
				header_.propertiesStart_ != -2) ++count;
		}}
		header_.propertiesStart_ -= count;

		size_t maxProperties = properties_.size();
		// Change Root Entry start block
		if (!properties_.empty())
		{
			size_t count = 0;
			for (size_t j = 0; j<maxIndices; ++j)
			{
				if (properties_[0]->startBlock_ >(int)indices[j] &&
					properties_[0]->startBlock_ != -2) ++count;
			}
			properties_[0]->startBlock_ -= count;
		}
		{for (size_t i = 1; i < maxProperties; ++i)
		{
			if (properties_[i]->size_ >= 4096)
			{
				// Change individual properties start block if their size is more than 4096
				size_t count = 0;
				for (size_t j = 0; j<maxIndices; ++j)
				{
					if (properties_[i]->startBlock_ > (int) indices[j] &&
						properties_[i]->startBlock_ != -2) ++count;
				}
				properties_[i]->startBlock_ -= count;
			}
		}}
	}

	void CompoundFile::SplitPath(const wchar_t* path,
		wchar_t*& parentpath,
		wchar_t*& propertyname)
		// PURPOSE: Get a path's parent path and its name.
		// EXPLAIN: E.g. path = "\\Abc\\def\\ghi => parentpath = "\\Abc\\def", propertyname = "ghi".
		// REQUIRE: Calling function is responsible for deleting the memory created for
		// REQUIRE: parentpath and propertyname.
	{
		size_t pathLength = wcslen(path);

		int npos;
		for (npos = pathLength - 1; npos > 0; --npos)
		{
			if (path[npos] == L'\\') break;
		}

		if (npos != 0)
		{
			// Get parent path if available
			parentpath = new wchar_t[npos + 1];
			copy(path, path + npos, parentpath);
			parentpath[npos] = 0;
			++npos;
		}

		// Get property name (ignore initial "\" if present)
		if (npos == 0 && pathLength > 0 && path[0] == L'\\') ++npos;
		propertyname = new wchar_t[pathLength - npos + 1];
		copy(path + npos, path + pathLength, propertyname);
		propertyname[pathLength - npos] = 0;
	}

	/*********************** Inaccessible Header Functions ***************************/
	bool CompoundFile::LoadHeader()
		// PURPOSE: Load header information for compound file.
		// PROMISE: Return true if file header contain magic number, false if otherwise.
	{
		file_.Read(0, &*(block_.begin()));
		header_.Read(&*(block_.begin()));

		// Check magic number to see if it is a compound file 
		if (header_.fileType_ != 0xE11AB1A1E011CFD0LL) return false;

		block_.resize(header_.bigBlockSize_);		// Resize buffer block
		file_.SetBlockSize(header_.bigBlockSize_);	// Resize block array block size
		return true;
	}

	void CompoundFile::SaveHeader()
		// PURPOSE: Save header information for compound file.
	{
		header_.Write(&*(block_.begin()));
		file_.Write(0, &*(block_.begin()));
	}

	/*********************** Inaccessible BAT Functions ***************************/
	void CompoundFile::LoadBAT()
		// PURPOSE: Load all block allocation table information for compound file.
	{
		// Read BAT indices
		{ for (int i = 0; i < header_.BATCount_; ++i)
		{
			// Load blocksIndices_
			blocksIndices_.resize(blocksIndices_.size() + 128, -1);
			file_.Read(header_.BATArray_[i] + 1, &*(block_.begin()));
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Read(&*(block_.begin()), blocksIndices_[j + i * 128], j * 4, 4);
			}
		} }

		// Read XBAT indices
		{
		for (int i = 0; i < header_.XBATCount_; ++i)
		{
			blocksIndices_.resize(blocksIndices_.size() + 128, -1);
			file_.Read(header_.XBATStart_ + i + 1, &*(block_.begin()));
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Read(&*(block_.begin()), blocksIndices_[j + ((i + 109) * 128)], j * 4, 4);
			}
		}
	}

		// Read SBAT indices
		{for (int i = 0; i < header_.SBATCount_; ++i)
		{
			sblocksIndices_.resize(sblocksIndices_.size() + 128, -1);
			file_.Read(header_.SBATStart_ + i + 1, &*(block_.begin()));
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Read(&*(block_.begin()), sblocksIndices_[j + i * 128], j * 4, 4);
			}
		}}
	}

	void CompoundFile::SaveBAT()
		// PURPOSE: Save all block allocation table information for compound file.
	{
		// Write BAT indices
		{ for (int i = 0; i < header_.BATCount_; ++i)
		{
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Write(&*(block_.begin()), blocksIndices_[j + i * 128], j * 4, 4);
			}
			file_.Write(header_.BATArray_[i] + 1, &*(block_.begin()));
		} }

		// Write XBAT indices
		{
		for (int i = 0; i < header_.XBATCount_; ++i)
		{
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Write(&*(block_.begin()), blocksIndices_[j + ((i + 109) * 128)], j * 4, 4);
			}
			file_.Write(header_.XBATStart_ + i + 1, &*(block_.begin()));
		}
	}

		// Write SBAT indices
		{for (int i = 0; i < header_.SBATCount_; ++i)
		{
			for (size_t j = 0; j < 128; ++j)
			{
				LittleEndian::Write(&*(block_.begin()), sblocksIndices_[j + i * 128], j * 4, 4);
			}
			file_.Write(header_.SBATStart_ + i + 1, &*(block_.begin()));
		}}
	}

	size_t CompoundFile::DataSize(size_t startIndex, bool isBig)
		// PURPOSE: Gets the total size occupied by a property, starting from startIndex.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
		// PROMISE: Returns the total size occupied by the property which is the total 
		// PROMISE: number of blocks occupied multiply by the block size.
	{
		vector<size_t> indices;
		if (isBig)
		{
			GetBlockIndices(startIndex, indices, true);
			return indices.size()*header_.bigBlockSize_;
		}
		else
		{
			GetBlockIndices(startIndex, indices, false);
			return indices.size()*header_.smallBlockSize_;
		}
	}

	size_t CompoundFile::ReadData(size_t startIndex, char* data, bool isBig)
		// PURPOSE: Read a property's data, starting from startIndex.
		// REQUIRE: data must be large enough to receive the property's data
		// REQUIRE: The required data size can be obtained by using DataSize().
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
		// PROMISE: Returns the total size occupied by the property which is the total 
		// PROMISE: number of blocks occupied multiply by the block size.
	{
		vector<size_t> indices;
		if (isBig)
		{
			GetBlockIndices(startIndex, indices, true);
			size_t maxIndices = indices.size();
			for (size_t i = 0; i < maxIndices; ++i)
			{
				file_.Read(indices[i] + 1, data + i*header_.bigBlockSize_);
			}
			return maxIndices*header_.bigBlockSize_;
		}
		else
		{
			GetBlockIndices(startIndex, indices, false);
			size_t minIndex = *min_element(indices.begin(), indices.end());
			size_t maxIndex = *max_element(indices.begin(), indices.end());
			size_t smallBlocksPerBigBlock = header_.bigBlockSize_ / header_.smallBlockSize_;
			size_t minBlock = minIndex / smallBlocksPerBigBlock;
			size_t maxBlock = maxIndex / smallBlocksPerBigBlock +
				(maxIndex % smallBlocksPerBigBlock ? 1 : 0);
			size_t totalBlocks = maxBlock - minBlock;
			char* buffer = new char[DataSize(properties_[0]->startBlock_, true)];
			ReadData(properties_[0]->startBlock_, buffer, true);

			size_t maxIndices = indices.size();
			for (size_t i = 0; i < maxIndices; ++i)
			{
				size_t start = (indices[i] - minBlock*smallBlocksPerBigBlock)*header_.smallBlockSize_;
				copy(buffer + start,
					buffer + start + header_.smallBlockSize_,
					data + i*header_.smallBlockSize_);
			}
			delete[] buffer;
			return maxIndices*header_.smallBlockSize_;
		}
	}

	size_t CompoundFile::WriteData(const char* data, size_t size, int startIndex, bool isBig)
		// PURPOSE: Write data to a property, starting from startIndex.
		// EXPLAIN: startIndex can be -2 if property initially has no data.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
		// PROMISE: The file's original data will be replaced by the new data.
		// PROMISE: Returns the startIndex of new data for the property.
	{
		if (isBig)
		{
			if (size == 0 && startIndex == -2) return startIndex;

			// Get present indices
			vector<size_t> indices;
			GetBlockIndices(startIndex, indices, true);
			size_t maxPresentBlocks = indices.size();

			// Calculate how many blocks does the data need
			size_t extraSize = size % header_.bigBlockSize_;
			size_t maxNewBlocks = size / header_.bigBlockSize_ + (extraSize ? 1 : 0);

			// Readjust indices and remove blocks if new data size is smaller than original
			int extraBlocks = maxPresentBlocks - maxNewBlocks;
			if (extraBlocks > 0)
			{
				// Place new end marker
				if (maxNewBlocks != 0) blocksIndices_[indices[maxNewBlocks] - 1] = -2;
				else startIndex = -2;

				// Get indices of blocks to delete
				vector<size_t> indicesToRemove(extraBlocks);
				copy(indices.begin() + maxNewBlocks, indices.end(), indicesToRemove.begin());
				indices.erase(indices.begin() + maxNewBlocks, indices.end());

				// Remove extra blocks and readjust indices
				FreeBlocks(indicesToRemove, true);
			}

			// Write blocks into available space
			size_t remainingFullBlocks = size / header_.bigBlockSize_;
			size_t curIndex = 0;
			if (maxPresentBlocks != 0)
			{
				for (; remainingFullBlocks && curIndex < maxPresentBlocks;
					--remainingFullBlocks, ++curIndex)
				{
					file_.Write(indices[curIndex] + 1, data + curIndex*header_.bigBlockSize_);
				}
			}

			// Check if all blocks have been written		
			size_t index;
			if (indices.empty()) index = 0;
			else if (curIndex == 0) index = indices[0];
			else index = (startIndex != -2) ? indices[curIndex - 1] : 0;
			if (remainingFullBlocks != 0)
			{
				// Require extra blocks to write data (i.e. new data is larger than original data
				do
				{
					size_t newIndex = GetFreeBlockIndex(true); // Get new free block to write data
					if (startIndex == -2) startIndex = newIndex; // Get start index
					else LinkBlocks(index, newIndex, true); // Link last index to new index
					file_.Write(newIndex + 1, data + curIndex*header_.bigBlockSize_);
					++curIndex;
					index = newIndex;
				} while (--remainingFullBlocks);
			}

			if (extraSize != 0)
			{
				size_t newIndex;
				if (curIndex >= maxPresentBlocks)
				{
					// No more free blocks to write extra block data
					newIndex = GetFreeBlockIndex(true); // Get new free block to write data			
					if (startIndex == -2) startIndex = newIndex;
					else LinkBlocks(index, newIndex, true);
				}
				else newIndex = indices[curIndex];

				// Write extra block after increasing its size to the minimum block size
				vector<char> tempdata(header_.bigBlockSize_, 0);
				copy(data + curIndex*header_.bigBlockSize_, data + curIndex*header_.bigBlockSize_ + extraSize, tempdata.begin());
				file_.Write(newIndex + 1, &*(tempdata.begin()));
			}
			return startIndex;
		}
		else
		{
			if (size == 0 && startIndex == -2) return startIndex;

			if (size != 0 && properties_[0]->startBlock_ == -2)
			{
				size_t newIndex = GetFreeBlockIndex(true);
				fill(block_.begin(), block_.end(), 0);
				file_.Insert(newIndex, &*(block_.begin()));
				IncreaseLocationReferences(vector<size_t>(1, newIndex));
				properties_[0]->startBlock_ = newIndex;
				properties_[0]->size_ = header_.bigBlockSize_;
			}

			// Get present indices
			vector<size_t> indices;
			GetBlockIndices(startIndex, indices, false);
			size_t maxPresentBlocks = indices.size();

			// Calculate how many blocks does the data need
			size_t extraSize = size % header_.smallBlockSize_;
			size_t maxNewBlocks = size / header_.smallBlockSize_ + (extraSize ? 1 : 0);

			vector<char> smallBlocksData;
			int extraBlocks = maxPresentBlocks - maxNewBlocks;
			if (extraBlocks > 0)
			{
				// Readjust indices and remove blocks
				// Place new end marker
				if (maxNewBlocks != 0) sblocksIndices_[indices[maxNewBlocks] - 1] = -2;
				else startIndex = -2;

				// Get indices of blocks to delete
				vector<size_t> indicesToRemove(extraBlocks);
				copy(indices.begin() + maxNewBlocks, indices.end(), indicesToRemove.begin());
				indices.erase(indices.begin() + maxNewBlocks, indices.end());

				// Remove extra blocks and readjust indices
				FreeBlocks(indicesToRemove, false);
			}
			else if (extraBlocks < 0)
			{
				size_t maxBlocks = properties_[0]->size_ / header_.bigBlockSize_ +
					(properties_[0]->size_ % header_.bigBlockSize_ ? 1 : 0);
				size_t actualSize = maxBlocks * header_.bigBlockSize_;
				smallBlocksData.resize(actualSize);
				ReadData(properties_[0]->startBlock_, &*(smallBlocksData.begin()), true);
				smallBlocksData.resize(properties_[0]->size_);

				// Readjust indices and add blocks
				size_t newBlocksNeeded = -extraBlocks;
				size_t index = maxPresentBlocks - 1;
				for (size_t i = 0; i < newBlocksNeeded; ++i)
				{
					size_t newIndex = GetFreeBlockIndex(false); // Get new free block to write data
					if (startIndex == -2) startIndex = newIndex; // Get start index
					else LinkBlocks(index, newIndex, false);  // Link last index to new index
					smallBlocksData.insert(smallBlocksData.begin() + newIndex,
						header_.smallBlockSize_, 0);
					index = newIndex;
				}
				properties_[0]->size_ = newBlocksNeeded * header_.smallBlockSize_;
			}
			if (smallBlocksData.empty())
			{
				size_t maxBlocks = properties_[0]->size_ / header_.bigBlockSize_ +
					(properties_[0]->size_ % header_.bigBlockSize_ ? 1 : 0);
				size_t actualSize = maxBlocks * header_.bigBlockSize_;
				smallBlocksData.resize(actualSize);
				ReadData(properties_[0]->startBlock_, &*(smallBlocksData.begin()), true);
				smallBlocksData.resize(properties_[0]->size_);
			}

			// Write blocks
			GetBlockIndices(startIndex, indices, false);
			size_t fullBlocks = size / header_.smallBlockSize_;
			for (size_t i = 0; i < fullBlocks; ++i)
			{
				copy(data + i*header_.smallBlockSize_,
					data + i*header_.smallBlockSize_ + header_.smallBlockSize_,
					smallBlocksData.begin() + indices[i] * header_.smallBlockSize_);
			}
			if (extraSize != 0)
			{
				copy(data + fullBlocks*header_.smallBlockSize_,
					data + fullBlocks*header_.smallBlockSize_ + extraSize,
					smallBlocksData.begin() + indices[fullBlocks] * header_.smallBlockSize_);
			}
			WriteData(&*(smallBlocksData.begin()), properties_[0]->size_,
				properties_[0]->startBlock_, true);
			return startIndex;
		}
	}

	void CompoundFile::GetBlockIndices(size_t startIndex, vector<size_t>& indices, bool isBig)
		// PURPOSE: Get the indices of blocks where data are stored, starting from startIndex.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
	{
		indices.clear();
		if (isBig)
		{
			for (size_t i = startIndex; i != -2; i = blocksIndices_[i]) indices.push_back(i);
		}
		else
		{
			for (size_t i = startIndex; i != -2; i = sblocksIndices_[i]) indices.push_back(i);
		}
	}

	size_t CompoundFile::GetFreeBlockIndex(bool isBig)
		// PURPOSE: Get the index of a new block where data can be stored.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
		// PROMISE: It does not physically create a new block in the compound file. 
		// PROMISE: It only adjust BAT arrays and indices or SBAT arrays and indices so that
		// PROMISE: it gives the index of a new block where data can be inserted.
	{
		size_t index;
		if (isBig)
		{
			// Find first free location
			index = distance(blocksIndices_.begin(),
				find(blocksIndices_.begin(),
				blocksIndices_.end(), -1));
			if (index == blocksIndices_.size())
			{
				ExpandBATArray(true);
				index = distance(blocksIndices_.begin(),
					find(blocksIndices_.begin(),
					blocksIndices_.end(), -1));
			}
			blocksIndices_[index] = -2;
		}
		else
		{
			// Find first free location
			index = distance(sblocksIndices_.begin(),
				find(sblocksIndices_.begin(),
				sblocksIndices_.end(), -1));
			if (index == sblocksIndices_.size())
			{
				ExpandBATArray(false);
				index = distance(sblocksIndices_.begin(),
					find(sblocksIndices_.begin(),
					sblocksIndices_.end(), -1));
			}
			sblocksIndices_[index] = -2;
		}
		return index;
	}

	void CompoundFile::ExpandBATArray(bool isBig)
		// PURPOSE: Create a new block of BAT or SBAT indices.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
	{
		size_t newIndex;
		fill(block_.begin(), block_.end(), -1);

		if (isBig)
		{
			size_t BATindex = distance(&header_.BATArray_[0],
				find(header_.BATArray_,
				header_.BATArray_ + 109, -1));
			if (BATindex < 109)
			{
				// Set new BAT index location
				newIndex = blocksIndices_.size(); // New index location
				file_.Insert(newIndex + 1, &*(block_.begin()));
				IncreaseLocationReferences(vector<size_t>(1, newIndex));

				// Update BAT array
				header_.BATArray_[BATindex] = newIndex;
				++header_.BATCount_;
			}
			else
			{
				// No free BAT indices. Increment using XBAT
				// Set new XBAT index location
				if (header_.XBATCount_ != 0)
				{
					newIndex = header_.XBATStart_ + header_.XBATCount_;
					file_.Insert(newIndex, &*(block_.begin()));
					IncreaseLocationReferences(vector<size_t>(1, newIndex));
				}
				else
				{
					newIndex = blocksIndices_.size();
					file_.Insert(newIndex, &*(block_.begin()));
					IncreaseLocationReferences(vector<size_t>(1, newIndex));
					header_.XBATStart_ = newIndex;
				}
				++header_.XBATCount_;
			}
			blocksIndices_.insert(blocksIndices_.begin() + newIndex, -3);
			blocksIndices_.resize(blocksIndices_.size() + 127, -1);
		}
		else
		{
			// Set new SBAT index location
			if (header_.SBATCount_ != 0)
			{
				newIndex = header_.SBATStart_ + header_.SBATCount_;
				file_.Insert(newIndex, &*(block_.begin()));
				IncreaseLocationReferences(vector<size_t>(1, newIndex));
			}
			else
			{
				newIndex = GetFreeBlockIndex(true);
				file_.Insert(newIndex, &*(block_.begin()));
				IncreaseLocationReferences(vector<size_t>(1, newIndex));
				header_.SBATStart_ = newIndex;
			}
			++header_.SBATCount_;
			sblocksIndices_.resize(sblocksIndices_.size() + 128, -1);
		}
	}

	void CompoundFile::LinkBlocks(size_t from, size_t to, bool isBig)
		// PURPOSE: Link one BAT index to another.
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
	{
		if (isBig) blocksIndices_[from] = to;
		else sblocksIndices_[from] = to;
	}

	void CompoundFile::FreeBlocks(vector<size_t>& indices, bool isBig)
		// PURPOSE: Delete blocks of data from compound file.
		// EXPLAIN: indices contains indices to blocks of data to be deleted. 
		// EXPLAIN: isBig is true if property uses big blocks, false if it uses small blocks.
	{
		if (isBig)
		{
			// Decrease all location references before deleting blocks from file.
			DecreaseLocationReferences(indices);
			size_t maxIndices = indices.size();
			{for (size_t i = 0; i < maxIndices; ++i) ++indices[i]; }	// Increase by 1 because Block index 1 corresponds to index 0 here
			file_.Erase(indices);

			// Shrink BAT indices if necessary
			vector<size_t> indicesToRemove;
			while (distance(find(blocksIndices_.begin(),
				blocksIndices_.end(), -1),
				blocksIndices_.end()) >= 128)
			{
				blocksIndices_.resize(blocksIndices_.size() - 128);
				if (header_.XBATCount_ != 0)
				{
					// Shrink XBAT first
					--header_.XBATCount_;
					indicesToRemove.push_back(header_.XBATStart_ + header_.XBATCount_ + 1); // Add 1 because Block index 1 corresponds to index 0 here
					if (header_.XBATCount_ == 0) header_.XBATStart_ = -2;
				}
				else
				{
					// No XBAT, delete last occupied BAT array element
					size_t BATindex = distance(&header_.BATArray_[0],
						find(header_.BATArray_,
						header_.BATArray_ + 109, -1));
					if (BATindex != 109)
					{
						--header_.BATCount_;
						indicesToRemove.push_back(header_.BATArray_[BATindex - 1] + 1); // Add 1 because Block index 1 corresponds to index 0 here
						header_.BATArray_[BATindex - 1] = -1;
					}
				}
			}
			// Erase extra BAT indices if present
			if (!indicesToRemove.empty()) file_.Erase(indicesToRemove);
		}
		else
		{
			// Erase block
			size_t maxIndices = indices.size();
			size_t maxBlocks = properties_[0]->size_ / header_.bigBlockSize_ +
				(properties_[0]->size_ % header_.bigBlockSize_ ? 1 : 0);
			size_t size = maxBlocks * header_.bigBlockSize_;
			char* data = new char[size];
			ReadData(properties_[0]->startBlock_, data, true);
			size_t maxSmallBlocks = properties_[0]->size_ / header_.smallBlockSize_;
			char* newdata = new char[properties_[0]->size_ - maxIndices*header_.smallBlockSize_];
			{for (size_t i = 0, j = 0; i < maxSmallBlocks; ++i)
			{
				if (find(indices.begin(), indices.end(), i) == indices.end())
				{
					copy(data + i*header_.smallBlockSize_,
						data + i*header_.smallBlockSize_ + header_.smallBlockSize_,
						newdata + j*header_.smallBlockSize_);
					++j;
				}
			}}
			properties_[0]->startBlock_ = WriteData(newdata, properties_[0]->size_ - maxIndices*header_.smallBlockSize_,
				properties_[0]->startBlock_, true);
			properties_[0]->size_ -= maxIndices*header_.smallBlockSize_;
			delete[] data;
			delete[] newdata;

			// Change SBAT indices
			size_t maxSBATindices = sblocksIndices_.size();
			{for (size_t i = 0; i < maxIndices; ++i)
			{
				for (size_t j = 0; j<maxSBATindices; ++j)
				{
					if (j == indices[i]) continue;
					if (sblocksIndices_[j] == indices[i]) sblocksIndices_[j] = sblocksIndices_[indices[i]];
					if (sblocksIndices_[j] > (int) indices[i] &&
						sblocksIndices_[j] != -1 &&
						sblocksIndices_[j] != -2) --sblocksIndices_[j];
				}
			}}
			sort(indices.begin(), indices.end(), greater<size_t>());
			{for (size_t i = 0; i < maxIndices; ++i)
			{
				sblocksIndices_.erase(sblocksIndices_.begin() + indices[i]);
				sblocksIndices_.push_back(-1);
			}}
			vector<size_t> indicesToRemove;
			while (distance(find(sblocksIndices_.begin(),
				sblocksIndices_.end(), -1),
				sblocksIndices_.end()) >= 128)
			{
				// Shrink SBAT indices if necessary
				sblocksIndices_.resize(sblocksIndices_.size() - 128);
				--header_.SBATCount_;
				indicesToRemove.push_back(header_.SBATStart_ + header_.SBATCount_);
				if (header_.SBATCount_ == 0) header_.SBATStart_ = -2;
			}
			FreeBlocks(indicesToRemove, true);
		}
	}

	/*********************** Inaccessible Properties Functions ***************************/
	void CompoundFile::LoadProperties()
		// PURPOSE: Load properties information for compound file.
	{
		// Read properties' data from compound file.
		size_t propertiesSize = DataSize(header_.propertiesStart_, true);
		char* buffer = new char[propertiesSize];
		ReadData(header_.propertiesStart_, buffer, true);

		// Split properties' data into individual property.
		size_t maxPropertiesBlock = propertiesSize / header_.bigBlockSize_;
		size_t propertiesPerBlock = header_.bigBlockSize_ / 128;
		size_t maxProperties = maxPropertiesBlock * propertiesPerBlock;
		size_t maxBlocks = maxProperties / propertiesPerBlock +
			(maxProperties % propertiesPerBlock ? 1 : 0);

		for (size_t i = 0; i < maxBlocks; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				// Read individual property
				Property* property = new Property;
				property->Read(buffer + i * 512 + j * 128);
				if (wcslen(property->name_) == 0)
				{
					delete property;
					break;
				}
				properties_.push_back(property);
			}
		}
		delete[] buffer;

		// Generate property trees
		propertyTrees_->parent_ = 0;
		propertyTrees_->self_ = properties_[0];
		propertyTrees_->index_ = 0;

		InsertPropertyTree(propertyTrees_,
			properties_[properties_[0]->childProp_],
			properties_[0]->childProp_);
	}

	void CompoundFile::SaveProperties()
		// PURPOSE: Save properties information for compound file.
	{
		// Calculate total size required by properties
		size_t maxProperties = properties_.size();
		size_t propertiesPerBlock = header_.bigBlockSize_ / 128;
		size_t maxBlocks = maxProperties / propertiesPerBlock +
			(maxProperties % propertiesPerBlock ? 1 : 0);
		size_t propertiesSize = maxBlocks*header_.bigBlockSize_;
		char* buffer = new char[propertiesSize];
		{for (size_t i = 0; i < propertiesSize; ++i) buffer[i] = 0; }
		{for (size_t i = 0; i < maxProperties; ++i)
		{
			// Save individual property
			properties_[i]->Write(buffer + i * 128);
		}}

		// Write properties' data to compound file.
		WriteData(buffer, propertiesSize, header_.propertiesStart_, true);
		delete[] buffer;
	}

	int CompoundFile::MakeProperty(const wchar_t* path, CompoundFile::Property* property)
		// PURPOSE: Create a new property in the compound file.
		// EXPLAIN: path is the full path name for the property.
		// EXPLAIN: property contains information on the type of property to be created.
	{
		wchar_t* parentpath = 0;
		wchar_t* propertyname = 0;

		// Change to the specified directory. If specified directory is not present,
		// create it.
		if (wcslen(path) != 0)
		{
			if (path[0] == L'\\') currentDirectory_ = propertyTrees_;
		}
		SplitPath(path, parentpath, propertyname);

		if (propertyname != 0)
		{
			if (parentpath != 0)
			{
				if (ChangeDirectory(parentpath) != SUCCESS)
				{
					int ret = MakeDirectory(parentpath);
					if (ret != SUCCESS)
					{
						delete[] parentpath;
						delete[] propertyname;
						return ret;
					}
					else ChangeDirectory(parentpath);
				}
				delete[] parentpath;
			}

			// Insert property into specified directory
			size_t propertynameLength = wcslen(propertyname);
			if (propertynameLength >= 32)
			{
				delete[] propertyname;
				return NAME_TOO_LONG;
			}
			wcscpy_s(property->name_, 32, propertyname);
			delete[] propertyname;
			property->nameSize_ = (short) (propertynameLength * 2 + 2);
			if (FindProperty(currentDirectory_, property->name_) == 0)
			{
				// Find location to insert property
				size_t maxProperties = properties_.size();
				size_t index;
				for (index = 1; index<maxProperties; ++index)
				{
					if (*(properties_[index]) > *property) break;
				}
				if (index != maxProperties)
				{
					// Change references for all properties affected by the new property
					IncreasePropertyReferences(propertyTrees_, index);
				}
				properties_.insert(properties_.begin() + index, property);
				InsertPropertyTree(currentDirectory_, property, index);
				return SUCCESS;
			}
			else return DUPLICATE_PROPERTY;
		}
		else
		{
			if (parentpath != 0) delete[] parentpath;
			return INVALID_PATH;
		}
	}

	CompoundFile::PropertyTree* CompoundFile::FindProperty(size_t index)
		// PURPOSE: Find property in the compound file, given the index of the property.
		// PROMISE: Returns a pointer to the property tree of the property if property
		// PROMISE: is present, 0 if otherwise.
	{
		if (previousDirectories_.empty()) previousDirectories_.push_back(propertyTrees_);
		PropertyTree* currentTree = previousDirectories_.back();
		if (currentTree->index_ != index)
		{
			size_t maxChildren = currentTree->children_.size();
			for (size_t i = 0; i < maxChildren; ++i)
			{
				previousDirectories_.push_back(currentTree->children_[i]);
				PropertyTree* child = FindProperty(index);
				if (child != 0)
				{
					previousDirectories_.pop_back();
					return child;
				}
			}
		}
		else
		{
			previousDirectories_.pop_back();
			return currentTree;
		}
		previousDirectories_.pop_back();
		return 0;
	}

	CompoundFile::PropertyTree* CompoundFile::FindProperty(const wchar_t* path)
		// PURPOSE: Find property in the compound file, given the path of the property.
		// PROMISE: Returns a pointer to the property tree of the property if property
		// PROMISE: is present, 0 if otherwise.
	{
		previousDirectories_.push_back(currentDirectory_);

		// Change to specified directory
		wchar_t* parentpath = 0;
		wchar_t* filename = 0;

		if (wcslen(path) != 0)
		{
			if (path[0] == L'\\') currentDirectory_ = propertyTrees_;
		}

		SplitPath(path, parentpath, filename);
		if (parentpath != 0)
		{
			int ret = ChangeDirectory(parentpath);
			delete[] parentpath;
			if (ret != SUCCESS)
			{
				// Cannot change to specified directory
				if (filename != 0) delete[] filename;
				currentDirectory_ = previousDirectories_.back();
				previousDirectories_.pop_back();
				PropertyTree* property = 0;
				return property;
			}
		}

		// Check to see if file is present in the specified directory.
		PropertyTree* property = 0;
		if (filename != 0)
		{
			property = FindProperty(currentDirectory_, filename);
			delete[] filename;
		}
		currentDirectory_ = previousDirectories_.back();
		previousDirectories_.pop_back();
		return property;
	}

	CompoundFile::PropertyTree*
		CompoundFile::FindProperty(CompoundFile::PropertyTree* parentTree,
		wchar_t* name)
		// PURPOSE: Find property in the compound file, given the parent property tree and its name.
		// PROMISE: Returns a pointer to the property tree of the property if property
		// PROMISE: is present, 0 if otherwise.
	{
			if (parentTree->self_->childProp_ != -1)
			{
				size_t maxChildren = parentTree->children_.size();
				for (size_t i = 0; i < maxChildren; ++i)
				{
					if (wcscmp(parentTree->children_[i]->self_->name_, name) == 0)
					{
						return parentTree->children_[i];
					}
				}
			}
			return 0;
		}

	void CompoundFile::InsertPropertyTree(CompoundFile::PropertyTree* parentTree,
		CompoundFile::Property* property,
		size_t index)
		// PURPOSE: Insert a property and all its siblings and children into the property tree.
		// REQUIRE: If the property is a new property and its index is already occupied by
		// REQUIRE: another property, the calling function has to call IncreasePropertyReferences()
		// REQUIRE: first before calling this function.
		// EXPLAIN: This function is used by LoadProperty() to initialize the property trees
		// EXPLAIN: and MakeProperty() thus resulting in the above requirements.
		// EXPLAIN: parentTree is the parent of the new property.
		// EXPLAIN: property is the property to be added.
		// EXPLAIN: index is the index of the new property.
		// PROMISE: The property will be added as the parent tree's child and the parent's 
		// PROMISE: child property and all the its children previous property and next property 
		// PROMISE: will be readjusted to accomodate the next property.
	{
		PropertyTree* tree = new PropertyTree;
		tree->parent_ = parentTree;
		tree->self_ = property;
		tree->index_ = index;

		if (property->previousProp_ != -1)
		{
			InsertPropertyTree(parentTree,
				properties_[property->previousProp_],
				property->previousProp_);
		}

		if (property->nextProp_ != -1)
		{
			InsertPropertyTree(parentTree,
				properties_[property->nextProp_],
				property->nextProp_);
		}

		if (property->childProp_ != -1)
		{
			InsertPropertyTree(tree,
				properties_[property->childProp_],
				property->childProp_);
		}

		// Sort children
		size_t maxChildren = parentTree->children_.size();
		size_t i;
		for (i = 0; i < maxChildren; ++i)
		{
			if (index < parentTree->children_[i]->index_) break;
		}
		parentTree->children_.insert(parentTree->children_.begin() + i, tree);

		// Update children indices
		UpdateChildrenIndices(parentTree);
	}

	void CompoundFile::DeletePropertyTree(CompoundFile::PropertyTree* tree)
		// PURPOSE: Delete a property from properties.
		// EXPLAIN: tree is the property tree to be deleted.
		// PROMISE: The tree's parent's child property and all the its children previous property 
		// PROMISE: and next property will be readjusted to accomodate the deleted property.
	{
		// Decrease all property references
		DecreasePropertyReferences(propertyTrees_, tree->index_);

		// Remove property
		if (properties_[tree->index_]) delete properties_[tree->index_];
		properties_.erase(properties_.begin() + tree->index_);

		// Remove property from property trees
		size_t maxChildren = tree->parent_->children_.size();
		size_t i;
		for (i = 0; i < maxChildren; ++i)
		{
			if (tree->parent_->children_[i]->index_ == tree->index_) break;
		}
		tree->parent_->children_.erase(tree->parent_->children_.begin() + i);

		// Update children indices
		UpdateChildrenIndices(tree->parent_);
	}

	void CompoundFile::UpdateChildrenIndices(CompoundFile::PropertyTree* parentTree)
	{
		// Update indices for 1st to middle child
		size_t maxChildren = parentTree->children_.size();
		if (maxChildren != 0)
		{
			vector<PropertyTree*>& children = parentTree->children_;
			size_t prevChild = 0;
			children[0]->self_->previousProp_ = -1;
			children[0]->self_->nextProp_ = -1;
			size_t curChild;
			for (curChild = 1; curChild <= maxChildren / 2; ++curChild)
			{
				children[curChild]->self_->previousProp_ = children[prevChild]->index_;
				children[curChild]->self_->nextProp_ = -1;
				prevChild = curChild;
			}

			// Update middle child
			--curChild;
			children[curChild]->parent_->self_->childProp_ = children[curChild]->index_;

			// Update from middle to last child
			size_t nextChild = curChild + 1;
			if (nextChild < maxChildren)
			{
				children[curChild]->self_->nextProp_ = children[nextChild]->index_;
				for (++curChild, ++nextChild;
					nextChild < maxChildren;
					++curChild, ++nextChild)
				{
					children[curChild]->self_->previousProp_ = -1;
					children[curChild]->self_->nextProp_ = children[nextChild]->index_;

				}
				children[curChild]->self_->previousProp_ = -1;
				children[curChild]->self_->nextProp_ = -1;
			}
		}
		else
		{
			parentTree->self_->childProp_ = -1;
		}
	}

	void CompoundFile::IncreasePropertyReferences(CompoundFile::PropertyTree* parentTree,
		size_t index)
		// PURPOSE: Increase all property references (previous property, next property
		// PURPOSE: and child property) which will be affected by the insertion of the new index.
		// EXPLAIN: The recursive method of going through each property tree is used instead of
		// EXPLAIN: using the iterative method of going through each property in properties_ is
		// EXPLAIN: because the index in property tree needs to be updated also.
	{
		if (parentTree->index_ >= index) ++parentTree->index_;
		if (parentTree->self_->previousProp_ != -1)
		{
			if (parentTree->self_->previousProp_ >= (int) index)
			{
				++parentTree->self_->previousProp_;
			}
		}
		if (parentTree->self_->nextProp_ != -1)
		{
			if (parentTree->self_->nextProp_ >= (int)index)
			{
				++parentTree->self_->nextProp_;
			}
		}
		if (parentTree->self_->childProp_ != -1)
		{
			if (parentTree->self_->childProp_ >= (int)index)
			{
				++parentTree->self_->childProp_;
			}
		}

		size_t maxChildren = parentTree->children_.size();
		for (size_t i = 0; i < maxChildren; ++i)
		{
			IncreasePropertyReferences(parentTree->children_[i], index);
		}
	}

	void CompoundFile::DecreasePropertyReferences(CompoundFile::PropertyTree* parentTree, size_t index)
		// PURPOSE: Decrease all property references (previous property, next property
		// PURPOSE: and child property) which will be affected by the deletion of the index.
		// EXPLAIN: The recursive method of going through each property tree is used instead of
		// EXPLAIN: using the iterative method of going through each property in properties_ is
		// EXPLAIN: because the index in property tree needs to be updated also.
	{
		if (parentTree->index_ > index) --parentTree->index_;
		if (parentTree->self_->previousProp_ != -1)
		{
			if (parentTree->self_->previousProp_ > (int)index)
			{
				--parentTree->self_->previousProp_;
			}
		}
		if (parentTree->self_->nextProp_ != -1)
		{
			if (parentTree->self_->nextProp_ > (int)index)
			{
				--parentTree->self_->nextProp_;
			}
		}
		if (parentTree->self_->childProp_ != -1)
		{
			if (parentTree->self_->childProp_ > (int)index)
			{
				--parentTree->self_->childProp_;
			}
		}

		size_t maxChildren = parentTree->children_.size();
		for (size_t i = 0; i < maxChildren; ++i)
		{
			DecreasePropertyReferences(parentTree->children_[i], index);
		}
	}
} // YCompoundFiles namespace end