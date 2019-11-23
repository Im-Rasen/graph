#ifndef shader_h
#define shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL

class Shader
{
public:
    // Идентификатор программы
    GLuint Program;
    // Конструктор считывает и собирает шейдер
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
        
        
        // 1. Получаем исходный код шейдера из filePath
        
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //Удостоверимся, что ifstream объекты могут выкидывать исключения
        vShaderFile.exceptions(std::ifstream::failbit);
        fShaderFile.exceptions(std::ifstream::failbit);
        try
        {
            //Открываем файлы
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //Считываем данные в потоки
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //Закрываем файлы
            vShaderFile.close();
            fShaderFile.close();
            //Преобразовываем потоки в массив GLchar
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();
        
        
        // 2. Сборка шейдеров
        
        GLuint vertex, fragment;
        GLint success; // Для ошибок
        GLchar infoLog[512]; // Для ошибок

        //Вершинный шейдер
        vertex = glCreateShader(GL_VERTEX_SHADER); // Идентификатор
        glShaderSource(vertex, 1, &vShaderCode, NULL); // Привязка исходного кода
        glCompileShader(vertex); // Компиляция
        //Ошибки
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        //Фрагментный шейдер
        fragment = glCreateShader(GL_FRAGMENT_SHADER); // Идентификатор
        glShaderSource(fragment, 1, &fShaderCode, NULL); // Привязка исходного кода
        glCompileShader(fragment);
        //Ошибки
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Шейдерная программа
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        //Ошибки
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };
    
    // Использование программы
    void Use() { glUseProgram(this->Program); }
};

#endif
