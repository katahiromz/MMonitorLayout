// MMonitorLayout.cpp --- Screen and monitors information
// This file is public domain software.
#include "MMonitorLayout.hpp"
#include <cassert>

/*static*/ BOOL CALLBACK
MMonitorLayout::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                LPRECT lprcMonitor, LPARAM lParam)
{
    MONITORINFOEX info;
    MMonitorLayout::get_monitor_info(hMonitor, info);

    MMonitorLayout *pinfo = (MMonitorLayout *)lParam;
    pinfo->m_items.push_back(info);
    pinfo->m_handles.push_back(hMonitor);
    return TRUE;
}

MMonitorIndex MMonitorLayout::primary_index() const
{
    assert(count() > 0);
    assert(m_items.size() == m_handles.size());
    MMonitorIndex index = 0;
    for (const auto& item : m_items)
    {
        if (item.dwFlags & MONITORINFOF_PRIMARY)
            return index;
        ++index;
    }
    return 0;
}

HMONITOR MMonitorLayout::primary_handle() const
{
    assert(count() > 0);
    assert(m_items.size() == m_handles.size());
    MMonitorIndex index = 0;
    for (const auto& item : m_items)
    {
        if (item.dwFlags & MONITORINFOF_PRIMARY)
            return m_handles[index];
        ++index;
    }
    return NULL;
}

MMonitorIndex MMonitorLayout::index_from_handle(HMONITOR hMon) const
{
    assert(count() > 0);
    assert(m_items.size() == m_handles.size());
    MMonitorIndex index = 0;
    for (HMONITOR handle : m_handles)
    {
        if (hMon == handle)
            return index;
        ++index;
    }
    return 0;
}
