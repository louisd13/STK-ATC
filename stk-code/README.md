# ATC Project: Adapting SuperTuxKart for a visually impaired player

To access the official README of the game, please go to OFFICIALSTK_README.md.

This project has been conducted by Louis Duval, Anouk Paulino, Laetitia Schwitter and Marianne Scoglio, in the context of EPFL Assistive Technologies Challenge. The additional code has been produced by Louis Duval and Anouk Paulino.

## Introduction

In order to make the game accessible to our Challenger, we have added several features to allow us to retrieve important information about the current state of the game. In that way, we were able to transmit this information to the user via means other than visual.

Most of what we have added was directly related to the kart and therefore is located in the corresponding file. We will specify each time where the modification has been made.

In the first two weeks of getting to know SuperTuxKart, most of our time was spent wandering around the different files to understand their purpose and see of what use they could be to us. 

## Additional features

If you want to check the code modification completely you can compare to this version of STK... on git 
You can also look for commented printf or cout which will often be were we were doing debbuging, it was necessary to add a tick counter to reduce the frequency of outputted information as the update time (dt) is pretty fast.

### Text-To-Speech

To express most of the game data that we have been able to collect, we used a TTS software external to the game. We have created a class to call for this TTS' help in audio/tts.

ANOUK

### General race data

Our Challenger needs to access different data about the race, just like we do. From the kart.cpp file, which allows to access the current race the kart is in, we have retrieved info such as lap number and ranking of the current player. This was mainly done directly in Kart::update.

### Rallye system

We have designed a rallye system that resembles the ones one can find in a classic race game. The goal of this system is therefore to announce the characteristics of a turn in advance: direction, intensity, length. The rallye system has been entirely implemented in kart.cpp since it is dependent on the position of the current player.

Tracks are defined by an XML file, and we expected to perhaps find a precomputed version of the entire track. Unfortunately, it seems that no thing exists in the code, so we had to find a way to get the data by our own means.

The XML file describes nodes. Tracks are made of a fixed number of nodes, that can vary from track to track. These nodes define sectors that follow each other along the track. Using this information and existing methods, we were able to get the angle of the current sector with respect to the start line. By scanning through the entire track, we have computed the angle between a given sector and its direct successor. We were then left with a table indicating the relative angle between the current sector and the next one (Kart::computeRelativeAngle). 

We have associated an intensity with each angle in this table. The intensity of a turn would be defined by the intensities of sectors composing it (Kart::categorizeAngle). 

We have then looked for a way of categorizing the turns in a track, also using this angle table and the associated intensities. For instance, we wanted angles in different directions to trigger the creation of a new turn. We have created a state machine (Kart::categorizeAngle) to get all our requirements together. It takes into account several parameters, such as angle direction, intensity, number of angles with no intensity before that, etc. Once a turn was determined, its data was stored along the corresponding sectors for further access.

With this system, we have been able to get a written representation of the different turns (and also straight lines with Kart::categorizeStraightLines) along a track (Kart::scanTrackForRallye). We were then able to announce the info of the turn a few sectors ahead using TTS.

### Menu 

As we want our visually-impaired challenger to be as independant as possible, we had to understand how the menu navigation and selection was working. Our objective was to get the name of the widget the player is hovering upon. Then we wanted to give the title of the screen that we are on at the changing of screens. Our challenger wanted to be able to only navigate with the keyboard and no mouse.

The game is composed of many screens and sometimes dialog screens (stk-code\src\states_screens) which all have different layouts and elements. Most of the clickable / selectable elements of the screens are made from a GUI class called Widget in the (src/guiengine) directory.
Many more specific widgets were then made from this base class and all had different behaviour and purposes in (src/guiengine/widgets) directory. At first we thought we could just output the tag of a widget in this base class which would then spread to the whole game screens unfortunately a lot of weird behaviours were created from such modifications.

That is why it was necessary to understand as much as possible the working of the game menu navigation. We made many printf to the terminal to understand when the focus function was called and added booleans so that we output via speech the name of the widget when necessary.
The screen names were outputted from functions and files in the (stk-code\src\states_screens).

This menu navigation is far from being perfect as they are many different screens but it is sufficient for the basic task of launching a race.


To further improve we can modify the xml files of the screens (stk-code\data\gui\screens) and add or modify the text attribute of any button which could then be retrieved by the TTS system. 

For further improvements files and functions to check : 
Widget.hpp : 
- virtual EventPropagation focused(const int playerID, bool printout = true, bool changed_ribbon = true) where we actually use the TTS function. 
RibbonWidget.hpp : check the all the calls of "void updateSelection(bool to_print = true)" which is a function that calls the focused function just above or any other function that calls this function. 
Check how the screens are made with what kind of widgets and add a text attribute to a widget in the xml corresponding file if necessary.



### Track relative position

In order for our player to understand his position on the track we used the getRelativeDistanceToCenter() function from the TrackSector class (stk-code\src\tracks\track_sector.hpp) which give a float number between -1.0 and 1.0 according to how far the kart is from the middle of the track. It returns 0 if you are perfectly at the middle, -1.0 if you are at the far left of the track and 1.0 at the far right. 

### Additionnal Guidance 

Even with this track relative position information it was still challenging to understand where to go and at where the kart was aiming at. Inspired by the AI (or bot) implementation of supertuxkart we reused the findNonCrashingPoint_player() function (stk-code\src\karts\controller\skidding_ai.hpp). This function gives the point relative to the point the kart is actually at where the kart would stay on track if going in a straight line towards it. By retrieving aswell the front of the kart coordinates we can then compute the angle the front of the kart should aim at to stay on track. A function in the get_direction.hpp file : "int aimDirection(float kartX, float kartZ, float frontX, float frontZ, float aimX, float aimZ, float deg_low, float deg_high)" in "stk-code\src\utils\get_direction.hpp" returns a integer between 1 and 4 according to where the kart aims at and a threshold of degrees. 


### Additional ambiant sounds

In addition to vocal indications, we have added a few sounds to make the game more immersive. The files in themselves have been registered in the game through a specific file, and they are used in kart.cpp, in Kart::update.

We have added different horn sounds to be played either when the kart overtakes another kart, or when it is overtaken.

Real-time beeps indicate the intensity of a turn, using the sectors intensities defined earlier. One beep per sector is played, with higher-pitched beeps signaling a higher intensity.

### Additional control key

To enable a more customizable game, we decided to allocate a new key for the specific needs of our Challenger. The goal of this key is to give out information on whatever the Challenger believed necessary. 

To add a new key, we had to alter numerous files, so many that we could not recall exactly which ones. It was a real treasure hunt to find all the places we had to modify to get a new key to work.

### Communication with the Arduino

Depending on the operating system there is two different arduino_communication.hpp and .cpp files. These files allow the construction of the arduino object so that we can send strings to it for the vibratory feedback. It is necessary to add the corresponding com port your arduino is connected at in the kart.cpp file at the start of the "void Kart::init(RaceManager::KartType type)" function (stk-code\src\karts\kart.cpp).
The data sent to the arduino is defined and sent in kart.cpp and skidding_ai.cpp.