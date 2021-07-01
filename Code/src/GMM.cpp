//
// Created by ggauthie on 30/06/2021.
//

#include <math.h>
#include "GMM.h"

static GaussianMixtureModel gmm_model;

void initGMM_Model(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* covariance, bool diag)
{
    gmm_model.diag = diag;
    if(!diag)
        initGMM_Model_covariance(dimension_data,number_gaussian_components,weight,mean,covariance);
    else
        initGMM_Model_diagonal_covariance(dimension_data,number_gaussian_components,weight,mean,covariance);
}
void initGMM_Model_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* covariance)
{
    gmm_model.dimension_data = dimension_data;
    gmm_model.number_gaussian_components = number_gaussian_components;
    gmm_model.weight = (float*)malloc(number_gaussian_components*sizeof(float));
    gmm_model.mean = (float*)malloc(dimension_data*number_gaussian_components*sizeof(float));
    gmm_model.covariance = (float*)malloc(dimension_data*dimension_data*number_gaussian_components*sizeof(float));
    gmm_model.inversed_covariance= (float*)malloc(dimension_data*dimension_data*number_gaussian_components*sizeof(float));
    gmm_model.determinant_covariance=(float*)malloc(number_gaussian_components*sizeof(float));
    for(int i=0; i<number_gaussian_components;i++)
    {
        gmm_model.weight[i] = weight[i];
        for(int j=0; j< dimension_data; j++)
        {
            gmm_model.mean[i*dimension_data + j] = mean[i*dimension_data + j];
            for(int k=0; k< dimension_data; k++)
            {
                gmm_model.covariance[i *(dimension_data*dimension_data) + j*dimension_data + k] = covariance[i *(dimension_data*dimension_data) + j*dimension_data + k];
            }
        }
        inverseMatrix(&gmm_model.covariance[i*(dimension_data*dimension_data)], &gmm_model.inversed_covariance[i*(dimension_data*dimension_data)], gmm_model.dimension_data);
        gmm_model.determinant_covariance[i] = determinant(&gmm_model.covariance[i*(gmm_model.dimension_data*gmm_model.dimension_data)],dimension_data);
    }
}

void initGMM_Model_diagonal_covariance(int dimension_data, int number_gaussian_components, float* weight, float* mean, float* diag_covariance)
{
    gmm_model.dimension_data = dimension_data;
    gmm_model.number_gaussian_components = number_gaussian_components;
    gmm_model.weight = (float*)malloc(number_gaussian_components*sizeof(float));
    gmm_model.mean = (float*)malloc(dimension_data*number_gaussian_components*sizeof(float));
    gmm_model.diag_covariance = (float*)malloc(dimension_data*number_gaussian_components*sizeof(float));
    for(int i=0; i<number_gaussian_components;i++)
    {
        gmm_model.weight[i] = weight[i];
        for(int j=0; j< dimension_data; j++)
        {
            gmm_model.mean[i*dimension_data + j] = mean[i*dimension_data + j];
            gmm_model.diag_covariance[i*dimension_data + j] = diag_covariance[i*dimension_data + j];
        }
    }
}

float gaussian_distribution_diagonal(unsigned char *data, int component_index)
{
    float determinant = 1;
    float result;
    float sum = 0;
    for (int i = 0; i < gmm_model.dimension_data; i++)
    {
        determinant *= (gmm_model.diag_covariance[component_index*gmm_model.dimension_data + i]);
        sum += (((float)data[i] - gmm_model.mean[component_index*gmm_model.dimension_data + i])) * (1 / (gmm_model.diag_covariance[component_index*gmm_model.dimension_data + i]) )* ((float)data[i] - gmm_model.mean[component_index * gmm_model.dimension_data + i]);
    }

    result= 1 / (pow(2 * 3.1415926535897931, gmm_model.dimension_data / 2.0) * sqrt(determinant)) * exp(-0.5 * sum);
    return result;
}

