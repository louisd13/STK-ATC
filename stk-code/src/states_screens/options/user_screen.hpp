//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009-2015 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


#ifndef __HEADER_USER_SCREEN_HPP__
#define __HEADER_USER_SCREEN_HPP__

#include <string>

#include "guiengine/screen.hpp"
#include "guiengine/widgets/spinner_widget.hpp"
#include "input/input.hpp"

#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/dynamic_ribbon_widget.hpp"
//#include "guiengine/widgets/text_box_widget.hpp"

namespace GUIEngine
{
    class CheckBoxWidget;
    class LabelWidget;
    class RibbonWidget;
    class TextBoxWidget;
    class Widget;
}

class PlayerProfile;
class DynamicUserScreenHoverListener;
class RWUserScreenHoverListener;


/**
  * \brief The user management screen. The screen cames in two variations:
  *  either as a stand-alone screen before the main menu (on first time STK
  *  is started, or it the user is not remembered), but also as tab in the
  *  options menu. To implement this, we use one common base class that
  *  implements nearly all functionality, and derive to classes - one for
  *  the stand alone version, one for the version with tabs.
  * \ingroup states_screens.
  */
class BaseUserScreen : public GUIEngine::Screen
{
    friend class DynamicUserScreenHoverListener;
    friend class RWUserScreenHoverListener;
protected:
    BaseUserScreen(const std::string &name);

private:

    /** The state of the user screen. Note that this is a bit mask, since the
     *  current user can be logged out, and the new one logged in at the
     *  same time. */
    enum UserScreenState { STATE_NONE=0, STATE_LOGIN=1, STATE_LOGOUT=2} m_state;

    /** The user name that is currently being logged out. Used to
     *  display more meaningful sign-out message. */
    irr::core::stringw m_sign_out_name;

    /** The user name that is currently being logged out. Used to
     *  display more meaningful sign-out message. */
    irr::core::stringw m_sign_in_name;

    /** Online check box. */
    GUIEngine::CheckBoxWidget *m_online_cb;

    /** User name entry field. */
    GUIEngine::TextBoxWidget *m_username_tb;

    /** Password widget. */
    GUIEngine::TextBoxWidget *m_password_tb;

    /** Label field for warning and error messages. */
    GUIEngine::LabelWidget * m_info_widget;

    /** The ribbon with all buttons. */
    GUIEngine::RibbonWidget *m_options_widget;

    /** The dynamic ribbon containing all players. */
    GUIEngine::DynamicRibbonWidget* m_players;

    /** Set to indicate when the sceen is initialised that new data from a
     *  registration are available, and therefore entry fields are not
     *  all cleared. */
    bool m_new_registered_data;

    /** Set from the register screen if the newly created account can be
     *  used directly without waiting to confirm the account. */
    bool m_auto_login;

    void selectUser(int index);
    void makeEntryFieldsVisible();
    void login();
    void closeScreen();
    void deletePlayer();
    void doDeletePlayer();
    PlayerProfile* getSelectedPlayer();
    virtual void onUpdate(float dt) OVERRIDE;

public:
    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget,
                               const std::string& name, const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void tearDown() OVERRIDE;

    /** \brief implement optional callback from parent class GUIEngine::Screen */
    virtual void unloaded() OVERRIDE;

    void setNewAccountData(bool online, bool auto_login,
                           const core::stringw &online_name="",
                           const core::stringw &password="");
    void loginSuccessful();
    void loginError(const irr::core::stringw &error_message, bool clear_password);
    void logoutSuccessful();
    void logoutError(const irr::core::stringw &error_message);

    virtual GUIEngine::EventPropagation filterActions(PlayerAction action,
        int deviceID,
        const unsigned int value,
        Input::InputType type,
        int playerId) OVERRIDE;
};   // class BaseUserScreen

// ============================================================================
class UserScreen : public BaseUserScreen,
                   public GUIEngine::ScreenSingleton<UserScreen>
{
private:
    UserScreen() : BaseUserScreen("user_screen.stkgui")
    {};
public:
    friend class GUIEngine::ScreenSingleton<UserScreen>;
};   // class UserScreenTabed

// ============================================================================
class TabbedUserScreen : public BaseUserScreen,
                         public GUIEngine::ScreenSingleton<TabbedUserScreen>
{
private:
    TabbedUserScreen() : BaseUserScreen("user_screen_tab.stkgui")
    {}

public:
    friend class GUIEngine::ScreenSingleton<TabbedUserScreen>;

    virtual void init() OVERRIDE;
    virtual void eventCallback(GUIEngine::Widget* widget,
                               const std::string& name, const int playerID) OVERRIDE;
};   // class TabbedUserScreen
class RWUserScreenHoverListener : public GUIEngine::RibbonWidget::IRibbonListener
{
    BaseUserScreen* m_parent;
    GUIEngine::RibbonWidget* m_ribbon_widget;
public:
    unsigned int m_magic_number;

/*     RibbonWidgetUserScreenHoverListener(RibbonWidgetUserScreenScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    } */

    RWUserScreenHoverListener(BaseUserScreen* parent, GUIEngine::RibbonWidget* ribbon_widget)
        : m_parent(parent), m_ribbon_widget(ribbon_widget)
    {
        m_magic_number = 0xCAFEC001;
    }

    // ------------------------------------------------------------------------
    virtual ~RWUserScreenHoverListener()
    {
        assert(m_magic_number == 0xCAFEC001);
        m_magic_number = 0xDEADBEEF;
    }

    void onRibbonWidgetScroll(const int delta_x) override
    {
        // Implement this method if needed
    }

    void onRibbonWidgetFocus(GUIEngine::RibbonWidget* emitter, const int playerID) override
    {
        // Implement this method if needed
    }

    // ------------------------------------------------------------------------
    void onSelectionChange() override
    {
        assert(m_magic_number == 0xCAFEC001);
        irr::core::stringw selectionText = m_ribbon_widget->getSelectionText(0);
        std::wstring ws(selectionText.c_str());
        std::string str(ws.begin(), ws.end());
        //std::cout << "RW Hover: " << str << std::endl;
    }
}; 

class DynamicUserScreenHoverListener : public GUIEngine::DynamicRibbonHoverListener
{
    BaseUserScreen* m_parent;
public:
    unsigned int m_magic_number;

    DynamicUserScreenHoverListener(BaseUserScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    }

    // ------------------------------------------------------------------------
    virtual ~DynamicUserScreenHoverListener()
    {
        assert(m_magic_number == 0xCAFEC001);
        m_magic_number = 0xDEADBEEF;
    }

    // ------------------------------------------------------------------------
    void onSelectionChanged(GUIEngine::DynamicRibbonWidget* theWidget,
                            const std::string& selectionID,
                            const irr::core::stringw& selectionText,
                            const int playerID)
    {
        assert(m_magic_number == 0xCAFEC001);

        // Convert the selection text to a string for logging
        std::wstring ws(selectionText.c_str());
        std::string str(ws.begin(), ws.end());
        //std::cout << "DRW Hover:" << str << std::endl;
    }
}; 



#endif
