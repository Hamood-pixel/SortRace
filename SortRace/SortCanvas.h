#pragma once

#include "SortLogic.h" // Needed for the SortData structure

class CSortCanvas : public CStatic
{
	DECLARE_DYNAMIC(CSortCanvas)

public:
	CSortCanvas();
	virtual ~CSortCanvas();

	SortData* m_pSortData = nullptr;

protected:
	afx_msg LRESULT OnRedrawCanvas(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};