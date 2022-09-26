#pragma once
//#include "ToolkitExportDef.h"
namespace BwProj {
	namespace toolkit {

		/** @addtogroup Toolkit
		* @{
		*/

		//! 文件锁（防止其他用户对文件进行处理）
		class COMMONTOOLS_API CFileMonopolize
		{
		public:
			CFileMonopolize();
			~CFileMonopolize();

			/**
			* 重置
			* @param [in] vFilenames		文件
			*/
			bool Reset(CString strFilenames);

			/**
			* 关闭
			*/
			void Release(void);

			/**
			* 检查文件是否可写，用于打开前的检查
			*/
			static bool IsReadable(LPCTSTR szFileName);

			/**
			* 检查文件是否可写，用于保存前的检查
			*/
			static bool IsWritable(LPCTSTR szFileName);
		protected:
			/** 文件句柄 */
			HANDLE _handle;
		};

		//! 文件写入锁辅助类
		class COMMONTOOLS_API CFileMonopolizeHelper
		{
		public:
			CFileMonopolizeHelper(LPCTSTR szFileName, bool bAnyFile = false);
			~CFileMonopolizeHelper();

		private:
			HANDLE _handle;
		};
		/** * @} */
	}
}
