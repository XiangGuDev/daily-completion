#pragma once

class Task;
class CTaskSaveUtility : YFramework::IUtility
{
public:
	CTaskSaveUtility();
	void SaveData(const std::vector<std::shared_ptr<Task>> &inVec);
	void LoadData(std::vector<std::shared_ptr<Task>> &outVec);
protected:
	void SaveFixed(const std::vector<std::shared_ptr<Task>> &inVec);
	void LoadFixed(std::vector<std::shared_ptr<Task>> &outVec);
private:
	CXmlDocument _doc;
	CXmlDocument _docFixed;
	CString _strDataPath;
	CString _strFixedPath;
	bool _bDataEmpty; // 文件数据为空
};

