#include "3Dgba/box3D.h"

void boxRotateYQ_c(AABBi &box, int32_t quadrant)
{
    switch (quadrant)
    {
    case 0:
        {
            int32_t swap = box.minX;
            box.maxX = -swap;
            box.minX = -box.maxX;
            swap = box.minZ;
            box.minZ = -box.maxZ;
            box.maxZ = -swap;
        }
        break;
    case 1:
        {
            int32_t swap = box.minX;
            box.minZ = swap;
            box.minX = -box.maxZ;
            swap = box.maxX;
            box.maxX = -box.minZ;
            box.maxZ = swap;
        }
        break;
    case 2:
        return;
        break;
    case 3:
        {
            int32_t swap = box.minX;
            box.maxZ = -swap;
            box.minX = box.minZ;
            swap = box.maxX;
            box.maxX = box.maxZ;
            box.minZ = -swap;
        }
        break;
    default:
        // not a valid quadrant
        break;
    }
}
