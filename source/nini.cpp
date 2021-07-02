#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>

int in_range(int a) {
    if (a < 0) {
        return 0;
    }
    if (a > 255) {
        return 255;
    }
    return a;
}

int trueX;
int trueY;
std::string path = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
int max;
int min;
int xs;
int ys;
int slider1;
int slider2;
int sliderB = 2;
float float_img[15000][15000];
int dispx;
int dispy;
int cmax;
int cmin;
int bind_size;
cv::Mat disp;
std::string windowName = "nini vision";
cv::Mat out;
cv::Mat img;
static void update_max(int slider1, void*) {
    max = slider1;

    cvSetTrackbarMax("Min", windowName.c_str(), max - 1);
    if (min > max) {
        cvSetTrackbarPos("Min", windowName.c_str(), max-1);
    }

    float m = -min;
    float k = 255 / (max + m);
    for (int x = 0; x < xs; x++) {
        for (int y = 0; y < ys; y++) {
            int gray_color = in_range(round((float_img[x][y] + m) * k));
            cv::Vec3b color = (char(gray_color), char(gray_color), char(gray_color));
            out.at<cv::Vec3b>(x, y)[0] = gray_color;
            out.at<cv::Vec3b>(x, y)[1] = gray_color;
            out.at<cv::Vec3b>(x, y)[2] = gray_color;
        }
    }
    cv::resize(out, disp, cv::Size(dispx, dispy));
    cv::imshow(windowName, disp);
    cv::imwrite("out.png", out);
}
static void update_min(int slider2, void*) {
    min = slider2;

    cvSetTrackbarMin("Max", windowName.c_str(), min+1);
    if (min > max) {
        cvSetTrackbarPos("Max", windowName.c_str(), min+1);
    }

    float m = -min;
    float k = 255 / (max + m);
    for (int x = 0; x < xs; x++) {
        for (int y = 0; y < ys; y++) {
            int gray_color = in_range(round((float_img[x][y] + m) * k));
            cv::Vec3b color = (char(gray_color), char(gray_color), char(gray_color));
            out.at<cv::Vec3b>(x, y)[0] = gray_color;
            out.at<cv::Vec3b>(x, y)[1] = gray_color;
            out.at<cv::Vec3b>(x, y)[2] = gray_color;
        }
    }
    cv::resize(out, disp, cv::Size(dispx, dispy));
    cv::imshow(windowName, disp);
    cv::imwrite("out.png", out);
}

static void update_bind(int sliderB, void*) {
    bind_size = sliderB;
    if (bind_size < 2) {
        bind_size = 2;
    }
    out = cv::Mat::zeros(cv::Size(int(floor(trueX / bind_size)), int(floor(trueY / bind_size))), CV_8UC3);
    max = 0;
    min = 255 * bind_size * bind_size;
    xs = out.size().height;
    ys = out.size().width;
    for (int x = 0; x < xs; x++) {
        for (int y = 0; y < ys; y++) {
            float total = 0;
            for (int bx = 0; bx < bind_size; bx++) {
                for (int by = 0; by < bind_size; by++) {
                    cv::Vec3b color = img.at<cv::Vec3b>(x * bind_size + bx, y * bind_size + by);
                    total += int(color[0]);
                    total += int(color[1]);
                    total += int(color[2]);
                }
            }
            total /= bind_size * bind_size * 3;
            float_img[x][y] = total;
            if (total > max) {
                max = total;
            }
            if (total < min) {
                min = total;
            }
        }
    }
    cmax = max;
    cmin = min;
    cvSetTrackbarMax("Max", windowName.c_str(), cmax);
    cvSetTrackbarPos("Max", windowName.c_str(), cmax);
    cvSetTrackbarMax("Min", windowName.c_str(), cmax-1);
    cvSetTrackbarMin("Min", windowName.c_str(), cmin);
    cvSetTrackbarPos("Min", windowName.c_str(), cmin);
    
    float m = -min;
    float k = 255 / (max + m);
    for (int x = 0; x < xs; x++) {
        for (int y = 0; y < ys; y++) {
            int gray_color = in_range(round((float_img[x][y] + m) * k));
            cv::Vec3b color = (char(gray_color), char(gray_color), char(gray_color));
            out.at<cv::Vec3b>(x, y)[0] = gray_color;
            out.at<cv::Vec3b>(x, y)[1] = gray_color;
            out.at<cv::Vec3b>(x, y)[2] = gray_color;
        }
    }
    cv::resize(out, disp, cv::Size(dispx, dispy));
    cv::imshow(windowName, disp);
    cv::imwrite("out.png", out);
}

int main()
{
    bind_size = 2;
    std::cout << "Path to image: ";
    std::cin >> path;
    img = cv::imread(path, cv::IMREAD_COLOR);
    trueX = img.size().width;
    trueY = img.size().height;

    out = cv::Mat::zeros(cv::Size(int(floor(img.size().width/bind_size)), int(floor(img.size().height / bind_size))), CV_8UC3);

    max = 0;
    min = 255*bind_size*bind_size;
    xs = out.size().height;
    ys = out.size().width;
    for (int x = 0; x < xs; x++) {
        for (int y = 0; y < ys; y++) {
            float total = 0;
            for (int bx = 0; bx < bind_size; bx++) {
                for (int by = 0; by < bind_size; by++) {
                    cv::Vec3b color = img.at<cv::Vec3b>(x * bind_size + bx, y * bind_size + by);
                    total += int(color[0]);
                    total += int(color[1]);
                    total += int(color[2]);
                }
            }
            total /= bind_size * bind_size * 3;
            float_img[x][y] = total;
            if (total > max) {
                max = total;
            }
            if (total < min) {
                min = total;
            }
        }
    }

    cmax = max;
    cmin = min;

    if (640 < round((ys / xs) * 640)) {
        dispx = round((float(ys) / float(xs)) * 640.0);
        dispy = 640;
    }
    else {
        dispx = 640;
        dispy = round((float(xs) / float(ys)) * 640.0);
    }

    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

    slider2 = cmin;
    slider1 = cmax;
    cv::createTrackbar("Binning", windowName, &sliderB, 100, update_bind);
    cv::createTrackbar("Max", windowName, &slider1, cmax, update_max);
    cv::createTrackbar("Min", windowName, &slider2, cmin, update_min);
    cvSetTrackbarMax("Max", windowName.c_str(), cmax);
    cvSetTrackbarPos("Max", windowName.c_str(), cmax);
    cvSetTrackbarMax("Min", windowName.c_str(), cmax - 1);
    cvSetTrackbarMin("Min", windowName.c_str(), cmin);
    cvSetTrackbarPos("Min", windowName.c_str(), cmin);

    update_max(max, 0);

    cv::waitKey(0);

    return 0;
}
