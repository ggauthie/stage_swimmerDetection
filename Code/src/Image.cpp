//
// Created by ggauthie on 01/06/2021.
//

#include "../include/Image.h"
#include <jsoncpp/json/json.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "string.h"

#define NB_FRAMES 121

using namespace std;
using namespace saliency;
using namespace xt;

static int ind = 0;

int const scalar1 = 179,scalar2 = 210,scalar3 = 110;

static unsigned char maskSurface[360*640];
unsigned char imgs[100*360*640*3];

void cannyMethod_cpp(unsigned char *y, unsigned char* dest, int width, int height)
{
    Mat canny_output, src_gray;
    //convert unsigned char* to Mat & gray conversion
    src_gray=Mat(height,width,CV_8UC1, y);
    // Blur Added
    blur( src_gray, src_gray, Size(3,3) );
    // Edge detection
    Canny( src_gray, canny_output, 100, 100*2 );
    //Contours Listing
    vector<vector<Point> > contours;
    findContours( canny_output, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    //Algorithm which detects the top-left and bottom-right points

    int x_br = 0;
    int y_br = 0;
    int x_tl = width;
    int y_tl = height;

    for(auto & contour : contours)
    {
        auto rect = boundingRect( contour );
        if(rect.tl().x< x_tl)
            x_tl = rect.tl().x;

        if(rect.tl().y< y_tl)
            y_tl = rect.tl().y;

        if(rect.br().x> x_br)
            x_br= rect.br().x;

        if(rect.br().y> y_br)
            y_br= rect.br().y;
    }

    //draw the rectangle on the source image
    rectangle( src_gray, Point(x_tl,y_tl), Point(x_br, y_br), Scalar(0,0,0), 2 );

    //Load the matrix values in an uchar tab
    dest = src_gray.data;
}

void otsuMethod_cpp(unsigned char *pixels, unsigned char* dest, int width, int height)
{

    Mat src = Mat(height,width,CV_8UC3, pixels);
    Mat im_gray, result;

    cvtColor(src, im_gray, COLOR_RGB2GRAY);
    auto detect_window = "Detection";
    namedWindow( detect_window );
    imshow( detect_window , im_gray);
    waitKey(0);
    threshold(im_gray, result, 0, 255, THRESH_BINARY | THRESH_OTSU);

    imshow( detect_window , result);
    waitKey(0);
    dest = result.data;
}
/* Dynamic Parameters*/

vector<Rect> readJson_cpp(string path)
{
    ifstream ifs(path);
    vector<Rect> tab_rect;
    if(!ifs)
    {
        cout << "Error opening json file" << endl;
        exit(1);
    }
    else
    {
        Json::Reader reader;
        Json::Value obj;
        reader.parse(ifs,obj);
        for(int i =0; i< obj["annotations"].size(); ++i)
        {
            Rect temp;
            temp.x = stoi(obj["annotations"][i]["bbox"][0].asString());
            temp.y = stoi(obj["annotations"][i]["bbox"][1].asString());
            temp.width = stoi(obj["annotations"][i]["bbox"][2].asString());
            temp.height = stoi(obj["annotations"][i]["bbox"][3].asString());

            tab_rect.push_back(temp);
        }
        return tab_rect;
    }
}

/*static void on_trackbar_mask(int, void*)
{
    inRange(img_hsv, Scalar(0,0,0), Scalar(scalar1, scalar2, scalar3), maskHSV);
    bitwise_and(maskHSV,maskHSV ,maskFinal ,maskSurface);

    //Image segmentation
    bitwise_and(img_src,img_src,img_segmented ,maskFinal);
    imshow( "Linear Blend", img_segmented);
}
*/
void medianFilter_cpp(unsigned char *src, unsigned char *dest, int width, int height, int nb_channels)
{
    Mat img_blur, img_src;
    if(nb_channels == 3)
    {
        img_src = Mat(height,width, CV_8UC3, src);
        medianBlur(img_src,img_blur, 9);
        memcpy(dest, img_blur.data, 3*height*width);
    }
    else if(nb_channels == 1)
    {
        img_src = Mat(height,width, CV_8UC1, src);
        medianBlur(img_src,img_blur, 9);
        memcpy(dest, img_blur.data, height*width);
    }
    else
    {
        cout << "Nb channels != 1 or 3" << endl;
        exit(1);
    }

}

void cvtColorHSV_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
    Mat img_hsv;
    Mat img_src(height,width, CV_8UC3, src);
    cvtColor(img_src,img_hsv, COLOR_RGB2HSV);
    memcpy(dest, img_hsv.data, 3*height*width);

}

