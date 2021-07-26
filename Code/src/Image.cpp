//
// Created by ggauthie on 01/06/2021.
//

#include "../include/Image.h"
#include <jsoncpp/json/json.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "string.h"

#define NB_FRAMES_SWIMMER 187

using namespace std;

static int id_frame = 1;

static unsigned char maskSurface[480*640];

static Json::Value event;

static double iou_moyen = 0;

Rect readJson_cpp(int id_frame, int width, int height)
{
    string id = id_frame < 10 ? ("00"+ to_string(id_frame)) : id_frame < 100 ? ("0"+ to_string(id_frame)) : to_string(id_frame);
    string path = "/home/ggauthie/dev/workspace_preesm/SwimmerDetection/Code/data/images/Testset/video2/Annotations/Annot" + id +".json";
    //cout << path << endl;
    ifstream ifs(path);
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

        Rect temp(0,0,0,0);

        temp.x = (int)(stof(obj["annotations"][0]["geometry"]["vertices"][0].asString())*(float)width);
        temp.y = (int)(stof(obj["annotations"][0]["geometry"]["vertices"][1].asString())*(float)height);
        temp.width = (int)(stof(obj["annotations"][0]["geometry"]["vertices"][2].asString())*(float)width) - temp.x;
        temp.height = (int)(stof(obj["annotations"][0]["geometry"]["vertices"][3].asString())*(float)height) - temp.y;
        return temp;
    }
}

void medianFilter_cpp(unsigned char *src, unsigned char *dest, int width, int height, int nb_channels)
{
    Mat img_blur, img_src;
    //cout <<"MEDIAN Filter" << endl;
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
    //Store src data in a Mat to use the OpenCV functions
    Mat img_src(height,width, CV_8UC3, src);
    //Conversion color RGB to HSV
    cvtColor(img_src,img_hsv, COLOR_RGB2HSV);
    //Store the data if the Mat in a uchar array
    memcpy(dest, img_hsv.data, 3*height*width);
}

void cvtColorRGBtoYUV_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
    Mat img_yuv;
    //Store src data in a Mat to use the OpenCV functions
    Mat img_src(height,width, CV_8UC3, src);
    //Conversion color RGB to YUV
    cvtColor(img_src,img_yuv, COLOR_RGB2YUV);
    //Store the data if the Mat in a uchar array
    memcpy(dest, img_yuv.data, 3*height*width);
}

void bitwise_and_cpp(unsigned char *src, unsigned char * mask, unsigned char *dest, int width, int height, int nb_channels)
{
    Mat mat_mask(height, width, CV_8UC1, mask);
    Mat img_src, maskFinal;
    // Compare the number of channels
    if(nb_channels == 3)
    {
    	//Store src data in a Mat to use the OpenCV functions
        img_src = Mat(height,width, CV_8UC3, src);
        //Apply the mask
        bitwise_and(img_src, img_src, maskFinal ,mat_mask);
        //Store the data if the Mat in a uchar array
        memcpy(dest, maskFinal.data, 3*height*width);
    }
    else if(nb_channels == 1)
    {
    	//Store src data in a Mat to use the OpenCV functions
        img_src = Mat(height,width, CV_8UC1, src);
        //Apply the mask
        bitwise_and(img_src, img_src, maskFinal ,mat_mask);
        //Store the data of the Mat in a uchar array
        memcpy(dest, maskFinal.data, height*width);
    }
    else
    {
    	//Print an error if the number of channels is not 1(WB) or 3(HSV,YUV,RGB)
        cout << "Nb channels != 1 or 3" << endl;
        exit(1);
    }
}

void deleteLight_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
	//Create a vector of Mat of a unique channel
    vector<Mat> img_split;
    //Store src data in a Mat to use the OpenCV functions
    Mat img_src(height, width,CV_8UC3, src);
    //Split the data into 3 different Mats store in a vector
    split(img_src, img_split);

    Mat mask_light, img_dest;
    //Apply a threshold on the V component
    threshold(img_split[2], mask_light, 250, 255, THRESH_BINARY_INV);
    //Apply a closing to delete isolated pixels
    morphologyEx(mask_light, img_dest, MORPH_CLOSE,  getStructuringElement( 0, Size( 2, 2 )));
    //Store the data of the Mat in a uchar array
    memcpy(dest, img_dest.data, height*width);
}

