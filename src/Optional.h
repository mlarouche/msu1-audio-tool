// MSU-1 Audio Tool
// Copyright (c) 2016 Michael Larouche <michael.larouche@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#pragma once

namespace Baroque
{
    template<typename T>
    class Optional
    {
    public:
        Optional()
        : _hasValue(false)
        {
        }

        explicit Optional(T value)
        {
            Assign(value);
        }

        Optional(const Optional &copy)
        : _hasValue(copy._hasValue)
        , _value(copy._value)
        {
        }

        Optional& operator=(const Optional& other)
        {
            if (this != &other)
            {
                _hasValue = other._hasValue;
                _value = other._value;
            }

            return *this;
        }

        void operator=(T value)
        {
            Assign(value);
        }

        void Clear()
        {
            _hasValue = false;
            _value = T();
        }

        bool HasValue() const { return _hasValue; }

        T Value() const { return _value; }

    private:
        void Assign(T value)
        {
            _value = value;
            _hasValue = true;
        }

    private:
        bool _hasValue;
        T _value;
    };
}
