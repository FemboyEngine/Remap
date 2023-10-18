/*
 *  Remap - A Memory Alteration Utility
 *
 *  Remap is an open-source memory alteration utility designed to provide users
 *  with powerful tools for interacting with memory and processes. This software
 *  is distributed under the GNU General Public License (GPL) and is free to
 *  use, modify, and distribute under the terms and conditions of the GPL.
 *  Please refer to the 'LICENSE' file for the full text of the GNU GPL License.
 *
 *  File: main.cpp
 *  Description: The entry point of the Remap utility.
 *  This file initializes the renderer, creates the main window, and handles
 *  the main loop.
 *
 *  GitHub: https://github.com/FemboyEngine/remap
 */

#include "interface/renderer.h"

int main() {
    RenderWindow window(1280, 720, "Remap");
    window.Render();

    return 0;
}