#include "include/Camera.h"

float CAM_YAW = -90.0f;
float CAM_PITCH = 0.0f;
float CAM_SPEED = 2.5f;
float CAM_SENSITIVITY = 0.1f;
float CAM_ZOOM = 45.0f;

bool CAM_FirstMouse = true;
double CAM_LastX = 400, CAM_LastY = 400;
double CAM_Xoffset = 400;
double CAM_Yoffset = 400;
//vec3 CAM_Position = {9.5f, -7.5f, 25.0f};
vec3 CAM_Position = {0.0f, 0.0f, 3.0f};
vec3 CAM_Front;
vec3 CAM_Up;
vec3 CAM_Right;
vec3 CAM_WorldUp = {0.0f, 1.0f, 0.0f};

void getCamFront (vec3* dest)
{
    glm_vec3_copy(CAM_Front, *dest);
}

void getCamPos (vec3* dest)
{
    glm_vec3_copy(CAM_Position, *dest);
}

void getCamViewMatrix (mat4* view)
{
    vec3 cameraPosFront;
    cameraPosFront[0] = CAM_Position[0] + CAM_Front[0];
    cameraPosFront[1] = CAM_Position[1] + CAM_Front[1];
    cameraPosFront[2] = CAM_Position[2] + CAM_Front[2];

    glm_lookat(CAM_Position, cameraPosFront, CAM_Up, *view);
}

float getCamZoom ()
{
    return CAM_ZOOM;
}

void processCamKeyboard (enum Camera_Movement dir, float dt)
{
    float velocity = CAM_SPEED * dt;
    vec3 res1, res2;
    res1[0] = CAM_Front[0] * velocity;
    res1[1] = CAM_Front[1] * velocity;
    res1[2] = CAM_Front[2] * velocity;
    glm_cross(CAM_Front, CAM_Up, res2);
    glm_normalize(res2);
    res2[0] *= velocity;
    res2[1] *= velocity;
    res2[2] *= velocity;

    if (dir == Camera_Forward){
        CAM_Position[0] += res1[0]; CAM_Position[1] += res1[1]; CAM_Position[2] += res1[2];
    }
    if (dir == Camera_Backward){
        CAM_Position[0] -= res1[0]; CAM_Position[1] -= res1[1]; CAM_Position[2] -= res1[2];
    }
    if (dir == Camera_Left){
        CAM_Position[0] -= res2[0]; CAM_Position[1] -= res2[1]; CAM_Position[2] -= res2[2];
    }
    if (dir == Camera_Right){
        CAM_Position[0] += res2[0]; CAM_Position[1] += res2[1]; CAM_Position[2] += res2[2];
    }
}

void processCamMouse (double xpos, double ypos, bool constrainPitch)
{
    CAM_Xoffset = xpos - CAM_LastX;
    CAM_Yoffset = CAM_LastY - ypos; // reversed since y-coordinates range from bottom to top

    CAM_LastX = xpos;
    CAM_LastY = ypos;

    CAM_Xoffset *= CAM_SENSITIVITY;
    CAM_Yoffset *= CAM_SENSITIVITY;

    CAM_YAW += CAM_Xoffset;
    CAM_PITCH += CAM_Yoffset;

    if(constrainPitch)
    {
        if(CAM_PITCH > 89.0f)
            CAM_PITCH = 89.0f;
        if(CAM_PITCH < -89.0f)
            CAM_PITCH = -89.0f;
    }

    CAM_Front[0] = cos(glm_rad(CAM_YAW)) * cos(glm_rad(CAM_PITCH));
    CAM_Front[1] = sin(glm_rad(CAM_PITCH));
    CAM_Front[2] = sin(glm_rad(CAM_YAW)) * cos(glm_rad(CAM_PITCH));
    glm_normalize(CAM_Front);

    glm_cross(CAM_Front, CAM_WorldUp, CAM_Right);
    glm_normalize(CAM_Right);
    glm_cross(CAM_Right, CAM_Front, CAM_Up);
    glm_normalize(CAM_Up);
}

void processCamMouseScroll(double yoffset)
{
    CAM_ZOOM -= yoffset;
    if (CAM_ZOOM < 1.0f)
        CAM_ZOOM = 1.0f;
    if (CAM_ZOOM > 45.0f)
        CAM_ZOOM = 45.0f;
}
