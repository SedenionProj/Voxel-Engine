#include "libs/R/R/R.h"

int main(void)
{
    //TODO : FBO (PBO,TBO), mesh, model, presets, scene, camera(better class), compute shaders, samper2D, image2D
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

    bool hold = false;

    glfwSetCursorPosCallback(R::win::window, R::PerspectiveCamera::mouse_callback);
    
    R::win::hideCursor();
    while (R::win::running)
    {
        

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

        R::renderer::draw(va,ib,shaders);

        ImGui::Text(std::to_string(1).c_str());

        R::win::drawGui();
        R::win::display();
    }

    R::win::closeGui();
    R::win::close();
    return 0;
}