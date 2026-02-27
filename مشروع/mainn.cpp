#include <iostream>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\n\0";

// Fragment Shader مع Uniform Alpha 
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"uniform float uAlpha;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, uAlpha);\n"
"}\n\0";

void processInput(GLFWwindow* window, int* expression, float* alpha, int* prev1, int* prev2, int* prevSpace) {
    int key1 = glfwGetKey(window, GLFW_KEY_1);
    int key2 = glfwGetKey(window, GLFW_KEY_2);
    int keySpace = glfwGetKey(window, GLFW_KEY_SPACE);

    // تغيير التعبير 
    if (key1 == GLFW_PRESS && *prev1 == GLFW_RELEASE) {
        *expression = 1;
        std::cout << " Happy" << std::endl;
    }
    if (key2 == GLFW_PRESS && *prev2 == GLFW_RELEASE) {
        *expression = 2;
        std::cout << " Sad" << std::endl;
    }

    // تبديل الشفافية (مشتقة من مثال )
    if (keySpace == GLFW_PRESS && *prevSpace == GLFW_RELEASE) {
        if (*alpha > 0.9f) *alpha = 0.4f;
        else *alpha = 1.0f;
        std::cout << "Alpha: " << *alpha << std::endl;
    }

    *prev1 = key1;
    *prev2 = key2;
    *prevSpace = keySpace;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // تهيئة )
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Robot", NULL, NULL);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // (تفعيل الشفافية )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //بناء الشيدرز  
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
    glUseProgram(shaderProgram);

    // ========== بيانات الرأس (6 أرقام فقط) ==========
    float headVertices[] = {
        -0.8f, -0.6f, 0.0f,   0.5f, 0.5f, 0.5f,
         0.8f, -0.6f, 0.0f,   0.5f, 0.5f, 0.5f,
         0.8f,  0.7f, 0.0f,   0.5f, 0.5f, 0.5f,
        -0.8f,  0.7f, 0.0f,   0.5f, 0.5f, 0.5f
    };
    unsigned int headIndices[] = { 0,1,2, 2,3,0 };

    // ========== بيانات العيون (6 أرقام فقط) ==========
    float eyesVertices[] = {
        -0.5f, 0.3f, 0.1f,   1.0f, 1.0f, 1.0f,
        -0.2f, 0.3f, 0.1f,   1.0f, 1.0f, 1.0f,
        -0.2f, 0.0f, 0.1f,   1.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.1f,   1.0f, 1.0f, 1.0f,
         0.2f, 0.3f, 0.1f,   1.0f, 1.0f, 1.0f,
         0.5f, 0.3f, 0.1f,   1.0f, 1.0f, 1.0f,
         0.5f, 0.0f, 0.1f,   1.0f, 1.0f, 1.0f,
         0.2f, 0.0f, 0.1f,   1.0f, 1.0f, 1.0f,
        -0.4f, 0.2f, 0.2f,   0.0f, 0.0f, 0.0f,
        -0.3f, 0.2f, 0.2f,   0.0f, 0.0f, 0.0f,
        -0.3f, 0.1f, 0.2f,   0.0f, 0.0f, 0.0f,
        -0.4f, 0.1f, 0.2f,   0.0f, 0.0f, 0.0f,
         0.3f, 0.2f, 0.2f,   0.0f, 0.0f, 0.0f,
         0.4f, 0.2f, 0.2f,   0.0f, 0.0f, 0.0f,
         0.4f, 0.1f, 0.2f,   0.0f, 0.0f, 0.0f,
         0.3f, 0.1f, 0.2f,   0.0f, 0.0f, 0.0f
    };

    // ========== الفم السعيد ==========
    float happyMouth[] = {
        -0.4f, -0.2f, 0.1f,   0.0f, 1.0f, 0.0f,
        -0.2f, -0.3f, 0.1f,   0.0f, 1.0f, 0.0f,
         0.0f, -0.35f, 0.1f,   0.0f, 1.0f, 0.0f,
         0.2f, -0.3f, 0.1f,   0.0f, 1.0f, 0.0f,
         0.4f, -0.2f, 0.1f,   0.0f, 1.0f, 0.0f
    };

    // ========== الفم الحزين ==========
    float sadMouth[] = {
        -0.4f, -0.35f, 0.1f,   0.0f, 0.0f, 1.0f,
        -0.2f, -0.25f, 0.1f,   0.0f, 0.0f, 1.0f,
         0.0f, -0.2f, 0.1f,   0.0f, 0.0f, 1.0f,
         0.2f, -0.25f, 0.1f,   0.0f, 0.0f, 1.0f,
         0.4f, -0.35f, 0.1f,   0.0f, 0.0f, 1.0f
    };

    // ========== إنشاء VAOs  ==========
    unsigned int headVAO, headVBO, headEBO;
    glGenVertexArrays(1, &headVAO);
    glGenBuffers(1, &headVBO);
    glGenBuffers(1, &headEBO);

    glBindVertexArray(headVAO);
    glBindBuffer(GL_ARRAY_BUFFER, headVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headVertices), headVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, headEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(headIndices), headIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int eyesVAO, eyesVBO;
    glGenVertexArrays(1, &eyesVAO);
    glGenBuffers(1, &eyesVBO);
    glBindVertexArray(eyesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, eyesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(eyesVertices), eyesVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int happyVAO, happyVBO;
    glGenVertexArrays(1, &happyVAO);
    glGenBuffers(1, &happyVBO);
    glBindVertexArray(happyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, happyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(happyMouth), happyMouth, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int sadVAO, sadVBO;
    glGenVertexArrays(1, &sadVAO);
    glGenBuffers(1, &sadVBO);
    glBindVertexArray(sadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sadMouth), sadMouth, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ========== متغيرات التحكم ==========
    int expression = 1;
    float alpha = 1.0f;
    int prev1 = GLFW_RELEASE, prev2 = GLFW_RELEASE, prevSpace = GLFW_RELEASE;

    // موقع الـ Uniform 
    int alphaLoc = glGetUniformLocation(shaderProgram, "uAlpha");

    while (!glfwWindowShouldClose(window)) {
        processInput(window, &expression, &alpha, &prev1, &prev2, &prevSpace);

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(alphaLoc, alpha);  // ضبط الشفافية  

        // رسم الرأس
        glBindVertexArray(headVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // رسم العيون
        glBindVertexArray(eyesVAO);
        for (int i = 0; i < 16; i += 4)
            glDrawArrays(GL_TRIANGLE_FAN, i, 4);

        // رسم الفم
        glLineWidth(5.0f);
        if (expression == 1) {
            glBindVertexArray(happyVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 5);
        }
        else {
            glBindVertexArray(sadVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 5);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // التنظيف
    glDeleteVertexArrays(1, &headVAO);
    glDeleteVertexArrays(1, &eyesVAO);
    glDeleteVertexArrays(1, &happyVAO);
    glDeleteVertexArrays(1, &sadVAO);
    glDeleteBuffers(1, &headVBO);
    glDeleteBuffers(1, &headEBO);
    glDeleteBuffers(1, &eyesVBO);
    glDeleteBuffers(1, &happyVBO);
    glDeleteBuffers(1, &sadVBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}