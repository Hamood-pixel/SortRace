// SortRaceDlg.h : header file
//

#pragma once

// --- Custom Includes ---
#include "SortLogic.h" 
#include "SortCanvas.h" 

// CSortRaceDlg dialog
class CSortRaceDlg : public CDialogEx
{
    // Construction
public:
    CSortRaceDlg(CWnd* pParent = nullptr);	// standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SORTRACE_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    // --- CUSTOM MEMBERS FOR SORT RACE PROJECT ---
public:
    // Controls for Visualization and Timing 
    CSortCanvas m_seqCanvas;
    CSortCanvas m_parCanvas;
    CEdit m_seqTimeEdit;
    CEdit m_parTimeEdit;

    // **Variable linked to the Array Size Edit Box (IDC_ARRAY_SIZE_EDIT)**
    int m_iArraySize = 100000;

    // Data structures for the race
    SortData m_seqData;
    SortData m_parData;

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // Handler for dark theme
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // Custom Message Handlers implemented in SortRaceDlg.cpp
    afx_msg void OnBnClickedStartRace();
    afx_msg LRESULT OnRaceFinished(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};