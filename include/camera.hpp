#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(std::size_t screen_width,
           std::size_t screen_height,
           float boundary) :
        lastx(screen_width / 2),
        lasty(screen_height / 2),
        upper_boundary(boundary - 0.2f),
        lower_boundary((-1 * boundary) + 0.2f)
        {}

    glm::vec3 get_pos() const;
    glm::vec3 get_front() const;
    glm::vec3 get_up() const;
    float get_fov() const;

    void update_pos(GLFWwindow* window);
    void update_angle(double xpos, double ypos);
    void update_pov(double yoffset);
    void update_frames();
private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    float lastx;
    float lasty;

    const float mouse_sensitivity = 0.05f;

    float yaw = -90.0f;
    float pitch = 0.0f;

    bool first_mouse = true;

    float fov = 45.0f;

    float upper_boundary;
    float lower_boundary;

    inline void constrain_to_boundary();
};

glm::vec3 Camera::get_pos() const
{
    return pos;
}

glm::vec3 Camera::get_front() const
{
    return front;
}

glm::vec3 Camera::get_up() const
{
    return up;
}

float Camera::get_fov() const
{
    return fov;
}

inline void Camera::constrain_to_boundary()
{
    if (pos.x > upper_boundary)
        pos.x = upper_boundary;
    if (pos.x < lower_boundary)
        pos.x = lower_boundary;

    if (pos.y > upper_boundary)
        pos.y = upper_boundary;
    if (pos.y < lower_boundary)
        pos.y = lower_boundary;

    if (pos.z > upper_boundary)
        pos.z = upper_boundary;
    if (pos.z < lower_boundary)
        pos.z = lower_boundary;
}

void Camera::update_pos(GLFWwindow* window)
{
    float camera_speed = 2.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos += camera_speed * front;
        constrain_to_boundary();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos -= camera_speed * front;
        constrain_to_boundary();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos -= glm::normalize(glm::cross(front, up)) * camera_speed;
        constrain_to_boundary();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos += glm::normalize(glm::cross(front, up)) * camera_speed;
        constrain_to_boundary();
    }
}

void Camera::update_angle(double xpos, double ypos)
{
    if (first_mouse)
    {
        lastx = xpos;
        lasty = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void Camera::update_pov(double yoffset)
{
    fov -= yoffset;

    if (fov <= 1.0f)
        fov = 1.0f;
    else if (fov >= 45.0f)
        fov = 45.0f;
}

void Camera::update_frames()
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}
