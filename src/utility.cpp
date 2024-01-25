#include "utility.hpp"

static int InitGLFW(GLFWwindow*& window, GLFWmonitor*& monitor, GLFWvidmode*& mode, const char* window_name)
{
    int return_value = 0;
    if (!glfwInit())                                 // Inicjalizacja Graphical Library Framework
    {                                                // Tylko niektore funkcje moga byc uzyte bez inicjalizacji
        spdlog::error("Failed to initialized GLFW"); // glfwGetVersion ,glfwGetVersionString, glfwGetError, glfwSetErrorCallback, glfwInitHint, glfwInit, glfwTerminate
        return_value = - 1;
    }
    else
    {
        spdlog::info("GLFW initialized!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                 // Konfiguracja przyszlego okna
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    monitor = glfwGetPrimaryMonitor();               // Pelny ekran w oknie
    mode = (GLFWvidmode * )glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, window_name, NULL, NULL); // Tworzenie okna

    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return_value = -1;
    }
    else
    {
        spdlog::info("Window created!");
    }

    glfwMakeContextCurrent(window); // Zmiana kontekstu
    return return_value;
}

static int InitGlad()
{
    int return_value = 0;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        return -1;
    }
    else
    {
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST); //Z buffer, dzieki temu nie ma przenikania
        spdlog::info("GLAD initialized!");
    }

    return return_value;
}

static int InitMyGUI(GLFWwindow*& window)
{
    int return_value = 0;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        spdlog::error("Failed to ImGui_ImplGlfw_InitForOpenGL!");
        return_value = 1;
    }

    if (!ImGui_ImplOpenGL3_Init())
    {
        spdlog::error("Failed to ImGui_ImplOpenGL3_Init!");
        return_value = 1;
    }

    spdlog::info("ImGUI initialized!");

    return return_value;
}

static int GetTexturePathsFromFile(const std::string path, std::vector<std::string>& textures_paths)
{
    int return_value = 0;
    std::ifstream inputFile1(path);
    if (inputFile1.is_open())
    {
        std::string line;
        while (std::getline(inputFile1, line))
        {
            textures_paths.push_back(line);
        }
        inputFile1.close();
    }
    else
    {
        return_value = 1;
        spdlog::error("Trouble oppening file!!!");
    }

    return return_value;
}

static unsigned int LoadSingleTexture(const std::string filePath) 
{
    unsigned int texture_id = 0;
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) 
    {        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else 
    {
        spdlog::error("Failed to load texture!!!");
        texture_id = 0;
    }

    return texture_id;
}

static int LoadTextures(std::vector<unsigned int>& textures, const std::vector<std::string>& textures_paths)
{
    int return_value = 0;

    for (auto& path : textures_paths)
    {
        unsigned int id = LoadSingleTexture(path);
        if (id)
        {
            textures.push_back(id);
        }
        else
        {
            return_value = 1;
            break;
        }
    }

    return return_value;
}

static void FixCoordinates(Shader* shader)
{
    // Zmiana systemu wspolrzednych
    glm::mat4 model_matrix = glm::mat4(1.0f);     // trafsformacja do world space
    model_matrix = glm::rotate(model_matrix, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->setMatrix4("model_matrix", model_matrix);

    glm::mat4 view_matrix = glm::mat4(1.0f);       // transformacja do view space
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, -2.0f, -15.0f));
    shader->setMatrix4("view_matrix", view_matrix);

    glm::mat4 projection_matrix = glm::mat4(1.0f); // transformacja do clip space
    projection_matrix = glm::perspective(glm::radians(100.0f), (1920.0f / 1080.0f), 0.1f, 1000.0f);
    shader->setMatrix4("projection_matrix", projection_matrix);
}

static void CalculateMousePosition(double& x, double& y, GLFWvidmode*& mode)
{
    if (mode)
    {
        if (x < 0)
        {
            x = 0.0;
        }
        else if (x > mode->width)
        {
            x = (double)mode->width;
        }

        if (y < 0)
        {
            y = 0.0;
        }
        else if (y > mode->height)
        {
            y = (double)mode->height;
        }

    /*    x -= (double)mode->width * 0.5;
        y -= (double)mode->height * 0.5;

        x /= (double)mode->width * 0.5;
        y /= (double)mode->height * 0.5;*/
    }
}

