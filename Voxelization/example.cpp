#include "libs/R/R/R.h"

int main(void)
{
    R::win::init();
    R::win::initGui();

    float vertices[] =
    {
            -1.0f, -1.0f, -0.0f,
             1.0f,  1.0f, -0.0f,
            -1.0f,  1.0f, -0.0f,
             1.0f, -1.0f, -0.0f
    };

    unsigned int indices[] =
    {
            0, 1, 2,
            0, 3, 1
    };

    Shader shaders("shaders/basicVert.glsl", "shaders/basicFrag.glsl");
    VertexArray va;
    R::VertexBuffer vb(sizeof(vertices),vertices);
    R::IndexBuffer ib(sizeof(indices),indices);
    va.AddBuffer(vb, 3, 3, 0);

    R::PerspectiveCamera cam(1);
    cam.setDirection(45, 0.0);

    std::vector<glm::vec4> S;
    int number = 200;

    for (int x = 0; x < number; x++)
        for (int y = 0; y < number; y++)
            for (int z = 0; z < number; z++) {
                if (glm::fract(sin(glm::dot(glm::vec3(x,y,z), glm::vec3(12.9898, 78.233,78))) * 43758.5453123)<0.001f) {
                    S.push_back(glm::vec4(0, 0, 0, 1));
                }
                else {
                    S.push_back(glm::vec4(0, 0, 0, 0));
                }

            }

    R::ShaderStorageBuffer SSBO(sizeof(glm::vec4)*number*number*number, S, 5);
    
    glfwSetCursorPosCallback(R::win::window, R::PerspectiveCamera::mouse_callback);

    bool hold = false;
    float t = 0;
    float r = 0;
    R::win::hideCursor();
    while (R::win::running)
    {
        
        t += 0.001;
		if (glfwGetKey(R::win::window, GLFW_KEY_F11) == GLFW_PRESS) {
			if (!hold) {
                R::win::fullScreen(true);
			}
			hold = true;
		}
		else {
			hold = false;
		}

        R::win::clear();
        R::win::clearGui();

        r = 100.0f * R::dt;
        t += 0.001;
        if (glfwGetKey(R::win::window, GLFW_KEY_L) == GLFW_PRESS)
            r = 10000.0f * R::dt;
        if (glfwGetKey(R::win::window, GLFW_KEY_W) == GLFW_PRESS)
            cam.pos += glm::normalize(glm::vec3(1.0, 0.0, 1.0) * cam.front) * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_S) == GLFW_PRESS)
            cam.pos -= glm::normalize(glm::vec3(1.0, 0.0, 1.0) * cam.front) * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_A) == GLFW_PRESS)
            cam.pos -= glm::normalize(glm::cross(cam.front, cam.up)) * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_D) == GLFW_PRESS)
            cam.pos += glm::normalize(glm::cross(cam.front, cam.up)) * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cam.pos += cam.up * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.pos -= cam.up * r;
        if (glfwGetKey(R::win::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetInputMode(R::win::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        shaders.setFloat("time", t);
        shaders.setInt("number", number);
        shaders.setVec3("rayOrigin", cam.pos.x, cam.pos.y, cam.pos.z);

        R::renderer::draw(va,ib,shaders);

        ImGui::Text(std::to_string(sizeof(glm::vec4)).c_str());

        R::win::drawGui();
        R::win::display();
    }

    R::win::closeGui();
    R::win::close();
    return 0;
}