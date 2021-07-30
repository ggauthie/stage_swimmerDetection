#ifndef SWIMMERDETECTION_C_GMM_H
#define SWIMMERDETECTION_C_GMM_H

#include <stdbool.h>
#include "hocl.h"

typedef struct
{
    int dimension_data; //number of datas, (ex : 3 for RGB pixels)
    int number_gaussian_components;  //number of clusters
    float * weight; //float array of dimension equal to number of gaussian clusters
    float * mean; // float array of dimension equal to number of gaussian clusters with a number of means equals to the number of datas
    float * covariance; // float array of dimension equal to number of gaussian clusters. Each component is a matrix of dimension dimension_data*dimension_data
    float * diag_covariance; // float array of dimension equal to number of gaussian clusters. Each component is a vector of the components of matrix diagonal
    float * inversed_covariance; // float array of dimension equal to number of gaussian clusters. Each component is a matrix of dimension dimension_data*dimension_data
    float * determinant_covariance; //float array of dimension equal to number of gaussian cluster corresponding to determinant of each covariance matrix
    bool diag;
}GaussianMixtureModel;

/**
* Function which initilaize the Gaussian Mixture Model structure with predefined values of weights, mean and covariance.
* Number of gaussian components defined by 2(swimmer and not swimmer).
* The dimension of data is equal to 3 for RGB values.
*
*/
void initGMM_Model_dynamic(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* covariance, bool diag);
/**
* Function which initilaize the Gaussian Mixture Model structure with predefined values of weights, mean and covariance.
* Number of gaussian components defined by 2(swimmer and not swimmer).
* The dimension of data is equal to 3 for RGB values.
*
*/
void initGMM_Model();
/**
* Function which initilaize the Gaussian Mixture Model structure with predefined values of weights, mean and covariance.
* The model of covariance is diagonal.
*/
void initGMM_Model_diagonal_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* diag_covariance);

/**
* Function which initilaize the Gaussian Mixture Model structure with predefined values of weights, mean and covariance.
* Number of gaussian components defined by 2(swimmer and not swimmer).
* The dimension of data is equal to 3 for RGB values.
* The model of covariance is not diagonal.
*/
void initGMM_Model_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* covariance);

/**
* Function which computes the Gaussian distribution on each pixel. This function works only on diagonal covariance model.
*
* @param data
*        The pointer on the data array of dimension equal to dimension_data
* @param component_index
* 		 The index used to select one of the gaussian component
*/
float gaussian_distribution_diagonal(unsigned char *data, int component_index);
/**
* Function which computes the Gaussian distribution on each pixel.
*
* @param data
*        The pointer on the data array of dimension equal to dimension_data
* @param component_index
* 		 The index used to select one of the gaussian component
*/
float gaussian_distribution(unsigned char *data, int component_index);

/**
* Function which computes the likelihood of each pixel on each gaussian component.
*
* @param data
*
* @param likelihood
* 		 The index used to select one of the gaussian component
*/
void calculate_likelihood(int width, int height, unsigned char* data, float* likelihood);

/**
* Function which displays the Gaussian Mixture Model values of weights, mean, determinant, inversed covariance and covariance.
*/
void display_GMM_Model();

/**
* Function which permits to create the segmented image taking into consideration of the likelihood map.
*/
void createMaskGMM_model(int  width, int height, int component_index, float* likelihood, unsigned char* mask);

/**
* Function which returns the determinant of a matrix.
* */
float determinant(float *matrix, int k);

/**
* Function which returns N, the inverse of a matrix M.
* */
void inverseMatrix(float *M, float *N, int n);

#endif //SWIMMERDETECTION_C_GMM_H
