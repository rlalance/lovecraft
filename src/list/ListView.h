#ifndef _LIST_VIEW_H_
#define _LIST_VIEW_H_

#include <view/YiListView.h>

class ListItem;
class ListViewDelegate;

class ListView : public CYIListView
{
    friend class ListItem;

public:

    ListView();
    virtual ~ListView();

    /*!
    \details Initialize ListView
    */
    bool Init();

    /*!
        \details Returns the delegate.
    */
    ListViewDelegate *GetDelegate() const;

    /*!
        \details Sets the delegate.
    */
    void SetDelegate(ListViewDelegate *pDelegate);

    /*!
        \details Returns true if the list is empty.
    */
    bool IsEmpty() const;

    /*!
        \details Returns true if the list is not empty.
    */
    bool IsNotEmpty() const;

    /*!
        \details Returns the number of columns.
    */
    YI_INT32 GetColumnCount() const;

    /*!
        \details Returns the number of sections.
    */
    YI_INT32 GetSectionCount() const;

    /*!
        \details Returns the number of items in the given section.
    */
    YI_INT32 GetItemCountInSection(YI_INT32 nSectionIndex) const;

    /*!
        \details Returns the item at the given index in the given section.
    */
    ListItem *GetItemInSection(YI_INT32 nSectionIndex, YI_INT32 nItemIndex) const;

    /*!
        \details Returns the first item in the given section.
    */
    ListItem *GetFirstItemInSection(YI_INT32 nSectionIndex) const;

    /*!
        \details Returns the last item in the given section.
    */
    ListItem *GetLastItemInSection(YI_INT32 nSectionIndex) const;

    /*!
        \details Adds a section to the end of the list.
    */
    void AddSection();

    /*!
        \details Inserts a section at the given index.
    */
    void InsertSection(YI_INT32 nSectionIndex);

    /*!
        \details Removes all list items from the section at the given index.
    */
    void ClearSection(YI_INT32 nSectionIndex, bool bAnimate);

    /*!
        \details Removes the section at the given index.
    */
    void RemoveSection(YI_INT32 nSectionIndex, bool bAnimate);

    /*!
        \details Adds the given item to the given section.
    */
    void Add(ListItem *pItem, YI_INT32 nSectionIndex, bool bAnimate);

    /*!
        \details Inserts the given item at the given index in the given section.
    */
    void Insert(ListItem *pItem, YI_INT32 nSectionIndex, YI_INT32 nItemIndex, bool bAnimate);

    /*!
        \details Removes the item at the given in index in the given section.
    */
    void Remove(YI_INT32 nSectionIndex, YI_INT32 nItemIndex, bool bAnimate);

    /*!
        \details Sets the stream range factor to the given factor.
    */
    void SetStreamRangeFactor(YI_FLOAT fStreamRangeFactor);

    /*!
        \details Returns the stream range factor.
    */
    YI_FLOAT GetStreamRangeFactor() const;

    /*!
        \details Custom focus management so we can handle focusing the first item with certain scenarios.
        This new behavior is currently using m_bFocusFirstListItem flag.
        Otherwise, the list behaves normally.
     */
    virtual bool RequestFocus(CYISceneView::FOCUS_DIRECTION eDirection, const CYIAABB &previousFocusRect);

    /*!
        \details This allows to change  the default list focus behavior.
        When set, m_bFocusFirstListItem will cause the list to focus the first list item instead of the closest item.
     */
    void SetFocusOnFirstListItem(bool bFocusFirstListItem);

    /*!
        \see SetFocusOnFirstListItem
     */
    bool GetFocusOnFirstListItem() const;

protected:

    /*!
        \details Hiding CYIListView::Add in favour of the section based Add.
    */
    using CYIListView::Add;

    /*!
        \details Hiding CYIListView::Insert in favour of the section based Insert.
    */
    using CYIListView::Insert;

    /*!
        \details Hiding CYIListView::Remove in favour of the section based Remove.
    */
    using CYIListView::Remove;

private:

    enum INDEX_USED_FOR
    {
        ACCESS,
        INSERT
    };

    static void SetStreamRangeFactor(CYIListItem *pItem, YI_FLOAT fStreamRangeFactor);
    bool IsSectionIndexValid(YI_INT32 nSectionIndex, INDEX_USED_FOR eIndexUsedFor) const;
    bool IsItemIndexValid(YI_INT32 nSectionIndex, YI_INT32 nItemIndex, INDEX_USED_FOR eIndexUsedFor) const;
    YI_INT32 GetCollapsedIndex(YI_INT32 nSectionIndex, YI_INT32 nItemIndex) const;
    std::pair<YI_INT32, YI_INT32> GetExpandedIndices(YI_INT32 nCollapsedIndex) const;

    YI_INT32 m_nColumnCount;

    ListViewDelegate *m_pDelegate;

    std::vector<YI_INT32> m_sectionsListItemCount;

    YI_FLOAT m_fStreamRangeFactor;

private:
    bool m_bFocusFirstListItem;

    YI_DISALLOW_COPY_AND_ASSIGN(ListView)

    YI_TYPE_DATA
};

#endif // _LIST_VIEW_H_

