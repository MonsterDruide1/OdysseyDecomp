#pragma once

#include <heap/seadHeap.h>

namespace al {
class AudioResourceDirector;

sead::Heap* getStationedHeap();
sead::Heap* getCurrentHeap();
sead::Heap* getSceneResourceHeap();
sead::Heap* getSceneHeap();
sead::Heap* getCourseSelectResourceHeap();
sead::Heap* getCourseSelectHeap();
sead::Heap* getWorldResourceHeap();
sead::Heap* tryFindNamedHeap(const char*);
sead::Heap* findNamedHeap(const char*);
void addNamedHeap(sead::Heap*, const char*);
void removeNamedHeap(const char*);
void createSequenceHeap();
void freeAllSequenceHeap();
void printAllSequenceHeap();
void createSceneHeap(const char*, bool);
void createSceneResourceHeap(const char*);
void isCreatedSceneResourceHeap();
void destroySceneHeap(bool);
void createCourseSelectHeap();
void destroyCourseSelectHeap();
void createWorldResourceHeap(bool);
void destroyWorldResourceHeap(bool);
void loadPlayerResource(const char*);
void freePlayerResource(const char*);
void setAudioResourceDirectorToMemorySystem(AudioResourceDirector*);

}  // namespace al