float gaussian_distribution(unsigned char *data, int component_index)
{
    float result;
    float sum = 0;
    if(!gmm_model.diag)
    {
        for (int i = 0; i < gmm_model.dimension_data; i++){
            float partial_sum = 0;

            for (int j = 0; j < gmm_model.dimension_data; j++){
                partial_sum += ((float)data[j]/255 - gmm_model.mean[component_index*gmm_model.dimension_data+j]) * gmm_model.inversed_covariance[component_index*(gmm_model.dimension_data*gmm_model.dimension_data) + gmm_model.dimension_data*i+j];
            }
            sum += partial_sum * ((float)data[i]/255 - gmm_model.mean[component_index*gmm_model.dimension_data + i]);
        }
        result = 1.0 / (pow(2 * 3.1415926535897931, gmm_model.dimension_data / 2.0) * sqrt(gmm_model.determinant_covariance[component_index])) * exp(-0.5 * sum);
        return result;
    }
    else
    {
        gaussian_distribution_diagonal(data,component_index);
    }

}

void calculate_likelihood(unsigned char* data, float* likelihood, int width, int height)
{
    float gauss[gmm_model.number_gaussian_components*height*width];
    for(int i =0; i < height;i++)
    {
        for(int j =0; j< width; j++)
        {
            float div = 0;
            for (int k = 0; k < gmm_model.number_gaussian_components; k++)
            {
                gauss[k*(height*width) + i*width + j] = gaussian_distribution(&data[3*(i*width + j)], k);
                div += gmm_model.weight[k] * gauss[k*(height*width) + i*width + j];
            }
            for (int k = 0; k < gmm_model.number_gaussian_components; k++)
            {
                likelihood[k*(width*height) + i*width +j] = (gmm_model.weight[k] * gauss[k*(height*width) + i*width + j])/div;
                //printf("%s\n", (gaussian_distribution_2(&data[3*(i*width + j)], k)==gaussian_distribution_1(&data[3*(i*width + j)], k)) ? "true" : "false");
            }
        }
    }
}

void createMaskGMM_model(float* likelihood, unsigned char* mask, float threshold, int height, int width, int component_index)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mask[i * width + j] = likelihood[component_index * (height * width) + i * width + j] > threshold ? 255 : 0;
        }
    }
}


float determinant( float *matrix, int k)
{
    float s = 1, det = 0;
    float temp[k*k];
    int i, j, m, n, c, l;
    if (k == 1)
    {
        return (matrix[0]);
    }
    else
    {
        det = 0;
        for (c = 0; c < k; c++)
        {
            m = 0;
            n = 0;
            for (i = 0;i < k; i++)
            {
                for (j = 0 ;j < k; j++)
                {
                    l = k-1;
                    temp[i*l + j] = 0;
                    if (i != 0 && j != c)
                    {
                        temp[m*l + n] = matrix[i*k + j];
                        if (n < (k - 2))
                            n++;
                        else
                        {
                            n = 0;
                            m++;
                        }
                    }
                }
            }
            det = det + s * (matrix[c] * determinant(temp, k - 1));
            s = -1 * s;
        }
    }
    return (det);
}

void inverseMatrix(float *M, float *N, int n)
{
    int m = n;

    if (m == 1)
    {
        N[0]= 1 / M[0];
    }
    else if (m >= 2)
    {
        float T[n*n];

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                    T[i*n+j] = M[i*n + j];
            }
        }

        // make identity matrix
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                 N[i*n + j] = (i == j) ? (1) : (0);
            }
        }

       // lower triangle elimination
       for (int k = 0; k < m - 1; k++)
       {
           for (int i = k + 1; i < m; i++)
           {
               float ratio = T[i*n + k] / T[k*n+k];

               for (int j = k; j < m; j++)
               {
                   T[i*n+j] -= T[k*n+j] * ratio;
               }
               for (int j = 0; j < m; j++)
               {
                   N[i*n+j] -= N[k*n+j] * ratio;
               }
           }
       }

       // make diagonal to 1.0
       for (int i = 0; i < m; i++)
       {
           float ratio = T[i*n+i];

           T[i*n+i] = 1.0;
           for (int j = i + 1; j < m; j++)
           {
               T[i*n+j] /= ratio;
           }
           for (int j = 0; j <= i; j++)
           {
               N[i*n+j] /= ratio;
           }
       }

       // upper triangle elimination
       for (int k = m - 1; k > 0; k--)
       {
           for (int i = k - 1; i >= 0; i--)
           {
               float ratio = T[i*n+k];
               T[i*n+k] = 0;
               for (int j = 0; j < m; j++)
               {
                   N[i*n+j] -= N[k*n+j] * ratio;
               }
           }
       }
    }
}