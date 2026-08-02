#pragma once
#include <memory>
#include <vector>
class IStackScene { public: virtual ~IStackScene()=default; virtual bool updatesBelow()const{return false;} virtual bool drawsBelow()const{return false;} virtual void update(float)=0; virtual void draw()const=0; };
class SceneStack { public: void push(std::unique_ptr<IStackScene> scene); void pop(); void replace(std::unique_ptr<IStackScene> scene); void clearAndPush(std::unique_ptr<IStackScene> scene); void update(float dt); void draw()const; int size()const; private: std::vector<std::unique_ptr<IStackScene>> m_scenes; };
