#include "vertexArray.h"
#include "vertexBufferLayout.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    //Bind vertex array
    bind();
    //Bind buffer
    vb.bind();

    //Set up layout
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), reinterpret_cast<const void *>((const intptr_t) offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const { 
    glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}