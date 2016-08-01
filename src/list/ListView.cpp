#include "list/ListView.h"

#include "list/ListItem.h"
#include "utilities/NodeUtilities.h"

#include <glm/gtc/epsilon.hpp>

static const CYIString TAG = "ListView";
static const YI_FLOAT s_nScrollThresholdScreenPercentage = 0.03f; // Specifies the vertical scroll threshold by a percentage amount of the screen height

YI_RTTI_DEF1_INST(ListView, "ListView", CYIListView)

ListView::ListView()
    : m_nColumnCount(1)
    , m_pDelegate(YI_NULL)
    , m_fStreamRangeFactor(2.0f)
    , m_bFocusFirstListItem(false)
{
    AddSection();

    SetVisibleRangeDisabled();
}

ListView::~ListView()
{
    m_pDelegate = YI_NULL;
}

bool ListView::Init()
{
    if (!CYIListView::Init())
    {
        return false;
    }

    YI_INT32 nColumnCount = 1;

    if (Utility::GetProperty<YI_INT32>(this, "columns", nColumnCount) && nColumnCount >= 1)
    {
        m_nColumnCount = nColumnCount;
    }

    m_nScrollThreshold = (YI_INT32)(GetSize().y * s_nScrollThresholdScreenPercentage);
    
    return true;
}

ListViewDelegate *ListView::GetDelegate() const
{
    return m_pDelegate;
}

void ListView::SetDelegate(ListViewDelegate *pDelegate)
{
    m_pDelegate = pDelegate;
}

bool ListView::IsEmpty() const
{
    return GetListItemCount() == 0;
}

bool ListView::IsNotEmpty() const
{
    return !IsEmpty();
}

YI_INT32 ListView::GetColumnCount() const
{
    return m_nColumnCount;
}

YI_INT32 ListView::GetSectionCount() const
{
    return static_cast<YI_INT32>(m_sectionsListItemCount.size());
}

YI_INT32 ListView::GetItemCountInSection(YI_INT32 nSectionIndex) const
{
    YI_ASSERT(IsSectionIndexValid(nSectionIndex, ACCESS), TAG, "Invalid section index");

    return m_sectionsListItemCount.at(nSectionIndex);
}

ListItem *ListView::GetItemInSection(YI_INT32 nSectionIndex, YI_INT32 nItemIndex) const
{
    YI_ASSERT(IsItemIndexValid(nSectionIndex, nItemIndex, ACCESS), TAG, "Invalid item or section index");

    return static_cast<ListItem *>(GetListItem(GetCollapsedIndex(nSectionIndex, nItemIndex)));
}

ListItem *ListView::GetFirstItemInSection(YI_INT32 nSectionIndex) const
{
    return GetItemInSection(nSectionIndex, 0);
}

ListItem *ListView::GetLastItemInSection(YI_INT32 nSectionIndex) const
{
    return GetItemInSection(nSectionIndex, GetItemCountInSection(nSectionIndex) - 1);
}

void ListView::AddSection()
{
    m_sectionsListItemCount.push_back(0);
}

void ListView::InsertSection(YI_INT32 nSectionIndex)
{
    YI_ASSERT(IsSectionIndexValid(nSectionIndex, INSERT), TAG, "Invalid section index");

    m_sectionsListItemCount.insert(m_sectionsListItemCount.begin() + nSectionIndex, 0);
}

void ListView::ClearSection(YI_INT32 nSectionIndex, bool bAnimate)
{
    YI_ASSERT(IsSectionIndexValid(nSectionIndex, ACCESS), TAG, "Invalid section index");

    const YI_INT32 nCollapsedIndex = GetCollapsedIndex(nSectionIndex, 0);

    for (YI_INT32 nItemIndex = m_sectionsListItemCount.at(nSectionIndex) - 1; nItemIndex >= 0; --nItemIndex)
    {
        Remove(nCollapsedIndex + nItemIndex, bAnimate);
    }
    
    m_sectionsListItemCount[nSectionIndex] = 0;
}

void ListView::RemoveSection(YI_INT32 nSectionIndex, bool bAnimate)
{
    YI_ASSERT(IsSectionIndexValid(nSectionIndex, ACCESS), TAG, "Invalid section index");

    ClearSection(nSectionIndex, bAnimate);

    m_sectionsListItemCount.erase(m_sectionsListItemCount.begin() + nSectionIndex);
}

bool ListView::RequestFocus(CYISceneView::FOCUS_DIRECTION eDirection, const CYIAABB &previousFocusRect)
{
    if (eDirection == FOCUS_DOWN && m_bFocusFirstListItem)
    {
        if (GetListItemCount() > 0)
        {
            GetFirstVisibleItem()->GetView()->RequestFocus();

            return true;
        }
    }

    return CYIListView::RequestFocus(eDirection, previousFocusRect);
}

