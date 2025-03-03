//gmm.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

//定义gmm模型用到的变量
#define GMM_MAX_COMPONT 6
#define GMM_LEARN_ALPHA 0.005    //该学习率越大的话，学习速度太快，效果不好
#define GMM_THRESHOD_SUMW 0.7    //如果取值太大了的话，则更多树的部分都被检测出来了
#define END_FRAME 200

bool pause = false;

Mat w[GMM_MAX_COMPONT];
Mat u[GMM_MAX_COMPONT];
Mat sigma[GMM_MAX_COMPONT];
Mat fit_num, gmask, foreground;
vector<Mat> output_m;
Mat output_img;

float temp_w, temp_sigma;
unsigned char temp_u;
int i = -1;


//For connected components:
int CVCONTOUR_APPROX_LEVEL = 2;   // Approx.threshold - the bigger it is, the simpler is the boundary
int CVCLOSE_ITR = 1;

//Just some convienience macros
#define CV_CVX_WHITE    CV_RGB(0xff,0xff,0xff)
#define CV_CVX_BLACK    CV_RGB(0x00,0x00,0x00)

//gmm整体初始化函数声明
void gmm_init(Mat img);

//gmm第一帧初始化函数声明
void gmm_first_frame(Mat img);

//gmm训练过程函数声明
void gmm_train(Mat img);

//对输入图像每个像素gmm选择合适的个数函数声明
void gmm_fit_num(Mat img);

//gmm测试函数的实现
void gmm_test(Mat img);

//连通域去噪函数声明
void find_connected_components(Mat img);
//void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int *num, CvRect *bbs, CvPoint *centers);

int main(int argc, const char* argv[])
{
	Mat img, img_gray;
	char str_num[5];


	//    char *str_num;//why does this definition not work?
	String str = "WavingTrees/b00";//string,the 's' can be a captial or lower-caseletters

								   /****read the first image,and reset the array w,u,sigma****/
	img = imread("WavingTrees/b00000.bmp");
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", img);
	if (img.empty())
	{
		cerr << "error" << endl;
		return -1;
	}

	output_img = Mat::zeros(img.size(), img.type());
	cvtColor(img, img_gray, CV_BGR2GRAY);//covert the colorful image to the corresponding gray-level image

										 /****initialization the three parameters ****/
	gmm_init(img_gray);
	fit_num = Mat(img.size(), CV_8UC1, -1);//初始化为1
	gmask = Mat(img.size(), CV_8UC1, -1);
	foreground = img.clone();
	split(img, output_m);

	output_m[0] = Mat::zeros(img.size(), output_m[0].type());
	output_m[1] = Mat::zeros(img.size(), output_m[0].type());
	output_m[2] = Mat::zeros(img.size(), output_m[0].type());

	namedWindow("src", WINDOW_AUTOSIZE);
	namedWindow("gmask", WINDOW_AUTOSIZE);

	//在定义视频输出对象时，文件名一定后面要加后缀，比如这里的.avi,否则是输出不了视频的！并且这里只能是avi格式的，当参数为('P','I','M','1')时
	VideoWriter output_src("src.avi", CV_FOURCC('P', 'I', 'M', '1'), 20, Size(160, 120), 1);//c++版本的opencv用Size函数即可，c版本的用cvSize()函数
																							//VideoWriter output_src("src.avi",CV_FOURCC('M','J','P','G'),5,Size(160,120),1);//c++版本的opencv用Size函数即可，c版本的用cvSize()函数
	VideoWriter output_dst("dst.avi", CV_FOURCC('P', 'I', 'M', '1'), 20, Size(160, 120), 1);//这样输出的是3个通道的数据
	while (1)

	{
		if (!pause)
		{
			/****read image from WavingTrees****/
			i++;
			_itoa_s(i, str_num, 10);//the latest name is _itoa_s or _itoa,not the itoa,although iota can be used,deprecated
			if (i<10)
				str += "00";
			else if (i<100)
				str += "0";
			else if (i>285)//we used the first 285 frames to learn the gmm model
				i = -1;
			str += str_num;
			str += ".bmp";

			img = imread(str);
			if (img.empty())
				break;
			str = "WavingTrees/b00";//after read,str must be reseted ;

			cvtColor(img, img_gray, CV_BGR2GRAY);//covert the colorful image to the corresponding gray-level image

												 /****when it is the first frame,set the default parameter****/
			if (1 == i)
			{
				gmm_first_frame(img_gray);
			}

			//the train of gmm phase
			//if(1<i&&i<5&&i!=3)//由此可知当i大于等于3以后，就会一直出现错误，且错误在内部排序的部分
			if (1<i<END_FRAME)
			{
				gmm_train(img_gray);
			}//end the train phase

			cout << i << endl;
			/****chose the fitting number of component in gmm****/
			if (END_FRAME == i)
			{
				gmm_fit_num(img_gray);
				//        cout<<fit_num<<endl;//其输出值有4个高斯的，但也有0个高斯的，why？照理说不可能的啊！
			}

			/****start the test phase****/
			if (i >= END_FRAME)
			{
				output_src << img;
				gmm_test(img_gray);
				find_connected_components(img_gray);

				output_m[0] = gmask.clone();
				output_m[1] = gmask.clone();
				output_m[2] = gmask.clone();

				merge(output_m, output_img);
				output_dst << output_img;
			}
			if (285 == i)
			{
				return 0;
			}
			imshow("src", img);
			imshow("gmask", gmask);
		}

		char c = (char)waitKey(1);
		if (c == 27)//if press the ESC key,the exit the proggram
			break;
		if (c == ' ')
			//    pause=~pause;//if use '~',then the pause key cannot work,why?
			pause = !pause;
	}
	return 0;
}


