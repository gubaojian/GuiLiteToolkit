
// GuiLiteToolkitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GuiLiteToolkit.h"
#include "GuiLiteToolkitDlg.h"
#include "afxdialogex.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CFontLattice::CFontLattice()
{
	utf_8_code = width = 0;
	p_data = 0;
}

CFontLattice::~CFontLattice()
{
	if (p_data)
	{
		free(p_data);
	}
}

// CGuiLiteToolkitDlg dialog
static wchar_t default_input[] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789(_+=./-\:;@%!#',?)<>";

CGuiLiteToolkitDlg::CGuiLiteToolkitDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GUILITETOOLKIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CGuiLiteToolkitDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGuiLiteToolkitDlg::OnBnClickedFont)
	ON_BN_CLICKED(IDC_BUTTON2, &CGuiLiteToolkitDlg::OnBnClickedGenerate)
END_MESSAGE_MAP()


// CGuiLiteToolkitDlg message handlers

BOOL CGuiLiteToolkitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CWnd* p_edit = GetDlgItem(IDC_EDIT1);
	p_edit->SetWindowTextW(default_input);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGuiLiteToolkitDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGuiLiteToolkitDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGuiLiteToolkitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGuiLiteToolkitDlg::OnBnClickedFont()
{
	// TODO: Add your control notification handler code here
	memset(mInput, 0, sizeof(mInput));
	GetDlgItemText(IDC_EDIT1, mInput, sizeof(mInput) / sizeof(wchar_t));
	if (0 == wcslen(mInput))
	{
		MessageBox(L"Input characters✍", L"o(╯□╰)o");
		return;
	}

	CWnd* generate_bt = GetDlgItem(IDC_BUTTON2);
	CWnd* font_bt = GetDlgItem(IDC_BUTTON1);
	generate_bt->EnableWindow(false);
	font_bt->EnableWindow(false);

	LOGFONT logFont;
	CFontDialog fdlg;
	if (IDOK == fdlg.DoModal())
	{
		fdlg.GetCurrentFont(&logFont);
		if (0 != wcslen(logFont.lfFaceName))
		{
			mCurrentFontLatticeMap.clear();
			mCurrentFontName.empty();
			mCurrentFontHeight = 0;
			GetStringInfo(mInput, logFont);
		}
	}
	generate_bt->EnableWindow(true);
	font_bt->EnableWindow(true);
}

int CGuiLiteToolkitDlg::GetStringInfo(wchar_t* str, LOGFONT& logFont)
{
	int length = wcslen(str);
	for (int i = 0; i < length; i++)
	{
		GetCharInfo(str[i], logFont);
	}
	return 0;
}

int CGuiLiteToolkitDlg::GetCharInfo(wchar_t character, LOGFONT& logFont)
{
	wchar_t tmp[2] = { 0, 0 };
	tmp[0] = character;

	CClientDC dc(this);
	CFont font;
	font.CreateFontIndirectW(&logFont);
	dc.SelectObject(&font);

	dc.FillRect(&CRect{0, 0, mCurrentFontHeight, mCurrentFontHeight }, &CBrush(RGB(255, 255, 255)));
	dc.TextOut(0, 0, tmp, wcslen(tmp));

	SIZE size;
	GetTextExtentPoint(dc, tmp, 1, &size);
	unsigned char* pixel_buffer = (unsigned char*)malloc(size.cx * size.cy);
	if (NULL == pixel_buffer)
	{
		return -1;
	}

	for (size_t y = 0; y < size.cy; y++)
	{
		for (size_t x = 0; x < size.cx; x++)
		{
			int value = 0xff - (GetPixel(dc, x, y) & 0xff);
			pixel_buffer[y * size.cx + x] = value;
		}
	}

	unsigned char utf8_buffer[4];
	memset(utf8_buffer, 0, sizeof(utf8_buffer));
	int len = WideCharToMultiByte(CP_UTF8, 0, tmp, -1, (char*)utf8_buffer, sizeof(utf8_buffer), NULL, NULL);

	unsigned int utf8_code = 0;
	switch (len - 1)
	{
	case 1:
		utf8_code = utf8_buffer[0];
		break;
	case 2:
		utf8_code = (utf8_buffer[0] << 8) | (utf8_buffer[1]);
		break;
	case 3:
		utf8_code = (utf8_buffer[0] << 16) | (utf8_buffer[1] << 8) | utf8_buffer[2];
		break;
	case 4:
		utf8_code = (utf8_buffer[0] << 24) | (utf8_buffer[1] << 16) | (utf8_buffer[2] << 8) | utf8_buffer[3];
		break;
	default:
		MessageBox(L"GuiLite could not support unicode over 4 bytes", L"✖✖✖");
		return -1;
	}

	mCurrentFontLatticeMap[utf8_code].utf_8_code = utf8_code;
	mCurrentFontLatticeMap[utf8_code].width = size.cx;
	mCurrentFontLatticeMap[utf8_code].p_data = pixel_buffer;
	
	char name[64];
	sprintf_s(name, "%ws_%d", logFont.lfFaceName, size.cy);
	mCurrentFontName = name;
	mCurrentFontName += (logFont.lfWeight > 400) ? "B" : "";

	std::replace(mCurrentFontName.begin(), mCurrentFontName.end(), '@', '_');		//replace '@' with '_'
	std::replace(mCurrentFontName.begin(), mCurrentFontName.end(), ' ', '_');		//replace ' ' with '_'
	
	mCurrentFontHeight = size.cy;
	TRACE(_T("font name = %s, utf8 = 0x%x, width = %d, height = %d\n"), logFont.lfFaceName, utf8_code, size.cx, size.cy);
	return 0;
}

