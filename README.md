# ATC Project: Adapting SuperTuxKart for a visually impaired player

To access the official README of the game, please go to ``stk-code/OFFICIALSTK_README.md``.

This project has been conducted by Louis Duval, Anouk Paulino, Laetitia Schwitter and Marianne Scoglio, in the context of EPFL Assistive Technologies Challenge. The additional code has been produced by Louis Duval and Anouk Paulino.

## Introduction

In order to make the game accessible to our Challenger, we have added several features to allow us to retrieve important information about the current state of the game. In that way, we were able to transmit this information to the user via means other than visual.

Most of what we have added was directly related to the kart and therefore is located in the corresponding file. We will specify each time where the modification has been made.

In the first two weeks of getting to know SuperTuxKart, most of our time was spent wandering around the different files to understand their purpose and see of what use they could be to us. 

## Additional features

If you want to check any code modification from the original game you can compare to the version of STK of the 20th of March 2024 (or sometime around it) : https://github.com/supertuxkart/stk-code
You can also look for commented printf or cout which will often be were we made additions.
It was necessary to add a tick counter to reduce the frequency of printed (for debugging) information as the update time (dt or ticks) is pretty fast.

### Text-To-Speech (TTS)

To express most of the game data that we have been able to collect, we used a TTS software external to the game. We have created a class to call for this TTS' help in ``audio/tts``.

To make sure the TTS does not freeze the game, we had to run it in different threads. This process made it difficult to ensure that no two voices were speaking at the same time. We had to come up with a system of queues and locks to make sure that threads would not speak at the same time and confuse the user. All the related classes are also in ``audio/tts``.

### General race data

Our Challenger needs to access different data about the race, just like we do. From the ``kart.cpp`` file, which allows to access the current race the kart is in, we have retrieved info such as lap number and ranking of the current player. This was mainly done directly in ``Kart::update(...)``.

Specifically, we have collected the number of lap, rank, whether the player was out of the road, whether the player was in a wall (edge of road + no speed), whether the player has just been rescued.

One other information was collected in ``linear_world.cpp``: whether the player is in the wrong way.

### Rallye system

We have designed a rallye system that resembles the ones one can find in a classic race game. The goal of this system is therefore to announce the characteristics of a turn in advance: direction, intensity, length. The rallye system has been entirely implemented in kart.cpp since it is dependent on the position of the current player.

Tracks are defined by an XML file, and we expected to perhaps find a precomputed version of the entire track. Unfortunately, it seems that no thing exists in the code, so we had to find a way to get the data by our own means.

The XML file describes nodes. Tracks are made of a fixed number of nodes, that can vary from track to track. These nodes define sectors that follow each other along the track. Using this information and existing methods, we were able to get the angle of the current sector with respect to the start line. By scanning through the entire track, we have computed the angle between a given sector and its direct successor. We were then left with a table indicating the relative angle between the current sector and the next one (``Kart::computeRelativeAngle(...)``). 

We have associated an intensity with each angle in this table. The intensity of a turn would be defined by the intensities of sectors composing it (``Kart::categorizeAngle(...)``). 

We have then looked for a way of categorizing the turns in a track, also using this angle table and the associated intensities. For instance, we wanted angles in different directions to trigger the creation of a new turn. We have created a state machine (``Kart::categorizeAngle(...)``) to get all our requirements together. It takes into account several parameters, such as angle direction, intensity, number of angles with no intensity before that, etc. Once a turn was determined, its data was stored along the corresponding sectors for further access.

With this system, we have been able to get a written representation of the different turns (and also straight lines with ``Kart::categorizeStraightLines()``) along a track (``Kart::scanTrackForRallye(...)``). We were then able to announce the info of the turn a few sectors ahead using TTS.

### Menu

As we want our visually-impaired challenger to be as independent as possible, we needed to understand how the menu navigation and selection worked. Our objective was to get the name of the widget the player is hovering over. Then, we wanted to give the title of the screen when a new screen was initialized. Our challenger wanted to navigate using only the keyboard, with no mouse.

The game consists of many screens and sometimes dialog screens, located in `stk-code/src/states_screens`, which all have different layouts and elements. Most of the clickable or selectable elements of the screens are made from a GUI class called Widget in the `src/guiengine` directory. Many more specific widgets were then made from this base class, all having different behaviors and purposes, located in the `src/guiengine/widgets` directory. At first, we thought we could simply output the tag of a widget in this base class, which would then spread to all game screens. Unfortunately, this approach created many weird behaviors.

