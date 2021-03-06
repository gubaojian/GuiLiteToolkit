
// GuiLiteToolkitDlg.h : header file
//
#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <vector> 

#pragma once
class CFontLattice
{
public:
	CFontLattice();
	~CFontLattice();

	unsigned int	utf_8_code;
	unsigned char	width;
	unsigned char*	p_data;
};

// CGuiLiteToolkitDlg dialog
class CGuiLiteToolkitDlg : public CDialogEx
{
// Construction
public:
	CGuiLiteToolkitDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GUILITETOOLKIT_DIALOG };
#endif
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFont();
	afx_msg void OnBnClickedGenerate();
	int GetCharInfo(wchar_t character, LOGFONT& logFont);
	int GetStringInfo(wchar_t* str, LOGFONT& logFont);
private:
	int WriteLatticeDataInCppFile(std::fstream& file);
	std::map<unsigned int, CFontLattice> mCurrentFontLatticeMap;
	std::string mCurrentFontName;
	int mCurrentFontHeight;
	wchar_t mInput[1024];
};
