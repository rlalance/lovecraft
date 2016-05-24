#ifndef BUTTONBARVIEW_H
#define BUTTONBARVIEW_H

#include <view/YiSceneView.h>

class CYIToggleButtonView;

class ButtonBarView : public CYISceneView
{
public:
    
    /*!
     \details Constructor.
     */
    ButtonBarView();
    
    /*!
     \details Destructor.
     */
    virtual ~ButtonBarView();
    
    /*!
     \details Initializes the ButtonBarView. Gets the list of all CYIToggleButtonView children
     and connects them to OnButtonToggled.
     */
    virtual bool Init();

    /*!
        \details Adds a button with the given text.
    */
    void AddButton(const CYIString &text);

    /*!
        \details Inserts a button with the given text at the given index.
    */
    void InsertButton(YI_INT32 nButtonIndex, const CYIString &text);
    
    /*!
     \details Changes the buttons' toggle states when a new page is selected.
     */
    void SelectButton(YI_INT32 nIndex);
    
    /*!
     \details Returns the button count.
     */
    YI_INT32 GetButtonCount() const;
    
    /*!
     \details Emitted when a button is pressed.
     */
    CYISignal<YI_INT32> ButtonSelected;

    
protected:
    
    /*!
     \details Handles button press events. bOn indicates the toggle state, 
     and nID indicates the button's ID.
     */
    void OnButtonToggled(bool bOn, YI_INT32 nID);
    
    /*!
     \details Goes through the vector of buttons and sets the button with 
     nIndex to toggle state TOGGLED_ON, and the rest to TOGGLED_OFF.
     */
    void ChangeToggleStates(YI_INT32 nIndex);

    virtual CYISceneView* FindNextFocusInDescendants(const CYISceneView *pCurrentFocusView, FOCUS_DIRECTION eDirection) const;
    virtual void OnFocusGained();
private:

    YI_INT32 AdjustInsertIndexForLayout(YI_INT32 nButtonIndex, CYILayout *pLayout);
    
    CYIToggleButtonView *CreateButton() const;

    std::vector<CYIToggleButtonView *> m_buttons;

    CYISharedPtr<CYIAssetViewTemplate> m_pButtonTemplate;
    
    YI_DISALLOW_COPY_AND_ASSIGN(ButtonBarView);
    
    YI_TYPE_DATA
};

#endif // BUTTONBARVIEW_H
