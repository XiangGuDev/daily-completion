//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           BaseControlDefine.H
//    @author         mahaidong;
//    @date           2015/4/9 10:45
//    @brief          标准控件消息、枚举定义;
//*****************************************************

#pragma once

// 选中消息
#define LCN_SELECTED				(WM_USER + 1001)
// 左键单击
#define LCN_LEFTCLICK				(WM_USER + 1002)
// 右键单击
#define LCN_RIGHTCLICK				(WM_USER + 1003)
// 双击事件
#define LCN_DBLCLICK				(WM_USER + 1004)
// 开始编辑事件
#define LCN_BEGINEDIT				(WM_USER + 1005)
// 结束编辑事件
#define LCN_ENDEDIT					(WM_USER + 1006)
// 超链接单击事件
#define LCN_HYPERLINK				(WM_USER + 1007)
// 树控件结束消息(内部使用)
#define LIST_CTRL_END				(WM_USER + 1008)
// 标签控件点击关闭标签
#define TAB_ITEM_CLOSE				(WM_USER + 1009)
// 列表控件鼠标右键
#define LISTBOX_RCLK				(WM_USER + 1314)
// 列表控件鼠标左键双击
#define LISTBOX_LDBCLK				(WM_USER + 1315)
// 列表控件鼠标左键
#define LISTBOX_LCLK				(WM_USER + 1316)
// 完成结束编辑事件
#define LCN_ENDEDITDONE				(WM_USER + 1317)
// 列头点击开始排序消息
#define LCH_COLUMN_BEGINSORT		(WM_USER + 1318)
// 列头点击排序消息
#define LCH_COLUMN_ENDSORT			(WM_USER + 1319)

// 时间轴控件左键单击按下消息
#define  TIMELINE_LBUTTONDOWN		(WM_USER + 1320)
// 时间轴控件左键弹起消息
#define  TIMELINE_LBUTTONUP			(WM_USER + 1321)
// 时间轴控件左键双击消息
#define  TIMELINE_DBLCLICK			(WM_USER + 1322)
// 时间轴控件右键单击击消息
#define  TIMELINE_RIGHTCLICK		(WM_USER + 1323)
// 时间轴控件移动关键帧消息
#define  TIMELINE_MOVECURFRAME		(WM_USER + 1324)

// 确定
#define LCNR_OK						(LRESULT) 0		
// 取消
#define LCNR_CANCEL					(LRESULT) 1	

#define TCS_SHOWCLOSEBTN 0x0001
#define TCS_SHOWADDBTN	 0x0002
#define TCS_ENABLEEDIT	 0x0003

#define TCN_DELETEITEM (TCN_FIRST - 10)
#define TCN_ENDEDITITEM (TCN_FIRST - 11)

// 选中状态被改变的事件
#define TCN_CHECKSTATUS_CHANGE (TCN_FIRST - 30)

/**
*  @class    树列表控件编辑标志位
*
*  @brief  给以下枚举类型增加枚举值时请在BaseControl中的TreeListCfg.cpp中给枚举值添加标志位
*/
enum ETreeListEditFlag
{
	eDiffColText = 1 << 9,			///< 多颜色文本显示
	eNoneEdit = 1 << 10,			///< 无编辑			1024
	eNormalEdit = 1 << 11,			///< 普通编辑			2048
	eValidEdit = 1 << 12,			///< 检验编辑			4096
	eFileNameEdit = 1 << 14,		///< 文件名编辑		16384
	eDateTimeEdit = 1 << 15,		///< 日期编辑框		32768
	eDropListCobEdit = 1 << 16,		///< 下拉选择框		65536
	eDropDownCobEdit = 1 << 17,		///< 可编辑的下拉选择框  131072
	eCheckBoxEdit = 1 << 18,		///< 复选框			262144
	eCheckBox3StateEdit = 1 << 19,	///< 三态复选框		524288
	eHyperLinkEdit = 1 << 20,		///< 超链接			1048576
	eAngleEdit = 1 << 21,			///< 角度			2097152
	eTreeComboEdit = 1 << 22,		///< 树下拉框			4194304
	eProgressEdit = 1 << 23,		///< 进度条			8388608
	eTreeCheckComboEdit = 1 << 24,	///< 下拉树多选框		16777216
	eRangelEdit = 1 << 25,			///< 范围编辑控件		
	eAutoSearchCombo = 1 << 26,		///< 自动搜索组合框	
	eTreeComboManualEdit = 1 << 27,		///< 可编辑的树下拉框
	eTreeCheckComboManualEdit = 1 << 28,		///< 可编辑的树下拉多选框
};

/**
*  @class    校验编辑标志位;
*
*  @brief
*/
enum EValidEditControlFlag
{
	eVeDecimal = 1,			///< 允许输入小数
	eVeNegative = 1 << 1,	///< 允许输入负数
	eVeDotNum = 1 << 2,		///< 限制输入小数位数
	eVeNoZero = 1 << 3,		///< 禁止输入0	！CValidEdit未实现！,在模态对话框中使用CValidEditEx可实现
	eVeNoEmpty = 1 << 4,	///< 不允许为空	！CValidEdit未实现！,在模态对话框中使用CValidEditEx可实现
	eVePreZero = 1 << 5,	///< 允许前置零
	eVeNotPositive = 1 << 6,///< 禁止输入正数
	eVeDigitLimit = 1 << 7,	///< 限制输入位数
	eVeLimitLength = 1 << 8,///< 限制输入长度
	eVeLimitRange = 1 << 9, ///< 限制输入范围
};

/**
*  @class    日期格式;
*
*  @brief
*/
enum EDateTimeEditFlag
{
	eDateSlashYMD = 1,			///< 斜杠年月日 2016/1/26
	eDateChineseYMD = 1 << 1,	///< 中文年月日 2016年1月26日
	eDateDashYMD = 1 << 2,		///< 破折年月日 2016-1-26
	eDateYear = 1 << 3,			///< 年 2017
};

enum ETreeComboEditControlFlag
{
	eTreeHasSelAll = 1,			///< 自动添加全选
	eTreeHasNoLimit = 1 << 1,	///< 自动添加不限
	eTreeAutoWidth = 1 << 2,	///< 自动调整下拉宽度
	eTreeSelNotByText = 1 << 3,    ///< 选择是否不通过界面值
};

/**
*  @class    进度条标志位;
*
*  @brief
*/
enum EProgressControlFlag
{
	ePrgShowText = 1,			///< 显示进度文字
};

#define LIST_HEAD_HEIGHT	28	///< 树形控件列头高
#define LIST_ROW_HEIGHT		26	///< 树形控件行高


struct TabInfo
{
	CString tabName;
	CWnd *pChildren;
	CRect rectSize;
	CRect closeBtPos;
	CRect iconPos;
	CRect txtPos;
	bool bSelected;
	bool bCloseBtnHightLight;
};
typedef std::vector<TabInfo> TabInfoList;

struct TabNotify : public NMHDR
{
	TabInfo tabInfo;
};

struct ComboCheckBoxNotify : public NMHDR
{
	int triggleItemIndex;	// 当前事件触发时，鼠标所处的item
};
