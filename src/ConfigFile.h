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

// STL includes
#include <string>
#include <vector>

// rapidjson includes
#include <rapidjson/document.h>

// Local includes
#include "GlobalSettings.h"
#include "Optional.h"
#include "TrackSettings.h"

namespace msu1
{
    class ConfigFile
    {
    public:
        ConfigFile();
        ConfigFile(const std::string& path);

        void Open(const std::string& path);

        bool IsValid() const { return _isValid; }

        explicit operator bool() { return IsValid(); }

        msu1::GlobalSettings GlobalSettings() const { return _globalSettings; }

        std::vector<TrackSettings> const& Tracks() const { return _tracks; }

    private:
        void parseGlobalSettings(const rapidjson::GenericValue<rapidjson::UTF8<>>& json);
        void parseTracks(const rapidjson::GenericValue<rapidjson::UTF8<>>& json);

        void parseString(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, std::string& value);
        void parseInt(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, int& value);
        void parseBool(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, bool& value);
        void parseOptionalInt(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, Baroque::Optional<int>& value);
        void parseOptionalBool(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, Baroque::Optional<bool>& value);

    private:
        bool _isValid;
        msu1::GlobalSettings _globalSettings;
        std::vector<TrackSettings> _tracks;
    };
}