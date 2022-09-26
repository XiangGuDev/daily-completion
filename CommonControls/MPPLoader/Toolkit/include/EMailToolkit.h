#pragma once

namespace BwProj { namespace toolkit {

class CEMailToolkit
{
public:
	// 调用本地邮件程序发送附件
	static bool SendFileToEmail(LPCTSTR filePath);
};

// end namespace
} }