#ifndef _LIST_ITEM_H_
#define _LIST_ITEM_H_

#include <utility>
#include "list/ListItem.h"

#include <view/YiListItem.h>

class ListView;
class ListViewDelegate;

class ListItem : public CYIListItem
{
    friend class ListView;

public:

    enum
    {
        WIDTH_AS_COLUMN_SPAN = -1,
        HEIGHT_AS_ROW_SPAN = -1
    };

    /*!
        \details Constructs a list item using the view template with the given name.
    */
    ListItem(const CYIString &viewTemplateName);

    /*!
        \details Constructs a list item using the view template with the 
        given name and spanning the given number of columns and rows.
    */
    ListItem(const CYIString &viewTemplateName, YI_INT32 nColumnSpan, YI_INT32 nRowSpan);

    virtual ~ListItem();

    /*!
        \details Returns the item's list.
    */
    ListView *GetListView() const;

    /*!
        \details Returns the section the item belongs to.
    */
    YI_INT32 GetSectionIndex() const;

    /*!
        \details Returns the index of the item in the section it belongs to.
    */
    YI_INT32 GetIndexInSection() const;

    /*!
        \details Returns the data.
    */
    const CYIAny GetData() const;

    /*!
        \details Sets the data.
    */
    void SetData(const CYIAny &data);

protected:

    CYISceneView *CreateView(CYISceneManager *pSceneManager);
    virtual void OnLoadIn();
    virtual void OnLoadOut();
    virtual void OnVisibleIn();
    virtual void OnVisibleOut();
    virtual void OnStreamIn();
    virtual void OnStreamOut();

private:

    CYIAny m_data;

    YI_DISALLOW_COPY_AND_ASSIGN(ListItem)
};

#endif // _DELEGATING_LIST_ITEM_H_

