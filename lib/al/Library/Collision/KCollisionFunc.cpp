#include "Library/Collision/KCollisionFunc.h"

#include "Library/Base/Macros.h"
#include "Library/Collision/KCollisionServer.h"

namespace alKCollisionFunc {

// NON_MATCHING: Instruction ordering (https://decomp.me/scratch/PmI4v)
ALWAYS_INLINE static void calcHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                                     const sead::Vector3f& position, const al::KCPrismData& data,
                                     const al::KCPrismHeader* header, u8 location) {
    switch (location) {
    case al::KCLocation_Face:
    case al::KCLocation_Edge1:
    case al::KCLocation_Edge2:
    case al::KCLocation_Edge3: {
        sead::Vector3f planePos;
        server->calcPosLocal(&planePos, &data, 0, header);
        projectToPlane(outHitPos, position, planePos,
                       *server->getNormal(data.faceNormalIndex, header));

        if (location == al::KCLocation_Face)
            return;

        sead::Vector3f edgeNormal;
        sead::Vector3f edgeDiff = position;

        if (location == al::KCLocation_Edge1) {
            edgeNormal = *server->getNormal(data.edgeNormalIndex[0], header);
            edgeDiff -= planePos;
        } else if (location == al::KCLocation_Edge2) {
            edgeNormal = *server->getNormal(data.edgeNormalIndex[1], header);
            edgeDiff -= planePos;
        } else if (location == al::KCLocation_Edge3) {
            edgeNormal = *server->getNormal(data.edgeNormalIndex[2], header);
            sead::Vector3f edgePos;
            server->calcPosLocal(&edgePos, &data, 1, header);
            edgeDiff -= edgePos;
        }

        f32 dist = edgeDiff.dot(edgeNormal);
        *outHitPos -= edgeNormal * dist;
        break;
    }

    case al::KCLocation_Corner12:
        server->calcPosLocal(outHitPos, &data, 0, header);
        break;
    case al::KCLocation_Corner23:
        server->calcPosLocal(outHitPos, &data, 1, header);
        break;
    case al::KCLocation_Corner31:
        server->calcPosLocal(outHitPos, &data, 2, header);
        break;
    }
}

// NON_MATCHING: See calcHitPos
void calcSphereHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                      const sead::Vector3f& position, const al::KCPrismData& data,
                      const al::KCPrismHeader* header, u8 location) {
    calcHitPos(outHitPos, server, position, data, header, location);
}

void projectToPlane(sead::Vector3f* out, const sead::Vector3f& position,
                    const sead::Vector3f& planePosition, const sead::Vector3f& planeNormal) {
    f32 dist = (position - planePosition).dot(planeNormal);
    out->setScaleAdd(-dist, planeNormal, position);
}

// NON_MATCHING: See calcHitPos
void calcDiskHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                    const sead::Vector3f& position, f32 halfHeight, const sead::Vector3f& normal,
                    const al::KCPrismData& data, const al::KCPrismHeader* header, u8 location) {
    calcHitPos(outHitPos, server, position, data, header, location);
}
}  // namespace alKCollisionFunc
