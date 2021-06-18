type uchar;

node ReadMP4
  in (width: int param, height: int param)
  out (pixels: uchar[3*height*width])
actor
  systemc(loop_fn="mp4Read", init_fn="initMp4Read", incl_file="include/mp4Read.h", src_file="src/mp4Read.c")
  preesm(loop_fn="mp4Read", init_fn="initMp4Read", incl_file="include/mp4Read.h", src_file="src/mp4Read.c")
end;

node DisplayMP4
  in (width: int param, height: int param, pixels: uchar[3*height*width])
  out ()
actor
  systemc(loop_fn="mp4Display", init_fn="initMp4Display", incl_file="include/mp4Read.h", src_file="src/mp4Read.c")
  preesm(loop_fn="mp4Display", init_fn="initMp4Display", incl_file="include/mp4Read.h", src_file="src/mp4Read.c")
end;

node SwimmerDetection
  in (width: int param, height: int param, src: uchar[3*height*width])
  out (dest: uchar[3*height*width])
actor
  systemc(loop_fn="swimmerAlgo", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.c")
  preesm(loop_fn="swimmerAlgo", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.c")
end;

graph main
  in (width: int param = 640, height: int param = 360)
  out () 
fun
  val (pixels) = ReadMP4 width height
  val dest = SwimmerDetection width height pixels 
  val _ = DisplayMP4 width height dest
end;