#include "cgamescenenode.hpp"

#include "../../memory/memory.hpp"

void CGameSceneNode::SetMeshGroupMask(uint64_t mask) {
    auto* skeleton = GetSkeletonInstance();
    if (skeleton) skeleton->m_modelState().m_MeshGroupMask() = mask;
}
