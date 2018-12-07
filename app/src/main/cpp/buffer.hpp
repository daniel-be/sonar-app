//
// Created by Daniel on 06.12.2018.
//

#ifndef SONAR_APP_BUFFER_HPP
#define SONAR_APP_BUFFER_HPP

template <typename T>
class Buffer
{
public:
    Buffer(T* data, unsigned int gl_target, unsigned long int size, unsigned int usage);
    void bind() const;
    void unbind() const;
    ~Buffer();

private:
    unsigned int gl_id;
    unsigned int gl_target;
};

#endif //SONAR_APP_BUFFER_HPP
