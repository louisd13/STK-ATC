//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2016 SuperTuxKart-Team
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

#ifndef HEADER_TRACKS_AND_GP_SCREEN_HPP
#define HEADER_TRACKS_AND_GP_SCREEN_HPP

#include "audio/tts/tts.hpp"
#include "guiengine/screen.hpp"
#include "guiengine/widgets/text_box_widget.hpp"
#include "guiengine/widgets/dynamic_ribbon_widget.hpp"
#include <deque>
#include <iostream>

namespace GUIEngine { class Widget; }

class TracksHoverListener;

/**
  * \brief screen where the user can select a track or grand prix
  * \ingroup states_screens
  */
class TracksAndGPScreen : public GUIEngine::Screen,
                          public GUIEngine::ScreenSingleton<TracksAndGPScreen>,
                          public GUIEngine::ITextBoxWidgetListener
{
    friend class GUIEngine::ScreenSingleton<TracksAndGPScreen>;
    friend class TracksHoverListener;

private:
    GUIEngine::TextBoxWidget* m_search_box;

    TracksAndGPScreen() : Screen("tracks_and_gp.stkgui") {}

    /** adds the tracks from the current track group into the tracks ribbon */
    void buildTrackList();

    std::deque<std::string> m_random_track_list;

public:

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE {};

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget,
                               const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** Rebuild the list of tracks based on search text */
    virtual void onTextUpdated() OVERRIDE
    {
        buildTrackList();
        // After buildTrackList the m_search_box may be unfocused
        m_search_box->focused(0, true, true);
    }

    void setFocusOnTrack(const std::string& trackName);
    void setFocusOnGP(const std::string& gpName);

};

class TracksHoverListener : public GUIEngine::DynamicRibbonHoverListener
{
    TracksAndGPScreen* m_parent;
public:
    unsigned int m_magic_number;

    TracksHoverListener(TracksAndGPScreen* parent)
    {
        m_magic_number = 0xCAFEC001;
        m_parent = parent;
    }

    // ------------------------------------------------------------------------
    virtual ~TracksHoverListener()
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
        std::cout << "Tracks & GP: " << str << std::endl;
        the_voice->speak(str, false, true);
    }
}; 

#endif
