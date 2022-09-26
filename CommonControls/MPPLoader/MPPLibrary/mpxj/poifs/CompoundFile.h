
#ifndef BASICEXCEL_HPP
#define BASICEXCEL_HPP

#include <atldef.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <windows.h>

using namespace std;

#define UTF16
#ifdef UTF16
#define SIZEOFWCHAR_T 2
#else
#define SIZEOFWCHAR_T sizeof(wchar_t)
#endif

namespace CompoundFiles
{
	// 文件数据块操作
	class Block
	{
	public:
		Block(void);

		// 文件句柄操作方法
		bool Create(const wchar_t* filename);
		bool Open(const wchar_t* filename, ios_base::openmode mode = ios_base::in | ios_base::out);
		bool Close(void);
		bool IsOpen(void);

		// 数据块操作方法
		bool Read(size_t index, char* block);
		bool Write(size_t index, const char* block);
		bool Swap(size_t index1, size_t index2);
		bool Move(size_t from, size_t to);
		bool Insert(size_t index, const char* block);
		bool Erase(size_t index);
		bool Erase(vector<size_t>& indices);

		// 取得／设置数据块
		size_t GetBlockSize(void) const { return blockSize_; }
		void SetBlockSize(size_t size)
		{
			blockSize_ = size;
			indexEnd_ = fileSize_ / blockSize_ + (fileSize_ % blockSize_ ? 1 : 0);
		}

	protected:
		wstring				filename_;		// 打开的文件名
		ios_base::openmode	mode_;			// 打开文件模式
		fstream				file_;			// 文件流
		size_t				blockSize_;		// 数据块大小
		size_t				indexEnd_;		// 文件结束索引
		size_t				fileSize_;		// 文件大小
	};

	// 文件数据块读／写
	struct LittleEndian
	{
		template<typename Type>
		static void Read(const char* buffer, Type& retVal, int pos = 0, int bytes = 0)
		{
			retVal = Type(0);
			if (bytes == 0)
			{
				bytes = sizeof(Type);
			}

			for (int i = 0; i < bytes; ++i)
			{
				retVal |= ((Type)((unsigned char)buffer[pos + i])) << 8 * i;
			}
		}

		template<typename Type>
		static void ReadString(const char* buffer, Type* str, int pos = 0, int bytes = 0)
		{
			for (size_t i = 0; i < bytes; ++i)
			{
				Read(buffer, str[i], pos + i*sizeof(Type));
			}
		}

		template<typename Type>
		static void Write(char* buffer, Type val, int pos = 0, int bytes = 0)
		{
			if (bytes == 0)
			{
				bytes = sizeof(Type);
			}

			for (int i = 0; i < bytes; ++i)
			{
				buffer[pos + i] = (unsigned char)val;
				val >>= 8;
			}
		}

		template<typename Type>
		static void WriteString(char* buffer, Type* str, int pos = 0, int bytes = 0)
		{
			for (size_t i = 0; i < bytes; ++i)
			{
				Write(buffer, str[i], pos + i*sizeof(Type));
			}
		}

		template<typename Type>
		static void Read(const vector<char>& buffer, Type& retVal, int pos = 0, int bytes = 0)
		{
			retVal = Type(0);
			if (bytes == 0)
			{
				bytes = sizeof(Type);
			}

			for (size_t i = 0; i < bytes; ++i)
			{
				retVal |= ((Type)((unsigned char)buffer[pos + i])) << 8 * i;
			}
		}

		template<typename Type>
		static void ReadString(const vector<char>& buffer, Type* str, int pos = 0, int bytes = 0)
		{
			for (size_t i = 0; i < bytes; ++i)
			{
				Read(buffer, str[i], pos + i*sizeof(Type));
			}
		}

		template<typename Type>
		static void Write(vector<char>& buffer, Type val, int pos = 0, int bytes = 0)
		{
			if (bytes == 0)
			{
				bytes = sizeof(Type);
			}

			for (size_t i = 0; i < bytes; ++i)
			{
				buffer[pos + i] = (unsigned char)val;
				val >>= 8;
			}
		}

		template<typename Type>
		static void WriteString(vector<char>& buffer, Type* str, int pos = 0, int bytes = 0)
		{
			for (size_t i = 0; i < bytes; ++i)
			{
				Write(buffer, str[i], pos + i*sizeof(Type));
			}
		}


		static void Read(const char* buffer, wchar_t& retVal, int pos = 0, int bytes = 0)
		{
			retVal = wchar_t(0);
			if (bytes == 0)
			{
				bytes = SIZEOFWCHAR_T;
			}

			for (int i = 0; i < bytes; ++i)
			{
				retVal |= ((wchar_t)((unsigned char)buffer[pos + i])) << 8 * i;
			}
		}

		static void ReadString(const char* buffer, wchar_t* str, int pos = 0, int bytes = 0)
		{
			for (int i = 0; i < bytes; ++i)
			{
				Read(buffer, str[i], pos + i*SIZEOFWCHAR_T);
			}
		}

