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
#include "BuildInfo.h"

namespace msu1
{
    BuildInfo::BuildInfo()
    {
    }

    int BuildInfo::Normalization() const
    {
        if (_track.Normalization.HasValue())
        {
            return _track.Normalization.Value();
        }

        return _globalSettings.Normalization;
    }

    bool BuildInfo::UseDithering() const
    {
        if (_track.UseDithering.HasValue())
        {
            return _track.UseDithering.Value();
        }

        return _globalSettings.UseDithering;
    }

    int BuildInfo::Loop() const
    {
        if (_track.Loop.HasValue())
        {
            return _track.Loop.Value();
        }

        return 0;
    }

    bool BuildInfo::LoopIsRelativeToSource() const
    {
        if (_track.LoopIsRelativeToSource.HasValue())
        {
            return _track.LoopIsRelativeToSource.Value();
        }

        return _globalSettings.LoopIsRelativeToSource;
    }
}
