#ifndef camera_h
#define camera_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Direction;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    float Yaw;
    float Pith;
    float MoveSpeed;
    float ObserveSpeed;
    float ScrollSpeed;
    
    //vector constructor. Определить через что задавать камеру, а что можно задать внутри списка инициализации
    //Что с Front?
    
    Camera(glm::vec3 position = vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
    {
        Position = position;
        WorldUp = up; //почему не просто up?
        Yaw = yaw;
        Pitch = pitch;
    }
}


#endif
