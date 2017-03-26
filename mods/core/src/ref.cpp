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

#include <ref.h>
#include <testing.h>

struct RefDummyBase {
  int a;
  RefDummyBase(int _a) : a(_a) {}

  virtual int Compute() const { return a; }
};

struct RefDummyDerived : public RefDummyBase {
  int b;

  RefDummyDerived(int _a, int _b) : RefDummyBase(_a), b(_b) {}
  virtual int Compute() const override { return a + b; }
};


void RunRefTests(){
  std::cout << "RunRefTests\n";
  auto derived = Ref<RefDummyDerived>::Create(5, 6);
  ASSERT(derived->Compute() == 11)

  auto base = (Ref<RefDummyBase>)derived;
  ASSERT(base->Compute() == 11)

  base.Reset(4);
  ASSERT(base->Compute() == 4)

  auto s1 = Ref<RefDummyBase>::Create(3);
  ASSERT(s1->Compute() == 3)
  auto s2 = Ref<RefDummyBase>::Create(5);
  ASSERT(s2->Compute() == 5)
  s1.Swap(s2);

  ASSERT(s1->Compute() == 5)
  ASSERT(s2->Compute() == 3)
}