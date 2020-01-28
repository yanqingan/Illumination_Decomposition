/*
*  Copyright (c) 2015-2018 Ling Zhang (lingzhang (at) whu.edu.cn) and 
*  Qingan Yan (yanqingan (at) whu.edu.cn &&
*  yanqinganssg (at) gmail.com) and Wuhan UNiversity.
*
*  This code implements the core part of paper [1].
*  If you use it for your research, please cite the following paper:
*
*  [1] Ling Zhang, Qingan Yan, Zheng Liu, Hua Zou and Chunxia Xiao.
*  Illumination Decomposition for Photograph with Multiple Light Sources.
*  IEEE Transactions on Image Processing, 2017.
*
*  For more information, please move to my homepage
*  https://yanqingan.github.io/ or github https://github.com/yanqingan
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*/

#include "stdafx.h"
#include <math.h>
#include <opencv.hpp>
using namespace cv;
using namespace std;

// compute sample mean value
void brush_average(int h, int w, double* avg,double* si1,double* si2,double* si3,double* brush_flag)
{
	double sum_11 = 0.0, sum_12 = 0.0, sum_13 = 0.0;
	double sum_21 = 0.0, sum_22 = 0.0, sum_23 = 0.0;
	double sum_31 = 0.0, sum_32 = 0.0, sum_33 = 0.0;
	double sum_41 = 0.0, sum_42 = 0.0, sum_43 = 0.0;
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0;
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			if (brush_flag[i*w+j] == 1)
			{
				sum_11 += si1[i*w+j];
				sum_12 += si2[i*w+j];
				sum_13 += si3[i*w+j];
				num1++;
			}
			if (brush_flag[i*w+j] == 2)
			{
				sum_21 += si1[i*w+j];
				sum_22 += si2[i*w+j];
				sum_23 += si3[i*w+j];
				num2++;
			}
			if (brush_flag[i*w+j] == 3)
			{
				sum_31 += si1[i*w+j];
				sum_32 += si2[i*w+j];
				sum_33 += si3[i*w+j];
				num3++;
			}
			if (brush_flag[i*w+j] == 4)
			{
				sum_41 += si1[i*w+j];
				sum_42 += si2[i*w+j];
				sum_43 += si3[i*w+j];
				num4++;
			}
		}
	}
	avg[0] = sum_11 / num1;
	avg[1] = sum_12 / num1;
	avg[2] = sum_13 / num1;
	avg[3] = sum_21 / num2;
	avg[4] = sum_22 / num2;
	avg[5] = sum_23 / num2;
	avg[6] = sum_31 / num3;
	avg[7] = sum_32 / num3;
	avg[8] = sum_33 / num3;
	avg[9] = sum_41 / num4;
	avg[10] = sum_42 / num4;
	avg[11] = sum_43 / num4;
}

void caculate_order(int &num_flag1,int &num_flag2, int &num_flag3,int h, int w, int &num_order1, int &num_order2, int &num_order3,double* brush_flag,
	               int* order_flag1,int* order_flag2,int* order_flag3,int** flag1,int** flag2,int** flag3,int* of)
{
	int t1 = 0, t2 = 1;
	while (t1<t2)
	{
		t1 = num_flag1;
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				if (order_flag1[i*w+j] == num_order1)
				{
					int i_s = (i-1)<0 ? 0 : (i-1);
					int i_e = (i+1)<h ? (i+1+1) : h;
					int j_s = (j-1)<0 ? 0 : (j-1);
					int j_e = (j+1)<w ? (j+1+1): w;
					//int tt = 0;
					if (of[i*w+j] == 0)
					{
						for (int k=i_s;k<i_e;k++)
						{
							for (int m=j_s;m<j_e;m++)
							{
								if (order_flag1[k*w+m] == -1 && of[k*w+m] == 0)
								{
									order_flag1[k*w+m] = num_order1+1;
									num_flag1++;
									//tt++;
								}
							}
						}
					}
				}
			}
		}
		t2 = num_flag1;
		if (t1<t2)
		{
			num_order1++;
		}
	 //  cout<<"size="<<size<<" "<<num_flag1+on<<" error="<<(size-num_flag1-on)<<endl;
	}
	cout<<"num_order1="<<num_order1<<endl;

	t1 = 0;
	t2 = 1;
	while (t1<t2)
	{
		t1 = num_flag2;
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				if (order_flag2[i*w+j] == num_order2 )
				{
					int i_s = (i-1)<0 ? 0 : (i-1);
					int i_e = (i+1)<h ? (i+1+1) : h;
					int j_s = (j-1)<0 ? 0 : (j-1);
					int j_e = (j+1)<w ? (j+1+1): w;
					if (of[i*w+j] == 0)
					{
						for (int k=i_s;k<i_e;k++)
						{
							for (int m=j_s;m<j_e;m++)
							{
								if (order_flag2[k*w+m] == -1 && of[k*w+m] == 0)
								{
									order_flag2[k*w+m] = num_order2+1;
									num_flag2++;
								}
							}
						}
					}
				}
			}
		}
		t2 = num_flag2;
		if (t1<t2)
		{
			num_order2++;
		}
		
		//  cout<<"size="<<size<<" "<<num_flag2+on<<" error="<<(size-num_flag2-on)<<endl;
	}
	cout<<"num_order2="<<num_order2<<endl;

	t1 = 0;
	t2 = 1;
	while (t1<t2)
	{
		t1 = num_flag3;
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				if (order_flag3[i*w+j] == num_order3 )
				{
					int i_s = (i-1)<0 ? 0 : (i-1);
					int i_e = (i+1)<h ? (i+1+1) : h;
					int j_s = (j-1)<0 ? 0 : (j-1);
					int j_e = (j+1)<w ? (j+1+1): w;
					if (of[i*w+j] == 0)
					{
						for (int k=i_s;k<i_e;k++)
						{
							for (int m=j_s;m<j_e;m++)
							{
								if (order_flag3[k*w+m] == -1 && of[k*w+m] == 0)
								{
									order_flag3[k*w+m] = num_order3+1;
									num_flag3++;
								}
							}
						}
					}
				}
			}
		}
		t2 = num_flag3;
		if (t1<t2)
		{
			num_order3++;
		}

		//  cout<<"size="<<size<<" "<<num_flag2+on<<" error="<<(size-num_flag2-on)<<endl;
	}
	cout<<"num_order3="<<num_order3<<endl;
}

