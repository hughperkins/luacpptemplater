// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>
#include <vector>

struct lua_State;

class LuaCppTemplater {
private:
  lua_State *L;

public:
  LuaCppTemplater();
  ~LuaCppTemplater();
  void set(std::string const &name, std::string const &value);
  void set(std::string const &name, float value);
  void set(std::string const &name, int value);
  void set(std::string const &name, std::vector<std::string> &values);
  void set(std::string const &name, std::vector<int> &values);
  void set(std::string const &name, std::vector<float> &values);
  std::string render(std::string const &template_string);
};

