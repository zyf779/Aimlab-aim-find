#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>
int main()
{
    cv::VideoCapture cap;
    cap.open("../example.mp4");
    double wait=1000.0/cap.get(5);
    cv::Mat now;
    std::cout<<wait;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    if(!cap.isOpened())
        {
            std::cout << "can't open the video";
            return 0;
        }
    for(;;)
    {
        if(cap.read(now)!=false)
            {
                clock_t start=clock();
                std::vector<cv::Mat> a,b;
                cv::Mat rgbnow,blue,test,afteradjust,light,before,handle1,handle2,handle3;
                cv::cvtColor(now,before,cv::COLOR_BGR2HSV);
                cv::split(before,b);
                light=b[2];
                cv::Mat mask;
                light.setTo(120, mask);
                b[2]=light;
                cv::merge(b,afteradjust);
                cv::cvtColor(afteradjust,rgbnow,cv::COLOR_HSV2RGB);
                cv::split(rgbnow,a);
                blue=a[0];
                cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2, 2));
                cv::dilate(blue,handle1,element,cv::Point(-1, -1),10);
                mask = handle1 > 105;
                handle1.setTo(1,mask);
                cv::Canny(handle1,test,20,25);
                cv::findContours(test,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE); 
                for(int i=0;i<contours.size();i++)
                {
                    if( cv::arcLength(contours[i],false)>200)
                        continue;
                    if( cv::arcLength(contours[i],false)<10)
                        continue;
                    if(hierarchy[i][4]!=-1)
                        continue;
                    cv::Rect need=cv::boundingRect(contours[i]);
                    cv::rectangle(now,need.tl(),need.br(),cv::Scalar(0, 0, 255),3);
                    double time=((double)(clock()-start))*1000/CLOCKS_PER_SEC;
                    int times=(int)time;
                    cv::putText(now,std::to_string(times),need.tl(),1,2,cv::Scalar(0, 0, 255),3);
                } 
                cv::imshow("now",now);
                cv::waitKey(wait);
            }
        else
            break;
    }
    return 0;
}