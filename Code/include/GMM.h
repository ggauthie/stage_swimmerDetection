#ifndef SWIMMERDETECTION_C_GMM_H
#define SWIMMERDETECTION_C_GMM_H

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

void initGMM_Model(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* diag_covariance, bool diag);
void initGMM_Model_diagonal_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* diag_covariance);
void initGMM_Model_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* covariance);
float gaussian_distribution_diagonal(unsigned char *data, int component_index);
float gaussian_distribution(unsigned char *data, int component_index);
void calculate_likelihood(unsigned char* data, float* likelihood, int width, int height);

void createMaskGMM_model(float* likelihood, unsigned char* mask, float threshold, int height, int width, int component_index);
float determinant( float *matrix, int k);
void inverseMatrix(float *M, float *N, int n);

#endif //SWIMMERDETECTION_C_GMM_H