		static void Write(char* buffer, wchar_t val, int pos = 0, int bytes = 0)
		{
			if (bytes == 0)
			{
				bytes = SIZEOFWCHAR_T;
			}

			for (int i = 0; i < bytes; ++i)
			{
				buffer[pos + i] = (unsigned char)val;
				val >>= 8;
			}
		}

		static void WriteString(char* buffer, wchar_t* str, int pos = 0, int bytes = 0)
		{
			for (int i = 0; i < bytes; ++i)
			{
				Write(buffer, str[i], pos + i*SIZEOFWCHAR_T);
			}
		}

		static void Read(const vector<char>& buffer, wchar_t& retVal, int pos = 0, int bytes = 0)
		{
			retVal = wchar_t(0);
			if (bytes == 0)
			{
				bytes = SIZEOFWCHAR_T;
			}

			for (int i = 0; i < bytes; ++i)
			{
				retVal |= ((wchar_t)((unsigned char)buffer[pos + i])) << 8 * i;
			}
		}

		static void ReadString(const vector<char>& buffer, wchar_t* str, int pos = 0, int bytes = 0)
		{
			for (int i = 0; i < bytes; ++i)
			{
				Read(buffer, str[i], pos + i*SIZEOFWCHAR_T);
			}
		}

		static void Write(vector<char>& buffer, wchar_t val, int pos = 0, int bytes = 0)
		{
			if (bytes == 0)
			{
				bytes = SIZEOFWCHAR_T;
			}

			for (int i = 0; i < bytes; ++i)
			{
				buffer[pos + i] = (unsigned char)val;
				val >>= 8;
			}
		}

		static void WriteString(vector<char>& buffer, wchar_t* str, int pos = 0, int bytes = 0)
		{
			for (int i = 0; i < bytes; ++i)
			{
				Write(buffer, str[i], pos + i*SIZEOFWCHAR_T);
			}
		}
	};

	class CompoundFile
	{
	public:
		enum
		{
			DUPLICATE_PROPERTY = -6,
			NAME_TOO_LONG = -5,
			FILE_NOT_FOUND = -4,
			DIRECTORY_NOT_EMPTY = -3,
			DIRECTORY_NOT_FOUND = -2,
			INVALID_PATH = -1,
			SUCCESS = 1
		};

		CompoundFile(void);
		~CompoundFile(void);

		// User accessible functions
	public:
		// Compound File functions
		bool Create(const wchar_t* filename);
		bool Open(const wchar_t* filename, ios_base::openmode mode = ios_base::in | ios_base::out);
		bool Close(void);
		bool IsOpen(void);

		// Directory functions
		int ChangeDirectory(const wchar_t* path);
		int MakeDirectory(const wchar_t* path);
		int PresentWorkingDirectory(wchar_t* path);
		int PresentWorkingDirectory(vector<wchar_t>& path);
		int RemoveDirectory(const wchar_t* path);
		int DelTree(const wchar_t* path);
		int DirectoryList(vector<vector<wchar_t> >& list, const wchar_t* path = 0);

		// File functions
		int MakeFile(const wchar_t* path);
		int RemoveFile(const wchar_t* path);
		int FileSize(const wchar_t* path, size_t& size);
		int ReadFile(const wchar_t* path, char* data);
		int ReadFile(const wchar_t* path, vector<char>&data);
		int WriteFile(const wchar_t* path, const char* data, size_t size);
		int WriteFile(const wchar_t* path, const vector<char>&data, size_t size);

		// Protected functions and data members
	protected:
		// General functions and data members
		void IncreaseLocationReferences(vector<size_t> indices);
		void DecreaseLocationReferences(vector<size_t> indices);
		void SplitPath(const wchar_t* path, wchar_t*& parentpath, wchar_t*& propertyname);
		vector<char>	block_;
		Block			file_;

		// Header related functions and data members
		bool LoadHeader(void);
		void SaveHeader(void);
		class Header
		{
		public:
			Header(void);
			void Write(char* block);
			void Read(char* block);

			long long fileType_;		// Magic number identifying this as a compound file system (0x0000)
			int uk1_;					// Unknown constant (0x0008)
			int uk2_;					// Unknown constant (0x000C)
			int uk3_;					// Unknown constant (0x0010)
			int uk4_;					// Unknown constant (0x0014)
			short uk5_;					// Unknown constant (revision?) (0x0018)
			short uk6_;					// Unknown constant	(version?) (0x001A)
			short uk7_;					// Unknown constant (0x001C)
			short log2BigBlockSize_;	// Log, base 2, of the big block size (0x001E)
			int log2SmallBlockSize_;	// Log, base 2, of the small block size (0x0020)
			int uk8_;					// Unknown constant (0x0024)
			int uk9_;					// Unknown constant (0x0028)
			int BATCount_;				// Number of elements in the BAT array (0x002C)
			int propertiesStart_;		// Block index of the first block of the property table (0x0030)
			int uk10_;					// Unknown constant (0x0034)
			int uk11_;					// Unknown constant (0x0038)
			int SBATStart_;				// Block index of first big block containing the small block allocation table (SBAT) (0x003C)
			int SBATCount_;				// Number of big blocks holding the SBAT (0x0040)
			int XBATStart_;				// Block index of the first block in the Extended Block Allocation Table (XBAT) (0x0044)
			int XBATCount_;				// Number of elements in the Extended Block Allocation Table (to be added to the BAT) (0x0048)
			int BATArray_[109];			// Array of block indices constituting the Block Allocation Table (BAT) (0x004C, 0x0050, 0x0054 ... 0x01FC)

