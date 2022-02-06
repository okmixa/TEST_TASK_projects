#pragma once

#include <QtWidgets/QApplication>

#include <QOpenGLWindow>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QtGui/QMatrix4x4>

namespace udpClient {

class PictureRender : public QOpenGLWindow
{
    Q_OBJECT
public:
    PictureRender(QImage& image);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vertPosBuffer;
    QOpenGLBuffer m_texCoordBuffer;
    QOpenGLTexture m_texture;
    QMatrix4x4 m_mvpMatrix;
    QMatrix4x4 m_projMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    int m_uMvpMatrixLocation;
    const float WORLD_HEIGHT = 50;
    QImage texture;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void initVertexBuffer(QOpenGLBuffer &buffer, float data[], int amount,
                          const char *locationName, int locationIndex, int tupleSize);
};

} // namespace udpClient

#include "opengl_widget.moc"
