#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent):QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();//设置为当前上下文
    delete recShader;
    core->glDeleteVertexArrays(1, VAO);
    core->glDeleteBuffers(1, VBO);
    texture1->destroy();
    texture2->destroy();
}

void OpenGLWidget::initializeGL()
{
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    recShader = new Shader("F:/hust/underwater/Re/Manipulator/resource/shader/recVertexShader.vert", "F:/hust/underwater/Re/Manipulator/resource/shader/recFragmentShader.frag");

    //    VAO，VBO数据部分  正方形
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    core->glGenVertexArrays(2,VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
    core->glGenBuffers(2,VBO);

    //---------------绑定第一个图形数据
    core->glBindVertexArray(VAO[0]);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    core->glEnableVertexAttribArray(0);
    // texture coord attribute
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    core->glEnableVertexAttribArray(1);

    //---------------绑定第二个图形数据
    //    core->glBindVertexArray(VAO[1]);
    //    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //    core->glBufferData(GL_ARRAY_BUFFER, sizeof(spheres), spheres, GL_STATIC_DRAW);
    //    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //    core->glEnableVertexAttribArray(0);

    //---------------绑定图片作为纹理，根据shader内容进行更改
    texture1 = new QOpenGLTexture(QImage("F:/hust/underwater/Re/Manipulator/resource/texture/al.jpg").mirrored(), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture1->isCreated()){
        qDebug() << "Failed to load texture" << endl;
    }
    texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture1->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    //---------------绑定图片作为纹理，根据shader内容进行更改
    texture2 = new QOpenGLTexture(QImage("F:/hust/underwater/Re/Manipulator/resource/texture/lab.png").mirrored());
    if(!texture2->isCreated()){
        qDebug() << "Failed to load texture" << endl;
    }
    texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture2->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture2->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //---------------设定着色器
    recShader->use();
    recShader->setInt("texture1", 0);
    recShader->setInt("texture2", 1);

    //开启计时器，返回毫秒
    time.start();

    //给着色器变量赋值
    QMatrix4x4 projection;
    projection.perspective(45.0f, static_cast<GLfloat>(width())/static_cast<GLfloat>(height()), 0.1f, 100.0f);

    recShader->use();
    recShader->setMat4("projection", projection);
    //开启状态
    core->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    core->glEnable(GL_DEPTH_TEST);

    //与视频相关的摄像机变量初始化
    cameraPos = QVector3D(0.0f, 0.0f,  7.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f,  0.0f);
    deltaTime = 0.0f;
    lastFrame = 0.0f;

    //与鼠标相关的摄像机变量初始化
    firstMouse = true;
    yaw   = -90.0f;	// 偏航角如果是0.0f,指向的是 x轴正方向，即右方向，所以向里转90度，初始方向指向z轴负方向
    pitch =  0.0f;
    lastX =  800.0f / 2.0;
    lastY =  600.0 / 2.0;
    fov   =  45.0f;

}

void OpenGLWidget::resizeGL(int w, int h)
{
    core->glViewport(0,0,w,h);
}

void OpenGLWidget::paintGL()
{
    GLfloat currentFrame = static_cast<GLfloat>(time.elapsed())/100;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    core->glActiveTexture(GL_TEXTURE0);
    texture1->bind();
    core->glActiveTexture(GL_TEXTURE1);
    texture2->bind();

    recShader->use();

    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    recShader->setMat4("view", view);//设置着色器值


    //----------底座--
    QMatrix4x4 model1;
    model1.translate(QVector3D( 0.0f,  0.0f,  0.0f));
    model1.scale(QVector3D( 0.2f,  0.2f,  0.2f));
    //    model1.rotate(1.0f * absAngle[0], QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model1);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------底座连杆--
    QMatrix4x4 model2;
    model2.translate(QVector3D( 0.0f,  0.2f,  0.0f));
    model2.scale(QVector3D( 0.1f,  0.4f,  0.1f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model2);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------底电机--
    QMatrix4x4 model3;
    model3.translate(QVector3D( 0.0f,  0.4f,  0.0f));
    model3.scale(QVector3D( 0.2f,  0.2f,  0.2f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model3);
    core->glBindVertexArray(VAO[0]);//绑定方形对象
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------底电机连杆--
    QMatrix4x4 model4;
    model4.translate(QVector3D( 0.3f,  0.4f,  0.0f));
    model4.scale(QVector3D( 0.6f,  0.1f,  0.1f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model4);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------肩电机--
    QMatrix4x4 model5;
    model5.translate(QVector3D( 0.6f,  0.4f,  0.0f));
    model5.scale(QVector3D( 0.2f,  0.2f,  0.2f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model5);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------肩电机连杆--
    QMatrix4x4 model6;
    model6.translate(QVector3D( 0.6f,  1.06f,  0.0f));
    model6.scale(QVector3D(  0.1f,  1.32f,  0.1f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model6);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //------------臂电机--
    QMatrix4x4 model7;
    model7.translate(QVector3D( 0.6f,  1.72f,  0.0f));
    model7.scale(QVector3D( 0.2f,  0.2f,  0.2f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model7);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------臂电机连杆--
    QMatrix4x4 model8;
    model8.translate(QVector3D( 0.28f,  1.72f,  0.0f));
    model8.scale(QVector3D( 0.64f,  0.1f,  0.1f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model8);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);

    //----------爪连杆--
    QMatrix4x4 model9;
    model9.translate(QVector3D( 0.01f,  2.3f,  0.0f));
    model9.scale(QVector3D( 0.1f,  1.06f,  0.1f));
    //    model2.rotate(20.0f,QVector3D( 0.0f,  1.0f,  0.0f));//这里角度改为固定角度
    recShader->setMat4("model", model9);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_TRIANGLES, 0, 36);
    update();
}


void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    GLfloat cameraSpeed = 1 * deltaTime;
    if(event->key() == Qt::Key_W){
        cameraPos += cameraFront * cameraSpeed;
    }
    if(event->key() == Qt::Key_S){
        cameraPos -= cameraFront * cameraSpeed;
    }
    if(event->key() == Qt::Key_A){
        cameraPos -= (QVector3D::crossProduct(cameraFront, cameraUp).normalized()) * cameraSpeed;
    }
    if(event->key() == Qt::Key_D){
        cameraPos += (QVector3D::crossProduct(cameraFront, cameraUp).normalized()) * cameraSpeed;
    }
    if(event->key() == Qt::Key_E){ //上升
        cameraPos += cameraUp * cameraSpeed;
    }
    if(event->key() == Qt::Key_Q){ //下降
        cameraPos -= cameraUp * cameraSpeed;
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    if(fov >= 1.0f && fov <=45.0f)
        fov -= (static_cast<GLfloat>(offset.y())/20);
    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > 45.0f)
        fov = 45.0f;
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat xpos = event->pos().x();
    GLfloat ypos = event->pos().y();

    if (firstMouse)
    {
        lastX = event->pos().x();
        lastY = event->pos().y();
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; //
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.01f; //
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    //防止头顶时出现视角晃动
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    QVector3D front(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    cameraFront = front.normalized();
}