void region_label(int h, int w, IplImage* object,IplImage* shadow1, IplImage* shadow2, IplImage* shadow3, double* avg,
	              double** k_weight,double** k_weight2,double** k_weight3,int &num_flag1,int &num_flag2,int &num_flag3,double* si1,double* si2,double* si3,
	              int* order_flag1,int* order_flag2,int* order_flag3,int** flag1,int** flag2,int** flag3,int* of,double* pa)
{
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar d = cvGet2D(object,i,j);
			of[i*w+j] = 0;
			if (d.val[0] > 10)
			{
				of[i*w+j] = 1;
			}
		}
	}
	// contributions of tree lights
	double* light = new double[9];
	for (int i=0;i<3;i++)
	{
		light[i] = avg[3+i]+avg[6+i]-avg[9+i];
		light[3+i] = avg[i]+avg[6+i]-avg[9+i];
		light[6+i] = avg[3+i]+avg[i]-avg[9+i];
		pa[i] = avg[i]+avg[3+i]+avg[6+i]-2*avg[9+i];
		cout<<light[i]<<" "<<light[3+i]<<" "<<light[6+i]<<" "<<pa[i]<<endl;
	}
	//cout<<light[i]<<" "<<light[3+i]<<" "<<light[6+i]<<endl;

	// shadow region labeling
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp1 = cvGet2D(shadow1,i,j);
			CvScalar temp2 = cvGet2D(shadow2,i,j);
			CvScalar temp3 = cvGet2D(shadow3,i,j);
			if (temp1.val[0] > 254)
			{
				order_flag2[i*w+j] = 0;
				flag2[i][j] = 1;
				num_flag2++;
				k_weight2[0][i*w+j] = light[3]*si1[i*w+j]/avg[0];
				k_weight2[1][i*w+j] = light[3+1]*si2[i*w+j]/avg[1];
				k_weight2[2][i*w+j] = light[3+2]*si3[i*w+j]/avg[2];
				order_flag3[i*w+j] = 0;
				flag3[i][j] = 1;
				num_flag3++;
				k_weight3[0][i*w+j] = light[6]*si1[i*w+j]/avg[0];
				k_weight3[1][i*w+j] = light[6+1]*si2[i*w+j]/avg[1];
				k_weight3[2][i*w+j] = light[6+2]*si3[i*w+j]/avg[2];
			}
			if (temp2.val[0] > 254)
			{
				order_flag1[i*w+j] = 0;
				flag1[i][j] = 1;
				num_flag1++;
				k_weight[0][i*w+j] = light[0]*si1[i*w+j]/avg[3];
				k_weight[1][i*w+j] = light[1]*si2[i*w+j]/avg[3+1];
				k_weight[2][i*w+j] = light[2]*si3[i*w+j]/avg[3+2];
				order_flag3[i*w+j] = 0;
				flag3[i][j] = 1;
				num_flag3++;
				k_weight3[0][i*w+j] = light[6]*si1[i*w+j]/avg[3];
				k_weight3[1][i*w+j] = light[6+1]*si2[i*w+j]/avg[3+1];
				k_weight3[2][i*w+j] = light[6+2]*si3[i*w+j]/avg[3+2];
			}
			if (temp3.val[0] > 254)
			{
				order_flag1[i*w+j] = 0;
				flag1[i][j] = 1;
				num_flag1++;
				k_weight[0][i*w+j] = light[0]*si1[i*w+j]/avg[6];
				k_weight[1][i*w+j] = light[1]*si2[i*w+j]/avg[6+1];
				k_weight[2][i*w+j] = light[2]*si3[i*w+j]/avg[6+2];
				order_flag2[i*w+j] = 0;
				flag2[i][j] = 1;
				num_flag2++;
				k_weight2[0][i*w+j] = light[3]*si1[i*w+j]/avg[6];
				k_weight2[1][i*w+j] = light[3+1]*si2[i*w+j]/avg[6+1];
				k_weight2[2][i*w+j] = light[3+2]*si3[i*w+j]/avg[6+2];
			}
		}
	}
	delete []light;
}

void boundary(int h, int w, IplImage* boundary_brush,double* brush_flag,int* of,int** flag1,int** flag2,int** flag3,int boundary_width)
{
	IplImage* boundarymask = cvCloneImage(boundary_brush);
	CvScalar b1 = cvScalarAll(255);
	CvScalar b2;
	b2.val[0] = 0;
	b2.val[1] = 0;
	b2.val[2] = 0;
	int *flag = new int[h*w];
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			cvSet2D(boundarymask,i,j,b2);
			if (flag1[i][j]==1 || flag2[i][j]==1 || flag3[i][j]==1)
			{
				flag[i*w+j] = 1;
			}
			else
				flag[i*w+j] = 0;
		}
	}
	int nn = 1;
	while (nn < boundary_width)
	{
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				if (flag[i*w+j] == nn)
				{
					int i_s = (i-1)<0 ? 0 : (i-1);
					int i_e = (i+1)<h ? (i+1+1) : h;
					int j_s = (j-1)<0 ? 0 : (j-1);
					int j_e = (j+1)<w ? (j+1+1): w;
					for (int k=i_s;k<i_e;k++)
					{
						for (int m=j_s;m<j_e;m++)
						{
							if (of[k*w+m] != 1 && k!= i && m!= j && flag[k*w+m] == 0)
							{
								brush_flag[k*w+m] = 254;
								cvSet2D(boundary_brush,k,m,b1);
								cvSet2D(boundarymask,k,m,b1);
								flag[k*w+m] = nn+1;
							}
						}
					}
				}
			}
		}
		nn++;
	}
	/*IplImage* bf = cvLoadImage("spp3.bmp");
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar bt = cvGet2D(bf,i,j);
			if (bt.val[0] == 255)
			{
				brush_flag[i*w+j] = 254;
				cvSet2D(boundary_brush,i,j,b1);
			}
		}
	}*/
	cvSaveImage("boundarymask.bmp",boundarymask);
	delete []flag;
}