//gmm初始化函数实现
void gmm_init(Mat img)
{
	/****initialization the three parameters ****/
	for (int j = 0;j<GMM_MAX_COMPONT;j++)
	{
		w[j] = Mat(img.size(), CV_32FC1, 0.0);//CV_32FC1本身体现了正负符号
		u[j] = Mat(img.size(), CV_8UC1, -1);//为什么这里赋值为0时，后面的就一直出错？暂时还不知道原因，先赋值-1，其实内部存储的也是0
		sigma[j] = Mat(img.size(), CV_32FC1, 0.0);//float类型
	}

	//为什么一下语句不能放在这个函数里面呢
	//    output_m[0]=Mat(img.size(),CV_8UC1,0);
	//    output_m[1]=Mat(img.size(),CV_8UC1,0);
	//    output_m[2]=Mat(img.size(),CV_8UC1,0);
}


//gmm第一帧初始化函数实现
void gmm_first_frame(Mat img)
{
	for (int m = 0;m<img.rows;m++)
		for (int n = 0;n<img.cols;n++)
		{
			w[0].at<float>(m, n) = 1.0;

			//if the pixvel is gray-clever,then we should use unsigned char,not the unsigned int
			u[0].at<unsigned char>(m, n) = img.at<unsigned char>(m, n);// 一定要注意其类型转换，否则会得不得预期的结果
			sigma[0].at<float>(m, n) = 15.0;//opencv 自带的gmm代码中用的是15.0

			for (int k = 1;k<GMM_MAX_COMPONT;k++)
			{
				/****when assigment this,we must be very carefully****/
				w[k].at<float>(m, n) = 0.0;
				u[k].at<unsigned char>(m, n) = -1;
				sigma[k].at<float>(m, n) = 15.0;//防止后面排序时有分母为0的情况
			}
		}
}


