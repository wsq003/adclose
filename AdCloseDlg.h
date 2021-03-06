
// AdCloseDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CAdCloseDlg 对话框
class CAdCloseDlg : public CDialog
{
// 构造
public:
	CAdCloseDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ADCLOSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWork();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnAcNid(WPARAM wp, LPARAM lp);

	void ToTray();
	afx_msg void OnTrayClose();
	afx_msg void OnClose();
	CListCtrl m_list;
	afx_msg void OnBnClickedViewpic();
	void SaveSysPics();
};
