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
#include <scene/meshrenderer.h>

Ref<Mesh> LoadMesh(const std::string &path);

NMeshRenderer *LoadMeshRenderer(const std::string &path, const RenderRequirements &rr);

NNode *LoadModel(const std::string &path, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc = nullptr);

/*
 * Function that can be registered for the ability to load models as shown below:
 *
 * [
 *   "Model",
 *   {
 *     "path": "mods/mod/res/model.blend",
 *     "prerender-func": "Default", // Optional
 *     "requirements": { ... },
 *     "NNode": { ... }
 *   }
 * ]
 */
NNode *ModelRegistration(const rapidjson::Value &val, JSON::TypeManager &manager);

/*
 * Function that can be registered for the ability to load single meshes as shown below:
 * [
 *   "NMeshRenderer",
 *   {
 *     "path": "mods/mod/res/mesh.blend",
 *     "prerender-func": "Default", // Optional
 *     "requirements": { ... },
 *     "NNode": { ... }
 *   }
 * ]
 */
NMeshRenderer *MeshRendererRegistration(const rapidjson::Value &val, JSON::TypeManager &manager);

#endif // _BASE__MODEL_H
