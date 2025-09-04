#pragma once

namespace al {
class LiveActorKit;

void executeUpdate(LiveActorKit* actorKit, const char* tableName);
void executeUpdateTable(LiveActorKit* actorKit, const char* tableName);
void executeUpdateList(LiveActorKit* actorKit, const char* tableName, const char* listName);
void executeDraw(const LiveActorKit* actorKit, const char* tableName);
void executeDrawList(const LiveActorKit* actorKit, const char* tableName, const char* listName);
bool isActiveDraw(const LiveActorKit* actorKit, const char* tableName);
void executeUpdateDrawBuffer(const LiveActorKit* actorKit);
void waitUpdateDrawBuffer(const LiveActorKit* actorKit);
void executeUpdateCalcView(const LiveActorKit* actorKit);
void waitUpdateCalcView(const LiveActorKit* actorKit);
void incrementDrawBufferCounter(const LiveActorKit* actorKit);

}  // namespace al
