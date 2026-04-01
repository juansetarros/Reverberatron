# Reverberatron Beta

## Overview
Reverb Plugin with not so common parameters such as Volume of the Room, whilst other plugins let you choose between rooms, halls, or cathedrals, this one lets you size it yourself as if you were transforming the room. 

Example:
A creative audio effect plugin built with JUCE that allows users to add reverb to any signal, adjust parameters to fit in your mix, and transform to the sound you're looking for.

---

## Features
- A RT Knob to adjust the reverb time
- A Volume of the room knob designed to adjust the size of the room instead of just selecting a preset room
- A lowpass filter knob to clean the top end
- A dry/wet slider to control the amount of reverb you want mixed with your signal (Don't worry, reverberatron won't bite you for messing with his face)

---

## Tech Stack
- C++
- JUCE Framework
- VST3 SDK

---

## Download
- Available in the Releases section

---

## Installation (Windows)
1. Download the latest release
2. Copy `.vst3` file to:
   C:\Program Files\Common Files\VST3\
3. Open your DAW and rescan plugins if necessary

---

## Tested On
- Reaper

---

## For Developers
1. Open the `.jucer` file using Projucer
2. Export project
3. Build in Release mode

---

## Screenshots
<img width="499" height="338" alt="image" src="https://github.com/user-attachments/assets/12fa81d0-67b7-4665-931c-9e81e0e017ec" />


---

## License
MIT License
