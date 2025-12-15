#ifndef LIB_CQ_MENTAL_MARKOV_H
#define LIB_CQ_MENTAL_MARKOV_H

#include <algorithm>
#include <regex>
#include <iterator>

#include "helpers.h"

using namespace Cq;

#define NO_WORD ""

StdStringVector makeCleanWords(const StdStringVector &lines)
{
	StdStringVector cleanedWords;
	std::regex wordRegex("\\b\\w+\\b");

	for(const auto &l : lines)
	{
		StdString trimmedLine = l;
		trim(trimmedLine);
		auto wordsBegin = std::sregex_iterator(trimmedLine.begin(), trimmedLine.end(), wordRegex);
		auto wordsEnd = std::sregex_iterator();
		std::transform(wordsBegin, wordsEnd, std::back_inserter(cleanedWords), [](auto word) {
			return word.str();
		});
	}

	return cleanedWords;
}


StdStringVector makeCleanSentences(const StdStringVector &lines)
{
	StdStringVector cleanedSentences;
	std::regex sentenceRegex("[^.!?]*[.!?]");

	for(const auto &l : lines)
	{
		StdString trimmedLine = l;
		trim(trimmedLine);
		auto sentencesBegin = std::sregex_iterator(trimmedLine.cbegin(), trimmedLine.cend(), sentenceRegex);
		auto sentencesEnd = std::sregex_iterator();
		std::transform(sentencesBegin, sentencesEnd, std::back_inserter(cleanedSentences), [](auto sentence) {
			return sentence.str();
		});
	}

	return cleanedSentences;
}

using MarkovModelStatePair = std::pair<StdString, StdMap<StdString, float>>;
using MarkovModelStatePairVector = StdVector<MarkovModelStatePair>;
using MarkovModel = StdMap<StdString, StdMap<StdString, float>>;

MarkovModel makeMarkovModel(const StdStringVector &textLines, int nGram = 3)
{
	MarkovModel model;
	for(int i = 0; i < textLines.size() - nGram; ++i)
	{
		StdString curState = NO_WORD, nextState = NO_WORD;
		for(int j = 0; j < nGram; ++j)
		{
			int curIndex = i + j;
			if(curIndex < textLines.size())
			{
				curState.append(textLines[curIndex] + " ");
			}

			int nextIndex = i + j + nGram;
			if(nextIndex < textLines.size())
			{
				nextState.append(textLines[nextIndex] + " ");
			}
		}

		trim(curState);
		trim(nextState);

		if(model.find(curState) == model.end())
		{
			model[curState] = {{nextState, 1.0f}};
		} else {
			if(model[curState].find(nextState) != model[curState].end())
			{
				model[curState][nextState] += 1.0f;
			} else {
				model[curState][nextState] = 1.0f;
			}
		}
	}

	for(const auto &[curState, transition] : model)
	{
		float total = std::accumulate(transition.cbegin(), transition.cend(), 0.0f, [](auto curSum, auto tran) {
			return curSum + tran.second;
		});

		for(const auto &[state, count] : transition)
		{
			model[curState][state] = count / total;
		}

	}

	return model;
}

int wordCount(const StdString &str)
{
	std::regex wordRegex("(\\w+)");
	auto wordsBegin = std::sregex_iterator(str.cbegin(), str.cend(), wordRegex);
	auto wordsEnd = std::sregex_iterator();
	return std::distance(wordsBegin, wordsEnd);
}

#endif
