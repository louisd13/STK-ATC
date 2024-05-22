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

#ifndef HEADER_RACE_SETUP_SCREEN_HPP
#define HEADER_RACE_SETUP_SCREEN_HPP

#include "guiengine/screen.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/dynamic_ribbon_widget.hpp"

namespace GUIEngine { class Widget; }

class RaceSetupHoverListener;
class DynamicRaceSetupHoverListener;

/**
  * \brief Screen with race setup options (difficulty, game mode, etc...)
  * \ingroup states_screens
  */
class RaceSetupScreen : public GUIEngine::Screen, public GUIEngine::ScreenSingleton<RaceSetupScreen>
{
    friend class GUIEngine::ScreenSingleton<RaceSetupScreen>;
    friend class RaceSetupHoverListener;
    friend class DynamicRaceSetupHoverListener;

    RaceSetupScreen();

    void onGameModeChanged();

    void assignDifficulty();

public:

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget, const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;
};

class RaceSetupHoverListener : public GUIEngine::RibbonWidget::IRibbonListener
{
    RaceSetupScreen* m_parent;
    GUIEngine::RibbonWidget* m_ribbon_widget;
public:
    unsigned int m_magic_number;

/*     RaceSetupHoverListener(RaceSetupScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    } */

    RaceSetupHoverListener(RaceSetupScreen* parent, GUIEngine::RibbonWidget* ribbon_widget)
        : m_parent(parent), m_ribbon_widget(ribbon_widget)
    {
        m_magic_number = 0xCAFEC001;
    }

    // ------------------------------------------------------------------------
    virtual ~RaceSetupHoverListener()
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
        //std::cout << "RaceSetup: " << str << std::endl;
    }
}; 

class DynamicRaceSetupHoverListener : public GUIEngine::DynamicRibbonHoverListener
{
    RaceSetupScreen* m_parent;
public:
    unsigned int m_magic_number;

    DynamicRaceSetupHoverListener(RaceSetupScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    }

    // ------------------------------------------------------------------------
    virtual ~DynamicRaceSetupHoverListener()
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
        //std::cout << "RaceSetupD: " << str << std::endl;
    }
}; 

#endif