			size_t bigBlockSize_;
			size_t smallBlockSize_;

		private:
			void Initialize(void);
		};

		Header header_;

		// BAT related functions and data members
		void LoadBAT(void);
		void SaveBAT(void);
		size_t DataSize(size_t startIndex, bool isBig);
		size_t ReadData(size_t startIndex, char* data, bool isBig);
		size_t WriteData(const char* data, size_t size, int startIndex, bool isBig);
		void GetBlockIndices(size_t startIndex, vector<size_t>& indices, bool isBig);
		size_t GetFreeBlockIndex(bool isBig);
		void ExpandBATArray(bool isBig);
		void LinkBlocks(size_t from, size_t to, bool isBig);
		void FreeBlocks(vector<size_t>& indices, bool isBig);

		vector<int> blocksIndices_;
		vector<int> sblocksIndices_;

		// Properties related functions and data members
		class Property
		{
		public:
			Property(void);
			void Write(char* block);
			void Read(char* block);

			friend bool operator==(const CompoundFile::Property& lhs, const CompoundFile::Property& rhs)
			{
				return (!wcscmp(lhs.name_, rhs.name_));
			}

			friend bool operator< (const CompoundFile::Property& lhs, const CompoundFile::Property& rhs)
			{
				size_t maxLen1 = wcslen(lhs.name_);
				size_t maxLen2 = wcslen(rhs.name_);
				if (maxLen1 < maxLen2)
				{
					return true;
				}
				else if (maxLen1 > maxLen2)
				{
					return false;
				}
				else
				{
					int result = wcscmp(lhs.name_, rhs.name_);
					if (result <= 0)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}

			friend bool operator!=(const CompoundFile::Property& lhs, const CompoundFile::Property& rhs) { return !(lhs == rhs); }
			friend bool operator> (const CompoundFile::Property& lhs, const CompoundFile::Property& rhs) { return (rhs < lhs); }
			friend bool operator<=(const CompoundFile::Property& lhs, const CompoundFile::Property& rhs) { return !(rhs < lhs); }
			friend bool operator>=(const CompoundFile::Property& lhs, const CompoundFile::Property& rhs) { return !(lhs < rhs); }

			wchar_t name_[32];				// A unicode null-terminated uncompressed 16bit string (lblocke the high bytes) containing the name of the property. (0x00, 0x02, 0x04, ... 0x3E)
			short nameSize_;				// Number of characters in the NAME field (0x40)
			unsigned char propertyType_;	// Property type (directory, file, or root) Byte 1 (directory), 2 (file), or 5 (root entry) (0x42)
			unsigned char nodeColor_;		// Node color (0x43)
			int previousProp_;				// Previous property index (0x44)
			int nextProp_;					// Next property index (0x48)
			int childProp_;					// First child property index (0x4c)
			int uk1_;
			int uk2_;
			int uk3_;
			int uk4_;
			int uk5_;
			int seconds1_;					// Seconds component of the created timestamp? (0x64)
			int days1_;						// Days component of the created timestamp? (0x68)
			int seconds2_;					// Seconds component of the modified timestamp? (0x6C)
			int days2_;						// Days component of the modified timestamp? (0x70)
			int startBlock_;				// Starting block of the file, used as the first block in the file and the pointer to the next block from the BAT (0x74)
			int size_;						// Actual size of the file this property points to. (used to truncate the blocks to the real size). (0x78)
		};

		class PropertyTree
		{
		public:
			PropertyTree(void);
			~PropertyTree(void);
			PropertyTree* parent_;
			Property* self_;
			size_t index_;
			vector<PropertyTree*> children_;
		};

		void LoadProperties(void);
		void SaveProperties(void);
		int MakeProperty(const wchar_t* path, Property* property);
		PropertyTree* FindProperty(size_t index);
		PropertyTree* FindProperty(const wchar_t* path);
		PropertyTree* FindProperty(PropertyTree* parentTree, wchar_t* name);
		void InsertPropertyTree(PropertyTree* parentTree, Property* property, size_t index);
		void DeletePropertyTree(PropertyTree* tree);
		void UpdateChildrenIndices(PropertyTree* parentTree);
		void IncreasePropertyReferences(PropertyTree* parentTree, size_t index);
		void DecreasePropertyReferences(PropertyTree* parentTree, size_t index);
		PropertyTree* propertyTrees_;
		PropertyTree* currentDirectory_;
		vector<Property*> properties_;
		vector<PropertyTree*> previousDirectories_;
	};
} // YCompoundFiles namespace end

#endif
