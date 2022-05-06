#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define ARRAY_SIZE 1000000

bool isComposite(int n) {
    if (n <= 3) return false;
    if (n % 2 == 0 || n % 3 == 0) {
        return true;
    }
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return true;
        }
    }
    return false;
}

int nums[ARRAY_SIZE];

int main() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        nums[i] = i;
    }
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (isComposite(nums[i])) {
//            printf("%d is a composite number\n", nums[i]);
        } else {
//            printf("%d is a prime number\n", nums[i]);
        }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    cout << time_span.count() << " Milliseconds" << endl;
    return 0;
}


