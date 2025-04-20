#pragma once

namespace al {
class LiveActorKit;

void executeUpdate(LiveActorKit*, const char*);
void executeUpdateTable(LiveActorKit*, const char*);
void executeUpdateList(LiveActorKit*, const char*, const char*);
void executeDraw(const LiveActorKit*, const char*);
void executeDrawList(const LiveActorKit*, const char*, const char*);
bool isActiveDraw(const LiveActorKit*, const char*);
void executeUpdateDrawBuffer(const LiveActorKit*);
void waitUpdateDrawBuffer(const LiveActorKit*);
void executeUpdateCalcView(const LiveActorKit*);
void waitUpdateCalcView(const LiveActorKit*);
void incrementDrawBufferCounter(const LiveActorKit*);

}  // namespace al
