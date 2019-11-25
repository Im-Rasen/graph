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

float mixRate = 0.2f;
//Реализация нажатий
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main()
{
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
    
    
    //Шейдеры
    Shader ourShader("/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/shader.vs", "/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/shader.frag");

    
    //Четырехугольник
    GLfloat vertices[] = {
        // Позиции          // Цвета             // Текстурные координаты
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };
    //Порядок обхода индексов
    GLuint indices[] = {
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };
    
    
    //Создаем Vertex Array Object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    //Привязка VAO
    glBindVertexArray(VAO);
    //{
    
    //Создаем Vertex Buffer Objects
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //Привязка буфера к VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Копируем вершинные данные в буфер + GL_STATIC_DRAW/GL_DYNAMIC_DRAW/GL_STREAM_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Создаем Element Buffer Object
    GLuint EBO;
    glGenBuffers(1, &EBO);
    //Привязка буфера к EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Копируем вершинные данные в буфер
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //Устанавливаем указатели на вершинные атрибуты //location = 0, vec3,,normalize,step_between_data_packs,смещение_начала_данных

    // Атрибут с координатами
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Атрибут с цветом
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Атрибут с текстурой
    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    //}
    //Отвязываем VAO
    glBindVertexArray(0);
    
    
    //___Текстуры___
    
    //Идентификатор текстуры
    GLuint texture1;
    glGenTextures(1, &texture1);
    //Привязка конкретной текстуры
    glActiveTexture(GL_TEXTURE0); //Активируем текстурный блок 
    glBindTexture(GL_TEXTURE_2D, texture1);
    
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR_MIPMAP_LINEAR / GL_LINEAR / GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    
    //Загрузка lodepng
    std::vector<unsigned char> image; //Пиксели будут тут
    unsigned texwidth, texheight;

    //Декодирование
    unsigned error = lodepng::decode(image, texwidth, texheight, "/Users/JulieClark/Documents/ВМК/graphics/mr_Meeseeks/mr_Meeseeks/graph/container.png");

    //Ошибки
    if(error) std::cout << "DECODER::ERROR " << error << ": " << lodepng_error_text(error) << std::endl;
    
    unsigned char* data = &image[0]; //RGBARGBARGBA...

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    
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

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    
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
    
    //___Текстуры_End___
    
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    //Игровой цикл
    while(!glfwWindowShouldClose(window))
    {
        //Клавиатура
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        
        //Фоновый цвет
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Шейдер
        ourShader.Use();
        
        //Обновление uniform
        //Цвет
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5; //что-то 0.0 .. 1.0
        //Локальные -> Мировые координаты
        glm::mat4 model(1.0f);
        model = glm::rotate(model, -glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //Движение сцены относительно Камеры
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //Проекция
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
        //Трансформация
        glm::mat4 trans(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        
        
        
        
        //Цвет
        GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "ourColor");
        //Сдвиг
        GLint vertexShiftLocation = glGetUniformLocation(ourShader.Program, "shiftX");
        //Блендинг
        GLint mixRateLocation = glGetUniformLocation(ourShader.Program, "mixRate");
        //Трансформация
        GLuint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
        //Локальные -> Мировые координаты
        GLuint modelLocation = glGetUniformLocation(ourShader.Program, "model");
        //Движение сцены относительно Камеры
        GLuint viewLocation = glGetUniformLocation(ourShader.Program, "view");
        //Проекция
        GLuint projectionLocation = glGetUniformLocation(ourShader.Program, "projection");
        
        
        
        glUseProgram(ourShader.Program);
        
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUniform1f(vertexShiftLocation, 0.5); // ПОТОМ ЗАДАТЬ ЗНАЧЕНИЕ СДВИГА
        glUniform1f(mixRateLocation, mixRate);
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        

        glBindVertexArray(VAO);
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

        
        //glActiveTexture(GL_TEXTURE0); //Активируем текстурный блок
        //glBindTexture(GL_TEXTURE_2D, texture);
        
        // previous version (only VAO without EBO)
        //Тип, начальный индекс массива с вершинами, количество вершин для отрисовки
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        /*
        //Второй объект
        
        //Трансформация
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleRate = sin(glfwGetTime());
        trans = glm::scale(trans, glm::vec3(scaleRate, scaleRate, scaleRate));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        */
        glBindVertexArray(0);
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
