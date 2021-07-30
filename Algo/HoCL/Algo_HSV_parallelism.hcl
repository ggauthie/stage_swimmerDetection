type uchar;
type Rectangle;

----------------
-- HSV Segmentation----
----------------

node Split
  in (width: int param, height: int param, nbSlice: int param, src: uchar[3*height*width])
  out (dest: uchar[nbSlice*3*(height/nbSlice + 2)*width])
actor
  preesm(loop_fn="split", incl_file="Code/include/splitMerge.h", src_file="Code/src/splitMerge.c")
end;

node CvtColorHSV
  in (width: int param, height: int param, src: uchar[3*height*width])
  out (dest: uchar[3*height*width])
actor
  preesm(loop_fn="cvtColorHSV", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node SegmentationHSV
  in (width: int param, height: int param, src: uchar[3*height*width])
  out (likelihood: uchar[2*height*width])
actor
  preesm(loop_fn="segmentationHSV", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Merge
  in (width: int param, height: int param, nbSlice : int param, src: uchar[nbSlice*3*(height/nbSlice+2)*width])
  out (dest: uchar[3*height*width])
actor
  preesm(loop_fn="merge", incl_file="Code/include/splitMerge.h", src_file="Code/src/splitMerge.c")
end;

node BoxConstruction
  in (width: int param, height: int param, src: uchar[height*width])
  out (bb: Rectangle)
actor
  preesm(loop_fn="boxConstruction", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;

node Algo_HSV_parallelism
  in (src: uchar[3*height*width], width: int param, height: int param, sliceHeight: int param, nbSlice : int param = 4, component_index : int param)
  out(bb : Rectangle)
fun 
  val slice = Split width height nbSlice src
  val dest1 = CvtColorHSV width sliceHeight slice
  val mask_hsv = SegmentationHSV width height dest1
  val dest3 = Merge width height nbSlice mask_hsv
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

node DisplayFrame
  in (width: int param, height: int param, pixels: uchar[3*height*width])
  out ()
actor
  preesm(loop_fn="mp4Display", init_fn="initMp4Display", incl_file="Code/include/mp4Read.h", src_file="Code/src/mp4Read.c")
end;

node DrawRectangle
  in (width: int param, height: int param, bb : Rectangle, src: uchar[3*height*width]) 
  out (dest: uchar[3*height*width])
actor
  preesm(loop_fn="drawRectangle", incl_file="Code/include/ImageWrapper.h", src_file="Code/src/ImageWrapper.cpp")
end;


graph top_Algo_GMM_parallelism
  in (width: int param = 640, height: int param = 480, sliceHeight : int param = 122, nbSlice : int param = 4, component_index : int param = 1) 
  out ()
fun
  val pixels = ReadFrame width height

  -- Path Swimmer Detection
  val bb =  Algo_HSV_parallelism pixels width height sliceHeight nbSlice component_index
  val dest = DrawRectangle width height bb pixels
  val _ = DisplayFrame width height dest
end;