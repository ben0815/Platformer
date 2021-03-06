#include "ForwardRenderer2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;


ForwardRenderer2D::
ForwardRenderer2D(const glm::ivec2& _bufferSize) {
  m_textureShader = nullptr;
  m_textShader = nullptr;
  m_vertexBuffer = nullptr;
  SetScreenSize(_bufferSize);
}


void
ForwardRenderer2D::
Init() {
  cout << "Initializing Renderer" << endl;

  // create a new matrix stack
  m_matrixStack = new MatrixStack;

  // pushing an identity matrix onto stack
  m_matrixStack->pushMatrix();

  // getting the sprite shader from resouces
  m_textureShader = Resources::Get()->GetShader("forward");

  // binding the sprite shader
  m_textureShader->Bind();
  
  // Setting attributes and uniforms
  m_textureShader->AddAttribute("vertex");

  m_textureShader->AddUniform("projection");
  m_textureShader->AddUniform("modelview");
  m_textureShader->AddUniform("textures");
  m_textureShader->AddUniform("color");


  // setting constant uniforms
  auto perspective = glm::ortho(0.0f, (float) m_bufferSize.x, (float) m_bufferSize.y, 0.0f, -1.0f, 1.0f);
  m_textureShader->SetMatrix4("projection", perspective);
  m_textureShader->SetVector3f("color", 1.0f, 1.0f, 1.0f);

  m_textureShader->Unbind();

  // getting the text shader, The sprite shader might be used to render this.
  m_textShader = Resources::Get()->GetShader("text");

  GLfloat vertices[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0
  };

  m_vertexArray = new VertexArray;
  m_vertexArray->Bind();

  m_vertexBuffer = new VertexBuffer(sizeof(vertices), vertices, StaticDraw);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  m_vertexArray->Unbind();
  glCheckError();

  cout << "Finished Renderer" << endl;
}


void
ForwardRenderer2D::
Submit(Renderable2D* _renderable) {
  auto texture = _renderable->Texture();

  m_textureShader->Bind();
  SubmitTexture(texture);
  BindTexture("textures", m_texture);

  m_matrixStack->pushMatrix();
  m_matrixStack->translate(_renderable->GetLocation());
  m_matrixStack->rotate(_renderable->GetRotation(), glm::vec3(0,0,1));
  m_matrixStack->scale(_renderable->GetSize());

  m_textureShader->SetMatrix4("modelview", m_matrixStack->topMatrix());
  m_textureShader->Unbind();

  m_matrixStack->popMatrix();

  Present();
}


void
ForwardRenderer2D::
Begin() {
}


void
ForwardRenderer2D::
Present() {
  // the texture is bound in Submit
  m_textureShader->Bind();
  m_vertexArray->Bind();

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  glCheckError();

  m_vertexArray->Unbind();
  m_texture->Unbind();
  m_textureShader->Unbind();
}


void
ForwardRenderer2D::
End() {
}


void
ForwardRenderer2D::
DrawString(const std::string& _text, float _x, float _y,
    const Font& _font,
    unsigned int _color) {

}


void
ForwardRenderer2D::
DrawLine(float _x0, float _y0, float _x1, float _y1,
    unsigned int _color, float _thickness) {

}


void
ForwardRenderer2D::
DrawLine(const glm::vec2& _start, const glm::vec2& _end,
    unsigned int _color, float _thickness) {

}


void
ForwardRenderer2D::
DrawRect(float _x, float _y, float _width, float _height,
    unsigned int _color, float _thickness) {

}


void
ForwardRenderer2D::
DrawRect(const glm::vec2& _location, const glm::vec2& _size,
    unsigned int _color, float _thickness) {

}


void
ForwardRenderer2D::
DrawRect(const Rect& _rect, unsigned int _color,
    float _thickness) {

}


void
ForwardRenderer2D::
DrawRect(const Rectangle& _rect, unsigned int _color,
    float _thickness) {

}


void
ForwardRenderer2D::
FillRect(float _x, float _y, float _width, float _height,
    unsigned int _color) {

}


void
ForwardRenderer2D::
FillRect(const glm::vec2& _location, const glm::vec2& _size,
    unsigned int _color) {

}


void
ForwardRenderer2D::
FillRect(const Rect& _rect, unsigned int _color) {

}


void
ForwardRenderer2D::
FillRect(const Rectangle& _rect, unsigned int _color) {

}

void
ForwardRenderer2D::
SetScreenSize(const glm::ivec2& _bufferSize) {
  m_bufferSize = _bufferSize;
}

void
ForwardRenderer2D::
SetCamera(Camera* _camera) {
  m_camera = _camera;
}


void
ForwardRenderer2D::
SubmitTexture(Texture2D* _texture) {
  m_texture = _texture;
}
void
ForwardRenderer2D::
BindTexture(const std::string& _att, Texture2D* _tex) {
  _tex->SetUnit(0);
  _tex->Bind();
  m_textureShader->SetInteger("textures", _tex->GetUnit());
}