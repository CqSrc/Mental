#ifndef LIB_CQ_MENTAL_HELPERS_H
#define LIB_CQ_MENTAL_HELPERS_H

#include <cstdio>
#include <string>
#include <vector>
#include <print>
#include <unordered_map>
#include <map>
#include <format>

using StdString = std::string;

template<typename ElemType>
using StdVector = std::vector<ElemType>;

using StdStringVector = StdVector<StdString>;

template<typename FirstType, typename SecondType>
using StdPair = std::pair<FirstType, SecondType>;

template<typename KeyType, typename ValueType>
using StdMap = std::map<KeyType, ValueType>;

template<typename KeyType, typename ValueType>
using StdUnorderedMap = std::unordered_map<KeyType, ValueType>;

template<typename ElemType>
ElemType mostFrequentElemen(StdVector<ElemType> &arr)
{
	int n = arr.size();
	StdUnorderedMap<ElemType, int> freq;

	for (int i = 0; i < n; i++) freq[arr[i]]++;

	int maxCnt = 0;
	ElemType result;

	for (const auto &[val, cnt] : freq)
	{
		if (maxCnt < cnt)
		{
			result = val;
			maxCnt = cnt;
		}
	}

	return result;
}

// Trim leading whitespace
void ltrim(StdString &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// Trim trailing whitespace
void rtrim(StdString &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// Trim both leading and trailing whitespace
void trim(StdString &s)
{
	ltrim(s);
	rtrim(s);
}

namespace Cq
{
	namespace Mental
	{
		struct Point2D
		{
			float x = 0.0f, y = 0.0f;
		};

		using Point2DVector = StdVector<Point2D>;

		float euclideanDistance(Point2D p1, Point2D p2)
		{
			Point2D diff{std::pow(p1.x - p2.x, 2.0f), std::pow(p1.y - p2.y, 2.0f)};
			auto sum = diff.x + diff.y;
			return std::sqrt(sum);
		}

		template<typename LabelType>
		LabelType knnPredict(const Point2DVector &trainingData, const StdVector<LabelType> &trainingLabels, Point2D testPoint, int k)
		{
			StdVector<std::pair<float, LabelType>> dists;

			for(int i = 0; i < trainingData.size(); ++i)
			{
				const auto d = euclideanDistance(testPoint, trainingData[i]);
				dists.push_back({d, trainingLabels[i]});
			}

			std::sort(std::begin(dists), std::end(dists), [](auto d1, auto d2) {
				return d1.first < d2.first;
			});

			StdVector<LabelType> knnLabels;
			knnLabels.reserve(k);

			for(int i = 0; i < k; ++i)
			{
				knnLabels.push_back(dists[i].second);
			}

			return mostFrequentElement(knnLabels);
		}
	}

	void test(void)
	{
		std::println("blah testing...");
	}

	template<typename T>
	void print(const T &t)
	{
		std::print("{}", t);
	}

	template<typename FirstType, typename SecondType>
	void print(const StdPair<FirstType, SecondType> &p)
	{
		print("(");
		print(p.first);
		print(", ");
		print(p.second);
		print(")");
	}

	template<typename T>
	void print(const StdVector<T> &v)
	{
		std::print("{}", v);
	}

	template<typename KeyType, typename ValueType>
	void print(const StdMap<KeyType, ValueType> &m)
	{
		std::print("{}", m);
	}

	template<typename T>
	void println(const T &t)
	{
		std::println("{}", t);
	}

	template<typename FirstType, typename SecondType>
	void println(const StdPair<FirstType, SecondType> &p)
	{
		print(p);
		print("\n");
	}

	template<typename T>
	void println(const StdVector<T> &v)
	{
		std::println("{}", v);
	}

	template<typename KeyType, typename ValueType>
	void println(const StdMap<KeyType, ValueType> &m)
	{

		std::println("{}", m);
	}

	template<typename KeyType, typename ValueType>
	void println(const StdUnorderedMap<KeyType, ValueType> &m)
	{
		std::println("{}", m);
	}
}

#endif
