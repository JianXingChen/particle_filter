//李飞，科大先研院
//sume.cn@aliyun.com
//just for fun

#include <stdio.h>
#include <vector>
#include <time.h>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;


#define NUM_PARTICLES 50

// set laser length as 80
// map resolution is 0.05m, so 80 pixel is 4m
#define LASER_RADIUS 80
const vector<int> compute_scan(int x, int y, const Mat & map)
{
    vector<int> scan;
    for (int theta = 0; theta < 360; ++theta)
    {
        int r = 0;
        for (; r < LASER_RADIUS; ++r)
        {
            int dx = r*cos(theta*2*CV_PI/360.0);
            int dy = r*sin(theta*2*CV_PI/360.0);
            if (map.at<unsigned char>(y+dy, x+dx) != 255)
            {
                break;
            }
        }
//        if((r==80)&&(map.at<unsigned char>(y+ 80*cos(theta*2*CV_PI/360.0), x+80*sin(theta*2*CV_PI/360.0)) ==255 )){
//            r=0;
//        }
        scan.push_back(r);
    }

    return scan;
}


double compute_score(const vector<int> v1, const vector<int> v2)
{
    // CV_ASSERT(v1.size() == v2.size());

    double score = 0.0;
    for (int i = 0; i < v1.size(); ++i)
    {
        score += pow(v1[i]-v2[i], 2.0);
    }

    score = sqrt(score);

    return score;
}


int rand_pn(int r)
{
    if (rand()%2 == 0)
    {
        return rand()%r;
    }
    else
    {
        return -rand()%r;
    }
}
Mat scan(int x,int y,vector<int> real_scan ,Mat map){
    Mat hh(map.rows,map.cols, 0);
    for (int i = 0; i < real_scan.size(); ++i)
    {
          float yyy=y+real_scan[i]*sin((i/180.0)*CV_PI);
          float xxx=x+real_scan[i]*cos((i/180.0)*CV_PI);
          cout<<real_scan[i]<<endl;
        hh.at<unsigned char>((int)yyy,(int)xxx)=255;
    }
return hh;
}
//int main(){

//      Mat map = imread("../img/map.bmp", CV_LOAD_IMAGE_GRAYSCALE);
//     vector<int> real_scan = compute_scan(150, 150, map);
//     cout<<real_scan.size()<<endl;
//     imshow( "../img/scan.png",  scan(150,150,real_scan,map));
//     waitKey(0);
// }


int main()
{
    srand((unsigned int)time(NULL));

    Mat map = imread("../img/map.bmp", CV_LOAD_IMAGE_GRAYSCALE);
cout<<"hello"<<endl;
    vector<Point> particles;
    for (int i = 0; i < NUM_PARTICLES; ++i)
    {
        int xx = 0;
        int yy = 0;
        do
        {
            xx = rand()%map.cols;
            yy = rand()%map.rows;
        } while (map.at<unsigned char>(yy,xx) != 255);

        particles.push_back(Point(xx,yy));
    }

    ///////////////////////////////////////////////////////////////

    Point point_start(30,330);
    Point point_end(240,330);

    double score[NUM_PARTICLES] = {0};

    for (int x = 30; x < 240; ++x)
    {
        // real position: x, 330
        vector<int> real_scan = compute_scan(x, 330, map);

        int offset = x-30;

        // compute each particle
        for (int i = 0; i < NUM_PARTICLES; ++i)
        {
            vector<int> v = compute_scan(particles[i].x+offset, particles[i].y, map);
            score[i] = compute_score(real_scan, v);
        }

        // kill worst particles and replace with mutations of the best
        double *maxLoc = max_element(score, score+NUM_PARTICLES);
        double *minLoc = min_element(score, score+NUM_PARTICLES);
        printf("max: %f, min: %f\n", *maxLoc, *minLoc);

        {
            int xx = 0;
            int yy = 0;
            do
            {
                xx = particles[minLoc-score].x + rand_pn(20);
                yy = particles[minLoc-score].y + rand_pn(20);
            } while(map.at<unsigned char>(yy,xx) != 255);

            particles[maxLoc-score].x = xx;
            particles[maxLoc-score].y = yy;
        }

        {
            Mat mat_disp = map.clone();
            for (int i = 0; i < NUM_PARTICLES; ++i)
            {
                circle(mat_disp, Point(particles[i].x+offset, particles[i].y), 2, Scalar(0));
            }
            char str[30];
            sprintf(str, "../img/disp%d.png", x);

            //imwrite(str, mat_disp);
        }
    }
}


