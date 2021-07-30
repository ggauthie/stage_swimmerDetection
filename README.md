# Swimmer Detection
Program used to detect a swimmer in a mp4 video file .

## Pre-requisites 

- SDL2
- PThreads
- OpenCV
- ffmpeg
- graphviz

## Application

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

### Generation of graph dot with HoCL

Go to Algo/HoCL and 
``make``
``cd dot``
``dot -Tpng ./<filename>.dot > <filename>.png``.

### Generation of graph .pi for PREESM with HoCL

Go to Algo/HoCL and 

``make SRCS='<name_of_the_hcl_file' preesm`

Then open your .pi files in Preesm.

## Result with the 3 combined algorithm(GMM conditional)

### VIDEO 1
![Video 1](Code/data/Video_1.gif) 

### VIDEO 2
![Video 2](Code/data/video_2.gif)

### VIDEO 3
![Video 3](Code/data/video_3.gif)



