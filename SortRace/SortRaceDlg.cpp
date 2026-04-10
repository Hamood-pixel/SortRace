// SortRaceDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SortRace.h"
#include "SortRaceDlg.h"
#include "afxdialogex.h"

// Custom Includes for Project Logic
#include "SortLogic.h"
#include "SortCanvas.h"
#include <thread>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSortRaceDlg dialog

// Static member to hold the dark background brush (used by OnCtlColor)
static CBrush darkBackgroundBrush(RGB(50, 50, 50));

CSortRaceDlg::CSortRaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SORTRACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSortRaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// DDX CONTROLS: Link UI Elements to Class Members
	DDX_Control(pDX, IDC_SEQ_CANVAS, m_seqCanvas);
	DDX_Control(pDX, IDC_PAR_CANVAS, m_parCanvas);
	DDX_Control(pDX, IDC_SEQ_TIME_EDIT, m_seqTimeEdit);
	DDX_Control(pDX, IDC_PAR_TIME_EDIT, m_parTimeEdit);

	// CRITICAL: Link Array Size variable to the Edit Box (IDC_ARRAY_SIZE_EDIT)
	DDX_Text(pDX, IDC_ARRAY_SIZE_EDIT, m_iArraySize);
}

BEGIN_MESSAGE_MAP(CSortRaceDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR() // Dark Theme Handler

	ON_BN_CLICKED(IDC_START_RACE_BUTTON, &CSortRaceDlg::OnBnClickedStartRace)
	ON_MESSAGE(WM_RACE_FINISHED, OnRaceFinished)
END_MESSAGE_MAP()


// CSortRaceDlg message handlers

BOOL CSortRaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// Display the initial default array size (100000) in the Edit Box
	UpdateData(FALSE);

	// Bind the data structures to the canvas controls
	m_seqData.pCanvasWnd = &m_seqCanvas;
	m_parData.pCanvasWnd = &m_parCanvas;
	m_seqCanvas.m_pSortData = &m_seqData;
	m_parCanvas.m_pSortData = &m_parData;

	return TRUE;
}

// Dark Theme Color Handler Implementation
HBRUSH CSortRaceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// Don't modify the canvases, their background is handled in OnPaint()
		if (pWnd->GetDlgCtrlID() == IDC_SEQ_CANVAS || pWnd->GetDlgCtrlID() == IDC_PAR_CANVAS)
		{
			pDC->SetBkMode(TRANSPARENT);
			return darkBackgroundBrush;
		}
		else
		{
			// Set all other labels (titles, time labels) to white text on dark background
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkMode(TRANSPARENT);
			return darkBackgroundBrush;
		}
	}
	// Handle the main dialog background
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		return darkBackgroundBrush;
	}

	// Handle Edit Controls (IDC_ARRAY_SIZE_EDIT, IDC_SEQ_TIME_EDIT, IDC_PAR_TIME_EDIT)
	else if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(RGB(255, 255, 255)); // White Text
		pDC->SetBkColor(RGB(80, 80, 80)); // Slightly Lighter Dark Gray for the box background
		return darkBackgroundBrush;
	}

	return hbr;
}

void CSortRaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CSortRaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// --- Custom Execution Logic Implementations ---

const int VISUALIZATION_CUTOFF = 10000;

void CSortRaceDlg::OnBnClickedStartRace()
{
	UpdateData(TRUE);

	// Safety check for empty or huge input
	if (m_iArraySize <= 1 || m_iArraySize > 1000000) {
		AfxMessageBox(_T("Please enter an array size between 2 and 1,000,000."));
		return;
	}

	// --- 1. Prepare Main Data Structures (m_seqData, m_parData) ---
	std::vector<int> original_data = GenerateRandomArray(m_iArraySize);

	m_seqData.arr = original_data;
	m_parData.arr = original_data;
	m_seqData.active_index1 = m_seqData.active_index2 = -1;
	m_parData.active_index1 = m_parData.active_index2 = -1;

	// --- 2. Set Visualization Mode on Main Data Structures ---
	bool visualization_enabled = m_iArraySize < VISUALIZATION_CUTOFF;

	// We use the main canvas pointers if visualization is enabled.
	// If not, we set the pointers to nullptr *temporarily* for the race.
	if (visualization_enabled) {
		m_seqData.pCanvasWnd = &m_seqCanvas;
		m_parData.pCanvasWnd = &m_parCanvas;
		m_seqTimeEdit.SetWindowText(_T("Running..."));
		m_parTimeEdit.SetWindowText(_T("Running..."));
	}
	else {
		m_seqData.pCanvasWnd = nullptr;
		m_parData.pCanvasWnd = nullptr;
		m_seqTimeEdit.SetWindowText(_T("Benchmarking..."));
		m_parTimeEdit.SetWindowText(_T("Benchmarking..."));
	}

	// --- 3. Prepare Controls ---
	m_seqCanvas.Invalidate();
	m_parCanvas.Invalidate();

	// Preserve the current state of the main structures for final cleanup
	CWnd* seqCanvasPtr = m_seqData.pCanvasWnd;
	CWnd* parCanvasPtr = m_parData.pCanvasWnd;


	// --- 4. Start Race Task (Capture data by REFERENCE: &m_seqData, &m_parData) ---
	auto race_task = [this, visualization_enabled, seqCanvasPtr, parCanvasPtr]() mutable {
		HighPrecisionTimer timer;
		long long sequential_time, parallel_time;

		// Note: The sort functions now directly use the current state of m_seqData and m_parData!
		timer.Start();
		SequentialMergeSort(this->m_seqData, 0, this->m_seqData.arr.size() - 1);
		sequential_time = timer.Stop();

		timer.Start();
		ParallelMergeSort(this->m_parData, 0, this->m_parData.arr.size() - 1);
		parallel_time = timer.Stop();

		// Post the results back to the main GUI thread
		::PostMessage(this->m_hWnd, WM_RACE_FINISHED, (WPARAM)sequential_time, (LPARAM)parallel_time);

		// Final cleanup: Restore canvas pointers to their original state 
		// (This is critical to prevent m_pSortData from being nullptr permanently)
		this->m_seqData.pCanvasWnd = &this->m_seqCanvas;
		this->m_parData.pCanvasWnd = &this->m_parCanvas;
		};

	// Start the race in a dedicated thread
	std::thread race_thread(race_task);
	race_thread.detach();
}
LRESULT CSortRaceDlg::OnRaceFinished(WPARAM wParam, LPARAM lParam)
{
	long long sequential_time = (long long)wParam;
	long long parallel_time = (long long)lParam;

	CString str_seq, str_par;
	str_seq.Format(_T("%lld ms"), sequential_time);
	str_par.Format(_T("%lld ms"), parallel_time);

	m_seqTimeEdit.SetWindowText(str_seq);
	m_parTimeEdit.SetWindowText(str_par);

	m_seqData.active_index1 = m_seqData.active_index2 = -1;
	m_parData.active_index1 = m_parData.active_index2 = -1;
	m_seqCanvas.Invalidate();
	m_parCanvas.Invalidate();

	return 0;
}