//gmm训练过程函数实现
void gmm_train(Mat img)
{
	for (int m = 0;m<img.rows;m++)
		for (int n = 0;n<img.cols;n++)
		{
			int k = 0;
			int nfit = 0;
			for (;k<GMM_MAX_COMPONT;k++)
			{
				//    if(w[k].at<float>(m,n)!=0)//只有在权值不为0的情况下才进行比较
				//    {
				int delam = abs(img.at<unsigned char>(m, n) - u[k].at<unsigned char>(m, n));//防止溢出
				long dis = delam*delam;
				if (dis<3.0*sigma[k].at<float>(m, n))//the present pixpel is fit the component
				{
					/****update the weight****/
					w[k].at<float>(m, n) = w[k].at<float>(m, n) + GMM_LEARN_ALPHA*(1 - w[k].at<float>(m, n));

					/****update the average****/
					u[k].at<unsigned char>(m, n) = u[k].at<unsigned char>(m, n) + (GMM_LEARN_ALPHA / w[k].at<float>(m, n))*delam;

					/****update the variance****/
					sigma[k].at<float>(m, n) = sigma[k].at<float>(m, n) + (GMM_LEARN_ALPHA / w[k].at<float>(m, n))*(dis - sigma[k].at<float>(m, n));

					//                break;
				}
				else {
					w[k].at<float>(m, n) = w[k].at<float>(m, n) + GMM_LEARN_ALPHA*(0 - w[k].at<float>(m, n));
					nfit++;
				}
				//        }
			}

			////训练过程加速算法
			//for(int bb=k+1;bb<GMM_MAX_COMPONT;bb++)
			//{
			//    w[bb].at<float>(m,n)=w[bb].at<float>(m,n)+GMM_LEARN_ALPHA*(0-w[bb].at<float>(m,n));
			//    nfit++;
			//}

			//对gmm各个高斯进行排序,从大到小排序,排序依据为w/sigma
			for (int kk = 0;kk<GMM_MAX_COMPONT;kk++)
			{
				for (int rr = kk;rr<GMM_MAX_COMPONT;rr++)
				{
					//怎样才能做到gmm结构体整体排序呢？
					if (w[rr].at<float>(m, n) / sigma[rr].at<float>(m, n)>w[kk].at<float>(m, n) / sigma[kk].at<float>(m, n))
					{
						//权值交换
						temp_w = w[rr].at<float>(m, n);
						w[rr].at<float>(m, n) = w[kk].at<float>(m, n);
						w[kk].at<float>(m, n) = temp_w;

						//均值交换
						temp_u = u[rr].at<unsigned char>(m, n);
						u[rr].at<unsigned char>(m, n) = u[kk].at<unsigned char>(m, n);
						u[kk].at<unsigned char>(m, n) = temp_u;

						//方差交换
						temp_sigma = sigma[rr].at<float>(m, n);
						sigma[rr].at<float>(m, n) = sigma[kk].at<float>(m, n);
						sigma[kk].at<float>(m, n) = temp_sigma;
					}
				}
			}

			//****如果没有满足条件的高斯，则重新开始算一个高斯分布****/
			if (nfit == GMM_MAX_COMPONT && 0 == w[GMM_MAX_COMPONT - 1].at<float>(m, n))//if there is no exit component fit,then start a new componen
			{
				//不能写为for(int h=0;h<MAX_GMM_COMPONT&&((0==w[h].at<float>(m,n)));h++)，因为这样明显h不会每次都加1
				for (int h = 0;h<GMM_MAX_COMPONT;h++)
				{
					if ((0 == w[h].at<float>(m, n)))
					{
						w[h].at<float>(m, n) = GMM_LEARN_ALPHA;//按照论文的参数来的
						u[h].at<unsigned char>(m, n) = (unsigned char)img.at<unsigned char>(m, n);
						sigma[h].at<float>(m, n) = 15.0;//the opencv library code is 15.0
						for (int q = 0;q<GMM_MAX_COMPONT&&q != h;q++)
						{
							/****update the other unfit's weight,u and sigma remain unchanged****/
							w[q].at<float>(m, n) *= 1 - GMM_LEARN_ALPHA;//normalization the weight,let they sum to 1
						}
						break;//找到第一个权值不为0的即可
					}
				}
			}
			//如果GMM_MAX_COMPONT都曾经赋值过，则用新来的高斯代替权值最弱的高斯，权值不变，只更新均值和方差
			else if (nfit == GMM_MAX_COMPONT&&w[GMM_MAX_COMPONT - 1].at<float>(m, n) != 0)
			{
				u[GMM_MAX_COMPONT - 1].at<unsigned char>(m, n) = (unsigned char)img.at<unsigned char>(m, n);
				sigma[GMM_MAX_COMPONT - 1].at<float>(m, n) = 15.0;//the opencv library code is 15.0
			}


		}
}//end the train phase


 //对输入图像每个像素gmm选择合适的个数
void gmm_fit_num(Mat img)
{
	//float sum_w=0.0;//重新赋值为0，给下一个像素做累积
	for (int m = 0;m<img.rows;m++)
		for (int n = 0;n<img.cols;n++)
		{
			float sum_w = 0.0;//重新赋值为0，给下一个像素做累积
							  //chose the fittest number fit_num
			for (unsigned char a = 0;a<GMM_MAX_COMPONT;a++)
			{
				//cout<<w[a].at<float>(m,n)<<endl;
				sum_w += w[a].at<float>(m, n);
				if (sum_w >= GMM_THRESHOD_SUMW)//如果这里THRESHOD_SUMW=0.6的话，那么得到的高斯数目都为1，因为每个像素都有一个权值接近1
				{
					fit_num.at<unsigned char>(m, n) = a + 1;
					break;
				}
			}
		}
}


