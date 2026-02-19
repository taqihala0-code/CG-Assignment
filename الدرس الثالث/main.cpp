#include <iostream>
#include <cmath> // للدوال الرياضية

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// متغيرات التحكم
float offsetX1 = 0.0f, offsetY1 = 0.0f;
float offsetX2 = 0.0f, offsetY2 = 0.0f;
float zPos2 = 0.2f;  // عمق المثلث الأزرق
float alpha2 = 0.3f;  // شفافية المثلث الأزرق
int polygonMode = 0;  // 0 = FILL, 1 = LINE, 2 = POINT

// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform float offsetX;\n"
"uniform float offsetY;\n"
"uniform float offsetZ;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offsetX, aPos.y + offsetY, aPos.z + offsetZ, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform float alpha;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, alpha);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// دالة معالجة المدخلات - 4 أزرار فقط
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // ========== الأزرار الأربعة لتغيير شكل المجسمات ==========

    // الزر 1: Z - زيادة عمق الأزرق (يبعد)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        zPos2 += 0.01f;
        std::cout << "عمق الأزرق: " << zPos2 << std::endl;
    }

    // الزر 2: X - تقليل عمق الأزرق (يقرب)
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        zPos2 -= 0.01f;
        std::cout << "عمق الأزرق: " << zPos2 << std::endl;
    }

    // الزر 3: A - زيادة شفافية الأزرق
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        alpha2 += 0.01f;
        if (alpha2 > 1.0f) alpha2 = 1.0f;
        std::cout << "شفافية الأزرق: " << alpha2 << std::endl;
    }

    // الزر 4: S - تقليل شفافية الأزرق
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        alpha2 -= 0.01f;
        if (alpha2 < 0.0f) alpha2 = 0.0f;
        std::cout << "شفافية الأزرق: " << alpha2 << std::endl;
    }

    // الزر P - تبديل نمط الرسم (اختياري)
    static bool p_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!p_pressed) {
            polygonMode = (polygonMode + 1) % 3;
            p_pressed = true;
        }
    }
    else {
        p_pressed = false;
    }

    switch (polygonMode) {
    case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    }
}

int main()
{
    // تهيئة GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "Assignment 3: 4 Buttons Only", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // تهيئة GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // تفعيل الخصائص
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // بناء الشيدرز
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // بيانات المثلثين
    float vertices1[] = {
        -0.9f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
        -0.1f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f
    };

    float vertices2[] = {
         0.2f, -0.5f, 0.2f, 0.2f, 0.5f, 1.0f,
         0.6f,  0.5f, 0.2f, 0.2f, 0.5f, 1.0f,
         1.0f, -0.5f, 0.2f, 0.2f, 0.5f, 1.0f
    };

    // إعداد VAO للشكل الأول
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // إعداد VAO للشكل الثاني
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // حلقة الرسم
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int offsetXLoc = glGetUniformLocation(shaderProgram, "offsetX");
        int offsetYLoc = glGetUniformLocation(shaderProgram, "offsetY");
        int offsetZLoc = glGetUniformLocation(shaderProgram, "offsetZ");
        int alphaLoc = glGetUniformLocation(shaderProgram, "alpha");

        // رسم المثلث الأحمر
        glUniform1f(offsetXLoc, offsetX1);
        glUniform1f(offsetYLoc, offsetY1);
        glUniform1f(offsetZLoc, 0.0f);
        glUniform1f(alphaLoc, 1.0f);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // رسم المثلث الأزرق
        glUniform1f(offsetXLoc, offsetX2);
        glUniform1f(offsetYLoc, offsetY2);
        glUniform1f(offsetZLoc, zPos2);
        glUniform1f(alphaLoc, alpha2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}