void segmentationHSV_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{

    Mat mask_hsv;
    Mat img_src(height,width, CV_8UC3, src);

    //Filter the 3 components H,S,V
    inRange(img_src, Scalar(0,0,0), Scalar(179, 235, 120), mask_hsv);

    //Fill the dest array
    memcpy(dest, mask_hsv.data, height*width);

    /*imshow("SegHSV", mask_hsv);
    waitKey(0);*/
}

void segmentationYUV_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
    Mat mask_yuv;
    Mat img_src(height,width, CV_8UC3, src);

    //Filter the 3 components Y,U,V
    inRange(img_src, Scalar(0,0,70), Scalar(90, 150, 255), mask_yuv);

    //fill the dest array
    memcpy(dest, mask_yuv.data, height*width);

    /*imshow("SegYUV", mask_yuv);
    waitKey(0);*/
}


void applyMaskSurface_cpp(unsigned char *src, unsigned char *dest, int width, int height)
{
	// Mask creation generated by the init function
	Mat maskSurfaceMat(height,width,CV_8UC1,maskSurface);

	Mat maskFinal;
	Mat img_src(height,width, CV_8UC1, src);

	//Combine the 2 masks
	bitwise_and(img_src, img_src, maskFinal ,maskSurfaceMat);

	//fill the dest array
	memcpy(dest, maskFinal.data, height*width);
}

