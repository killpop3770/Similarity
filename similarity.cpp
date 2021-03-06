#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/core/cvdef.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace cv;

typedef std::pair<std::string, std::string> Item;

void similarity(std::string str1, std::string str2);
int thresholdImg = 50;

int main(int argc, char* argv[])
{
    std::string str;
    std::vector<std::string> names;
    while(std::getline(std::cin,str) && str != "") {
        names.push_back(str);
    }

    thresholdImg = stoi(names.at(0));

    std::vector<Item> result;
	for (size_t i = 0; i < names.size(); ++i) {
		for (size_t j = i + 1; j < names.size(); ++j) {
			Item item(names.at(i), names.at(j));
			result.push_back(item);
		}
	}

    for (const auto &item : result) {
		similarity(item.first, item.second);
	}

  return 0;
}

void similarity(std::string str1, std::string str2)
{
    Mat img1 = imread( samples::findFile(str1), IMREAD_GRAYSCALE );
    Mat img2 = imread( samples::findFile(str2), IMREAD_GRAYSCALE );
    
    if ( img1.empty() || img2.empty() )
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
    }

    Ptr<ORB> detector = ORB::create();
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
    detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);

    std::vector< cv::DMatch > firstMatches, secondMatches;
    matcher->match( descriptors1, descriptors2, firstMatches );
    matcher->match( descriptors2, descriptors1, secondMatches );

    int bestMatchesCount = 0;
    std::vector< cv::DMatch > bestMatches;

    for(uint i = 0; i < firstMatches.size(); ++i)
    {
        cv::Point matchedPt1 = keypoints1[i].pt;
        cv::Point matchedPt2 = keypoints2[firstMatches[i].trainIdx].pt;

        bool foundInReverse = false;

        for(uint j = 0; j < secondMatches.size(); ++j)
        {
            cv::Point tmpSecImgKeyPnt = keypoints2[j].pt;
            cv::Point tmpFrstImgKeyPntTrn = keypoints1[secondMatches[j].trainIdx].pt;
            if((tmpSecImgKeyPnt == matchedPt2) && ( tmpFrstImgKeyPntTrn == matchedPt1))
            {
                foundInReverse = true;
                break;
            }
        }
        if(foundInReverse)
        {
            bestMatches.push_back(firstMatches[i]);
            bestMatchesCount++;
        }
    }

    double minKeypoints = keypoints1.size() <= keypoints2.size() ? keypoints1.size() : keypoints2.size();
    double number = ((bestMatchesCount/minKeypoints) * 100);
    int tmpNumber = (int)number;
    if(tmpNumber >= thresholdImg)
        std::cout<<str1<<" "<<str2<<" "<<number<<std::endl;
}