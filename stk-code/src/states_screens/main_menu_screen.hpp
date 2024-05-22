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

#ifndef HEADER_MAIN_MENU_SCREEN_HPP
#define HEADER_MAIN_MENU_SCREEN_HPP

#include "guiengine/screen.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"

#include <iostream>

namespace GUIEngine { class Widget;       class ListWidget; 
                      class ButtonWidget; class IconButtonWidget; }

class MainMenuHoverListener;

/**
  * \brief Handles the main menu
  * \ingroup states_screens
  */
class MainMenuScreen : public GUIEngine::Screen, public GUIEngine::ScreenSingleton<MainMenuScreen>
{
private:
    friend class GUIEngine::ScreenSingleton<MainMenuScreen>;
    friend class MainMenuHoverListener;

    /** Keep the widget to to the user name. */
    GUIEngine::ButtonWidget *m_user_id;

    core::stringw m_news_text;

    MainMenuScreen();

    void startTutorial();
public:
    virtual void onUpdate(float delta) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;
    
    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget, const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void tearDown() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void onDisabledItemClicked(const std::string& item) OVERRIDE;

    /** \brief implement optional callback from parent
     *  class GUIEngine::Screen */
    virtual bool onEscapePressed() OVERRIDE;
};

class MainMenuHoverListener : public GUIEngine::RibbonWidget::IRibbonListener
{
    MainMenuScreen* m_parent;
    GUIEngine::RibbonWidget* m_ribbon_widget;
public:
    unsigned int m_magic_number;

/*     MainMenuHoverListener(MainMenuScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    } */

    MainMenuHoverListener(MainMenuScreen* parent, GUIEngine::RibbonWidget* ribbon_widget)
        : m_parent(parent), m_ribbon_widget(ribbon_widget)
    {
        m_magic_number = 0xCAFEC001;
    }

    // ------------------------------------------------------------------------
    virtual ~MainMenuHoverListener()
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
        //std::cout << "Hovered Text: " << str << std::endl;
    }
}; 


#endif