char* include = "#include \"../core_include/resource.h\"\n\n";
void CGuiLiteToolkitDlg::OnBnClickedGenerate()
{
	// TODO: Add your control notification handler code here
	if (0 == mCurrentFontLatticeMap.size())
	{
		return;
	}

	CWnd* generate_bt = GetDlgItem(IDC_BUTTON2);
	generate_bt->EnableWindow(false);

	char font_major_name[LF_FACESIZE];
	memset(font_major_name, 0, sizeof(font_major_name));
	
	std::fstream file(std::string("./") + mCurrentFontName + ".cpp", std::ios::trunc | std::ios::out | std::ios::binary);
	file.write(include, strlen(include));
	WriteLatticeDataInCppFile(file);
	file.close();

	generate_bt->EnableWindow(true);
	MessageBox(L"Generate lattice cpp file success✌", L"\\^o^/");
}

int CGuiLiteToolkitDlg::WriteLatticeDataInCppFile(std::fstream& file)
{
	if (0 == mCurrentFontLatticeMap.size())
	{
		return -1;
	}

	for (auto& elem : mCurrentFontLatticeMap)
	{
		std::string data_type = "static const unsigned char ";
		std::string data_name = "_" + (std::to_string(elem.second.utf_8_code));

		std::string define_lattice_data = data_type + data_name + "[] = {\n";
		for (int y = 0; y < mCurrentFontHeight; y++)
		{
			define_lattice_data += "    ";
			for (int x = 0; x < elem.second.width; x++)
			{
				define_lattice_data += std::to_string(elem.second.p_data[y * elem.second.width + x]);
				define_lattice_data += ", ";
			}
			define_lattice_data += "\n";
		}
		define_lattice_data += "};\n";
		file.write(define_lattice_data.c_str(), define_lattice_data.length());
	}

	std::string define_lattice_array = "static  LATTICE lattice_array[] = {\n";
	for (auto& elem : mCurrentFontLatticeMap)
	{
		define_lattice_array += "        {" + std::to_string(elem.second.utf_8_code) + ", " +
			std::to_string(elem.second.width) + ", " +
			"_" + (std::to_string(elem.second.utf_8_code)) + "},\n";
	}
	define_lattice_array += "};\n";
	file.write(define_lattice_array.c_str(), define_lattice_array.length());

	std::string data_type = "const FONT_INFO ";
	std::string define_font_info = "extern " + data_type + mCurrentFontName + ";\n";
	define_font_info += data_type + mCurrentFontName + " ={\n";
	define_font_info += "    " + std::to_string(mCurrentFontHeight) + ",\n";
	define_font_info += "    " + std::to_string(mCurrentFontLatticeMap.size()) + ",\n";
	define_font_info += "    lattice_array\n};\n";
	
	file.write(define_font_info.c_str(), define_font_info.length());
	return 0;
}
