#ifndef PLUGINS1_H
#define PLUGINS1_H

#include <iostream>

#include "plugin_loader/plugin_loader.hpp"

#include "base.hpp"

class Dog : public Base
{
public:
  virtual void saySomething() {std::cout << "Bark" << std::endl;}
};

class Cat : public Base
{
public:
  virtual void saySomething() {std::cout << "Meow" << std::endl;}
};

class Duck : public Base
{
public:
  virtual void saySomething() {std::cout << "Quack" << std::endl;}
};

class Cow : public Base
{
public:
  virtual void saySomething() {std::cout << "Moooo" << std::endl;}
};

class Sheep : public Base
{
public:
  virtual void saySomething() {std::cout << "Baaah" << std::endl;}
};

#endif // PLUGINS1_H
