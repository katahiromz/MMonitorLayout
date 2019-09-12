// MMonitorLayout.hpp --- Screen and monitors information
// This file is public domain software.
#ifndef MMONITORLAYOUT_HPP_
#define MMONITORLAYOUT_HPP_   4   // Version 4

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <vector>
#include <cassert>

/////////////////////////////////////////////////////////////////////////////

namespace MVirtualScreen
{
    INT get_x();
    INT get_y();
    INT get_cx();
    INT get_cy();
    POINT get_point();
    SIZE get_size();
    RECT get_rect();
    HDC create_dc();
    void delete_dc(HDC hdc);
}

namespace MPrimaryScreen
{
    INT get_x();
    INT get_y();
    INT get_cx();
    INT get_cy();
    POINT get_point();
    SIZE get_size();
    RECT get_rect();
    HDC get_dc();
    void release_dc(HDC hdc);
}

namespace MPrimaryWorkArea
{
    POINT get_point();
    SIZE get_size();
    RECT get_rect();
}

typedef std::size_t MMonitorIndex;

struct MMonitorLayout
{
    std::vector<MONITORINFOEX> m_items;
    std::vector<HMONITOR> m_handles;

    BOOL reload(HDC hDC = NULL, LPCRECT prc = NULL);
    size_t count() const;

    MMonitorIndex index_from_handle(HMONITOR hMon) const;
    HMONITOR handle_from_index(MMonitorIndex index) const;

    MMonitorIndex primary_index() const;
    HMONITOR primary_handle() const;

    static BOOL get_monitor_info(HMONITOR hMon, MONITORINFOEX& info);

    enum { def_flags = MONITOR_DEFAULTTONEAREST };
    static HMONITOR monitor_from_point(POINT pt, DWORD dwFlags = def_flags);
    static HMONITOR monitor_from_rect(LPCRECT prc, DWORD dwFlags = def_flags);
    static HMONITOR monitor_from_window(HWND hwnd, DWORD dwFlags = def_flags);

protected:
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                         LPRECT lprcMonitor, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////
// MVirtualScreen

inline INT MVirtualScreen::get_x()
{
    return GetSystemMetrics(SM_XVIRTUALSCREEN);
}

inline INT MVirtualScreen::get_y()
{
    return GetSystemMetrics(SM_YVIRTUALSCREEN);
}

inline INT MVirtualScreen::get_cx()
{
    return GetSystemMetrics(SM_CXVIRTUALSCREEN);
}

inline INT MVirtualScreen::get_cy()
{
    return GetSystemMetrics(SM_CYVIRTUALSCREEN);
}

inline HDC MVirtualScreen::create_dc()
{
    return ::CreateDCW(L"DISPLAY", NULL, NULL, NULL);
}

inline void MVirtualScreen::delete_dc(HDC hdc)
{
    ::DeleteDC(hdc);
}

inline POINT MVirtualScreen::get_point()
{
    POINT ret;
    ret.x = get_x();
    ret.y = get_y();
    return ret;
}

inline SIZE MVirtualScreen::get_size()
{
    SIZE ret;
    ret.cx = get_cx();
    ret.cy = get_cy();
    return ret;
}

inline RECT MVirtualScreen::get_rect()
{
    RECT ret;
    ret.left = get_x();
    ret.top = get_y();
    ret.right = ret.left + get_cx();
    ret.bottom = ret.top + get_cy();
    return ret;
}

/////////////////////////////////////////////////////////////////////////////
// MPrimaryScreen

inline INT MPrimaryScreen::get_x()
{
    return 0;
}

inline INT MPrimaryScreen::get_y()
{
    return 0;
}

inline INT MPrimaryScreen::get_cx()
{
    return ::GetSystemMetrics(SM_CXSCREEN);
}

inline INT MPrimaryScreen::get_cy()
{
    return ::GetSystemMetrics(SM_CYSCREEN);
}

inline POINT MPrimaryScreen::get_point()
{
    POINT ret = { 0, 0 };
    return ret;
}

inline SIZE MPrimaryScreen::get_size()
{
    SIZE ret;
    ret.cx = get_cx();
    ret.cy = get_cy();
    return ret;
}

inline RECT MPrimaryScreen::get_rect()
{
    RECT ret;
    ret.left = 0;
    ret.top = 0;
    ret.right = get_cx();
    ret.bottom = get_cy();
    return ret;
}

inline HDC MPrimaryScreen::get_dc()
{
    return ::GetDC(::GetDesktopWindow());
}

inline void MPrimaryScreen::release_dc(HDC hdc)
{
    ::ReleaseDC(::GetDesktopWindow(), hdc);
}

/////////////////////////////////////////////////////////////////////////////
// MPrimaryWorkArea

inline POINT MPrimaryWorkArea::get_point()
{
    RECT rc = get_rect();
    POINT ret;
    ret.x = rc.left;
    ret.y = rc.top;
    return ret;
}

inline SIZE MPrimaryWorkArea::get_size()
{
    RECT rc = get_rect();
    SIZE ret;
    ret.cx = rc.right - rc.left;
    ret.cy = rc.bottom - rc.top;
    return ret;
}

inline RECT MPrimaryWorkArea::get_rect()
{
    RECT ret;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &ret, 0);
    return ret;
}

/////////////////////////////////////////////////////////////////////////////
// MMonitorLayout

inline BOOL MMonitorLayout::reload(HDC hDC, LPCRECT prc)
{
    m_items.clear();
    m_handles.clear();
    return ::EnumDisplayMonitors(hDC, prc, MonitorEnumProc, (LPARAM)this);
}

inline size_t MMonitorLayout::count() const
{
    assert(m_items.size() == m_handles.size());
    return m_items.size();
}

inline HMONITOR MMonitorLayout::handle_from_index(MMonitorIndex index) const
{
    assert(count() > 0);
    return m_handles[index];
}

inline HMONITOR MMonitorLayout::monitor_from_point(POINT pt, DWORD dwFlags)
{
    return ::MonitorFromPoint(pt, dwFlags);
}

inline HMONITOR MMonitorLayout::monitor_from_rect(LPCRECT prc, DWORD dwFlags)
{
    return ::MonitorFromRect(prc, dwFlags);
}

inline HMONITOR MMonitorLayout::monitor_from_window(HWND hwnd, DWORD dwFlags)
{
    return ::MonitorFromWindow(hwnd, dwFlags);
}

inline BOOL MMonitorLayout::get_monitor_info(HMONITOR hMon, MONITORINFOEX& info)
{
    info.cbSize = sizeof(info);
    return ::GetMonitorInfo(hMon, (LPMONITORINFO)&info);
}

/////////////////////////////////////////////////////////////////////////////

#endif  // ndef MMONITORLAYOUT_HPP_