// decomposition map in non-shadow region
void shadow_free_k(int h, int w,int num_order,int* order_flag,int** flag,int* of,double* brush_flag,double** k_weight,double* si1,double* si2,double* si3)
{
	for (int ki=1;ki<(num_order+1);ki++)
	{
	//	cout<<ki<<endl;
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				if (order_flag[i*w+j] == ki)
				{
					double zij1 = 0.0, zij2 = 0.0, zij3 = 0.0;
					double sz1 = 0.0, sz2 = 0.0, sz3 = 0.0;
					double sigma = 5;
					if (brush_flag[i*w+j] == 254 || brush_flag[i*w+j] == 255)
					{
						int r1 = 4;
						int i_s = (i-r1)<0 ? 0 : (i-r1);
						int i_e = (i+r1)<h ? (i+r1+1) : h;
						int j_s = (j-r1)<0 ? 0 : (j-r1);
						int j_e = (j+r1)<w ? (j+r1+1): w;						
						int num_1 = 0;
						double temp = 0.0;
						double temp2 = 0.0;
						double temp3 = 0.0;
						double tidu = 0.0;						
						if (of[i*w+j] == 0)
						{
							for (int k=i_s;k<i_e;k++)
							{
								for (int m=j_s;m<j_e;m++)
								{
									if (k!=i || m!=j)
									{
										if (flag[k][m] != 0 && of[k*w+m]==0)
										{
											temp += k_weight[0][k*w+m];
											temp2 += k_weight[1][k*w+m];
											temp3 += k_weight[2][k*w+m];
											num_1++;
										}
									}
								}
							}
							k_weight[0][i*w+j] = temp/num_1;
							k_weight[1][i*w+j] = temp2/num_1;
							k_weight[2][i*w+j] = temp3/num_1;
							flag[i][j] = 1;
						}
					}
					else
					{
						int num_1 = 0;
						double temp = 0.0;
						double tidu = 0.0;
						double temp2 = 0.0;
						double tidu2 = 0.0;
						double temp3 = 0.0;
						double tidu3 = 0.0;
						if (of[i*w+j] == 0)
						{
							int r1 = 4;
							int i_s = (i-r1)<0 ? 0 : (i-r1);
							int i_e = (i+r1)<h ? (i+r1+1) : h;
							int j_s = (j-r1)<0 ? 0 : (j-r1);
							int j_e = (j+r1)<w ? (j+r1+1): w;
							for (int k=i_s;k<i_e;k++)
							{
								for (int m=j_s;m<j_e;m++)
								{
									if (k!=i || m!=j)
									{
										if (flag[k][m] != 0 && of[k*w+m]==0)
										{
											/*tidu = si1[i*w+j]-si1[k*w+m];
											tidu = k_weight[0][k*w+m]+tidu;

											tidu2 = si2[i*w+j]-si2[k*w+m];
											tidu2 = k_weight[1][k*w+m]+tidu2;

											tidu3 = si3[i*w+j]-si3[k*w+m];
											tidu3 = k_weight[2][k*w+m]+tidu3;*/

											tidu = si1[i*w+j]*k_weight[0][k*w+m]/si1[k*w+m];
											tidu2 = si2[i*w+j]*k_weight[1][k*w+m]/si2[k*w+m];
											tidu3 = si3[i*w+j]*k_weight[2][k*w+m]/si3[k*w+m];
											zij1 = - (si1[i*w+j]-si1[k*w+m])*(si1[i*w+j]-si1[k*w+m])/(2*sigma*sigma);
											zij1 = exp(zij1);
											zij2 = - (si2[i*w+j]-si2[k*w+m])*(si2[i*w+j]-si2[k*w+m])/(2*sigma*sigma);
											zij2 = exp(zij2);
											zij3 = - (si3[i*w+j]-si3[k*w+m])*(si3[i*w+j]-si3[k*w+m])/(2*sigma*sigma);
											zij3 = exp(zij3);
											/*if (tidu<0 || tidu2<0 || tidu3<0)
											{
												break;
											}*/
											sz1 += zij1;
											sz2 += zij2;
											sz3 += zij3;
											temp += zij1*tidu;
											temp2 += zij2*tidu2;
											temp3 += zij3*tidu3;
											//cout<<tidu<<" ";
											num_1++;
										}
									}
								}
							}
							if (num_1>0)
							{
								k_weight[0][i*w+j] = temp/sz1/*num_1*/;
								k_weight[1][i*w+j] = temp2/sz2/*num_1*/;
								k_weight[2][i*w+j] = temp3/sz3/*num_1*/;
								flag[i][j] = 1;
							}					
						//	/*k_weight[0][i*w+j] = 255;
						//	k_weight2[0][i*w+j] = 255;
						//	k_weight3[0][i*w+j] = 255;
						//	flag1[i][j] = 1;*/
						}
					}
				}
			}
		}
	}
}


