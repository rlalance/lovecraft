# lovecraft

## Minimum Software Requirements
- cmake version 3.1.0
- ruby 2.1.5p273
- youiengine 4.1.4
- vs2013 community edition

## Project/Solution generation using cmake
> lovecraft\build\vs2013\build.bat

## Adding source files and groups
- To add a new group add a source_group cmake instruction

> source_group("Quest"    REGULAR_EXPRESSION "quest/.*")

## Symbolic link to the youiengine folder

> mklink /J youiengine E:\Projets\youiengine

