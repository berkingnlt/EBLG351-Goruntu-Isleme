#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 1. Görüntüyü gri tonlamalı oku
    Mat img = imread("image1.jpeg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "Gorsel bulunamadi!" << endl;
        return -1;
    }

    // 2. Histogram değişkenlerini ayarla
    int histSize = 256; 
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

    Mat hist;

    // 3. Histogramı hesapla
    calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

    // 4. Histogram grafiğini çizmek için boş bir tuval (görüntü) oluştur
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    // 5. Histogram değerlerini grafiğe sığacak şekilde normalize et (ölçeklendir)
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    // 6. Çizgi çizerek grafiği oluştur
    for (int i = 1; i < histSize; i++) {
        line(histImage,
            Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);
    }

    // 7. Görüntüleri ekranda göster
    imshow("Orijinal Gorsel", img);
    imshow("Histogram Grafigi", histImage);

    // Ekranda pencerelerin açık kalması için bir tuşa basılmasını bekle
    cout << "Grafikler ekranda acildi. Pencereleri kapatmak icin bir tusa basin..." << endl;
    waitKey(0);

    return 0;
}