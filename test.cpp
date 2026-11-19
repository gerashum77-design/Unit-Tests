#include "pch.h"
#include <gtest/gtest.h>
// g++ -std=c++17 test_sorting.cpp algo.cpp -lgtest -lgtest_main -pthread && ./a.out
#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std; 




// Unit test Sorting (Сортировка) 
void bubbleSortBad(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1]) std::swap(arr[j], arr[j + 1]);
}

std::vector<int> mergeSortGood(const std::vector<int>& arr) {
    if (arr.size() <= 1) return arr;
    int mid = static_cast<int>(arr.size()) / 2;
    std::vector<int> left = mergeSortGood({ arr.begin(), arr.begin() + mid });
    std::vector<int> right = mergeSortGood({ arr.begin() + mid, arr.end() });
    std::vector<int> result;
    size_t i = 0, j = 0;
    while (i < left.size() && j < right.size())
        left[i] < right[j] ? result.push_back(left[i++]) : result.push_back(right[j++]);
    result.insert(result.end(), left.begin() + i, left.end());
    result.insert(result.end(), right.begin() + j, right.end());
    return result;
}

static std::vector<int> ref(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return v;
}

TEST(Sorting, TaskExamples) {
    std::vector<int> a = { 64, 34, 25, 12, 22, 11, 90 };
    bubbleSortBad(a);
    EXPECT_EQ(a, ref({ 64, 34, 25, 12, 22, 11, 90 }));
    EXPECT_EQ(mergeSortGood({ 38, 27, 43, 3, 9, 82, 10 }),
        ref({ 38, 27, 43, 3, 9, 82, 10 }));
}

TEST(Sorting, EmptyAndSingleElement) {
    std::vector<int> empty = {}, single = { 42 };
    bubbleSortBad(single);
    EXPECT_EQ(single, ref({ 42 }));
    EXPECT_EQ(mergeSortGood(empty), ref(empty));
    EXPECT_EQ(mergeSortGood({ 42 }), ref({ 42 }));
}

TEST(Sorting, DuplicatesAndNegatives) {
    EXPECT_EQ(mergeSortGood({ 2, 2, 2, 2 }), ref({ 2, 2, 2, 2 }));
    std::vector<int> a = { -3, 7, -1, 0, 7 };
    bubbleSortBad(a);
    EXPECT_EQ(a, ref({ -3, 7, -1, 0, 7 }));
}

TEST(Sorting, SortedReversedAndImmutability) {
    std::vector<int> original = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(mergeSortGood(original), original);
    EXPECT_EQ(original, (std::vector<int>{1, 2, 3, 4, 5}));  // вход не изменён
    std::vector<int> rev(50);
    for (int i = 0; i < 50; ++i) rev[i] = 50 - i;
    bubbleSortBad(rev);
    EXPECT_EQ(rev, ref(rev));
}

TEST(Sorting, RandomArraysAgreeWithReference) {
    // 1000 случайных массивов против эталонного std::sort
    std::mt19937 rng(123);
    for (int t = 0; t < 1000; ++t) {
        int n = static_cast<int>(rng() % 51);
        std::vector<int> arr(n);
        for (auto& x : arr) x = static_cast<int>(rng() % 201) - 100;
        std::vector<int> copy = arr;
        bubbleSortBad(arr);
        EXPECT_EQ(arr, ref(copy));
        EXPECT_EQ(mergeSortGood(copy), ref(copy));
    }
}

TEST(Sorting, MergeSortHandlesLargeInput) {
    // 100000 элементов: merge sort обязан справиться
    std::mt19937 rng(7);
    std::vector<int> arr(100'000);
    for (auto& x : arr) x = static_cast<int>(rng() % 2'000'001) - 1'000'000;
    EXPECT_EQ(mergeSortGood(arr), ref(arr));
}







// Unit Test Search (Поиск) 
// g++ -std=c++17 test_search.cpp algo.cpp -lgtest -lgtest_main -pthread && ./a.out
#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

int linearSearchBad(const vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i)
        if (arr[i] == target) return static_cast<int>(i);
    return -1;
}

