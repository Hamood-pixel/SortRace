#include "pch.h"
#include "SortCanvas.h"
#include <cmath>
#include "SortLogic.h" 

IMPLEMENT_DYNAMIC(CSortCanvas, CStatic)

CSortCanvas::CSortCanvas() {}
CSortCanvas::~CSortCanvas() {}

BEGIN_MESSAGE_MAP(CSortCanvas, CStatic)
    ON_MESSAGE(WM_REDRAW_CANVAS, OnRedrawCanvas)
    ON_WM_PAINT()
END_MESSAGE_MAP()

LRESULT CSortCanvas::OnRedrawCanvas(WPARAM wParam, LPARAM lParam) {
    Invalidate(FALSE);
    return 0;
}

void CSortCanvas::OnPaint()
{
    CPaintDC dc(this);

    if (!m_pSortData || m_pSortData->arr.empty()) return;

    // Lock the mutex while reading the array data
    std::lock_guard<std::mutex> lock(g_array_mutex);

    CRect rect;
    GetClientRect(&rect);

    // --- 1. Draw Dark Background ---
    CBrush backgroundBrush(RGB(50, 50, 50));
    dc.FillRect(rect, &backgroundBrush);

    int nHeight = rect.Height();
    int arraySize = m_pSortData->arr.size(); // Corrected size access

    bool is_sequential_canvas = (m_pSortData->pCanvasWnd == this);

    // --- 2. Bar Dimensions ---
    float barWidth = (float)rect.Width() / arraySize;
    float barGap = 1.0f;
    float valueScale = (float)nHeight / arraySize;

    for (int i = 0; i < arraySize; i++)
    {
        int barValue = m_pSortData->arr[i];
        int barHeight = (int)(barValue * valueScale);

        CRect barRect;
        barRect.left = (int)(i * barWidth + barGap / 2);
        barRect.right = (int)((i + 1) * barWidth - barGap / 2);
        if (barRect.right <= barRect.left) barRect.right = barRect.left + 1;

        barRect.bottom = nHeight;
        barRect.top = nHeight - barHeight;

        // --- 3. Simple Color Test ---
        CBrush brush;
        COLORREF defaultColor;
        COLORREF activeColor = RGB(255, 0, 0); // BRIGHT RED

        if (is_sequential_canvas) {
            defaultColor = RGB(255, 255, 0); // BRIGHT YELLOW
        }
        else {
            defaultColor = RGB(0, 255, 0);    // BRIGHT GREEN
        }

        if (m_pSortData->active_index1 != -1) // If sort is active, use ACTIVE color
        {
            brush.CreateSolidBrush(activeColor);
        }
        else // Otherwise, use the default color (for final sorted state)
        {
            brush.CreateSolidBrush(defaultColor);
        }

        dc.FillRect(barRect, &brush);
    }
}