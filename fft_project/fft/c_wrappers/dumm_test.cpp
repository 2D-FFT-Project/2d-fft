#include <complex>
#include <vector>
#include <cmath>
#include <iostream>

// testing base cpp case speed

const double PI = std::acos(-1);

void fft(std::vector<std::complex<double>>& a) {
    int n = a.size();
    if (n == 1) return;

    std::vector<std::complex<double>> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i + 1];
    }

    fft(a0);
    fft(a1);

    double ang = 2 * PI / n;
    std::complex<double> w(1), wn(std::cos(ang), std::sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n/2] = a0[i] - w * a1[i];
        w *= wn;
    }
}

void fft2d(std::vector<std::vector<std::complex<double>>>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        fft(a[i]);
    }
    for (int j = 0; j < n; j++) {
        std::vector<std::complex<double>> column(n);
        for (int i = 0; i < n; i++) {
            column[i] = a[i][j];
        }
        fft(column);
        for (int i = 0; i < n; i++) {
            a[i][j] = column[i];
        }
    }
}

int main() {
    std::vector<std::vector<std::complex<double>>> a = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}
    };
    fft2d(a);
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
