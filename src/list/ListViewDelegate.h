#ifndef _LIST_VIEW_DELEGATE_H_
#define _LIST_VIEW_DELEGATE_H_

#include <asset/YiAssetViewTemplate.h>
#include <smartptr/YiSharedPtr.h>

class ListItem;

class ListViewDelegate
{

public:
    ListViewDelegate();
    virtual ~ListViewDelegate();

    /*!
        \details Reimplement to handle stream in for the given list item.
    */
    virtual void HandleStreamInForListItem(ListItem *pItem);

    /*!
        \details Reimplement to handle stream out for the given list item.
    */
    virtual void HandleStreamOutForListItem(ListItem *pItem);

    /*!
        \details Reimplement to handle stream in for the given list item.
    */
    virtual void HandleLoadInForListItem(ListItem *pItem);

    /*!
        \details Reimplement to handle stream out for the given list item.
    */
    virtual void HandleLoadOutForListItem(ListItem *pItem);

    /*!
        \details Reimplement to handle stream in for the given list item.
    */
    virtual void HandleVisibleInForListItem(ListItem *pItem);

    /*!
        \details Reimplement to handle stream out for the given list item.
    */
    virtual void HandleVisibleOutForListItem(ListItem *pItem);

    /*!
        \details Reimplement and return true to delete the list item's view when the item is unloaded.
    */
    virtual bool ShouldDeleteViewOnLoadOutForListItem(ListItem *pItem);

};

#endif // _LIST_VIEW_DELEGATE_H_

