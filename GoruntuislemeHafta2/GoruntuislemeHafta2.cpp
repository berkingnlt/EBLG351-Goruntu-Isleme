#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

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

void runTest(const Mat& original, int num_threads) {
    Mat image = original.clone();
    auto start = chrono::high_resolution_clock::now();

    vector<thread> threads;
    int rows_per_thread = image.rows / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * rows_per_thread;
        int height = (i == num_threads - 1) ? (image.rows - start_row) : rows_per_thread;

        Mat roi = image(Rect(0, start_row, image.cols, height));
        threads.emplace_back(quantizeImage, roi, 4);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << num_threads << " Thread (Is parcacigi) ile sure:\t" << duration << " mikrosaniye\n";
}

int main() {
    Mat original = imread("image1.jpeg", IMREAD_GRAYSCALE);
    if (original.empty()) {
        cerr << "Hata: image1.jpeg bulunamadi!" << endl;
        return -1;
    }

    cout << "--- COK CEKIRDEKLI PERFORMANS TESTI BASLIYOR ---\n";
    cout << "Gorsel Boyutu: " << original.cols << "x" << original.rows << " piksel\n";

    unsigned int max_threads = thread::hardware_concurrency();
    cout << "Sisteminizdeki maksimum mantiksal cekirdek sayisi: " << max_threads << "\n\n";

    vector<int> test_threads = { 1, 2, 4, 8 };

    if (find(test_threads.begin(), test_threads.end(), max_threads) == test_threads.end() && max_threads > 0) {
        test_threads.push_back(max_threads);
    }

    for (int threads : test_threads) {
        runTest(original, threads);
    }

    cout << "\nTest tamamlandi. Sonuclari inceleyebilirsiniz." << endl;

    return 0;
}