void maskCreation_cpp(unsigned char *src, unsigned char* mask, unsigned char *dest, int width, int height)
{
    // Mask creation generated by the init function
    Mat maskSurface = Mat(height,width,CV_8UC1,mask);
    Mat mask_hsv, maskFinal;
    Mat img_src(height,width, CV_8UC3, src);
    //Filter the 3 components H,S,V
    inRange(img_src, Scalar(0,0,0), Scalar(scalar1, scalar2, scalar3), mask_hsv);
    //Combine the 2 masks
    bitwise_and(mask_hsv, mask_hsv, maskFinal ,maskSurface);
    memcpy(dest, maskFinal.data, height*width);
}

void maskCreationSimple_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
    Mat mask_hsv;
    Mat img_src(height,width, CV_8UC3, src);
    //Filter the 3 components H,S,V
    inRange(img_src, Scalar(0,0,0), Scalar(scalar1, scalar2, scalar3), mask_hsv);
    //Combine the 2 masks
    memcpy(dest, mask_hsv.data, height*width);
}

void boxConstruction_cpp(unsigned char* src, int width, int height, int *x_rect,
                         int *y_rect, int *width_rect, int *height_rect)
{
    //Box construction
    vector<vector<cv::Point>> contours;
    //Transform uchar tab mask in Mat
    Mat img_src(height,width, CV_8UC1, src);
    Rect rect_in(0,0,0,0);

    findContours(img_src, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    for( auto i = 0; i < contours.size(); i++ )
    {
        auto rect = boundingRect( contours[i] );
        if(contourArea(contours[i]) > 500 && rect.height > 5 && contourArea(contours[i])<150000)
        {
            rect_in = Rect(rect);
        }
    }
    *x_rect=rect_in.x;
    *y_rect=rect_in.y;
    *width_rect=rect_in.width;
    *height_rect=rect_in.height;
}

void swimmerAlgoDynamic_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
    Mat img_blur, img_src, img_hsv, matMaskSurface, maskHSV, img_segmented, maskFinal, maskHSV_closed;
    //cout << "Size of pixels = "<<sizeof(pixels) << endl;
    img_src = Mat(height, width, CV_8UC3, src);
    //Read the json file to display hte labeled box
    /*vector<Rect> tab_rect = readJson_cpp("../data/train.json");
    Rect temp;*/

    //Median Filter wit kernel size 9
    medianBlur(img_src,img_blur, 9);

    //HSV conversion
    cvtColor(img_src, img_hsv, COLOR_RGB2HSV);

    //Mask Creation
    matMaskSurface = Mat(height,width,CV_8UC1,maskSurface);
    //Closing kernSize = 14
    /*imshow("MASK", matMaskSurface);
    waitKey(0);*/

    inRange(img_hsv, Scalar(0,0,0), Scalar(scalar1, scalar2, scalar3), maskHSV);

    bitwise_and(maskHSV,maskHSV ,maskFinal ,matMaskSurface);

    morphologyEx( maskFinal, maskFinal, MORPH_OPEN, getStructuringElement( 0, Size( 3, 3 ), Point( 1, 1) ));
    morphologyEx( maskFinal, maskFinal, MORPH_CLOSE, getStructuringElement( 0, Size( 5, 5 ), Point( 2, 2) ));
    //Image segmentation
    bitwise_and(img_src,img_src,img_segmented ,maskFinal);

    //Box construction
    vector<vector<cv::Point>> contours;
    findContours(maskFinal, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    Rect rect_in = Rect(0,0,0,0);

	vector<Rect> rects;

    for (auto & cnt : contours)
    {
        auto rect1 = boundingRect(cnt);
        if(contourArea(cnt) > 400)
        {
        	bool find = false;
        	for(auto & rect: rects)
        	{
        		Rect tempInter = Rect(rect.x -20 , rect.y, rect.width + 40 , rect.height) & Rect(rect1.x -20 , rect1.y, rect1.width+40, rect1.height);
        		if(tempInter.width!=0 && tempInter.height!=0)
        		{
        			Rect rectUnion = rect | rect1;
        			rect = rectUnion;
        			find = true;
        		}
        	}
            rectangle(img_src, rect1, Scalar(0,255,0),2);
            if(!find)
            	rects.push_back(rect1);
        }
        Rect bestRect;
        if (rects.size() !=0)
        {
        	Rect tempMin(0,0,width,height);
        	for(auto & rect : rects)
        	{
        		tempMin = rect.y < tempMin.y ? rect: tempMin;
        	}
        	bestRect = tempMin;
        }

    }


    /*temp = tab_rect[ind++];
    //rectangle(img_src, rect_in, Scalar(0,255,0),2);
    if(rect_in.area()>0)
    {
        cout << "IOU: " << to_string(iouMetrics_cpp(rect_in, temp)) << endl;
    }
    else
    {
        cout << "IOU :" << 0 << endl;
    }*/
    //rectangle(img_src, rect_in, Scalar(0,255,0),2);
    //rectangle(img_src, temp, Scalar(0,0,255), 2);

    memcpy(dest, img_src.data, 3*height*width);

    /*for(int i=0; i<width*height*3;i++)
    {
        dest[i] = img_segmented.data[i];
    }
    *x_rect=rect_in.x;
    *y_rect=rect_in.y;
    *width_rect=rect_in.width;
    *height_rect=rect_in.height;*/

}

/* Static Parameters*/

void swimmerAlgo_cpp(unsigned char *pixels, unsigned char* dest, int width, int height)
{
	Mat img_hsv, img_blur, mask, img_segmented;
	Mat img_src = Mat(height, width, CV_8UC3, pixels);
    vector<Rect> tab_rect = readJson_cpp("../data/train.json");
	Rect temp;

	//Median Filter wit kernel size 9
	medianBlur(img_src,img_blur, 9);;

	//HSV conversion
	cvtColor(img_src, img_hsv, COLOR_RGB2HSV);

	//Mask Creation
	inRange(img_hsv, Scalar(0,0,0), Scalar(179,235,120), mask);

	//Image segmentation
	bitwise_and(img_src,img_src,img_segmented ,mask);
	//Line surface Detection
    //surfaceDetection()

	//Box construction
    temp = tab_rect[ind++];

	vector<vector<cv::Point>> contours;
	findContours(mask, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    Rect rect_in = Rect(0,0,0,0);
    for( auto i = 0; i < contours.size(); i++ )
    {
        auto rect = boundingRect( contours[i] );
        if(contourArea(contours[i]) > 500 && rect.height > 5 && contourArea(contours[i])<150000)
	    {
            rect_in = Rect(rect);
	    }
	}
    if(rect_in.area()>0)
    {
        std::cout << "IOU: " << std::to_string(iouMetrics_cpp(rect_in, temp)) << std::endl;
    }
    else
    {
        cout << "IOU :" << 0 << endl;
    }
    rectangle(img_src, rect_in, Scalar(0,255,0),2);
    rectangle(img_src, temp, Scalar(0,0,255), 2);
    for(int i=0; i<width*height*3;i++)
    {
        dest[i] = img_src.data[i];
    }

}

void cvtToWB_cpp(unsigned char* src, unsigned char* dest, int width, int height)
{
	Mat img_wb;
	Mat img_src = Mat(height, width, CV_8UC3, src);
	cvtColor(img_src, img_wb, COLOR_RGB2GRAY);
	for(int i=0; i<width*height;i++)
	{
	   dest[i] = img_wb.data[i];
	}
}

void saliencySpectralRes_cpp(unsigned char* src, unsigned char* dest, int width, int height)
{
    Mat img_src = Mat(height,width, CV_8UC3, src);
    //instantiates the specific Saliency
    Ptr<Saliency> saliencyAlgorithm;

    Mat binaryMap;
    Mat image;

    img_src.copyTo( image );

    Mat saliencyMap;

    saliencyAlgorithm = StaticSaliencySpectralResidual::create();
    if( saliencyAlgorithm->computeSaliency( image, saliencyMap ) )
    {
        StaticSaliencySpectralResidual spec;
        spec.computeBinaryMap( saliencyMap, binaryMap );
        for(int i=0; i<width*height;i++)
        {
            dest[i] = saliencyMap.data[i];
        }
    }

}

void saliencyFineGrained_cpp(unsigned char* src, unsigned char* dest, int width, int height)
{
    Mat img_src = Mat(height,width, CV_8UC3, src);
    //instantiates the specific Saliency
    Ptr<Saliency> saliencyAlgorithm;
    Mat image, saliencyMap, saliencyMap_C1, binaryMap, img_blur;
    img_src.copyTo( image );
    medianBlur(image,image, 7);
    saliencyAlgorithm = StaticSaliencyFineGrained::create();
    if (saliencyAlgorithm->computeSaliency(image, saliencyMap))
    {
        saliencyMap.convertTo(saliencyMap_C1, CV_8UC1, 255);
        threshold(saliencyMap_C1, binaryMap, 0, 255, THRESH_BINARY | THRESH_OTSU);

        for(int i=0; i<width*height;i++)
        {
            //cout << "DATA \n" << (int)saliencyMap_C1.data[i] << endl;
            dest[i] = saliencyMap_C1.data[i];
        }
    }
}

double iouMetrics_cpp(const Rect r1, const Rect r2)
{
    return (double)((r1&r2).area())/(double)((r1|r2).area())*100;
}

//Long operation variance
void variance_cpp(unsigned char *src, unsigned char *dest, int width, int height, int nb_frames)
{
    xt::xarray<unsigned char> tabImages = xt::adapt(src, nb_frames*height*width*3 ,xt::no_ownership(),std::vector<std::size_t>{(unsigned long)nb_frames, (unsigned long)height, (unsigned long)width, 3});
    xt::xarray<unsigned char> var = xt::variance(tabImages, {0});
    memcpy(dest, var.data(), 3*height*width);
}

void threshold_cpp(unsigned char *src, unsigned char *dest, int width, int height, int thresh)
{
    Mat mat_dest;
    Mat mat_src(height, width, CV_8UC1, src);
    cv::threshold(mat_src, mat_dest, thresh, 255,THRESH_BINARY);
    memcpy(dest, mat_dest.data, height*width);
}

void opening_cpp(unsigned char *src, unsigned char *dest, int width, int height, int k)
{
    Mat mat_dest;
    Mat mat_src(height, width, CV_8UC1, src);
    morphologyEx( mat_src, mat_dest, MORPH_OPEN, getStructuringElement( 0, Size( 2*k + 1, 2*k+1 ), Point( k, k) ));
    memcpy(dest, mat_dest.data, height*width);
}

void closing_cpp(unsigned char *src, unsigned char *dest, int width, int height, int k)
{
    Mat mat_dest;
    Mat mat_src(height, width, CV_8UC1, src);
    morphologyEx( mat_src, mat_dest, MORPH_CLOSE, getStructuringElement( 0, Size( 2*k+ 1, 2*k+1 ), Point( k, k) ));
    memcpy(dest, mat_dest.data, height*width);
}

void surfaceDetection_cpp(unsigned char* src, unsigned char *image, unsigned char *dest, int width, int height)
{
    Mat normBlur;
    Mat img(height, width, CV_8UC3, image);
    Mat norm(height, width, CV_8UC1, src);
    medianBlur(norm, normBlur, 9);
    cv::threshold(normBlur, normBlur, 90, 255,THRESH_BINARY);

    Mat dataUp, dataBot;
    	Point pt_left, pt_right, pt3_up, pt3_bot, pt4_bot, pt4_up;
    	vector<Point> best_line{ pt_left, pt_right};
    	double min_het = 100000000;
    	for (int y = 100; y <= 320; y=y+2)
    	{
    	    for(int degrees = -30 ; degrees <= 30; degrees = degrees+2)
    	    {
    	        pt_left = Point(0, y);
    	        pt_right = Point(width, (int)(y - tan(degrees*3.14/360)*height));

    	        // Copy of the variance map for the two areas
    	        dataUp = normBlur.clone();
    	        dataBot = normBlur.clone();

    	        // Construction of other points
    	        pt3_up = Point(width, 0);
    	        pt4_up = Point(0, 0);
    	        pt3_bot = Point(width, height);
    	        pt4_bot = Point(0, height);
    	        //
    	        //Area construction
    	        vector<Point> contourPolyUp{pt_left, pt_right, pt3_up, pt4_up};
    	        vector<Point> contourPolyBot{pt_left, pt_right, pt3_bot, pt4_bot};
    	        // Metric computation
    	        // Upper area
    	        double area_up = contourArea(contourPolyUp);
    	        fillPoly(dataUp, contourPolyBot, 0);
    	        int nb_white_pixel = cv::sum(dataUp)[0];

    	        double per_up = (double)nb_white_pixel / area_up;

    	        // Bottom area
    	        double area_bot = contourArea(contourPolyBot);
    	        fillPoly(dataBot, contourPolyUp, 0);
    	        nb_white_pixel = cv::sum(dataBot)[0];

    	        double per_bot = nb_white_pixel / area_bot;

    	        //Gini concentration
    	        double het = 2 * per_up * (1 - per_up) * area_up + 2 * per_bot * (1 - per_bot) * area_bot;
    	        if (het < min_het)
    	        {
    	            best_line[0] = pt_left;
    	            best_line[1] = pt_right;
    	            min_het = het;
    	        }
    	    }
    	}

    	// Retrieval of the best line
    	pt_left = best_line[0];
    	pt_right = best_line[1];
    	// Height adjustment of the line
    	    //pt_left = Point((int)pt_left.x, (int)(pt_left.y - adjust_pt1));
    	    //pt_right = Point((int)pt_right.x, (int)(pt_right.y + adjust_pt2));
    	// Line construction y = a*x + b
    	double a = ((double)pt_right.y - (double)pt_left.y) / ((double)pt_right.x - (double)pt_left.x);
    	double b = (double)pt_left.y - a * (double)pt_left.x;

    	line(img, pt_left, pt_right, Scalar(0, 0, 255), 3);

    	//Mask construction
    	for(int y =0; y< height; y++)
    	{
    	    for(int x =0;x<width;x++)
    	    {
    	        maskSurface[x+y*width] = (a*x+b)<y ? 1 : 0;
    	    }
    	}
    	memcpy(dest, img.data, 3* height*width);
}

void maskSurface_cpp(unsigned char* src, unsigned char * image, unsigned char *dest, int width, int height)
{
	Mat dataUp, dataBot, normBlur;
	normBlur = Mat(height,width, CV_8UC1, src);
	Point pt_left, pt_right, pt3_up, pt3_bot, pt4_bot, pt4_up;
	vector<Point> best_line{ pt_left, pt_right};
	double min_het = 100000000;
	for (int y = 100; y <= 320; y=y+2)
	{
	    for(int degrees = -30 ; degrees <= 30; degrees = degrees+2)
	    {
	        pt_left = Point(0, y);
	        pt_right = Point(width, (int)(y - tan(degrees*3.14/360)*height));

	        // Copy of the variance map for the two areas
	        dataUp = normBlur.clone();
	        dataBot = normBlur.clone();

	        // Construction of other points
	        pt3_up = Point(width, 0);
	        pt4_up = Point(0, 0);
	        pt3_bot = Point(width, height);
	        pt4_bot = Point(0, height);
	        //
	        //Area construction
	        vector<Point> contourPolyUp{pt_left, pt_right, pt3_up, pt4_up};
	        vector<Point> contourPolyBot{pt_left, pt_right, pt3_bot, pt4_bot};
	        // Metric computation
	        // Upper area
	        double area_up = contourArea(contourPolyUp);
	        fillPoly(dataUp, contourPolyBot, 0);
	        int nb_white_pixel = cv::sum(dataUp)[0];

	        double per_up = (double)nb_white_pixel / area_up;

	        // Bottom area
	        double area_bot = contourArea(contourPolyBot);
	        fillPoly(dataBot, contourPolyUp, 0);
	        nb_white_pixel = cv::sum(dataBot)[0];

	        double per_bot = nb_white_pixel / area_bot;

	        //Gini concentration
	        double het = 2 * per_up * (1 - per_up) * area_up + 2 * per_bot * (1 - per_bot) * area_bot;
	        if (het < min_het)
	        {
	            best_line[0] = pt_left;
	            best_line[1] = pt_right;
	            min_het = het;
	        }
	    }
	}

	// Retrieval of the best line
	pt_left = best_line[0];
	pt_right = best_line[1];
	// Height adjustment of the line
	    //pt_left = Point((int)pt_left.x, (int)(pt_left.y - adjust_pt1));
	    //pt_right = Point((int)pt_right.x, (int)(pt_right.y + adjust_pt2));
	// Line construction y = a*x + b
	double a = ((double)pt_right.y - (double)pt_left.y) / ((double)pt_right.x - (double)pt_left.x);
	double b = (double)pt_left.y - a * (double)pt_left.x;

	Mat matImage(height, width, CV_8UC3, image);
	line(matImage, pt_left, pt_right, Scalar(0, 0, 255), 3);

	//Mask construction
	for(int y =0; y< height; y++)
	{
	    for(int x =0;x<width;x++)
	    {
	        if((a*x+b)<y)
	        {
	            maskSurface[x+y*width] = 1;
	        }
	        else
	        {
	            maskSurface[x+y*width] = 0;
	        }
	    }
	}
	memcpy(dest, matImage.data, 3* height*width);
}
/*
xt::xarray<unsigned char> normRGB(xt::xarray<unsigned char> images, int width, int height)
{
    if(images.dimension()==3)
    {
        xt::xarray<unsigned char> norm =  xt::empty<unsigned char>({360,640});
        for(int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                norm(i,j)=sqrt(pow(images(i, j, 0),2) + pow(images(i, j, 1),2) + pow(images(i, j, 2),2));
            }
        }
        return norm;
    }
    else
    {
        cout << " Error dimension array" << endl;
        exit(1);
    }

}*/

