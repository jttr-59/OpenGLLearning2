//simply makes sure that there isnt duplicate copies of this header in one file (causes errors)
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

//stuff for file and string management
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
    //program ID
    unsigned int ID;

    //constructor
    Shader(const char* vertexPath, const char* fragmentPath)
    {

        //getting the files, and converting them to a string path
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();
 
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(const std::ifstream::failure& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cerr << "Reason: " << e.what() << "\n";
            std::cerr << "Vertex path: " << vertexPath << "\n";
            std::cerr << "Fragment path: " << fragmentPath << "\n";
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        //-- taking the entered shader files and compiling them to our project --//
        //vertex compilation
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        //error checking
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //fragment compilation
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        //error checking
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //program compilation and shader attachment
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        //error checking
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        //shader deletion as they are linked to the program
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        std::cout << "Shader compiled and linked successfully. Program ID: " << ID << std::endl;
    }
    //put this shader (vertex fragment pairing) into use
    void use() 
    {
        glUseProgram(ID);
    }
    // uniform setting functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};


#endif