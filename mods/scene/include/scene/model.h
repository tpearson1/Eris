/*
-------------------------------------------------------------------------------
This file is part of Eris Engine
-------------------------------------------------------------------------------
Copyright (c) 2017 Thomas Pearson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
-------------------------------------------------------------------------------
*/

#ifndef _BASE__MODEL_H
#define _BASE__MODEL_H

#include <vector>
#include <assimp/scene.h>
#include <scene/scene.h>

NNode *LoadModel(const std::string &path, const RenderRequirements &rr);

/*
 * Function that can be registered for the ability to load models as shown below:
 *
 * [
 *   "Model",
 *   {
 *     "path": "mods/mod/res/model.blend",
 *     "texture": "mods/mod/res/tex/texture.png",
 *     "shader": {
 *       "vertex": "mods/base/res/shaders/default-unlit.vs",
 *       "fragment": "mods/base/res/shaders/default-unlit.frag"
 *     },
 *     "NNode": { ... }
 *   }
 * ]
 */
NNode *ModelRegistration(const rapidjson::Value &val, JSONTypeManager &manager);

#endif // _BASE__MODEL_H
