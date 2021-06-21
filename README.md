# stage_swimmerDetection
Program used to detect a swimmer in a video.

## Pre-requisites 

- SDL2
- PThreads
- OpenCV
- ffmpeg
- graphviz

## Compilation

### Application


`` mkdir build && cd build``
`` cmake ..``
`` make``

### Generation of graph with dot

Go to /Code/dot and 
``make``
``cd dot``
``dot -Tpng ./swimmerAlgorithm_main.dot > graphAlgo.png``

## Execution

### Application

``./Release/SwimmerDetection_c``


