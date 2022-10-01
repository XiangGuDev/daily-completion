#pragma once

class Task;
class CTaskSaveUtility : YFramework::IUtility
{
public:
	CTaskSaveUtility();
	void SaveData(const std::vector<std::shared_ptr<Task>> &inVec);
	void LoadData(std::vector<std::shared_ptr<Task>> &outVec);
private:
	CXmlDocument _doc;
	CString _strDataPath;
};

