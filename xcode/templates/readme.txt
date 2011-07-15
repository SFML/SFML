XCODE 4 TEMPLATES
=================

These are templates to create easily a new project in Xcode 4. 

Features
--------

 * You can choose between command line tool or bundle application.
 * You can select or not each module of SFML you'll use into your project.
 * A basic example is inserted automatically into your project's code.

Install
-------

Copy the four folders into ~/Library/Developer/Xcode/Templates folder (you might need to create it first).

Usage
-----

To use these templates follow these steps :

 * open Xcode 4, 
 * select «create a new Xcode project» from the «Welcome to Xcode» window or select menu File > New > New Project, 
 * select «Templates» under «Mac OS X»,
 * then select either «SFML Application» or «SFML Command Line Tool»,
 * fill in the requested information and you're done.

Note
----

If you wish to add/remove any module of SFML from your project without rebuilding a new one follow these steps :

 * select your project from the project navigator panel (cmd+1),
 * select your project's target on the main area,
 * go to the «Build Settings» tab,
 * go down to the bottom,
 * edit any MODULEX_CONFIG variable (e.g. AUDIO_DEBUG, NETWORK_RELEASE) you want to.

Examples : 

 * to disable the audio module simply erase the content of AUDIO_DEBUG and AUDIO_RELEASE.
 * to add the network module set NETWORK_DEBUG to '-lsfml-network-d' and NETWORK_RELEASE to '-lsfml-network'.