void sove_initialization(IplImage* Input, double* si1,double* si2,double* si3, double* siV, double* brush_flag, 
	int num_light,double** k_weight,double** k_weight2,double** k_weight3,double* pa)
{	
	int w = Input->width;
	int h = Input->height;
	int size = h*w;
	int** flag1 = new int*[h];
	int** flag2 = new int*[h];
	int** flag3 = new int*[h];
	for (int i=0;i<h;i++)
	{
		flag1[i] = new int [w];
		flag2[i] = new int [w];
		flag3[i] = new int [w];
	}
	
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			flag1[i][j] = 0;
			flag2[i][j] = 0;
			flag3[i][j] = 0;
		}
	}

	double* avg = new double[12];
	for (int i=0;i<12;i++)
	{
		avg[i] = 0.0;
	}
	brush_average(h, w, avg, si1, si2,si3,brush_flag);

	//IplImage* bou = cvLoadImage("271pp.bmp");
	IplImage* shadow1 = cvLoadImage("s2.png");    // mask of shadow1, no overlap
	IplImage* shadow2 = cvLoadImage("s1.png");    // mask of shadow2, no overlap
	IplImage* shadow3 = cvLoadImage("shadow1.bmp");    // mask of shadow3, no overlap
	//IplImage* so = cvLoadImage("2l_self11.bmp");    // self-shadow
	IplImage* object = cvLoadImage("object.bmp");    // object1
	//IplImage* object1 = cvLoadImage("27_m1.bmp");    // object2
	//IplImage* object2 = cvLoadImage("27_m2.bmp");    // object3
	IplImage* obdilate = cvCloneImage(object);
	cvDilate(object,obdilate,NULL,1);
	cvSaveImage("obdi.bmp",obdilate);
	IplImage* erode3 = cvCloneImage(object);
	cvErode(object,erode3,NULL,2);

	int* order_flag1 = new int[size];
	int* order_flag2 = new int[size];
	int* order_flag3 = new int[size];
	int* o_flag = new int[size];
	int* sf = new int[size];
	for (int i=0;i<size;i++)
	{
		order_flag1[i] = -1;
		order_flag2[i] = -1;
		order_flag3[i] = -1;
		sf[i] = -1;
		o_flag[i] = 0;
	}
	int num_flag1 = 0, num_flag2 = 0, num_flag3 = 0;    // how many pixels are labeled
	int* of = new int[size];

	region_label(h, w, object,shadow1, shadow2, shadow3, avg, k_weight,k_weight2,k_weight3,num_flag1,num_flag2,num_flag3,si1,si2,si3,
		order_flag1,order_flag2,order_flag3,flag1,flag2,flag3,of,pa);

	IplImage* boundary_brush = cvCloneImage(Input);
	boundary(h, w, boundary_brush,brush_flag,of,flag1,flag2,flag3,7);
	cvSaveImage("boundary.bmp",boundary_brush);

	int num_order1 = 0, num_order2 = 0, num_order3 = 0; 

	caculate_order(num_flag1,num_flag2, num_flag3, h, w, num_order1, num_order2, num_order3,brush_flag,order_flag1,order_flag2,
		order_flag3,flag1,flag2,flag3,of);
	cout<<num_order1<<" "<<num_order2<<" "<<num_order3<<endl;
	
	// light source estimating 
	shadow_free_k(h, w,num_order1,order_flag1,flag1,of,brush_flag,k_weight,si1,si2,si3);
	estimate_object1(h, w,flag1,of,k_weight,si1,si2,si3);

	// light source estimating 
	shadow_free_k(h, w,num_order2,order_flag2,flag2,of,brush_flag,k_weight2,si1,si2,si3);    
	estimate_object1(h, w,flag2,of,k_weight2,si1,si2,si3);

	// light source estimating 
	shadow_free_k(h, w,num_order3,order_flag3,flag3,of,brush_flag,k_weight3,si1,si2,si3);
	estimate_object1(h, w,flag3,of,k_weight3,si1,si2,si3);

	IplImage* light1 = cvCreateImage(cvSize(w,h),8,3);
	IplImage* light2 = cvCreateImage(cvSize(w,h),8,3);
	IplImage* light3 = cvCreateImage(cvSize(w,h),8,3);

	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp = cvScalarAll(0.0);
			temp.val[0] = k_weight[0][i*w+j];
			temp.val[1] = k_weight[1][i*w+j];
			temp.val[2] = k_weight[2][i*w+j];
			cvSet2D(light1,i,j,temp);
			temp.val[0] = k_weight2[0][i*w+j];
			temp.val[1] = k_weight2[1][i*w+j];
			temp.val[2] = k_weight2[2][i*w+j];
			cvSet2D(light2,i,j,temp);
			temp.val[0] = k_weight3[0][i*w+j];
			temp.val[1] = k_weight3[1][i*w+j];
			temp.val[2] = k_weight3[2][i*w+j];
			cvSet2D(light3,i,j,temp);
		}
	}
	cvSaveImage("ini_1.bmp",light1);
	cvSaveImage("ini_2.bmp",light2);
	cvSaveImage("ini_3.bmp",light3);
	cout<<"initialization end!"<<endl;

	cvReleaseImage(&light1);
	cvReleaseImage(&light2);
	cvReleaseImage(&light3);
	for (int i=0;i<h;i++)
	{
		delete []flag1[i];
		delete []flag2[i];
		delete []flag3[i];
	}
	delete []order_flag1;
	delete []o_flag;
	delete []order_flag2;
	delete []order_flag3;
	delete []sf;
}

