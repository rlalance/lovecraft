#include "list/ListViewDelegate.h"

ListViewDelegate::ListViewDelegate()
{
}

ListViewDelegate::~ListViewDelegate()
{
}

void ListViewDelegate::HandleStreamInForListItem(ListItem *)
{
}

void ListViewDelegate::HandleStreamOutForListItem(ListItem *)
{
}

void ListViewDelegate::HandleLoadInForListItem(ListItem *)
{
}

void ListViewDelegate::HandleLoadOutForListItem(ListItem *)
{
}

void ListViewDelegate::HandleVisibleInForListItem(ListItem *)
{
}

void ListViewDelegate::HandleVisibleOutForListItem(ListItem *)
{
}

bool ListViewDelegate::ShouldDeleteViewOnLoadOutForListItem(ListItem *)
{
    return false;
}
