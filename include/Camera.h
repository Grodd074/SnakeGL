#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <glad/glad.h>
#include "../dependencies/cglm/cglm.h"

enum Camera_Movement {
    Camera_Forward,
    Camera_Backward,
    Camera_Left,
    Camera_Right
};

float getCamZoom ();
void getCamFront (vec3* dest);
void getCamPos (vec3* dest);
void getCamViewMatrix (mat4* view);
void processCamKeyboard (enum Camera_Movement dir, float dt);
void processCamMouse (double xpos, double ypos, bool constrainPitch);
void processCamMouseScroll(double yoffset);

#endif //LEARNOPENGL_CAMERA_H
