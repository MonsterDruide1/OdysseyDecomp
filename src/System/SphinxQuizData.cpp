#include "System/SphinxQuizData.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

SphinxQuizData::SphinxQuizData(s32 arraySize) : mArraySize(arraySize) {
    mIsAnswerCorrectSphinxQuiz = new bool[mArraySize];
    mIsAnswerCorrectSphinxQuizAll = new bool[mArraySize];

    init();
}

void SphinxQuizData::init() {
    for (s32 i = 0; i < mArraySize; i++) {
        mIsAnswerCorrectSphinxQuiz[i] = false;
        mIsAnswerCorrectSphinxQuizAll[i] = false;
    }
}

void SphinxQuizData::answerCorrectSphinxQuiz(s32 index) {
    mIsAnswerCorrectSphinxQuiz[index] = true;
}

void SphinxQuizData::answerCorrectSphinxQuizAll(s32 index) {
    mIsAnswerCorrectSphinxQuizAll[index] = true;
}

bool SphinxQuizData::isAnswerCorrectSphinxQuiz(s32 index) const {
    return mIsAnswerCorrectSphinxQuiz[index];
}

bool SphinxQuizData::isAnswerCorrectSphinxQuizAll(s32 index) const {
    return mIsAnswerCorrectSphinxQuizAll[index];
}

void SphinxQuizData::write(al::ByamlWriter* writer) {
    for (s32 i = 0; i < mArraySize; i++) {
        writer->pushHash();
        writer->addBool("IsAnswerCorrectSphinxQuiz", mIsAnswerCorrectSphinxQuiz[i]);
        writer->addBool("IsAnswerCorrectSphinxQuizAll", mIsAnswerCorrectSphinxQuizAll[i]);
        writer->pop();
    }
}

void SphinxQuizData::read(const al::ByamlIter& save) {
    init();

    for (s32 i = 0; i < mArraySize; i++) {
        al::ByamlIter sphinxData{};
        save.tryGetIterByIndex(&sphinxData, i);
        sphinxData.tryGetBoolByKey(&mIsAnswerCorrectSphinxQuiz[i], "IsAnswerCorrectSphinxQuiz");
        sphinxData.tryGetBoolByKey(&mIsAnswerCorrectSphinxQuizAll[i],
                                   "IsAnswerCorrectSphinxQuizAll");
    }
}
