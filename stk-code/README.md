# ATC Project: Adapting SuperTuxKart for a visually impaired player

To access the official README of the game, please go to OFFICIALSTK_README.md.

This project has been conducted by Louis Duval, Anouk Paulino, Laetitia Schwitter and Marianne Scoglio, in the context of EPFL Assistive Technologies Challenge. The additional code has been produced by Louis Duval and Anouk Paulino.

## Introduction

In order to make the game accessible to our Challenger, we have added several features to allow us to retrieve important information about the current state of the game. In that way, we were able to transmit this information to the user via means other than visual.

Most of what we have added was directly related to the kart and therefore is located in the corresponding file. We will specify each time where the modification has been made.

In the first two weeks of getting to know SuperTuxKart, most of our time was spent wandering around the different files to understand their purpose and see of what use they could be to us. 

## Additional features

### Text-To-Speech

To express most of the game data that we have been able to collect, we used a TTS software external to the game. We have created a class to call for this TTS' help in audio/tts.

ANOUK

### General race data

Our Challenger needs to access different data about the race, just like we do. From the kart.cpp file, which allows to access the current race the kart is in, we have retrieved info such as lap number and ranking of the current player.

### Rallye system

We have designed a rallye system that resembles the ones one can find in a classic race game. The goal of this system is therefore to announce the characteristics of a turn in advance: direction, intensity, length. The rallye system has been entirely implemented in kart.cpp since it is dependent on the position of the current player.

Tracks are defined by an XML file, and we expected to perhaps find a precomputed version of the entire track. Unfortunately, it seems that no thing exists in the code, so we had to find a way to get the data by our own means.

The XML file describes nodes. Tracks are made of a fixed number of nodes, that can vary from track to track. These nodes define sectors that follow each other along the track. Using this information and existing methods, we were able to get the angle of the current sector with respect to the start line. By scanning through the entire track, we have computed the angle between a given sector and its direct successor. We were then left with a table indicating the relative angle between the current sector and the next one. 

We have associated an intensity with each angle in this table. The intensity of a turn would be defined by the intensities of sectors composing it. 

We have then looked for a way of categorizing the turns in a track, also using this angle table and the associated intensities. For instance, we wanted angles in different directions to trigger the creation of a new turn. We have created a state machine to get all our requirements together. It takes into account several parameters, such as angle direction, intensity, number of angles with no intensity before that, etc. Once a turn was determined, its data was stored along the corresponding sectors for further access.

With this system, we have been able to get a written representation of the different turns (and also straight lines) along a track. We were then able to announce the info of the turn a few sectors ahead using TTS.

### Menu

LOUIS

### Guidance (test AI thing)

LOUIS

### Anything else?

### Additional ambiant sounds

In addition to vocal indications, we have added a few sounds to make the game more immersive. The files in themselves have been registered in the game through a specific file, and they are used in kart.cpp.

We have added different horn sounds to be played either when the kart overtakes another kart, or when it is overtaken.

Real-time beeps indicate the intensity of a turn, using the sectors intensities defined earlier. One beep per sector is played, with higher-pitched beeps signaling a higher intensity.

### Additional control key

To enable a more customizable game, we decided to allocate a new key for the specific needs of our Challenger. The goal of this key is to give out information on whatever the Challenger believed necessary. 

To add a new key, we had to alter numerous files, so many that we could not recall exactly which ones. It was a real treasure hunt to find all the places we had to modify to get a new key to work.