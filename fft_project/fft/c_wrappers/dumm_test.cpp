#include <complex>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>

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
    std::vector<std::vector<std::complex<double>>> a (1024, std::vector<std::complex<double>>(1024, 0));
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    fft2d(a);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate and print the elapsed time in seconds
    double time_taken = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    std::cout << "Time taken by function: " << time_taken << " seconds" << std::endl;
    return 0;
}
