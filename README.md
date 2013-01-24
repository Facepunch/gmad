gmad
====

Garry's Mod Addon Creator and Extractor

Usage
=====

`gmad <command> <options>`

To create a new .gma file

`gmad.exe create -folder "C:\path\to\addon\folder\" -out "C:\where\to\save\file\out.gma"`

To extract an existing .gma file into its parent folder

`gmad.exe extract -file "C:\steam\etc\garrysmod\addons\my_addon_12345.gma"`

To extract an existing .gma file into another folder

`gmad.exe extract -file "C:\steam\etc\garrysmod\addons\my_addon_12345.gma" -out "C:\this\folder"`

Compiling
=========

Compiling requires Bootil

https://github.com/garrynewman/bootil

You can either add the lib and include folder to your path, or copy all the files in, or set them on the premake command line..

`premake4 --outdir="bin/" --bootil_lib="c:/bootil/lib/" --bootil_inc="c:/bootil/include/" vs2010`

Or on linux

`premake4 --outdir="bin/" --bootil_lib="/whatevs/bootil/lib/" --bootil_inc="/whatevs/bootil/include/" gmake`
