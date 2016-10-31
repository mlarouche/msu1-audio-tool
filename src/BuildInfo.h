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

#include "GlobalSettings.h"
#include "TrackSettings.h"

namespace msu1
{
    class BuildInfo
    {
    public:
        BuildInfo();

        int Normalization() const;
        bool UseDithering() const;
        std::string const& Filename() const { return _track.Filename; }
        std::string const& OutputPrefix() const { return _globalSettings.OutputPrefix; }
        int TrackIndex() const { return _track.TrackIndex; }
        int Loop() const;
        bool LoopIsRelativeToSource() const;

        Baroque::Optional<int> TrimStart() const { return _track.TrimStart; }
        Baroque::Optional<int> TrimEnd() const { return _track.TrimEnd; }

        void SetGlobalSettings(GlobalSettings value) { _globalSettings = value; }
        void SetTrackSettings(TrackSettings value) { _track = value; }

        bool KeepWav() const { return _keepWav; }
        void SetKeepWav(bool value) { _keepWav = value; }

    private:
        GlobalSettings _globalSettings;
        TrackSettings _track;
        bool _keepWav = false;
    };
}