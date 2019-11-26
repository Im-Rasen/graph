// GLEW нужно подключать до GLFW.
// GLEW
#define GLEW_STATIC
#define cimg_use_jpeg

#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include "shader.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include "lodepng.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Время, прошедшее между последним и текущим кадром
GLfloat deltaTime = 0.0f;
//Время вывода последнего кадра
GLfloat lastFrame = 0.0f;
//Степень смешивания тестур
float mixRate = 0.0f;
//Буфер нажатий
bool keys[1024];
//Камера
glm::vec3 cameraPosition  = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp        = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraTarget    = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
glm::vec3 cameraFront     = -cameraDirection;
//Углы
GLfloat yaw   = -90.0f;
GLfloat pitch = 0.0f;
//Смещение мыши
GLfloat lastX;
GLfloat lastY;
bool firstMouse = true;
GLfloat fov = 45.0f;
//Освещение
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

//Реализация нажатий
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Значения углов от мыши
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//Колесико мыши
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void doMovement();


int main()
{
    //Настройки GLFW и GLEW
    //Инициализация GLFW
    glfwInit();
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL.
    //Мажорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Для Mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
   
    
    
    //Создание объекта окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Good_Morning", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
        
    //Размер отрисовываемого окна
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    glViewport(0, 0, screenWidth, screenHeight); //Левый нижний угол, ширина, высота
    
    //Отслеживание глубины
    glEnable(GL_DEPTH_TEST);
    //Заполнение полигонов
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    //Шейдеры
    Shader ourShader("/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/shader.vs", "/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/shader.frag");
    Shader lampShader("/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/light_shader.vs","/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/light_shader.frag");

    //Куб
    float vertices[] = {
        // Позиции           // Текстурные // Нормали
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f
    };
    
    //Сдвиги кубиков
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    //VBO, VAO, EBO
    //Создаем Vertex Array Object
    GLuint objectVAO;
    glGenVertexArrays(1, &objectVAO);
    
    //Создаем Vertex Buffer Objects
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //Привязка буфера к VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Копируем вершинные данные в буфер + GL_STATIC_DRAW/GL_DYNAMIC_DRAW/GL_STREAM_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /*
    //Создаем Element Buffer Object
    GLuint EBO;
    glGenBuffers(1, &EBO);
    //Привязка буфера к EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Копируем вершинные данные в буфер
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
     */
    
    //Привязка VAO
    glBindVertexArray(objectVAO);
    
    //Устанавливаем указатели на вершинные атрибуты //location = 0, vec3,,normalize,step_between_data_packs,смещение_начала_данных

    // Атрибут с координатами
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    /*
    // Атрибут с цветом
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
     */
    // Атрибут с текстурой
    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    //Атрибут с нормалью
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    
    //Создание VAO для лампы
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    //Привязка VAO
    glBindVertexArray(lightVAO);
    //Привязка буфера к VBO (VBO - тот же)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Атрибут с координатами
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    //___Текстуры___
    //Идентификатор текстуры
    GLuint texture1;
    glGenTextures(1, &texture1);
    //Привязка конкретной текстуры
    glActiveTexture(GL_TEXTURE0); //Активируем текстурный блок
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    //Параметры семплинга
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT); // 3D
    /* Для GL_CLAMP_TO_BORDER параметра
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */

    // select modulate to mix texture with color for shading
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //Параметры МипМапов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR_MIPMAP_LINEAR / GL_LINEAR / GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    //Загрузка lodepng
    std::vector<unsigned char> image; //Пиксели будут тут
    unsigned texwidth, texheight;

    //Декодирование
    unsigned error = lodepng::decode(image, texwidth, texheight, "/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/container.png");

    //Ошибки
    if(error) std::cout << "DECODER::ERROR " << error << ": " << lodepng_error_text(error) << std::endl;
    
    unsigned char* data = &image[0]; //RGBARGBARGBA...
    
    //Генерация текстуры
    //Цель,уровень мипмапа,формат хранения,ширина,высота,прост,формат и тип данных исходника,данные изображения
    if (data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //Генерация мипмапов
    glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR IN TEXTURE LOADING" << std::endl;
    }
    
    //Освобождение памяти и отвзяка от изображения
    std::vector<unsigned char>().swap(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //----------------------------------------------------------
    
    //Идентификатор текстуры
    GLuint texture2;
    glGenTextures(1, &texture2);
    //Привязка конкретной текстуры
    glActiveTexture(GL_TEXTURE0); //Активируем текстурный блок
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    //Параметры семплинга
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
       //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT); // 3D
       /* Для GL_CLAMP_TO_BORDER параметра
        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        */
       
       // select modulate to mix texture with color for shading
       //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
       
       //Параметры МипМапов
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR_MIPMAP_LINEAR / GL_LINEAR / GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    //Загрузка lodepng
    //Декодирование
    error = lodepng::decode(image, texwidth, texheight, "/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/stay_cozy.png");

    //Ошибки
    if(error) std::cout << "DECODER::ERROR " << error << ": " << lodepng_error_text(error) << std::endl;
    
    data = &image[0]; //RGBARGBARGBA...
    
    //Генерация текстуры
    //Цель,уровень мипмапа,формат хранения,ширина,высота,прост,формат и тип данных исходника,данные изображения
    if (data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //Генерация мипмапов
    glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR IN TEXTURE LOADING" << std::endl;
    }
    
    //Освобождение памяти и отвзяка от изображения
    std::vector<unsigned char>().swap(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    //---Texture::END---
    
    //Игровой цикл
    while(!glfwWindowShouldClose(window))
    {
        //Клавиатура
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        //Захват мыши
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        //Колесико
        glfwSetScrollCallback(window, scroll_callback);
        //WASD
        doMovement();
        
        //Вычисление времени
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //Фоновый цвет
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //Шейдер
        
        ourShader.Use();
        ourShader.setVec3("lightPosition", lightPosition);
        //ourShader.setFloat("shiftX", 0.5);
        //ourShader.setFloat("mixRate", mixRate);
        
        //Обновление uniform
        
        //Цвет
        /*
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5; //0.0 .. 1.0
        ourShader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
         */
        
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.3f);
        ourShader.setVec3("lightColor", lightColor);
        ourShader.setVec3("objectColor", objectColor);
        
        
        //Локальные -> Мировые координаты
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //model = glm::rotate(model, -glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        
        //Движение сцены относительно Камеры
        glm::mat4 view = glm::mat4(1.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //view = glm::rotate(view, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.5, 0.3, 0.0));
        //Камера (Грама-Шмидта)
        GLfloat radius = 10.0f;
        //GLfloat camX = sin(glfwGetTime()) * radius;
        //GLfloat camY = 5.0f;
        //GLfloat camZ = cos(glfwGetTime()) * radius;
        //cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        //cameraPosition = glm::vec3(camX, camY, camZ);
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget); // Камера -> Z+
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // Камера -> X+
        cameraUp = glm::cross(cameraDirection, cameraRight); // Камера -> Y+
        //Позиция камеры, Цель камеры, Орт вверх
        //view = glm::lookAt(cameraPosition, cameraTarget, up);
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, up);
        // [R_x R_y R_z 0]   [1 0 0 -P_x]
        // [U_x U_y U_z 0] * [0 1 0 -P_y]
        // [D_x D_y D_z 0]   [0 0 1 -P_z]
        // [0   0   0   1]   [0 0 0   1 ]
        ourShader.setMat4("view", view);

        
        //Проекция
        glm::mat4 projection = glm::mat4(1.0f);
        //fov,,distance_min,distance_max
        projection = glm::perspective(glm::radians(fov), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        
        //Трансформация
        /*
        glm::mat4 trans(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        //ourShader.setMat4("transform", trans);
         */
        
        //glUseProgram(ourShader.Program);
        
        

        glBindVertexArray(objectVAO);
        
        /*
        Текстуры
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
         */

        
        //glActiveTexture(GL_TEXTURE0); //Активируем текстурный блок
        //glBindTexture(GL_TEXTURE_2D, texture);
        
        //Отрисовка
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        /*
        for(GLuint i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = (GLfloat)glfwGetTime() * glm::radians(15.0f + i*10.0f);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */
        
        //Лампа
        
        lampShader.Use();
        model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPosition);
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        GLfloat shiftX = sin(glfwGetTime()) * radius;
        GLfloat shiftZ = cos(glfwGetTime()) * radius;
        lampShader.setFloat("shiftX", shiftX);
        lampShader.setFloat("shiftZ", shiftZ);
        glm::vec3 shift = glm::vec3(shiftX, 0.0f, shiftZ);
        lightPosition += shift;
        
        //glUseProgram(lampShader.Program);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Смена буферов
        glfwSwapBuffers(window);
    }
    
    
    //Очищаем ресурсы
    glfwTerminate();
    
    return 0;
}

