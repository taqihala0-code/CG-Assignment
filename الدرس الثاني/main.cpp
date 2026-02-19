#include <iostream>
#include <cmath> // للدوال الرياضية

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// متغيرات للتدوير (للبونص - للمستطيل فقط)
float rotationAngle = 0.0f;
bool rotateX = false, rotateY = false, rotateZ = true; // تدوير Z افتراضيًا

// Vertex Shader (يدعم التدوير)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

// Fragment Shader (يستقبل لون مختلف لكل شكل)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 shapeColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(shapeColor, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// دالة معالجة المدخلات (فقط للتدوير)
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // التحكم في التدوير (للبونص - للمستطيل فقط)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotationAngle += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rotationAngle -= 1.0f;

    // اختيار محور التدوير
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        rotateX = true;
        rotateY = false;
        rotateZ = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        rotateX = false;
        rotateY = true;
        rotateZ = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        rotateX = false;
        rotateY = false;
        rotateZ = true;
    }
}

int main()
{
    // --- تهيئة GLFW ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "Assignment: Triangle + Rectangle (Rectangle Rotates Only)", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- تهيئة GLEW ---
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // --- بناء الشيدرز ---
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

    // --- 📌 بيانات المثلث (المهمة 1) - على اليسار - ثابت ---
    float triangleVertices[] = {
        -0.9f, -0.3f, 0.0f, // يسار سفلي
        -0.5f,  0.5f, 0.0f, // رأس
        -0.1f, -0.3f, 0.0f  // يمين سفلي
    };

    // --- 📌 بيانات المستطيل (المهمة 2) - على اليمين - اللي رح يدور ---
    // بأبعاد فريدة: عرض 0.4, ارتفاع 0.8
    float rectangleVertices[] = {
        // مثلث سفلي
         0.3f, -0.4f, 0.0f,
         0.7f, -0.4f, 0.0f,
         0.3f,  0.4f, 0.0f,
         // مثلث علوي
          0.7f, -0.4f, 0.0f,
          0.7f,  0.4f, 0.0f,
          0.3f,  0.4f, 0.0f
    };

    // --- إعداد VBO و VAO (للمثلث) ---
    unsigned int VBO_triangle, VAO_triangle;
    glGenVertexArrays(1, &VAO_triangle);
    glGenBuffers(1, &VBO_triangle);

    glBindVertexArray(VAO_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --- إعداد VBO و VAO (للمستطيل) ---
    unsigned int VBO_rectangle, VAO_rectangle;
    glGenVertexArrays(1, &VAO_rectangle);
    glGenBuffers(1, &VBO_rectangle);

    glBindVertexArray(VAO_rectangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rectangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // فك الربط
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ألوان مختلفة وفريدة
    float triangleColor[3] = { 1.0f, 0.5f, 0.2f }; // برتقالي للمثلث (ثابت)
    float rectangleColor[3] = { 0.3f, 0.7f, 0.9f }; // أزرق سماوي للمستطيل (اللي رح يدور)

    // --- حلقة الرسم ---
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.15f, 0.2f, 1.0f); // خلفية داكنة
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // --- ✨ رسم المثلث أولاً (بدون تدوير) ---
        // مصفوفة الوحدة (Identity Matrix) - يعني بدون أي تدوير
        float identityMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        // رسم المثلث بدون تدوير
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, identityMatrix);
        glUniform3f(glGetUniformLocation(shaderProgram, "shapeColor"),
            triangleColor[0], triangleColor[1], triangleColor[2]);
        glBindVertexArray(VAO_triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // --- 👈 إنشاء مصفوفة التدوير (للمستطيل فقط) ---
        float radians = rotationAngle * 3.14159f / 180.0f;
        float rotationMatrix[16];

        // اختيار محور التدوير للمستطيل
        if (rotateX) {
            // تدوير حول X
            rotationMatrix[0] = 1.0f; rotationMatrix[1] = 0.0f; rotationMatrix[2] = 0.0f; rotationMatrix[3] = 0.0f;
            rotationMatrix[4] = 0.0f; rotationMatrix[5] = cos(radians); rotationMatrix[6] = -sin(radians); rotationMatrix[7] = 0.0f;
            rotationMatrix[8] = 0.0f; rotationMatrix[9] = sin(radians); rotationMatrix[10] = cos(radians); rotationMatrix[11] = 0.0f;
            rotationMatrix[12] = 0.0f; rotationMatrix[13] = 0.0f; rotationMatrix[14] = 0.0f; rotationMatrix[15] = 1.0f;
        }
        else if (rotateY) {
            // تدوير حول Y
            rotationMatrix[0] = cos(radians); rotationMatrix[1] = 0.0f; rotationMatrix[2] = sin(radians); rotationMatrix[3] = 0.0f;
            rotationMatrix[4] = 0.0f; rotationMatrix[5] = 1.0f; rotationMatrix[6] = 0.0f; rotationMatrix[7] = 0.0f;
            rotationMatrix[8] = -sin(radians); rotationMatrix[9] = 0.0f; rotationMatrix[10] = cos(radians); rotationMatrix[11] = 0.0f;
            rotationMatrix[12] = 0.0f; rotationMatrix[13] = 0.0f; rotationMatrix[14] = 0.0f; rotationMatrix[15] = 1.0f;
        }
        else {
            // تدوير حول Z (افتراضي)
            rotationMatrix[0] = cos(radians); rotationMatrix[1] = -sin(radians); rotationMatrix[2] = 0.0f; rotationMatrix[3] = 0.0f;
            rotationMatrix[4] = sin(radians); rotationMatrix[5] = cos(radians); rotationMatrix[6] = 0.0f; rotationMatrix[7] = 0.0f;
            rotationMatrix[8] = 0.0f; rotationMatrix[9] = 0.0f; rotationMatrix[10] = 1.0f; rotationMatrix[11] = 0.0f;
            rotationMatrix[12] = 0.0f; rotationMatrix[13] = 0.0f; rotationMatrix[14] = 0.0f; rotationMatrix[15] = 1.0f;
        }

        // --- ✨ رسم المستطيل مع التدوير ---
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rotationMatrix);
        glUniform3f(glGetUniformLocation(shaderProgram, "shapeColor"),
            rectangleColor[0], rectangleColor[1], rectangleColor[2]);
        glBindVertexArray(VAO_rectangle);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // تنظيف
    glDeleteVertexArrays(1, &VAO_triangle);
    glDeleteVertexArrays(1, &VAO_rectangle);
    glDeleteBuffers(1, &VBO_triangle);
    glDeleteBuffers(1, &VBO_rectangle);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}