type uchar;
type Rectangle;
type float;

--------------------
-- Surface Detection
--------------------

node NormRGB
  in (width: int param, height: int param, src: uchar[3*height*width*selectLineConstruction])
  out (dest: uchar[height*width*selectLineConstruction])
actor
  preesm(loop_fn="normRGB", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Threshold
  in (width: int param, height: int param, thresh : int param = 90, src: uchar[height*width*selectLineConstruction])
  out (dest: uchar[height*width*selectLineConstruction])
actor
  preesm(loop_fn="threshold", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node LineSurface
  in (width: int param, height: int param, src: uchar[height*width*selectLineConstruction])
  out ()
actor
  preesm(loop_fn="generateMaskSurface", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node LineDetection 
  in (width: int param = 640, height: int param = 480, thresh : int param = 90, selectLineConstruction : int param, src: uchar[3*height*width*selectLineConstruction])
  out()
fun 
  val dest1 = NormRGB width height src
  val dest2 = Threshold width height thresh dest1
  val _ = LineSurface width height dest2
end;

----------------
-- GMM Model----
----------------

node Split
  in (nbSlice: int param, width: int param, height: int param, src: uchar[3*height*width*(initDone)])
  out (dest: uchar[nbSlice*3*sliceHeight*width*(initDone)])
actor
  preesm(loop_fn="split", incl_file="Code/include/splitMerge.h", src_file="Code/src/splitMerge.c")
end;

node CvtColorHSV
  in (width: int param, height: int param, src: uchar[3*height*width*(initDone)])
  out (dest: uchar[3*height*width*(initDone)])
actor
  preesm(loop_fn="cvtColorHSV", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Likelihood
  in (width: int param, height: int param, data: uchar[3*height*width*(initDone)])
  out (likelihood: float[2*height*width*(initDone)])
actor
  preesm(loop_fn="calculate_likelihood",  init_fn="initGMM_Model", incl_file="Code/include/GMM.h", src_file="Code/src/GMM.c")
end;

node MaskCreation
  in (width: int param, height: int param, component_index: int param, likelihood: float[2*height*width*(initDone)])
  out (mask: uchar[height*width*(initDone)])
actor
  preesm(loop_fn="createMaskGMM_model", incl_file="Code/include/GMM.h", src_file="Code/src/GMM.c")
end;

node Merge
  in (nbSlice : int param, width: int param, height: int param, src: uchar[nbSlice*sliceHeight*width*(initDone)])
  out (dest: uchar[height*width*(initDone)])
actor
  preesm(loop_fn="mergeGray", incl_file="Code/include/splitMerge.h", src_file="Code/src/splitMerge.c")
end;

node ApplyMaskSurface
  in (width: int param, height: int param, src: uchar[nbSlice*sliceHeight*width*(initDone)])
  out (dest: uchar[height*width*(initDone)])
actor
  preesm(loop_fn="applyMaskSurface", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node BoxConstruction
  in (width: int param, height: int param, src: uchar[height*width*(initDone)])
  out (bb: Rectangle[initDone])
actor
  preesm(loop_fn="boxConstruction", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Algo_GMM_parallelism_dynamic
  in (src: uchar[3*height*width*initDone], initDone : int param, width: int param, height: int param, sliceHeight: int param, nbSlice : int param = 4, component_index : int param)
  out(bb : Rectangle[initDone])
fun 
  val slice = Split nbSlice width height src
  val dest1 = CvtColorHSV width sliceHeight slice
  val likelihood = Likelihood width sliceHeight dest1
  val dest2 = MaskCreation width sliceHeight component_index likelihood
  val dest3 = Merge nbSlice width height dest2
  val bb = BoxConstruction width height dest3
end;

-------------------
-- Top Graph ------
-------------------

node ReadFrame
  in (width: int param, height: int param)
  out (pixels: uchar[3*height*width])
actor
  preesm(loop_fn="mp4Read", init_fn="initMp4Read", incl_file="Code/include/mp4Read.h", src_file="Code/src/mp4Read.c")
end;

node Selector 
  in (nb_frames_init : int param) 
  out(initDone: int param, selectLineConstruction: int param)
actor
  preesm(loop_fn="selector", incl_file="Code/include/switch.h", src_file="Code/src/switch.c")
end;

node AlgoPath
  in (width: int param, height: int param, initDone : int param = 0, src: uchar[3*height*width]) 
  out (dest1: uchar[3*height*width*(1-initDone)], dest2: uchar[3*height*width*(initDone)])
actor
  preesm(loop_fn="conditinalActor_2", incl_file="Code/include/conditinalActor.h", src_file="Code/src/conditionalActor.c")
end;

node LineConstructionPath
  in (width: int param, height: int param, initDone : int param, selectLineConstruction : int param = 0, src: uchar[3*height*width*(1-initDone)]) 
  out (dest :uchar[3*height*width*(selectLineConstruction)])
actor
  preesm(loop_fn="conditinalActor_1", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Variance
  in (width: int param, height: int param, initDone : int param, src: uchar[3*height*width*(1-initDone)]) 
  out (dest: uchar[3*height*width*selectLineConstruction])
actor
  preesm(loop_fn="varianceIter", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node DisplayFrame
  in (width: int param, height: int param, pixels: uchar[3*height*width])
  out ()
actor
  preesm(loop_fn="mp4Display", init_fn="initMp4Display", incl_file="Code/include/mp4Read.h", src_file="Code/src/mp4Read.c")
end;

node DrawRectangle
  in (width: int param, height: int param, initDone : int param, src: uchar[3*height*width*initDone], bb : Rectangle) 
  out (dest: uchar[3*height*width*initDone])
actor
  preesm(loop_fn="drawRectangle", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

graph top_Algo_GMM_dynamic
  in (width: int param = 640, height: int param = 480, thresh : int param = 90, sliceHeight : int param = 122, nb_frames_init : int param, nbSlice : int param = 4, component_index : int param = 1) 
  out ()
fun
  val pixels = ReadFrame width height
  val initDone, selectLineConstruction = Selector nb_frames_init
  val o0, o1 = AlgoPath width height initDone pixels

  -- Path Initialization
  val var = Variance width height initDone o0
  val o0_1 = LineConstructionPath width height initDone selectLineConstruction var
  val _ = LineDetection width height thresh selectLineConstruction o0_1 

  -- Path Swimmer Detection
  val bb = Algo_GMM_parallelism_dynamic pixels initDone width height sliceHeight nbSlice component_index
  val dest = DrawRectangle width height initDone pixels bb
  val _ = DisplayFrame width height dest
end;