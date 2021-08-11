//
// Created by user on 31.07.2021.
//
#include <iostream>
#include "Utils/Bitmap.h"
#include "Utils/Utils.h"
#include "Utils/dataStructures/QuadTree.h"
#include "Utils/dataStructures/KDTree.h"
#include "Network/Network.h"
#include "Network/layers/ConvolutionLayer.h"
#include <opencv2/opencv.hpp>
#include <fstream>

int main(){
    cv::Mat mat = cv::imread("resources/aPhoto.jpg");
    cn::Network network(800, 800, 3);


    //QuadTree quadTree(1024, 1024);
    int t = 3;

//    PointData pointData1({7,2}, &t);
//    PointData pointData2({5,4}, &t);
//    PointData pointData3({2,3}, &t);
//    PointData pointData4({9,6}, &t);
//    PointData pointData5({8,1}, &t);
//    PointData pointData6({4,7}, &t);
//    std::vector<PointData *> points = {&pointData1, &pointData2, &pointData3, &pointData4, &pointData5, &pointData6};

    int pointsCount, testsCount;
    std::cin >> pointsCount >> testsCount;

    std::vector<PointData *> points;
    points.reserve(pointsCount);

    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> tests;//test, answer
    tests.reserve(pointsCount);
    for(int i = 0; i < pointsCount; i ++){
        float x,y;
        std::cin >> x >> y;
        points.push_back(new PointData({x, y}));
    }
    for(int i = 0; i < testsCount; i ++){
        float xt,yt,xa,ya;
        std::cin >> xt >> yt >> xa >> ya;
        tests.push_back({{xt, yt}, {xa, ya}});
    }

    KDTree kdTree(points);

    for(auto p : tests){
        auto res = kdTree.findNearestNeighbour(p.first);
        if(res.first->point == p.second){
            std::cout<<"OK \n";
        }else{
            std::cout<<"WRONG \n";
        }
    }
    for(auto p : points){
        delete p;
    }
    for(auto p : tests){
    }
    //std::pair<int, int> neighbor = quadTree.getNearestNeighbour({4, 4});
    return 0;
}