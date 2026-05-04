#pragma once

#include <lw/scene/Object3D.h>

#include <vector>
#include <memory>

namespace lw
{

class Scene
{
public:
  template<typename T>
  T* addObject(T object)
  {
    // make_unique создаёт unique_ptr который сам проследит за очисткой памяти
    // std::move() - позволяет не копировать данные а сразу перенсти весь объект в нужное хранилище
    // например object_ptr может иметь только 1 хозяина, поэтому мы можем его только перенести
    // object_ptr.get(); - позволяет получить обычный сырой указатель. Через этот указатель мы не можем удалить наш основной
    auto object_ptr = std::make_unique<T>(std::move(object));
    T* raw = object_ptr.get();
    objects.push_back(std::move(object_ptr));
    return raw;
  }

  std::vector<std::shared_ptr<Object3D>> getObjects() const { return objects; }


private:
  std::vector<std::shared_ptr<Object3D>> objects;
};


} // namespace lw