//Реализация нажатий
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
      keys[key] = true;
    else if(action == GLFW_RELEASE)
      keys[key] = false;
    
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
    // и приложение после этого закроется
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    // Изменение степени проявления текстур кнопками-стрелками (вверх и вниз)
    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixRate += 0.1f;
    if(mixRate >= 1.0f)
        mixRate = 1.0f;
    }
    
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixRate -= 0.1f;
    if (mixRate <= 0.0f)
        mixRate = 0.0f;
    }
}

void doMovement()
{
  // Управление камерой
  GLfloat cameraSpeed = 5.0f * deltaTime;
  if(keys[GLFW_KEY_W])
      cameraPosition += cameraSpeed * cameraFront;
  if(keys[GLFW_KEY_S])
      cameraPosition -= cameraSpeed * cameraFront;
  if(keys[GLFW_KEY_A])
      cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if(keys[GLFW_KEY_D])
      cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }
    GLfloat xoffset = xpos - lastX;
    //Оконные Y-координаты возрастают с верху вниз
    GLfloat yoffset = -(ypos - lastY);
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;
    
    //Вычисление угла
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
      fov -= yoffset;
  if(fov <= 1.0f)
      fov = 1.0f;
  if(fov >= 45.0f)
      fov = 45.0f;
}