//gmm测试函数的实现
void gmm_test(Mat img)
{
	for (int m = 0;m<img.rows;m++)
		for (int n = 0;n<img.cols;n++)
		{
			unsigned char a = 0;
			for (;a<fit_num.at<unsigned char>(m, n);a++)
			{
				//如果对sigma取根号的话，树枝当做前景的概率会更大，不过人被检测出来的效果也更好些；用2相乘，不用开根号效果还不错
				//        if(abs(img.at<unsigned char>(m,n)-u[a].at<unsigned char>(m,n))<(unsigned char)(2*(sigma[a].at<float>(m,n))))
				if (abs(img.at<unsigned char>(m, n) - u[a].at<unsigned char>(m, n))<(unsigned char)(2.5*(sigma[a].at<float>(m, n))))
				{
					gmask.at<unsigned char>(m, n) = 1;//背景
					break;
				}
			}
			if (a == fit_num.at<unsigned char>(m, n))
				gmask.at<unsigned char>(m, n) = 255;//前景
		}
}

//连通域去噪函数实现
void find_connected_components(Mat img)
{
	morphologyEx(gmask, gmask, MORPH_OPEN, Mat());
	//    morphologyEx(gmask,gmask,MORPH_CLOSE,Mat());
}

////连通域去噪函数实现
//void find_connected_components(Mat img)
//{
//    morphologyEx(gmask,gmask,MORPH_OPEN,Mat());
//    morphologyEx(gmask,gmask,MORPH_CLOSE,Mat());
////    erode(gmask,gmask,Mat());//只腐蚀是不行的，人来了也被腐蚀掉了
//
//    vector<vector<Point>> contours;//由点向量组成的向量，所以有2个层次结构
//    vector<Vec4i> hierarchy;//typedef Vec<int,4>Vec4i;即由4个整数组成的向量
//    
//    //找到gmask的轮廓，存储在contours中，其拓扑结构存储在hierarchy中，且仅仅找出最外面的轮廓，用压缩算法只存储水平，垂直，斜对角线的端点
//    //其中暗含了hierarchy[i][2]=hierarchy[3]=-1,即其父轮廓和嵌套轮廓不用考虑
//    findContours(gmask,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
//    if(contours.size()==0)
//        return;
//
//    int idex=0;
//    for(idex=0;idex<contours.size();idex++)
//    {
//        const vector<Point>& c=contours[idex];
////        const vector<Point>& cnull::zeros();
//        double len=arcLength(c,false);//求出轮廓的周长，并不一定要求其是封闭的
//        double q=(img.rows+img.cols)/4;
//        if(q>=len)
//        {
//            const vector<Point> &cnew=contours[idex];
//    //        Mat mcnew=Mat(cnew);
//    //        Mat mcnew;
//    //        approxPolyDP(Mat(c),mcnew,CVCONTOUR_APPROX_LEVEL,false);//多边形曲线拟合，并不一定要求其轮廓闭合
//    //        approxPolyDP(Mat(c),Mat(cnew),CVCONTOUR_APPROX_LEVEL,false);//多边形曲线拟合，并不一定要求其轮廓闭合
//            approxPolyDP(Mat(c),cnew,CVCONTOUR_APPROX_LEVEL,false);//多边形曲线拟合，并不一定要求其轮廓闭合
//    //        cnew=vector<Point>(mcnew);
//    //        contours[idex]=cnew;
//        }
////        else contours[idex]=vector<Point(0,0,0)>;
//    }    
//
//}

