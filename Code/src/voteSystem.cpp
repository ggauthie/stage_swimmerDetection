//
// Created by ggauthie on 06/07/2021.
//
#include "voteSystem.h"

void poolingDistance(Rectangle *box1, Rectangle *box2, Rectangle *box3, Rectangle *boxFinal)
{
    float compare_1_2, compare_2_3, compare_1_3;
    compare_1_2 = sqrt(pow((box1->x - box2->x),2) + pow((box1->y - box2->y),2)) + sqrt(pow((box1->x + box1->w) - (box2->x + box2->w),2) + pow((box1->y+box1->h) - (box2->y+box2->h),2));
    printf("iou 1 & 2 = %f\n", compare_1_2);
    compare_2_3 = sqrt(pow((box2->x - box3->x),2) + pow((box2->y - box3->y),2)) + sqrt(pow((box2->x + box2->w) - (box3->x + box3->w),2) + pow((box2->y+box2->h) - (box3->y+box3->h),2));
    printf("iou 2 & 3 = %f\n", compare_2_3);
    compare_1_3 = sqrt(pow((box1->x - box3->x),2) + pow((box1->y - box3->y),2)) + sqrt(pow((box1->x + box1->w) - (box3->x + box3->w),2) + pow((box1->y+box1->h) - (box3->y+box3->h),2));
   	printf("iou 1 & 3 = %f\n", compare_1_3);

    if(compare_1_2 <= compare_2_3 && compare_1_2 <= compare_1_3)
    {
        mergeBox(box1,box2, boxFinal);
    }
    else if(compare_2_3 <= compare_1_3 && compare_2_3 <= compare_1_2)
    {
        mergeBox(box2,box3, boxFinal);
    }
    else
    {
        mergeBox(box1,box3, boxFinal);
    }
}

void poolingIoU(Rectangle *box1, Rectangle *box2, Rectangle *box3, Rectangle *boxFinal)
{
	float compare_1_2, compare_2_3, compare_1_3;

	compare_1_2 = iou_1(box1, box2);
	printf("iou 1 & 2 = %f\n", compare_1_2);
	compare_2_3 = iou_1(box2, box3);
	printf("iou 2 & 3 = %f\n", compare_2_3);
	compare_1_3 = iou_1(box1, box3);
	printf("iou 1 & 3 = %f\n", compare_1_3);

	if((compare_2_3 <= compare_1_2) && (compare_1_3 <= compare_1_2))
	{
	    mergeBox(box1,box2, boxFinal);
	}
	else if((compare_1_3 <= compare_2_3) && (compare_1_2 <= compare_2_3))
	{
	    mergeBox(box2,box3, boxFinal);
	}
	else
	{
	    mergeBox(box1,box3, boxFinal);
	}
}


