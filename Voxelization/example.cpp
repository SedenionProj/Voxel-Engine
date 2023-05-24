#include "libs/R/R/R.h"
int mouse = 0;
int scroll = 16.f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouse = 1;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        mouse = 2;
    else
        mouse = 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll += yoffset;
    if (scroll < 0)
        scroll = 0;
    else if (scroll > 100)
        scroll = 100;
}

int main(void)
{
    R::win::init("voxel",1280,720);
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

    std::vector<float> S;
    int number = 400;
    for (int x = 0; x < number; x++)
        for (int y = 0; y < number; y++)
            for (int z = 0; z < number; z++) {
                S.push_back(1);
                }


    R::ShaderStorageBuffer SSBO(sizeof(float)*number*number*number);
    SSBO.data(0, S, sizeof(float) * number * number * number,5);
    
    glfwSetCursorPosCallback(R::win::window, R::PerspectiveCamera::mouse_callback);

    bool hold = false;
    
    float t = 0;
    float r = 0;
    R::win::hideCursor();
    glfwSetMouseButtonCallback(R::win::window, mouse_button_callback);
    glfwSetScrollCallback(R::win::window, scroll_callback);


    GLint size;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &size);
    std::cout << "GL_MAX_SHADER_STORAGE_BLOCK_SIZE is " << size << " bytes." << std::endl;

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
        if (glfwGetKey(R::win::window, GLFW_KEY_Q) == GLFW_PRESS)
            r = 10.0f * R::dt;
        if (glfwGetKey(R::win::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            r = 1000.0f * R::dt;
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
        shaders.setInt("scroll", scroll);
        shaders.setInt("mouse", mouse);
        shaders.setInt("number", number);
        shaders.setVec3("rayOrigin", cam.pos.x, cam.pos.y, cam.pos.z);

        R::renderer::draw(va,ib,shaders);

        ImGui::Text(std::to_string(1/R::dt).c_str());
        ImGui::Text(std::to_string(scroll).c_str());

        R::win::drawGui();
        R::win::display();
    }

    R::win::closeGui();
    R::win::close();
    return 0;
}