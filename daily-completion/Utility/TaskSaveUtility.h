#pragma once

class Task;
class CTaskSaveUtility : YFramework::IUtility
{
public:
	CTaskSaveUtility();
	void SaveData(const std::vector<std::shared_ptr<Task>> &inVec);
	void LoadData(std::vector<std::shared_ptr<Task>> &outVec);
	void LoadFixed(std::vector<std::shared_ptr<Task>> &outVec);
	void SaveFixed(const std::vector<std::shared_ptr<Task>> &inVec);
	bool IsDataEmpty() { return _bDataEmpty; }
	void SetDate(int y, int m, int d);
private:
	CString _strDataPath;
	CString _strFixedPath;
	bool _bDataEmpty; // 文件数据为空
};

