#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

struct Camera {
    float x, y, z;
    float roll,
          pitch,
          yaw;
};

void move_camera(struct Camera *cam);

#endif
