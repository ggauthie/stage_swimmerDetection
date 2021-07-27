# stage_swimmerDetection
Program used to detect a swimmer in a video file mp4.

## Pre-requisites 

- SDL2
- PThreads
- OpenCV
- ffmpeg
- graphviz

## Compilation

### Application

Open the project in Preesm : https://github.com/preesm/preesm
Different algorithms are available :

**STATIC :**

- FullAlgo_GMM_parallelism
- FullAlgo_HSV_parallelism
- FullAlgo_YUV_parallelism

Choose one of the algorithms in the *1/4core.scenario* file.
Run the workflow CodeGen.workflow selecting the 1core.scenario or 4core.scenario.
Run `cmake -DSTATIC=ON .` in /Code.
Run `make && cd /Release`
Run `./Swimmer_Detection_c`

**DYNAMIC :**

- Full_Algo_GMM_dynamic
- Full_Algo_YUV_dynamic
- Full_Algo_HSV_dynamic
- Full_Algo_Combined(pooling 3 boxes)
- Full_Algo_Combined_2(pooling 2 boxes and active GMM depending on iou)

Choose one of the algorithms in the *1core.scenario* file.
Run the workflow SpiderCodeGen.workflow selecting the 1core.scenario.
Run `cmake -DSTATIC=OFF .` in /Code.
Run `make && cd /Release`
Run `./Swimmer_Detection_c`

### Generation of graph with HoCL

Go to /Code/Algo/HoCL and 
``make``
``cd dot``
``dot -Tpng ./swimmerAlgorithm_main.dot > graphAlgo.png``

![ Alt text](Code/data/video_1.gif) / ! [](Code/data/video_1.gif)