void sove_pixel(int h, int w, int i, int j,double* si1,double* si2,double* si3, double* siY,double* siV,double* brush_flag, 
	          IplImage* brush, double** k_weight1,double** k_weight2,double** k_weight3,double lambda, double lambda1,double lambda2,double lambda3)
{
	double sum_jk1 = 0.0, sum_jk2 = 0.0, sum_jk3 = 0.0;
	double sum_zij1 = 0.0, sum_zij2 = 0.0, sum_zij3 = 0.0;
	double sum_ij1 = 0.0, sum_ij2 = 0.0, sum_ij3 = 0.0;
	double zij1 = 0.0, zij2 = 0.0, zij3 = 0.0, zij = 0.0;
	double sigma = 5;
	int r1 = 2;
	int i_s = (i-r1)<0 ? 0 : (i-r1);
	int i_e = (i+r1)<h ? (i+r1+1) : h;
	int j_s = (j-r1)<0 ? 0 : (j-r1);
	int j_e = (j+r1)<w ? (j+r1+1): w;
	double temp1 = 0.0, temp2 = 0.0, temp3 = 0.0;
	double grad1 = 0.0, grad2 = 0.0, grad3 = 0.0;
	int n = 0;
	for (int k=i_s;k<i_e;k++)
	{
		for (int m=j_s;m<j_e;m++)
		{
			if (k != i || m != j)
			{
				zij = - (siV[i*w+j]-siV[k*w+m])*(siV[i*w+j]-siV[k*w+m])/(2*sigma*sigma);
				zij = exp(zij);
				zij1 = - (si1[i*w+j]-si1[k*w+m])*(si1[i*w+j]-si1[k*w+m])/(2*sigma*sigma);
				zij1 = exp(zij1);	
				zij2 = - (si2[i*w+j]-si2[k*w+m])*(si2[i*w+j]-si2[k*w+m])/(2*sigma*sigma);
				zij2 = exp(zij2);
				zij3 = - (si3[i*w+j]-si3[k*w+m])*(si3[i*w+j]-si3[k*w+m])/(2*sigma*sigma);
				zij3 = exp(zij3);
				grad1 = si1[i*w+j] - si1[k*w+m];
				grad2 = si2[i*w+j] - si2[k*w+m];
				grad3 = si3[i*w+j] - si3[k*w+m];
				if (brush_flag[i*w+j] == 254)
				{
					sum_jk1 = sum_jk1 + 2 * lambda3* zij * grad1;
					sum_zij1 = sum_zij1 + 2*lambda2*zij1 + + 2*lambda3*zij;
					sum_ij1 = sum_zij1 + (2*lambda2*zij1 + + 2*lambda3*zij)*k_weight1[0][k*w+m];
					sum_jk2 = sum_jk2 + 2 * lambda3* zij * grad2;
					sum_zij2 = sum_zij2 + 2*lambda2*zij2 + + 2*lambda3*zij;
					sum_ij2 = sum_zij2 + (2*lambda2*zij2 + + 2*lambda3*zij)*k_weight1[1][k*w+m];
					sum_jk3 = sum_jk3 + 2 * lambda3* zij * grad3;
					sum_zij3 = sum_zij3 + 2*lambda2*zij3 + + 2*lambda3*zij;
					sum_ij3 = sum_zij3 + (2*lambda2*zij3 + + 2*lambda3*zij)*k_weight1[2][k*w+m];
				}
				else
				{
					sum_jk1 = sum_jk1 + 2 * lambda3* zij1 * grad1;
					sum_zij1 = sum_zij1 + 2*lambda2*zij1 + + 2*lambda3*zij1;
					sum_ij1 = sum_zij1 + (2*lambda2*zij1 + + 2*lambda3*zij1)*k_weight1[0][k*w+m];
					sum_jk2 = sum_jk2 + 2 * lambda3* zij2 * grad2;
					sum_zij2 = sum_zij2 + 2*lambda2*zij2 + + 2*lambda3*zij2;
					sum_ij2 = sum_zij2 + (2*lambda2*zij2 + + 2*lambda3*zij2)*k_weight1[1][k*w+m];
					sum_jk3 = sum_jk3 + 2 * lambda3* zij3 * grad3;
					sum_zij3 = sum_zij3 + 2*lambda2*zij3 + + 2*lambda3*zij3;
					sum_ij3 = sum_zij3 + (2*lambda2*zij3 + + 2*lambda3*zij3)*k_weight1[2][k*w+m];
				}
				n++;
			}
		}
	}
	if (n>0)
	{
		temp1 = (lambda1*si1[i*w+j]-lambda1*(k_weight2[0][i*w+j]+k_weight3[0][i*w+j])+sum_jk1+sum_ij1)  / (lambda1+sum_zij1);
		//cout<<temp1<<" ";
		temp1 = lambda * temp1+ (1-lambda) * k_weight1[0][i*w+j];
		temp2 = (lambda1*si2[i*w+j]-lambda1*(k_weight2[1][i*w+j]+k_weight3[1][i*w+j])+sum_jk2+sum_ij2)  / (lambda1+sum_zij2);
		//cout<<temp2<<" ";
		temp2 = lambda * temp2+ (1-lambda) * k_weight1[1][i*w+j];
		temp3 = (lambda1*si3[i*w+j]-lambda1*(k_weight2[2][i*w+j]+k_weight3[2][i*w+j])+sum_jk3+sum_ij3)  / (lambda1+sum_zij3);
		//cout<<temp3<<"    ";
		temp3 = lambda * temp3+ (1-lambda) * k_weight1[2][i*w+j];
		k_weight1[0][i*w+j] = temp1;
		k_weight1[1][i*w+j] = temp2;
		k_weight1[2][i*w+j] = temp3;
	}
}

