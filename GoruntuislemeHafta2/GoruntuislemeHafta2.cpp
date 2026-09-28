#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 1. Orijinal çizgili görseli oku
    Mat img = imread("cizgili.jpeg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "Gorsel bulunamadi! Adini cizgili.jpg yaptigindan emin ol." << endl;
        return -1;
    }

    Mat resized_x05, resized_x025;

    // 2. Çözünürlüğü x0.5 (yarı yarıya) düşür
    resize(img, resized_x05, Size(), 0.5, 0.5, INTER_NEAREST);

    // 3. Çözünürlüğü x0.25 (çeyreğe) düşür
    resize(img, resized_x025, Size(), 0.25, 0.25, INTER_NEAREST);

    // 4. Sonuçları klasöre kaydet
    imwrite("cizgili_x05.jpeg", resized_x05);
    imwrite("cizgili_x025.jpeg", resized_x025);

    cout << "Cozunurluk ve Aliasing testi tamamlandi! Klasoru kontrol edebilirsin." << endl;

    return 0;
}