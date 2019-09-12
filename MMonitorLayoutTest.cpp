// MMonitorLayoutTest.cpp --- Test for MMonitorLayout
// This file is public domain software.
#include "MMonitorLayout.hpp"
#include <cstdio>

using std::printf;

void print_rect(LPCRECT prc)
{
    printf("(%ld, %ld) - (%ld, %ld)", prc->left, prc->top, prc->right, prc->bottom);
}

int main(void)
{
    MMonitorLayout layout;

    layout.reload();

    MMonitorIndex index = 0;
    for (const auto& item : layout.m_items)
    {
        printf("handle: %p\n", (void *)layout.m_handles[index]);
        printf("item.rcMonitor: ");
        print_rect(&item.rcMonitor);
        printf("\nitem.rcWork: ");
        print_rect(&item.rcWork);
        printf("\n");
        ++index;
    }
    printf("primary_index: %d\n", layout.primary_index());
    printf("primary_handle: %p\n", (void *)layout.primary_handle());

    RECT rc;

    rc = MVirtualScreen::get_rect();
    printf("MVirtualScreen::get_rect: ");
    print_rect(&rc);
    printf("\n");

    rc = MPrimaryScreen::get_rect();
    printf("MPrimaryScreen::get_rect: ");
    print_rect(&rc);
    printf("\n");

    rc = MPrimaryWorkArea::get_rect();
    printf("MPrimaryWorkArea::get_rect: ");
    print_rect(&rc);
    printf("\n");

    return 0;
}