///////////////////////////////////////////////////////////////////////////////////////////
//void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int *num, CvRect *bbs, CvPoint *centers)
// This cleans up the forground segmentation mask derived from calls to cvbackgroundDiff
//
// mask            Is a grayscale (8 bit depth) "raw" mask image which will be cleaned up
//
// OPTIONAL PARAMETERS:
// poly1_hull0    If set, approximate connected component by (DEFAULT) polygon, or else convex hull (0)
// perimScale     Len = image (width+height)/perimScale.  If contour len < this, delete that contour (DEFAULT: 4)
// num            Maximum number of rectangles and/or centers to return, on return, will contain number filled (DEFAULT: NULL)
// bbs            Pointer to bounding box rectangle vector of length num.  (DEFAULT SETTING: NULL)
// centers        Pointer to contour centers vectore of length num (DEFULT: NULL)
//
//void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int *num, CvRect *bbs, CvPoint *centers)
//{
//    static CvMemStorage*    mem_storage    = NULL;
//    static CvSeq*            contours    = NULL;
////    static CvSeq**            firstContour;
//
//    //CLEAN UP RAW MASK
//    //开运算作用：平滑轮廓，去掉细节,断开缺口
//    cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_OPEN, CVCLOSE_ITR );//对输入mask进行开操作，CVCLOSE_ITR为开操作的次数，输出为mask图像
//    //闭运算作用：平滑轮廓，连接缺口
//    cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_CLOSE, CVCLOSE_ITR );//对输入mask进行闭操作，CVCLOSE_ITR为闭操作的次数，输出为mask图像
//
//    //FIND CONTOURS AROUND ONLY BIGGER REGIONS
//    if( mem_storage==NULL ) mem_storage = cvCreateMemStorage(0);
//    else cvClearMemStorage(mem_storage);
//
//    //CV_RETR_EXTERNAL=0是在types_c.h中定义的，CV_CHAIN_APPROX_SIMPLE=2也是在该文件中定义的
//    CvContourScanner scanner = cvStartFindContours(mask,mem_storage,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
////    CvContourScanner scanner = cvFindContours(mask,mem_storage,firstContour,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
//    CvSeq* c;
//    int numCont = 0;
//    while( (c = cvFindNextContour( scanner )) != NULL )
//    {
//        double len = cvContourPerimeter( c );
//        double q = (mask->height + mask->width) /perimScale;   //calculate perimeter len threshold
//        if( len < q ) //Get rid of blob if it's perimeter is too small
//        {
//            cvSubstituteContour( scanner, NULL );
//        }
//        else //Smooth it's edges if it's large enough
//        {
//            CvSeq* c_new;
//            if(poly1_hull0) //Polygonal approximation of the segmentation
//                c_new = cvApproxPoly(c,sizeof(CvContour),mem_storage,CV_POLY_APPROX_DP, CVCONTOUR_APPROX_LEVEL,0);
//            else //Convex Hull of the segmentation
//                c_new = cvConvexHull2(c,mem_storage,CV_CLOCKWISE,1);
//            cvSubstituteContour( scanner, c_new );
//            numCont++;
//        }
//    }
//    contours = cvEndFindContours( &scanner );
//
//    // PAINT THE FOUND REGIONS BACK INTO THE IMAGE
//    cvZero( mask );
//    IplImage *maskTemp;
//    //CALC CENTER OF MASS AND OR BOUNDING RECTANGLES
//    if(num != NULL)
//    {
//        int N = *num, numFilled = 0, i=0;
//        CvMoments moments;
//        double M00, M01, M10;
//        maskTemp = cvCloneImage(mask);
//        for(i=0, c=contours; c != NULL; c = c->h_next,i++ )
//        {
//            if(i < N) //Only process up to *num of them
//            {
//                cvDrawContours(maskTemp,c,CV_CVX_WHITE, CV_CVX_WHITE,-1,CV_FILLED,8);
//                //Find the center of each contour
//                if(centers != NULL)
//                {
//                    cvMoments(maskTemp,&moments,1);
//                    M00 = cvGetSpatialMoment(&moments,0,0);
//                    M10 = cvGetSpatialMoment(&moments,1,0);
//                    M01 = cvGetSpatialMoment(&moments,0,1);
//                    centers[i].x = (int)(M10/M00);
//                    centers[i].y = (int)(M01/M00);
//                }
//                //Bounding rectangles around blobs
//                if(bbs != NULL)
//                {
//                    bbs[i] = cvBoundingRect(c);
//                }
//                cvZero(maskTemp);
//                numFilled++;
//            }
//            //Draw filled contours into mask
//            cvDrawContours(mask,c,CV_CVX_WHITE,CV_CVX_WHITE,-1,CV_FILLED,8); //draw to central mask
//        } //end looping over contours
//        *num = numFilled;
//        cvReleaseImage( &maskTemp);
//    }
//    //ELSE JUST DRAW PROCESSED CONTOURS INTO THE MASK
//    else
//    {
//        for( c=contours; c != NULL; c = c->h_next )
//        {
//            cvDrawContours(mask,c,CV_CVX_WHITE, CV_CVX_BLACK,-1,CV_FILLED,8);
//        }
//    }
//}