gmad
====

Garry's Mod Addon Creator and Extractor

Usage
=====

`gmad <command> <options>`

To create a new .gma file

`gmad.exe create -folder "C:\path\to\addon\folder\" -out "C:\where\to\save\file\out.gma"`

Compiling
=========

Compiling requires Bootil

https://github.com/garrynewman/bootil

You can either add the lib and include folder to your path, or copy all the files in, or set them on the premake command line..

`premake4 --outdir="bin/" --bootil_lib="c:/bootil/lib/" --bootil_inc="c:/bootil/include/" vs2010`

Or on linux

`premake4 --outdir="bin/" --bootil_lib="/whatevs/bootil/lib/" --bootil_inc="/whatevs/bootil/include/" gmake`
