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
#include "ConfigFile.h"

// STL includes
#include <cstdio>

// rapidjson includes
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

// Local includes
#include "CFileWrapper.h"

namespace msu1
{
    ConfigFile::ConfigFile()
    : _isValid(false)
    {
    }

    ConfigFile::ConfigFile(const std::string& path)
    : _isValid(false)
    {
        Open(path);
    }

    void ConfigFile::Open(const std::string& path)
    {
        _globalSettings = msu1::GlobalSettings();
        _tracks.clear();

        _isValid = true;

        auto inputFile = Baroque::CFileWrapper(path.c_str(), "r");
        if (!inputFile)
        {
            // TODO: Log error with real logger class
            fprintf(stderr, "Cannot open input config file %s\n", path.c_str());
            _isValid = false;
            return;
        }

        char readBuffer[64 * 1024];
        rapidjson::FileReadStream readStream(inputFile, readBuffer, sizeof(readBuffer));

        rapidjson::Document json;
        json.ParseStream(readStream);

        if (json.HasParseError())
        {
            rapidjson::ParseResult parseError = json;

            // TODO: Log error with real logger class
            fprintf(stderr, "Parsing error in the config file at offset %u: %s\n", parseError.Offset(), rapidjson::GetParseError_En(parseError.Code()));

            _isValid = false;
            return;
        }

        parseGlobalSettings(json);
        parseTracks(json);
    }

    void ConfigFile::parseGlobalSettings(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
    {
        parseInt(json, "Normalization", _globalSettings.Normalization);
        parseBool(json, "UseDithering", _globalSettings.UseDithering);
        parseString(json, "OutputPrefix", _globalSettings.OutputPrefix);
        parseBool(json, "LoopIsRelativeToSource", _globalSettings.LoopIsRelativeToSource);

        if (_globalSettings.OutputPrefix.empty())
        {
            // TODO: Log error with real logger class
            fprintf(stderr, "OutputPrefix is required in the config file.\n");
            _isValid = false;
        }
    }

    void ConfigFile::parseTracks(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
    {
        auto trackIt = json.FindMember("Tracks");
        if (trackIt != json.MemberEnd())
        {
            for (auto& item : trackIt->value.GetArray())
            {
                TrackSettings track;

                parseString(item, "Filename", track.Filename);
                parseInt(item, "TrackIndex", track.TrackIndex);
                parseOptionalInt(item, "TrimStart", track.TrimStart);
                parseOptionalInt(item, "TrimEnd", track.TrimEnd);
                parseOptionalInt(item, "Loop", track.Loop);
                parseOptionalInt(item, "Normalization", track.Normalization);
                parseOptionalBool(item, "UseDithering", track.UseDithering);
                parseOptionalBool(item, "LoopIsRelativeToSource", track.LoopIsRelativeToSource);

                if (track.Filename.empty())
                {
                    fprintf(stderr, "A track did not specify its Filename\n");
                    _isValid = false;
                    return;
                }

                if (track.TrackIndex == -1)
                {
                    fprintf(stderr, "A track did not specify its TrackIndex\n");
                    _isValid = false;
                    return;
                }

                _tracks.emplace_back(track);
            }
        }
        else
        {
            fprintf(stderr, "Tracks is required in the config file.\n");
            _isValid = false;
        }
    }

    void ConfigFile::parseString(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, std::string& value)
    {
        auto memberIt = json.FindMember(name);
        if (memberIt != json.MemberEnd())
        {
            value = memberIt->value.GetString();
        }
    }

    void ConfigFile::parseInt(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, int& value)
    {
        auto memberIt = json.FindMember(name);
        if (memberIt != json.MemberEnd())
        {
            value = memberIt->value.GetInt();
        }
    }

    void ConfigFile::parseBool(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, bool& value)
    {
        auto memberIt = json.FindMember(name);
        if (memberIt != json.MemberEnd())
        {
            value = memberIt->value.GetBool();
        }
    }

    void ConfigFile::parseOptionalInt(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, Baroque::Optional<int>& value)
    {
        auto memberIt = json.FindMember(name);
        if (memberIt != json.MemberEnd())
        {
            value = memberIt->value.GetInt();
        }
    }

    void ConfigFile::parseOptionalBool(const rapidjson::GenericValue<rapidjson::UTF8<>>& json, const char* name, Baroque::Optional<bool>& value)
    {
        auto memberIt = json.FindMember(name);
        if (memberIt != json.MemberEnd())
        {
            value = memberIt->value.GetBool();
        }
    }
}