int binarySearchGood(const vector<int>& arr, int target) {
    int left = 0, right = static_cast<int>(arr.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

static void expectFound(const vector<int>& arr, int result, int target) {
    ASSERT_NE(result, -1);                       // при дублях любой индекс валиден,
    EXPECT_EQ(arr[result], target);              // если указывает на цель
}

TEST(Search, ExamplesFromTask) {
    EXPECT_EQ(linearSearchBad({ 1, 3, 5, 7, 9 }, 7), 3);
    EXPECT_EQ(linearSearchBad({ 10, 20, 30, 40 }, 25), -1);
    EXPECT_EQ(binarySearchGood({ 1, 3, 5, 7, 9 }, 7), 3);
    EXPECT_EQ(binarySearchGood({ 2, 4, 6, 8, 10 }, 5), -1);
}

TEST(Search, EmptyAndSingleElement) {
    EXPECT_EQ(linearSearchBad({}, 5), -1);
    EXPECT_EQ(binarySearchGood({}, 5), -1);
    EXPECT_EQ(linearSearchBad({ 5 }, 5), 0);
    EXPECT_EQ(binarySearchGood({ 5 }, 5), 0);
    EXPECT_EQ(binarySearchGood({ 5 }, 4), -1);
}

TEST(Search, FirstLastAndOutOfBounds) {
    vector<int> arr = { 1, 3, 5, 7, 9 };
    EXPECT_EQ(binarySearchGood(arr, 1), 0);
    EXPECT_EQ(binarySearchGood(arr, 9), 4);
    EXPECT_EQ(linearSearchBad(arr, 0), -1);
    EXPECT_EQ(linearSearchBad(arr, 100), -1);
    EXPECT_EQ(binarySearchGood(arr, 0), -1);
    EXPECT_EQ(binarySearchGood(arr, 100), -1);
}

TEST(Search, Duplicates) {
    vector<int> ones(5, 1);
    EXPECT_EQ(linearSearchBad(ones, 1), 0);
    expectFound(ones, binarySearchGood(ones, 1), 1);
    vector<int> mixed = { -5, -2, -2, 0, 3, 3, 3, 8 };
    EXPECT_EQ(linearSearchBad(mixed, 3), 4);
    expectFound(mixed, binarySearchGood(mixed, 3), 3);
    expectFound(mixed, binarySearchGood(mixed, -2), -2);
}

TEST(Search, RandomAgreementWithStdFind) {
    // 500 случайных массивов: обе реализации согласованы с эталоном std::find
    mt19937 rng(42);
    for (int t = 0; t < 500; ++t) {
        int n = static_cast<int>(rng() % 61);
        vector<int> arr(n);
        for (auto& x : arr) x = static_cast<int>(rng() % 201) - 100;
        sort(arr.begin(), arr.end());
        for (int k = 0; k < 10; ++k) {
            int target = static_cast<int>(rng() % 211) - 105;
            bool exists = find(arr.begin(), arr.end(), target) != arr.end();
            int lin = linearSearchBad(arr, target);
            int bin = binarySearchGood(arr, target);
            EXPECT_EQ(lin == -1, !exists);
            EXPECT_EQ(bin == -1, !exists);
            if (exists) { EXPECT_EQ(arr[lin], target); EXPECT_EQ(arr[bin], target); }
        }
    }
}

TEST(Search, BinarySearchOnLargeSortedArray) {
    // Миллион элементов: границы и промах мимо нечётного числа
    vector<int> arr;
    arr.reserve(1'000'000);
    for (int x = 0; x < 2'000'000; x += 2) arr.push_back(x);
    EXPECT_EQ(binarySearchGood(arr, 0), 0);
    EXPECT_EQ(binarySearchGood(arr, 1'999'998), 999'999);
    EXPECT_EQ(binarySearchGood(arr, 999'999), -1);
    EXPECT_EQ(linearSearchBad(vector<int>(arr.begin(), arr.begin() + 1000), 500), 250);
}





