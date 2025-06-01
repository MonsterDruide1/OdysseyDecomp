#pragma once

#include <heap/seadHeap.h>

namespace al {
class AudioResourceDirector;

sead::Heap* getStationedHeap();
sead::Heap* getSequenceHeap();
sead::Heap* getSceneResourceHeap();
sead::Heap* getSceneHeap();
sead::Heap* getCourseSelectResourceHeap();
sead::Heap* getCourseSelectHeap();
sead::Heap* getWorldResourceHeap();
sead::Heap* tryFindNamedHeap(const char* heapName);
sead::Heap* findNamedHeap(const char* heapName);
void addNamedHeap(sead::Heap* heap, const char* heapName);
void removeNamedHeap(const char* heapName);
void createSequenceHeap();
void freeAllSequenceHeap();
void printAllSequenceHeap();
void createSceneHeap(const char* stageName, bool backwards);
void createSceneResourceHeap(const char* stageName);
bool isCreatedSceneResourceHeap();
void destroySceneHeap(bool removeCategory);
void createCourseSelectHeap();
void destroyCourseSelectHeap();
void createWorldResourceHeap(bool useCategory);
void destroyWorldResourceHeap(bool removeCategory);
void loadPlayerResource(const char* categoryName);
void freePlayerResource(const char* categoryName);
void setAudioResourceDirectorToMemorySystem(AudioResourceDirector* audioResourceDirector);
}  // namespace al
