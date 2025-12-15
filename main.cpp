#include <print>
#include <algorithm>
#include <iterator>
#include <random>

#include "helpers.h"
#include "markov.h"

using namespace Cq;

const StdString TESTSTR = "Cq here hello world here Cq here good world";

const StdStringVector testLines = {
//	"This lines of code generate a general purpose Markov model from a given list of cleaned words (cleaned_text).",
//	"The function considers an n_gram here the default n value is 3 so itb",
//	"ll consider trigrams to create word pairs and then builds a Markov model representing the transitions between these pairs.",
	"Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, and what is the use of a book, thought Alice without pictures or conversations?"
	"So she was considering in her own mind (as well as she could, for the hot day made her feel very sleepy and stupid), whether the pleasure of making a daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly a White Rabbit with pink eyes ran close by her."
	"There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, Oh dear! Oh dear! I shall be late! (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge."
	"In another moment down went Alice after it, never once considering how in the world she was to get out again."
	"The rabbit-hole went straight on like a tunnel for some way, and then dipped suddenly down, so suddenly that Alice had not a moment to think about stopping herself before she found herself falling down a very deep well."
	"Either the well was very deep, or she fell very slowly, for she had plenty of time as she went down to look about her and to wonder what was going to happen next. First, she tried to look down and make out what she was coming to, but it was too dark to see anything; then she looked at the sides of the well, and noticed that they were filled with cupboards and book-shelves; here and there she saw maps and pictures hung upon pegs. She took down a jar from one of the shelves as she passed; it was labelled ORANGE MARMALADE, but to her great disappointment it was empty: she did not like to drop the jar for fear of killing somebody underneath, so managed to put it into one of the cupboards as she fell past it."
	"Well! thought Alice to herself, after such a fall as this, I shall think nothing of tumbling down stairs! How brave they ll all think me at home! Why, I wouldnt say anything about it, even if I fell off the top of the house! (Which was very likely true.)"
	"Down, down, down. Would the fall never come to an end? I wonder how many miles Ive fallen by this time? she said aloud. I must be getting somewhere near the centre of the earth. Let me see: that would be four thousand miles down, I think (for, you see, Alice had learnt several things of this sort in her lessons in the schoolroom, and though this was not a very good opportunity for showing off her knowledge, as there was no one to listen to her, still it was good practice to say it over) yes, thats about the right distance but then I wonder what Latitude or Longitude Ive got to? (Alice had no idea what Latitude was, or Longitude either, but thought they were nice grand words to say.)"
	"Presently she began again. I wonder if I shall fall right through the earth! How funny it ll seem to come out among the people that walk with their heads downward! The Antipathies, I think (she was rather glad there was no one listening, this time, as it didnt sound at all the right word) but I shall have to ask them what the name of the country is, you know. Please, Maam, is this New Zealand or Australia? (and she tried to curtsey as she spoke fancy curtseying as youre falling through the air! Do you think you could manage it?) And what an ignorant little girl shell think me for asking! No, itll never do to ask: perhaps I shall see it written up somewhere."
};

int main(void)
{
	const int N_GRAM = 3;
	const int NUM_ITERS = 2048;

	std::println("# of lines = {}", testLines.size());

	auto cleanSentences = makeCleanSentences(testLines);
	std::println("# of sentences = {}", cleanSentences.size());

	auto cleanWords = makeCleanWords(cleanSentences); //cleanSentences[0]});
	std::println("# of words = {}", cleanWords.size());
//	println(cleanWords);
	auto model = makeMarkovModel(cleanWords, N_GRAM);
	std::println("# of states = {}", model.size());
	//println(model);
	std::random_device rd;
	std::default_random_engine rne(rd());

	auto getRandState = [&rne, &model]() {
		MarkovModelStatePairVector randStates;
		std::sample(model.cbegin(), model.cend(), std::back_inserter(randStates), 1, rne);
		return randStates[0];
	};
	StdString curState = getRandState().first;
	std::println("Starting state = {}", curState);
//	StdString nextState = NO_WORD;
	StdString text = curState + " ";

	if(wordCount(curState) != N_GRAM)
	{
		println("Error: make sure that the initial curState is the same as N_GRAM!!!");
		return -1;
	}

	int n = 0;
	while(n < NUM_ITERS)
	{
		StdStringVector possibleWords;
		const auto modelCurStateBegin = model[curState].cbegin();
		const auto modelCurStateEnd = model[curState].cend();

		possibleWords.reserve(std::distance(modelCurStateBegin, modelCurStateEnd));
		std::transform(modelCurStateBegin, modelCurStateEnd, std::back_inserter(possibleWords), [](auto tran) {
			return tran.first;
		});

		if(possibleWords.size() == 0)
		{
			curState = getRandState().first;
			++n;
			continue;
		}

		//std::print("curState = \"{}\", possible next words = ", curState);
		//println(possibleWords);

		StdVector<float> probabilities;
		probabilities.reserve(possibleWords.size());
		std::transform(modelCurStateBegin, modelCurStateEnd, std::back_inserter(probabilities), [](auto tran) {
			return tran.second;
		});

		StdVector<int> indices;
		indices.reserve(possibleWords.size());

		std::discrete_distribution<> dist(probabilities.cbegin(), probabilities.cend());
		std::generate_n(std::back_inserter(indices), possibleWords.size(), [&dist, &rne]() {
			return dist(rne);
		});

		StdStringVector randWords;
		randWords.reserve(indices.size());
		std::transform(indices.cbegin(), indices.cend(), std::back_inserter(randWords), [&possibleWords](auto i) {
			return possibleWords[i];
		});

		curState = randWords[0];
		text.append(curState + " ");

		++n;
	}

	auto wc = wordCount(text);
	std::println("\n\nGenerated text (# of words = {}): \"{}\"", wc, text);

	return 0;
}