void sove_with_initial(IplImage* shading, double* si1,double* si2,double* si3, double* siY,double* siV,double* brush_flag, int ntIter, int num_light, IplImage* brush, 
	double lambda,double** k_weight,double** k_weight2,double** k_weight3)
{
	// (0, 0, 255) is 1, (0, 255, 0) is 2, (255, 0, 0) is 3, (255, 255, 0) is 4
	int w = shading->width;
	int h = shading->height;
	int size = h*w;

	//initial value
	IplImage* init1 = cvLoadImage("inis_1.bmp");
	IplImage* init2 = cvLoadImage("inis_2.bmp");
	IplImage* init3 = cvLoadImage("inis_3.bmp");
	IplImage* shadow1 = cvLoadImage("s-self1.bmp");    // corresponding to light1
	IplImage* shadow2 = cvLoadImage("s-self2.bmp");    // corresponding to light2
	IplImage* shadow3 = cvLoadImage("s-s3.bmp");    // corresponding to light3

	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar t1 = cvGet2D(init1,i,j);
			CvScalar t2 = cvGet2D(init2,i,j);
			CvScalar t3 = cvGet2D(init3,i,j);
			for (int k=0;k<3;k++)
			{
				k_weight[k][i*w+j] = t1.val[k];
				k_weight2[k][i*w+j] = t2.val[k];
				k_weight3[k][i*w+j] = t3.val[k];
			}
		}
	}

	int* s_m1 = new int[size];
	int* s_m2 = new int[size];
	int* s_m3 = new int[size];
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar tt = cvGet2D(shadow1,i,j);
			if (tt.val[0] > 0)
			{
				s_m1[i*w+j] = 1;
			}
			else
				s_m1[i*w+j] = 0;
			tt = cvGet2D(shadow2,i,j);
			if (tt.val[0] > 0)
			{
				s_m2[i*w+j] = 1;
			}
			else
				s_m2[i*w+j] = 0;
			tt = cvGet2D(shadow3,i,j);
			if (tt.val[0] > 0)
			{
				s_m3[i*w+j] = 1;
			}
			else
				s_m3[i*w+j] = 0;
		}
	}

	int it=0;
	double lamda1 = 0.0;
	double lamda2 = 10;
	double lamda3 = 0;
	
	
	while (it<ntIter)
	{
		cout<<it<<endl;

		//------------------------------------------------//
		//---------  Gauss Seidel iteration  ---------//
		//------------------------------------------------//
		for (int i=0;i<h;i++)
		{
			for (int j=0;j<w;j++)
			{
				lamda1 = 0.4;
				lamda2 = 10;

				if (s_m1[i*w+j]==0)
					sove_pixel(h, w, i, j,si1,si2,si3, siY,siV,brush_flag,brush, k_weight,k_weight2,k_weight3,lambda, lamda1,lamda2,lamda3);
				
				if (s_m2[i*w+j]==0)
					sove_pixel(h, w, i, j,si1,si2,si3, siY,siV,brush_flag,brush,k_weight2, k_weight,k_weight3,lambda, lamda1,lamda2,lamda3);

				if (s_m3[i*w+j]==0)
					sove_pixel(h, w, i, j,si1,si2,si3, siY,siV,brush_flag,brush,k_weight3, k_weight,k_weight2,lambda, lamda1,lamda2,lamda3);
			}
		}
		it++;
	}

	IplImage* light1 = cvCreateImage(cvSize(w,h),8,3);
	IplImage* light2 = cvCreateImage(cvSize(w,h),8,3);
	IplImage* light3 = cvCreateImage(cvSize(w,h),8,3);

	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp = cvScalarAll(0.0);
			temp.val[0] = k_weight[0][i*w+j];
			temp.val[1] = k_weight[1][i*w+j];
			temp.val[2] = k_weight[2][i*w+j];
			cvSet2D(light1,i,j,temp);
			temp.val[0] = k_weight2[0][i*w+j];
			temp.val[1] = k_weight2[1][i*w+j];
			temp.val[2] = k_weight2[2][i*w+j];
			cvSet2D(light2,i,j,temp);
			temp.val[0] = k_weight3[0][i*w+j];
			temp.val[1] = k_weight3[1][i*w+j];
			temp.val[2] = k_weight3[2][i*w+j];
			cvSet2D(light3,i,j,temp);
		}
	}
	cvSaveImage("light1.bmp",light1);
	cvSaveImage("light2.bmp",light2);
	cvSaveImage("light3.bmp",light3);
	delete []s_m1;
	delete []s_m2;
	delete []s_m3;
	cvReleaseImage(&light1);
	cvReleaseImage(&light2);
	cvReleaseImage(&light3);
}

void addselfshadow(IplImage* Input,double* si1, double* si2, double* si3,double* pa)
{
	IplImage* ini_1 = cvLoadImage("ini_12.bmp");
	IplImage* ini_2 = cvLoadImage("ini_22.bmp");
	IplImage* ini_3 = cvLoadImage("ini_32.bmp");
	
	IplImage* shadow11 = cvLoadImage("2shadowmatte1.bmp");    // corresponding to light1
	//IplImage* shadow12 = cvLoadImage("2l_self11.bmp");
	IplImage* shadow21 = cvLoadImage("shadow2.bmp");    // corresponding to light2
	//IplImage* shadow22 = cvLoadImage("27self3.bmp");
	IplImage* shadow3 = cvLoadImage("2shadowmatte3.bmp");    // corresponding to light3
	double sigma = 5;
	int w = Input->width;
	int h = Input->height;
	CvScalar zero = cvScalarAll(0);
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp1 = cvGet2D(shadow11,i,j);
			//CvScalar temp2 = cvGet2D(shadow12,i,j);
			if (temp1.val[0] >0 /*|| temp2.val[0] > 0*/)
			{
				CvScalar tt = cvGet2D(ini_1,i,j);
				double alpha  = 0.0;
				//if (temp1.val[0] >0)
				//{
					double t1 = temp1.val[0] - pa[0]-25;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				//}
				/*else
				{
					double t1 = temp2.val[0] - 70;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				}*/
				tt.val[0] = alpha*tt.val[0];
				t1 = temp1.val[0] - pa[1]-25;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[1] = alpha*tt.val[1];
				t1 = temp1.val[0] - pa[2]-25;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[2] = alpha*tt.val[2];
				cvSet2D(ini_1, i,j,tt);
			}
			temp1 = cvGet2D(shadow21,i,j);
		//	CvScalar temp2 = cvGet2D(shadow22,i,j);
			if (temp1.val[0] >0 /*|| temp2.val[0] > 0*/)
			{
				CvScalar tt = cvGet2D(ini_2,i,j);
				double alpha  = 0.0;
				//if (temp1.val[0] >0)
				//{
					double t1 = temp1.val[0]-pa[0]-5;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				//}
				/*else
				{
					double t1 = temp2.val[0]-120;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				}*/
				tt.val[0] = alpha*tt.val[0];
				t1 = temp1.val[0]-pa[1]-5;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[1] = alpha*tt.val[1];
				t1 = temp1.val[0]-pa[2]-5;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[2] = alpha*tt.val[2];
				cvSet2D(ini_2, i,j,tt);
			}
			temp1 = cvGet2D(shadow3,i,j);
	
			if (temp1.val[0] >0 )
			{
				CvScalar tt = cvGet2D(ini_3,i,j);
				double alpha  = 0.0;
				//{
					double t1 = temp1.val[0]-pa[0]-35;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				//}
				tt.val[0] = alpha*tt.val[0];
				t1 = temp1.val[0]-pa[1]-35;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[1] = alpha*tt.val[1];
				t1 = temp1.val[0]-pa[2]-35;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[2] = alpha*tt.val[2];
				cvSet2D(ini_3, i,j,tt);
			}
		}
	}
	cvSaveImage("inis_1.bmp",ini_1);
	cvSaveImage("inis_2.bmp",ini_2);
	cvSaveImage("inis_3.bmp",ini_3);
 }

 void addselfshadow1(IplImage* Input,double* si1, double* si2, double* si3,double* pa)
{
	IplImage* ini_1 = cvLoadImage("inis_2.bmp");
	IplImage* ini_2 = cvLoadImage("inis_3.bmp"); 
	
	IplImage* shadow11 = cvLoadImage("2shadowmatte2.bmp");    // corresponding to light1
	//IplImage* shadow12 = cvLoadImage("2l_self11.bmp");
	IplImage* shadow21 = cvLoadImage("2shadowmatte4.bmp");    // corresponding to light2
	//IplImage* shadow22 = cvLoadImage("27self3.bmp");
	double sigma = 5;
	int w = Input->width;
	int h = Input->height;
	CvScalar zero = cvScalarAll(0);
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp1 = cvGet2D(shadow11,i,j);
			//CvScalar temp2 = cvGet2D(shadow12,i,j);
			if (temp1.val[0] >0 /*|| temp2.val[0] > 0*/)
			{
				CvScalar tt = cvGet2D(ini_1,i,j);
				double alpha  = 0.0;
				//if (temp1.val[0] >0)
				//{
					double t1 = temp1.val[0] -200;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				//}
				/*else
				{
					double t1 = temp2.val[0] - 70;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				}*/
				tt.val[0] = alpha*tt.val[0];
				t1 = temp1.val[0] - 200;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[1] = alpha*tt.val[1];
				t1 = temp1.val[0] - 200;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[2] = alpha*tt.val[2];
				cvSet2D(ini_1, i,j,tt);
			}
			temp1 = cvGet2D(shadow21,i,j);
		//	CvScalar temp2 = cvGet2D(shadow22,i,j);
			if (temp1.val[0] >0 /*|| temp2.val[0] > 0*/)
			{
				CvScalar tt = cvGet2D(ini_2,i,j);
				double alpha  = 0.0;
				//if (temp1.val[0] >0)
				//{
					double t1 = temp1.val[0]-200;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				//}
				/*else
				{
					double t1 = temp2.val[0]-120;
					if (t1<0)
					{
						t1 = 0;
					}
					alpha = (255-t1)/255;
				}*/
				tt.val[0] = alpha*tt.val[0];
				t1 = temp1.val[0]-200;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[1] = alpha*tt.val[1];
				t1 = temp1.val[0]-200;
				if (t1<0)
				{
					t1 = 0;
				}
				alpha = (255-t1)/255;
				tt.val[2] = alpha*tt.val[2];
				cvSet2D(ini_2, i,j,tt);
			}

	
		}
	}
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar tt = cvGet2D(shadow11,i,j);
			tt.val[0] = 255 - tt.val[0];
			tt.val[1] = tt.val[0];
			tt.val[2] = tt.val[0];
			cvSet2D(shadow11,i,j,tt);
			tt = cvGet2D(shadow21,i,j);
			tt.val[0] = 255 - tt.val[0];
			tt.val[1] = tt.val[0];
			tt.val[2] = tt.val[0];
			cvSet2D(shadow21,i,j,tt);
		}
	}
	cvSaveImage("inis_13.bmp",ini_1);
	cvSaveImage("inis_23.bmp",ini_2);
	cvSaveImage("ss1.bmp",shadow11);
	cvSaveImage("ss2.bmp",shadow21);