void boxConstruction_cpp(unsigned char* src, int width, int height, Rectangle* box)
{
    vector<vector<cv::Point>> contours;

    //Transform uchar tab mask in Mat
    Mat img_src(height,width, CV_8UC1, src);
    Rect rect_in(0,0,0,0);

    findContours(img_src, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    //drawContours(img_src, contours,  -1,  Scalar(255,255,255),2);

    vector<Rect> rects;
    Rect bestRect = Rect(0,0,0,0);

    for (auto & cnt : contours)
    {
        auto rect1 = boundingRect(cnt);
        //rectangle(img_src,rect1, Scalar(255,255,255), 2);
        if (contourArea(cnt) > 800)
        {
            bool find = false;
            for (auto &rect: rects)
            {
                Rect tempInter = Rect(rect.x - 20, rect.y, rect.width + 40, rect.height) &
                                 Rect(rect1.x - 20, rect1.y, rect1.width + 40, rect1.height);
                if (tempInter.width != 0 && tempInter.height != 0)
                {
                    Rect rectUnion = rect | rect1;
                    rect = rectUnion;
                    find = true;
                }
            }
            if (!find)
                rects.push_back(rect1);
        }

        if (rects.size() != 0) {
            Rect tempMin(width, height, 0, 0);
            for (auto &rect : rects) {
                tempMin = rect.y < tempMin.y ? rect : tempMin;
            }
            bestRect = tempMin;
        }
    }
    /*rectangle(img_src, bestRect, Scalar(255,255,255),5);
    imshow("Rectangle", img_src);
    waitKey(0);*/
    //cv::Rect to Rectangle
    box->x = bestRect.x;
    box->y = bestRect.y;
    box->w = bestRect.width;
    box->h = bestRect.height;
}

void swimmerAlgoDynamic_cpp(unsigned char *src, unsigned char *dest, int width, int height, Rectangle *box)
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

    inRange(img_hsv, Scalar(0,0,0), Scalar(179, 210, 110), maskHSV);

    bitwise_and(maskHSV,maskHSV ,maskFinal ,matMaskSurface);

    morphologyEx( maskFinal, maskFinal, MORPH_OPEN, getStructuringElement( 0, Size( 3, 3 ), Point( 1, 1) ));
    morphologyEx( maskFinal, maskFinal, MORPH_CLOSE, getStructuringElement( 0, Size( 5, 5 ), Point( 2, 2) ));
    //Image segmentation
    bitwise_and(img_src,img_src,img_segmented ,maskFinal);

    //Box construction
    vector<vector<cv::Point>> contours;
    findContours(maskFinal, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	vector<Rect> rects;
    Rect bestRect = Rect(0,0,0,0);

    for (auto & cnt : contours)
    {
        auto rect1 = boundingRect(cnt);
        if(contourArea(cnt) > 800)
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
    }*/
    //cv::Rect to Rectangle
    box->x = bestRect.x;
    box->y = bestRect.y;
    box->w = bestRect.width;
    box->h = bestRect.height;

}

/*void cropFrame(unsigned char *src, unsigned char *dest, int width, int height, int x_prec, int y_prec, int w_prec, int h_prec, int margin)
{
	if(w_prec !=0 && h_prec!=0)
	{
		x_prec = std::max(x_prec - margin, 0);
		y_prec = std::max(y_prec - self.margin, 0);

		w_prec = std::min(w_prec + 2 * margin, width - x_prec);
		h_prec = std::min(h_prec + 2 * margin, height - y_prec);
	}
	else
	{

	}
}*/
/* Static Parameters*/

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

double iouMetrics_cpp(const Rect& r1, const Rect& r2)
{
    return (double)((r1&r2).area())/(double)((r1.area() + r2.area()) - (r1&r2).area());
}

float iou_cpp(Rectangle *r1, Rectangle* r2)
{
	Rect r1_oc(r1->x, r1->y, r1->h, r1->w);
	Rect r2_oc(r2->x, r2->y, r2->h, r2->w);
	return (float)iouMetrics_cpp(r1_oc, r2_oc);
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

void drawRectangle_cpp(unsigned char *src, unsigned char *dest, Rectangle *box, int width, int height)
{
    Mat img_src(height, width, CV_8UC3, src);
    Rect label = readJson_cpp(id_frame++, width, height);
    Rect bb(box->x, box->y, box->w, box->h);
    rectangle(img_src, label , Scalar(255,0,0),2);
    rectangle(img_src, bb, Scalar(0,255,0),2);

    double iou = iouMetrics_cpp(label, bb);

    iou_moyen += iou;

    event["iou_metrics"][id_frame] = iou;
    std::ofstream file("/home/ggauthie/dev/workspace_preesm/SwimmerDetection_Preesm/Code/data/images/Testset/video2/Annotations/iouYUV.json");
    file << event;

    putText(img_src, "IOU : " + to_string(iou), Point(10, 20),
            cv::FONT_HERSHEY_SIMPLEX,
            0.75,
            CV_RGB(0, 255, 255), //font color
            2);
    if(id_frame == NB_FRAMES_SWIMMER)
    {
    	printf("IOU moyen = %f\n", iou_moyen/NB_FRAMES_SWIMMER);
    }
    memcpy(dest, img_src.data, 3*width*height);
}

void generateMaskSurface_cpp(unsigned char* src, int width, int height)
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
	        int nb_white_pixel = cv::sum(dataUp)[0]/255;

	        double per_up = (double)nb_white_pixel / area_up;

	        // Bottom area
	        double area_bot = contourArea(contourPolyBot);
	        fillPoly(dataBot, contourPolyUp, 0);
	        nb_white_pixel = cv::sum(dataBot)[0]/255;

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
	    pt_left = Point((int)pt_left.x, (int)(pt_left.y - 0));
	    pt_right = Point((int)pt_right.x, (int)(pt_right.y + 30));
	// Line construction y = a*x + b
	double a = ((double)pt_right.y - (double)pt_left.y) / ((double)pt_right.x - (double)pt_left.x);
	double b = (double)pt_left.y - a * (double)pt_left.x;

	//Mat matImage(height, width, CV_8UC3, image);
	//line(matImage, pt_left, pt_right, Scalar(0, 0, 255), 3);

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
    cout << "A = "<< a<<" && B  =  "<< b<<endl;
	//memcpy(dest, matImage.data, 3* height*width);
}

void mergeBox_cpp(Rectangle *box1, Rectangle *box2, Rectangle *boxMerged)
{
    Rect box1_oc(box1->x, box1->y, box1->w, box1->h);
    Rect box2_oc(box2->x, box2->y, box2->w, box2->h);

    Rect boxMerged_oc = box1_oc & box2_oc;

    boxMerged->x = boxMerged_oc.x;
    boxMerged->y = boxMerged_oc.y;
    boxMerged->w = boxMerged_oc.width;
    boxMerged->h = boxMerged_oc.height;
}