void ListView::Add(ListItem *pItem, YI_INT32 nSectionIndex, bool bAnimate)
{
    YI_ASSERT(IsSectionIndexValid(nSectionIndex, ACCESS), TAG, "Invalid section index");

    SetStreamRangeFactor(pItem, m_fStreamRangeFactor);

    Insert(pItem, nSectionIndex, m_sectionsListItemCount.at(nSectionIndex), bAnimate);
}

void ListView::Insert(ListItem *pItem, YI_INT32 nSectionIndex, YI_INT32 nItemIndex, bool bAnimate)
{
    YI_ASSERT(IsItemIndexValid(nSectionIndex, nItemIndex, INSERT), TAG, "Invalid item or section index");

    const YI_INT32 nCollapsedIndex = GetCollapsedIndex(nSectionIndex, nItemIndex);

    CYIListView::Insert(pItem, nCollapsedIndex, bAnimate);

    m_sectionsListItemCount[nSectionIndex] += 1;
}

void ListView::Remove(YI_INT32 nSectionIndex, YI_INT32 nItemIndex, bool bAnimate)
{
    YI_ASSERT(IsItemIndexValid(nSectionIndex, nItemIndex, ACCESS), TAG, "Invalid item or section index");

    const YI_INT32 nCollapsedIndex = GetCollapsedIndex(nSectionIndex, nItemIndex);

    CYIListView::Remove(nCollapsedIndex, bAnimate);

    m_sectionsListItemCount[nSectionIndex] -= 1;
}

bool ListView::IsSectionIndexValid(YI_INT32 nSectionIndex, INDEX_USED_FOR eIndexUsedFor) const
{
    return nSectionIndex >= 0 && (nSectionIndex < GetSectionCount() + (eIndexUsedFor == INSERT ? 1 : 0));
}

bool ListView::IsItemIndexValid(YI_INT32 nSectionIndex, YI_INT32 nItemIndex, INDEX_USED_FOR eIndexUsedFor) const
{
    return IsSectionIndexValid(nSectionIndex, ACCESS) && (nItemIndex >= 0 && nItemIndex < m_sectionsListItemCount.at(nSectionIndex) + (eIndexUsedFor == INSERT ? 1 : 0));
}

YI_INT32 ListView::GetCollapsedIndex(YI_INT32 nSectionIndex, YI_INT32 nItemIndex) const
{
    YI_INT32 nCollapsedIndex = nItemIndex;

    for (--nSectionIndex; nSectionIndex >= 0; --nSectionIndex)
    {
        nCollapsedIndex += m_sectionsListItemCount.at(nSectionIndex);
    }
    return nCollapsedIndex;
}

std::pair<YI_INT32, YI_INT32> ListView::GetExpandedIndices(YI_INT32 nCollapsedIndex) const
{
    for (YI_INT32 nSectionIndex = 0; nSectionIndex < GetSectionCount(); ++nSectionIndex)
    {
        nCollapsedIndex -= m_sectionsListItemCount.at(nSectionIndex);

        if (nCollapsedIndex < 0)
        {
            return std::make_pair(nSectionIndex, m_sectionsListItemCount.at(nSectionIndex) + nCollapsedIndex);
        }
    }
    return std::make_pair(-1, -1);
}

void ListView::SetStreamRangeFactor(YI_FLOAT fStreamRangeFactor)
{
    if (glm::epsilonNotEqual(m_fStreamRangeFactor, fStreamRangeFactor, glm::epsilon<YI_FLOAT>()))
    {
        m_fStreamRangeFactor = fStreamRangeFactor;

        const YI_INT32 nListItemCount = GetListItemCount();

        if (nListItemCount > 0)
        {
            for (YI_INT32 nItemIndex = 0; nItemIndex < nListItemCount; ++nItemIndex)
            {
                SetStreamRangeFactor(GetListItem(nItemIndex), fStreamRangeFactor);
            }
            UpdateContents();
        }
    }
}

void ListView::SetStreamRangeFactor(CYIListItem *pItem, YI_FLOAT fStreamRangeFactor)
{
    pItem->SetLoadRangeFactor(fStreamRangeFactor);
    pItem->SetStreamingRangeFactor(fStreamRangeFactor);
}

YI_FLOAT ListView::GetStreamRangeFactor() const
{
    return m_fStreamRangeFactor;
}

void ListView::SetFocusOnFirstListItem(bool bFocusFirstListItem)
{
    m_bFocusFirstListItem = bFocusFirstListItem;
}

bool ListView::GetFocusOnFirstListItem() const
{
    return m_bFocusFirstListItem;
}