Therefore, it was necessary to understand the workings of the game menu navigation as thoroughly as possible. We used many printf statements to the terminal to understand when the `focused()` function (see the "further improvements" section) was called and added booleans to output via speech the name of the widget when necessary. The screen names were output from functions and files in the `stk-code/src/states_screens` directory.

This menu navigation is far from perfect, as there are many different screens, but it is sufficient for the basic task of launching a race. There is still some widgets names that are returned multiples times or some that don't work perfectly. 

To further improve, we can modify the XML files of the screens (`stk-code/data/gui/screens`) and add or modify the text attribute of any button, which could then be retrieved by the TTS system.

**Further improvements**

Files and functions to check : 

- **Widget.hpp** : `virtual EventPropagation focused(const int playerID, bool printout = true, bool changed_ribbon = true)` where we actually use the TTS function for all widgets. 

- **RibbonWidget.hpp** : check all the calls of "void updateSelection(bool to_print = true)" which is a function that calls the focused function just above. Other functions may calls this function aswell. 

- **states_screens directory** : Check how the screens are made with what kind of widgets to adapt and add a text attribute to a widget in the xml corresponding file if necessary.

Tuning the booleans according to the screens or adding logic to some calls would allow a better experience, even if it is not really future-proof for upcoming game updates.


### Track relative position

In order for our player to understand their position on the track, we used the `getRelativeDistanceToCenter()` function from the `TrackSector` class (`stk-code/src/tracks/track_sector.hpp`). This function returns a float number between -1.0 and 1.0, indicating how far the kart is from the middle of the track. It returns 0 if the kart is perfectly in the middle, -1.0 if it is at the far left of the track, and 1.0 if it is at the far right. This was implemented inside the `void Kart::update(int ticks)` function, which includes a significant portion of our race modifications.

### Additionnal Guidance 

Even with this track-relative position information, it was still challenging to understand where to go and where the kart was aiming. Inspired by the AI (or bot) implementation of SuperTuxKart, we reused the `findNonCrashingPoint()` function (`stk-code/src/karts/controller/skidding_ai.hpp`) with a new one `findNonCrashingPoint_player()`. This function determines the point, relative to the kart's position, where the kart would remain on track if it continued in a straight line towards that point. By also retrieving the front coordinates of the kart, we can compute the angle the front of the kart should aim at to stay on track. A function in the `get_direction.hpp` file: `int aimDirection(float kartX, float kartZ, float frontX, float frontZ, float aimX, float aimZ, float deg_low, float deg_high)` in `stk-code/src/utils/get_direction.hpp` returns an integer between 1 and 4 according to where the kart aims and a threshold of degrees.

### Additional ambiant sounds

In addition to vocal indications, we have added a few sounds to make the game more immersive. The files in themselves have been registered in the game through a specific file, and they are used in ``Kart::update(...)``, located in ``kart.cpp``.

We have added different horn sounds to be played either when the kart overtakes another kart, or when it is overtaken.

Real-time beeps indicate the intensity of a turn, using the sectors intensities defined earlier. One beep per sector is played, with higher-pitched beeps signaling a higher intensity.

### Additional control key

To enable a more customizable game, we decided to allocate a new key for the specific needs of our Challenger. The goal of this key is to give out information on whatever the Challenger believed necessary. 

To add a new key, we had to alter numerous files, so many that we could not recall exactly which ones. It was a real treasure hunt to find all the places we had to modify to get a new key to work.

The key could virtually serve any purpose. The most interesting that we could do within the time we had was to allow the player to get the information of the current turn once again. 

### No objects

We disabled objects so as not to favor any non-visually impaired player. We have not had the time to add feedback for objects so this was the easy way. To do so, we have changed the default value of object to ``false``.

### Communication with the Arduino for vibratory feedback

Depending on the operating system, there are two different `arduino_com` files: `arduino_com_windows.hpp`/`.cpp` and `arduino_com_linux.hpp`/`.cpp`. These files enable the construction of the Arduino object, allowing us to send strings to the Arduino for vibratory feedback. It is necessary to add the corresponding COM port your Arduino is connected to in the `kart.cpp` file at the start of the `void Kart::init(RaceManager::KartType type)` function (`stk-code/src/karts/kart.cpp`, line 261). The data sent to the Arduino is defined and sent in `kart.cpp` and `skidding_ai.cpp`.

### Arduino Code for vibratory feedback

In the `STK_GetData.ino` file there is the arduino code that interprets the data sent from the game. It activates the vibrators according to the data received from the game with different parameters.
You may tune the parameters such as the pulse duration, frequency and intensity of the vibrations according to your wishes.