#include "list/ListItem.h"

#include "list/ListViewDelegate.h"
#include "list/ListView.h"

#include <utilities/TimelineUtilities.h>

static const CYIString TAG = "ListItem";

ListItem::ListItem(const CYIString &viewTemplateName)
{
    YI_ASSERT(viewTemplateName.IsNotEmpty(), TAG, "Invalid view template name");
    
    SetListItemTemplate(CYIViewTemplate::GetViewTemplate(viewTemplateName));
}

ListItem::ListItem(const CYIString &viewTemplateName, YI_INT32 nColumnSpan, YI_INT32 nRowSpan)
{
    YI_ASSERT(viewTemplateName.IsNotEmpty(), TAG, "Invalid view template name");
    YI_ASSERT(nColumnSpan >= WIDTH_AS_COLUMN_SPAN, TAG, "Invalid column span");
    YI_ASSERT(nRowSpan >= HEIGHT_AS_ROW_SPAN, TAG, "Invalid row span");

    CYISharedPtr<CYIAssetViewTemplate> pViewTemplate = CYIViewTemplate::GetViewTemplate(viewTemplateName);

    YI_ASSERT(pViewTemplate, TAG, "Invalid view template");

    SetListItemTemplate(pViewTemplate);

    if (nColumnSpan == WIDTH_AS_COLUMN_SPAN)
    {
        nColumnSpan = (YI_INT32)pViewTemplate->GetTemplate()->GetWidth();
    }

    if (nRowSpan == HEIGHT_AS_ROW_SPAN)
    {
        nRowSpan = (YI_INT32)pViewTemplate->GetTemplate()->GetHeight();
    }

    SetCellsRect(YI_RECT_REL(0, 0, nColumnSpan, nRowSpan));
}

ListItem::~ListItem()
{
}

CYISceneView *ListItem::CreateView(CYISceneManager *pSceneManager)
{
    CYISceneView *pView = CYIListItem::CreateView(pSceneManager);
    
    SetFocusable(TimelineUtilities::HasViewFocusInTimelines(pView));

    return pView;
}

ListView *ListItem::GetListView() const
{
    return YiDynamicCast<ListView>(CYIListItem::GetListView());
}

YI_INT32 ListItem::GetSectionIndex() const
{
    return GetListView()->GetExpandedIndices(GetIndexInList()).first;
}

YI_INT32 ListItem::GetIndexInSection() const
{
    return GetListView()->GetExpandedIndices(GetIndexInList()).second;
}

const CYIAny ListItem::GetData() const
{
    return m_data;
}

void ListItem::SetData(const CYIAny &data)
{
    m_data = data;
}

void ListItem::OnLoadIn()
{
    CYIListItem::OnLoadIn();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleLoadInForListItem(this);
    }
}

void ListItem::OnLoadOut()
{
    CYIListItem::OnLoadOut();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleLoadOutForListItem(this);

        if (pDelegate->ShouldDeleteViewOnLoadOutForListItem(this))
        {
            delete m_pView;
            m_pView = YI_NULL;
        }
    }
}

void ListItem::OnStreamIn()
{
    CYIListItem::OnStreamIn();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleStreamInForListItem(this);
    }
}

void ListItem::OnStreamOut()
{
    CYIListItem::OnStreamOut();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleStreamOutForListItem(this);
    }
}

void ListItem::OnVisibleIn()
{
    CYIListItem::OnVisibleIn();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleVisibleInForListItem(this);
    }
}

void ListItem::OnVisibleOut()
{
    CYIListItem::OnVisibleOut();

    ListView *pListView = GetListView();
    ListViewDelegate *pDelegate = YI_NULL;
    if (pListView && (pDelegate = pListView->m_pDelegate))
    {
        pDelegate->HandleVisibleOutForListItem(this);
    }
}
