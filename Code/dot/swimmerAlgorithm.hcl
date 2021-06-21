type uchar;

node ReadMP4
  in (width: int param, height: int param, video_file: int param, pix_fmt : int param)
  out (pixels: uchar[3*height*width], output2: uchar[3*height*width])
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

node CvtColor
  in (width: int param, height: int param, src: uchar[3*height*width])
  out (dest: uchar[3*height*width])
actor
  systemc(loop_fn="cvtColorHSV", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
  preesm(loop_fn="cvtColorHSV", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
end;

node MedianFilter
  in (width: int param, height: int param, nb_channels : int param, src: uchar[3*height*width])
  out (dest: uchar[3*height*width])
actor
  systemc(loop_fn="medianFilter", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
  preesm(loop_fn="medianFilter", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
end;

node MaskCreation
  in (width: int param, height: int param, src: uchar[3*height*width])
  out (dest: uchar[height*width])
actor
  systemc(loop_fn="maskCreationSimple", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
  preesm(loop_fn="maskCreationSimple", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
end;

node BoxConstruction
  in (width: int param, height: int param, src: uchar[height*width])
  out (x_rect: int param, y_rect: int param, width_rect: int param, height_rect: int param)
actor
  systemc(loop_fn="boxConstruction", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
  preesm(loop_fn="boxConstruction", incl_file="include/ImageWrapper.h", src_file="src/ImageWrapper.cpp")
end;

node Resize
  in (width: int param, height: int param, width_final: int param, height_final: int param, src: uchar[3*height*width], x_rect: int param, y_rect: int param, width_rect: int param, height_rect: int param)
  out (dest: uchar[3*height_final*width_final])
actor
  systemc(loop_fn="resize", incl_file="include/resize.h", src_file="src/resize.c")
  preesm(loop_fn="resize", incl_file="include/resize.h", src_file="src/resize.c")
end;

graph main
  in (width: int param = 640, height: int param = 360, width_final: int param = 224, height_final: int param = 224, nb_channels : int param = 3, video_file : int param = 1, pix_fmt : int param = 0)
  out () 
fun
  val (pixels, output2) = ReadMP4 width height video_file pix_fmt
  val dest = CvtColor width height pixels
  val dest = MedianFilter width height nb_channels dest
  val dest = MaskCreation width height dest
  val x_rect, y_rect, width_rect, height_rect = BoxConstruction width height dest
  val dest = Resize width height width_final height_final output2 x_rect y_rect width_rect height_rect
  val _ = DisplayMP4 width_final height_final dest
end;