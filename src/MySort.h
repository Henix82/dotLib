#pragma once
#include<iostream>
#include <vector>

template<typename T>
static void merge(std::vector<T>& arr, size_t leftIndex, size_t rightIndex) {
	size_t i = leftIndex;
	size_t midleIndex = leftIndex + (rightIndex - leftIndex) / 2;
	size_t j = midleIndex + 1;
	size_t pos = 0;
	std::vector<T> temp(rightIndex - leftIndex + 1);
	while (i <= midleIndex && j <= rightIndex) {
		if (arr[i] < arr[j]) {
			temp[pos++] = arr[i++];
		}
		else {
			temp[pos++] = arr[j++];
		}
	}
	while (i <= midleIndex) {
		temp[pos++] = arr[i++];
	}
	while (j <= rightIndex) {
		temp[pos++] = arr[j++];
	}
	for (size_t i = leftIndex; i <= rightIndex; i++)
	{
		arr[i] = std::move(temp[i - leftIndex]);
	}
}

template<typename T>
static void merge_sort(std::vector<T>& arr, size_t leftIndex, size_t rightIndex) {
	if (rightIndex <= leftIndex) { return; }
	size_t midleIndex = leftIndex + (rightIndex - leftIndex) / 2;
	merge_sort(arr, leftIndex, midleIndex);
	merge_sort(arr, midleIndex + 1, rightIndex);
	merge(arr, leftIndex, rightIndex);
}

template<typename T>
static void fast_sort(std::vector<T>& arr, size_t startIndex, size_t endIndex) {
	if (endIndex <= startIndex) { return; }
	T& pivot = arr[endIndex];
	size_t storeIndex = startIndex;
	for (size_t i = startIndex; i <= endIndex-1; i++)
	{
		if (arr[i] <= pivot) {
			std::swap(arr[i], arr[storeIndex++]);
		}
	}
	std::swap(arr[storeIndex], arr[endIndex]);
	if (storeIndex > startIndex) {
		fast_sort(arr, startIndex, storeIndex - 1);
	}
	if (storeIndex < endIndex) {
		fast_sort(arr, storeIndex + 1, endIndex);
	}
}

namespace MySort {
	template<typename T>
	void bubleSort(std::vector<T>& arr, bool(*compare)(const T&, const T&) 
		= [](const T& a, const T& b) {
		return a > b;  // Функция по умолчанию для сортировки по возрастанию
		}) 
	{
		bool swaped = false;
		auto length = arr.size();
		for (size_t i = length-1; i > 0; i--)
		{
			swaped = false;
			for (size_t j = 1; j <= i; j++)
			{
				if (compare(arr[j - 1], arr[j])) {
					std::swap(arr[j - 1], arr[j]);
					swaped = true;
				}
			}
			if (!swaped) {
				break;
			}
		}
	}

	template<typename T>
	void mergeSort(std::vector<T>& arr) {
		merge_sort(arr, 0, arr.size() - 1);
	}

	template<typename T>
	void fastSort(std::vector<T>& arr) {
		fast_sort(arr, 0, arr.size() - 1);
	}
}

