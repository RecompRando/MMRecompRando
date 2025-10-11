#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Obj_Lupygamelift/z_obj_lupygamelift.h"

void func_80AF0394(ObjLupygamelift* this);

RECOMP_PATCH void func_80AF0530(ObjLupygamelift* this, PlayState* play) {
    f32 distRemaining;
    Vec3f target;

    target.x = this->points[this->pointIndex].x;
    target.y = this->points[this->pointIndex].y;
    target.z = this->points[this->pointIndex].z;
    distRemaining = Math_Vec3f_StepTo(&this->dyna.actor.world.pos, &target, this->dyna.actor.speed);
    if (distRemaining > 30.0f) {
        Math_SmoothStepToF(&this->dyna.actor.speed, this->targetSpeedXZ, 0.5f, 5.0f, 0.1f);
    } else if (distRemaining > 0.0f) {
        Math_SmoothStepToF(&this->dyna.actor.speed, 5.0f, 0.5f, 5.0f, 1.0f);
    } else {
        if (this->pointIndex < (this->count - 1)) {
            this->pointIndex++;
        } else {
            this->pointIndex = 0;
        }
    }
    // @rando fix bug where the game crashes when there is no deku flower child (soul)
    if (this->dyna.actor.child != NULL) {
        if (this->dyna.actor.child->update == NULL) {
            this->dyna.actor.child = NULL;
        } else {
            this->dyna.actor.child->world.pos.x = this->dyna.actor.world.pos.x;
            this->dyna.actor.child->world.pos.y = this->dyna.actor.world.pos.y;
            this->dyna.actor.child->world.pos.z = this->dyna.actor.world.pos.z;
        }
    }
    func_80AF0394(this);
}