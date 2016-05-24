#include "customviews/ButtonBarView.h"

#include "utilities/NodeUtilities.h"

#include <asset/YiAssetViewTemplate.h>
#include <import/YiViewTemplate.h>
#include <layout/YiLayout.h>
#include <smartptr/YiScopedPtr.h>
#include <utility/YiFocus.h>
#include <view/YiToggleButtonView.h>

YI_RTTI_DEF1_INST(ButtonBarView, "ButtonBarView", CYISceneView);

static const CYIString TAG = "ButtonBarView";

ButtonBarView::ButtonBarView()
{
}

ButtonBarView::~ButtonBarView()
{
}

bool ButtonBarView::Init()
{
    if (!CYISceneView::Init())
    {
        return false;
    }

    if (CYISceneView *pButtonTemplateView = GetNode<CYISceneView>("ButtonTemplate"))
    {
        m_pButtonTemplate = pButtonTemplateView->GetViewTemplate();
        pButtonTemplateView->RequestDeletion();
    }

    m_buttons = Utility::GetIndexedNodes<CYIToggleButtonView>(this, "Button");

    if (GetButtonCount() > 0)
    {
        for (YI_INT32 nButtonIndex = 0; nButtonIndex < GetButtonCount(); ++nButtonIndex)
        {
            CYIToggleButtonView *pButton = m_buttons.at(nButtonIndex);
            pButton->ButtonToggled.Connect(*this, &ButtonBarView::OnButtonToggled);
            pButton->SetButtonID(nButtonIndex);
        }
    }

    SetFocusable(true);

    return true;
}

void ButtonBarView::AddButton(const CYIString &text)
{
    InsertButton(GetButtonCount(), text);
}

void ButtonBarView::InsertButton(YI_INT32 nButtonIndex, const CYIString &text)
{
    YI_ASSERT(nButtonIndex >= 0 && nButtonIndex <= GetButtonCount(), TAG, "Invalid button index");

    CYIToggleButtonView *pButton = CreateButton();
    pButton->SetText(text);
    pButton->SetButtonID(GetButtonCount());
    pButton->ButtonToggled.Connect(*this, &ButtonBarView::OnButtonToggled);
    m_buttons.insert(m_buttons.begin() + nButtonIndex, pButton);

    nButtonIndex = AdjustInsertIndexForLayout(nButtonIndex, GetLayout());

    AddChild(pButton, nButtonIndex);
}

void ButtonBarView::SelectButton(YI_INT32 nIndex)
{
    if (nIndex < 0 || nIndex >= GetButtonCount())
    {
        YI_ASSERT(false, TAG, "Invalid index. Out of bounds.");
    }
    
    if (!m_buttons[nIndex]->IsToggledOn())
    {
        ChangeToggleStates(m_buttons[nIndex]->GetButtonID());
    }
}

CYISceneView* ButtonBarView::FindNextFocusInDescendants(const CYISceneView *pCurrentFocusView, FOCUS_DIRECTION eDirection) const
{
    return CYIFocus::FindNextFocus(this, CYIFocus::GetScreenSpaceFocusRegion(pCurrentFocusView), eDirection);
}

void ButtonBarView::OnFocusGained()
{
    CYISceneView *pView = FindNextFocusInDescendants(this, FOCUS_FORWARD);
    if(pView)
    {
        pView->RequestFocus();
    }
}

YI_INT32 ButtonBarView::GetButtonCount() const
{
    return static_cast<YI_INT32>(m_buttons.size());
}

void ButtonBarView::OnButtonToggled(bool bOn, YI_INT32 nID)
{
    // We only set the toggle states for all buttons when a CYIToggleButtonView state is set to ON
    if (bOn)
    {
        ChangeToggleStates(nID);
    }
}

void ButtonBarView::ChangeToggleStates(YI_INT32 nID)
{
    // loop through children, turn off any toggles that are not this ID
    std::vector<CYIToggleButtonView*>::iterator it;
    YI_INT32 nIndex = 0;
    
    for (it = m_buttons.begin(); it != m_buttons.end(); ++it)
    {
        CYIToggleButtonView *pToggle = *it;
        if (pToggle->GetButtonID() == nID)
        {
            // disabling the button will remove focus from it at the end of the next update.
            // lets set the focus elsewhere first
            if (GetViewWithFocus() == pToggle)
            {
                CYISceneView *pNextFocus = CYIFocus::FindNextFocus(this, CYIFocus::GetScreenSpaceFocusRegion(pToggle), CYISceneView::FOCUS_FORWARD);
                if (pNextFocus)
                {
                    pNextFocus->RequestFocus();
                }
            }
                
            pToggle->SetState(CYIToggleButtonView::DISABLED, CYIToggleButtonView::TOGGLED_ON);
            ButtonSelected(nIndex);
        }
        else
        {
            pToggle->SetState(CYIToggleButtonView::ENABLED, CYIToggleButtonView::TOGGLED_OFF);
        }
        
        nIndex++;
    }
}

YI_INT32 ButtonBarView::AdjustInsertIndexForLayout(YI_INT32 nButtonIndex, CYILayout *pLayout)
{
    if (pLayout != YI_NULL)
    {
        if (pLayout->IsUsingReverseChildrenOrder())
        {
            nButtonIndex = GetChildCount() - nButtonIndex;
        }
    }
    return nButtonIndex;
}

CYIToggleButtonView *ButtonBarView::CreateButton() const
{
    YI_ASSERT(m_pButtonTemplate, TAG, "Missing button template");

    const CYISharedPtr<CYIViewTemplate> pViewTemplate = m_pButtonTemplate->GetTemplate();

    CYIScopedPtr<CYISceneView> pButtonView(pViewTemplate->BuildView(GetSceneManager(), m_pButtonTemplate));
    YI_ASSERT(pButtonView, TAG, "Could not instantiate button view");
    CYIScopedPtr<CYIToggleButtonView> pButton(YiDynamicCast<CYIToggleButtonView>(pButtonView.Take()));
    YI_ASSERT(pButton, TAG, "Could not instantiate button view");

    if (!pButton->Init())
    {
        YI_ASSERT(false, TAG, "Could not initialize button");

        return YI_NULL;
    }

    return pButton.Take();
}
