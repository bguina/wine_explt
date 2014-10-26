#!/bin/bash

LD_PRELOAD="`pwd`/`dirname "$0"`/zuberwine.so" PYTHONPATH=. WINEDEBUG=-all wine "/home/guina_b/.wine/drive_c/Program Files/Diablo III/Diablo III.exe" -launch; pkill wineserver

