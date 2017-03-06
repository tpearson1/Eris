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

#ifndef _2D__SPRITE_H
#define _2D__SPRITE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <base/gl.h>
#include <base/texture.h>

/*
 * Struct defining regions for sprites in a spritesheet
 */
struct Rect {
  /*
   * The four corners of the region
   */
  unsigned x1, y1, x2, y2;
  Rect() : x1(0), y1(0), x2(0), y2(0) {}
  Rect(unsigned x1, unsigned y1, unsigned x2, unsigned y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

/*
 * Struct for images to be used in 2D games
 */
struct Sprite {
  /*
   * The spritesheet this sprite is part of
   */
  class SpriteSheet *sheet;

  /*
   * The region of it's spritesheet
   */
  Rect region;

  Sprite() : sheet(nullptr) {}
  Sprite(SpriteSheet *sprSheet, Rect reg) : sheet(sprSheet), region(reg) {}

  std::vector<GLfloat> TexCoords() const;
};

/*
 * Class associated with one image, containing one or more sprites
 */
class SpriteSheet {
  /*
   * Prevents copying of the class
   */
  SpriteSheet(const SpriteSheet &other);
  SpriteSheet &operator=(const SpriteSheet &other);

  /*
   * Texture object the spritesheet controls
   */
  Texture texture;

  /*
  * Sprites that are part of the image
  */
  std::unordered_map<std::string, Sprite> sprites;

public:
  SpriteSheet() {}
  explicit SpriteSheet(const Texture &tex) : texture(tex) {}
  bool Load(const std::string &imagePath);

  const Texture &Tex() const { return texture; }

  /*
   * Gets a sprite in the sheet
   * @param name The name of the sprite
   * @returns the Sprite corresponding to the name
   */
  Sprite &operator[](const std::string &name) {
    return sprites.at(name);
  }

  /*
   * Adds a sprite to the sheet
   * @param name The name of the sprite
   * @param region The area of the image the sprite consists of
   */
   void AddSprite(const std::string &name, Rect region) {
     Sprite spr{this, region};
     sprites[name] = spr;
   }
};

#endif // _2D__SPRITE_H