//	cvSaveImage("inis_3.bmp",ini_3);
 }

 void estimate_object1(int h, int w, int** flag, int* of, double** k_weight, double* si1, double* si2, double* si3)
 {

	 int num_0 = 0, t1 = 0, t2 = 1;
	 while (t1<t2)
	 {
		 cout << num_0 << endl;
		 t1 = num_0;
		 for (int i = 0; i<h; i++)
		 {
			 for (int j = 0; j<w; j++)
			 {
				 double zij1 = 0.0, zij2 = 0.0, zij3 = 0.0;
				 double sz1 = 0.0, sz2 = 0.0, sz3 = 0.0;
				 double sigma = 5;
				 if (flag[i][j] == 0 && of[i*w + j] == 2)
				 {
					 /*k_weight[0][i*w+j] = 0;
					 k_weight[1][i*w+j] = 0;
					 k_weight[2][i*w+j] = 255;
					 flag[i][j] = 1;
					 num_0++;*/
					 int r1 = 10;
					 int i_s = (i - r1)<0 ? 0 : (i - r1);
					 int i_e = (i + r1)<h ? (i + r1 + 1) : h;
					 int j_s = (j - r1)<0 ? 0 : (j - r1);
					 int j_e = (j + r1)<w ? (j + r1 + 1) : w;
					 int num_1 = 0;
					 double temp = 0.0;
					 double tidu = 0.0;
					 double temp1 = 0.0;
					 double tidu1 = 0.0;
					 double temp2 = 0.0;
					 double tidu2 = 0.0;
					 double temp3 = 0.0;
					 double tidu3 = 0.0;
					 double temp21 = 0.0;
					 double tidu21 = 0.0;
					 double temp31 = 0.0;
					 double tidu31 = 0.0;
					 for (int k = i_s; k<i_e; k++)
					 {
						 for (int m = j_s; m<j_e; m++)
						 {
							 if (k != i || m != j)
							 {
								 if (flag[k][m] == 1 && of[k*w + m] != 1)
								 {
									 tidu = si1[i*w + j] - si1[k*w + m];
									 tidu = k_weight[0][k*w + m]/*+tidu*/;
									 tidu2 = si2[i*w + j] - si2[k*w + m];
									 tidu2 = k_weight[1][k*w + m]/*+tidu2*/;
									 tidu3 = si3[i*w + j] - si3[k*w + m];
									 tidu3 = k_weight[2][k*w + m]/*+tidu3*/;
									 //	if (of[i*w+j] == 2 && of[k*w+m] == 2)
									 {
										 /*tidu = si1[i*w+j]*k_weight[0][k*w+m]/si1[k*w+m];
										 tidu2 = si2[i*w+j]*k_weight[1][k*w+m]/si2[k*w+m];
										 tidu3 = si3[i*w+j]*k_weight[2][k*w+m]/si3[k*w+m];*/
										 zij1 = -(si1[i*w + j] - si1[k*w + m])*(si1[i*w + j] - si1[k*w + m]) / (2 * sigma*sigma);
										 zij1 = exp(zij1);
										 zij2 = -(si2[i*w + j] - si2[k*w + m])*(si2[i*w + j] - si2[k*w + m]) / (2 * sigma*sigma);
										 zij2 = exp(zij2);
										 zij3 = -(si3[i*w + j] - si3[k*w + m])*(si3[i*w + j] - si3[k*w + m]) / (2 * sigma*sigma);
										 zij3 = exp(zij3);
										 sz1 += zij1;
										 sz2 += zij2;
										 sz3 += zij3;
										 /*if (of[i*w+j] == 2 && of[k*w+m] == 1)
										 {
										 tidu = tidu ;
										 tidu2 = tidu2 ;
										 tidu3 = tidu3 ;
										 }*/
										 temp += tidu*zij1;
										 temp2 += tidu2*zij2;
										 temp3 += tidu3*zij3;
										 num_1++;
									 }
								 }
							 }
						 }
					 }
					 if (num_1 != 0)
					 {
						 k_weight[0][i*w + j] = temp / sz1/*num_1*/;
						 k_weight[1][i*w + j] = temp2 / sz2/*num_1*/;
						 k_weight[2][i*w + j] = temp3 / sz3/*num_1*/;
						 flag[i][j] = 1;
						 //tt[i*w+j]=1;
						 num_0++;
					 }
				 }
			 }
		 }
		 t2 = num_0;
	 }
 }


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* Input = cvLoadImage("2.png");
	int w = Input->width;
	int h = Input->height;
	int size = h*w;
	int num_light = 3;
	IplImage* src1 = cvCreateImage(cvGetSize(Input),IPL_DEPTH_8U,1);
	IplImage* src2 = cvCreateImage(cvGetSize(Input),IPL_DEPTH_8U,1);
	IplImage* src3 = cvCreateImage(cvGetSize(Input),IPL_DEPTH_8U,1);
	//cvSplit(Input, src1, src2, src3, NULL);
	//cvSaveImage("b.bmp",src1);
	IplImage* inputgray = cvCreateImage(cvGetSize(Input),8,1);
	cvCvtColor(Input,inputgray,CV_BGR2GRAY);

	IplImage* YUV = cvCloneImage(Input);
	cvCvtColor(Input,YUV,CV_BGR2YUV);
	cvSplit(Input, src1, src2, src3, NULL);
	cvSaveImage("0B.bmp",src1);
	cvSaveImage("0G.bmp",src2);
	cvSaveImage("0R.bmp",src3);

	double* si1 = new double [size];
	double* si2 = new double [size];
	double* si3 = new double [size];
	double* siY = new double [size];
	double* siV = new double [size];
	//int* boundary_flag = new int [size];
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp = cvGet2D(Input,i,j);
			CvScalar temp1 = cvGet2D(YUV,i,j);
			si1[i*w+j] = temp.val[0];
			si2[i*w+j] = temp.val[1];
			si3[i*w+j] = temp.val[2];
			siY[i*w+j] = temp1.val[0];
			siV[i*w+j] = temp1.val[2];
		}
	}
	                             
	IplImage* brush = cvLoadImage("2image2.bmp");
	double* brush_flag = new double[size]; 
	IplImage* maskflag1 = cvCloneImage(inputgray);
	IplImage* maskflag2 = cvCloneImage(inputgray);
	CvScalar b0 = cvScalarAll(0.0);
	CvScalar w1 = cvScalarAll(255);
	double sample_lit_avg = 0.0, lit_sum = 0.0;
	int lit_num = 0;
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			CvScalar temp = cvGet2D(brush,i,j);
			CvScalar temp_input = cvGet2D(Input,i,j);
			if (temp_input.val[0] != temp.val[0])
			{
				if (temp.val[0] == 0 && temp.val[1] == 0 && temp.val[2] ==255)
				{
					brush_flag[i*w+j] = 1;
					cvSet2D(maskflag1,i,j,w1);
				}
				else
					cvSet2D(maskflag1,i,j,b0);

				if (temp.val[0] == 0 && temp.val[1] == 255 && temp.val[2] ==0)
				{
					brush_flag[i*w+j] = 2;
					//cvSet2D(maskflag2,i,j,w1);
				}
				/*else
					cvSet2D(maskflag2,i,j,b0);*/

				if (temp.val[0] == 255 && temp.val[1] == 0 && temp.val[2] ==0)
				{
					brush_flag[i*w+j] = 3;
				}  
				if (temp.val[0] == 0 && temp.val[1] == 255 && temp.val[2] ==255)
				{
					brush_flag[i*w+j] = 4;
				}
			}
			else
			{
				cvSet2D(maskflag1,i,j,b0);
				//cvSet2D(maskflag2,i,j,b0);
			}
		}
	}
	sample_lit_avg = lit_sum / lit_num;
	cvSaveImage("mask1.bmp",maskflag1);
	//cvSaveImage("mask2.bmp",maskflag2);

	double* pa = new double[3];    // ambient light
	for (int i=0;i<3;i++)
	{
		pa[i] = 0.0;
	}
	double sample_red = 0.0, sampe_green = 0.0;

	double** k_weight;
	k_weight = new double*[3];
	double** k_weight2;
	k_weight2 = new double*[3];
	double** k_weight3;
	k_weight3 = new double*[3];
	for (int i=0;i<3;i++)
	{
		k_weight[i] = new double [size];
		k_weight2[i] = new double [size];
		k_weight3[i] = new double [size];
	}
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<size;j++)
		{
			k_weight[i][j] = 0.0;
			k_weight2[i][j] = 0.0;
			k_weight3[i][j] = 0.0;
		}
	}
	sove_initialization(Input, si1, si2, si3,siV,brush_flag,  num_light,k_weight,k_weight2,k_weight3,pa);
	//cout<<brush_flag[0]<<endl;
	// addselfshadow1(Input, si1, si2, si3,pa);
	addselfshadow(Input, si1, si2, si3,pa);
	
	//sove_with_initial(inputgray, si1,si2, si3, siY, siV,brush_flag, 10, num_light, brush, 0.9,k_weight,k_weight2,k_weight3);
	delete []si1;
	delete []si2;
	delete []si3;
	delete []siY;
	delete []siV;
	delete []brush_flag;
	for (int i=0;i<3;i++)
	{
		delete []k_weight[i];
		delete []k_weight2[i];
		delete []k_weight3[i];
	}
	//delete []boundary_flag;
	return 0;
}

