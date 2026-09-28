#include <opencv2/opencv.hpp>

#include <chrono>

#include <iostream>

#include <thread>

#include <vector>



using namespace cv;

using namespace std;



void quantizeImage(Mat image, int bits) {

    const uchar mask = static_cast<uchar>(0xFF << (8 - bits));

    for (int i = 0; i < image.rows; i++) {

        uchar* p = image.ptr<uchar>(i);

        for (int j = 0; j < image.cols; j++)

            p[j] &= mask;

    }

}



int main() {

    Mat original = imread("image1.jpeg", IMREAD_GRAYSCALE);

    if (original.empty()) {

        cerr << "Could not open or find the image!" << endl;

        return -1;

    }







    // Sequential

    Mat image = original.clone();

    auto start = chrono::high_resolution_clock::now();

    quantizeImage(image, 4);

    auto end = chrono::high_resolution_clock::now();

    imwrite("quantized_image.png", image);

    cout << "Quantization time: "

        << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";







    // Parallel (4 quadrants)

    image = original.clone();

    int w1 = image.cols / 2, w2 = image.cols - w1;

    int h1 = image.rows / 2, h2 = image.rows - h1;



    start = chrono::high_resolution_clock::now();

    vector<thread> threads;

    threads.emplace_back(quantizeImage, image(Rect(0, 0, w1, h1)), 4);

    threads.emplace_back(quantizeImage, image(Rect(w1, 0, w2, h1)), 4);

    threads.emplace_back(quantizeImage, image(Rect(0, h1, w1, h2)), 4);

    threads.emplace_back(quantizeImage, image(Rect(w1, h1, w2, h2)), 4);

    for (auto& t : threads) t.join();

    end = chrono::high_resolution_clock::now();



    cout << "Parallel quantization time: "

        << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";

    imwrite("quantized_image1.jpeg", image);

    return 0;

}