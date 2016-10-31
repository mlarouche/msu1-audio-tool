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
#include "Builder.h"

// SOX includes
#include <sox.h>

// Local includes
#include "CFileWrapper.h"
#include "StringFormat.h"

namespace msu1
{
    static const int BufferSize = 4*1024;

    struct SoxFormatWrapper
    {
        SoxFormatWrapper(sox_format_t* format)
            : _format(format)
        {
        }
        ~SoxFormatWrapper()
        {
            if (_format)
            {
                sox_close(_format);
            }
        }

        operator sox_format_t*()
        {
            return _format;
        }

        sox_format_t* operator->() const
        {
            return _format;
        }

    private:
        sox_format_t* _format;
    };

    struct SoxChainWrapper
    {
        SoxChainWrapper(sox_effects_chain_t* chain)
        : _chain(chain)
        {}

        ~SoxChainWrapper()
        {
            if (_chain)
            {
                sox_delete_effects_chain(_chain);
            }
        }

        operator sox_effects_chain_t*()
        {
            return _chain;
        }

    private:
        sox_effects_chain_t* _chain;
    };

    Builder::Builder()
    {
    }

    Builder::Builder(const msu1::BuildInfo& info)
    : _info(info)
    {
    }

    void Builder::Build()
    {
        sox_effect_t* effect = nullptr;
        char* args[2];

        auto fileIn = SoxFormatWrapper(sox_open_read(_info.Filename().c_str(), nullptr, nullptr, nullptr));
        if (!fileIn)
        {
            return;
        }

        sox_format_t* memOutFormat = new sox_format_t;
        memcpy(memOutFormat, fileIn, sizeof(sox_format_t));

        memOutFormat->signal.rate = 44100;
        memOutFormat->signal.channels = 2;
        memOutFormat->signal.precision = 16;
        memOutFormat->signal.length = SOX_UNSPEC;

        // NOTE: Ideally I would have use a memory stream but SoX doesn't support it on Windows MSVC
        char tempWavFilename[1024];
        sprintf(tempWavFilename, "%s-%d.wav", _info.OutputPrefix().c_str(), _info.TrackIndex());

        auto memOut = SoxFormatWrapper(sox_open_write(tempWavFilename, &memOutFormat->signal, nullptr, "wav", nullptr, nullptr));

        if (!memOut)
        {
            return;
        }

        auto chain = SoxChainWrapper(sox_create_effects_chain(&fileIn->encoding, &memOut->encoding));

        effect = sox_create_effect(sox_find_effect("input"));
        args[0] = reinterpret_cast<char*>((sox_format_t*)fileIn);
        sox_effect_options(effect, 1, args);
        sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);
        free(effect);

        if (fileIn->signal.rate != memOut->signal.rate)
        {
            effect = sox_create_effect(sox_find_effect("rate"));
            sox_effect_options(effect, 0, nullptr);
            sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);
            free(effect);
        }

        if (fileIn->signal.channels != memOut->signal.channels)
        {
            effect = sox_create_effect(sox_find_effect("channels"));
            sox_effect_options(effect, 0, nullptr);
            sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);
            free(effect);
        }

        if (_info.TrimStart().HasValue() || _info.TrimEnd().HasValue())
        {
            effect = sox_create_effect(sox_find_effect("trim"));

            std::vector<char*> trimParameters;

            if (_info.TrimStart().HasValue())
            {
                trimParameters.emplace_back(Baroque::RawStringFormat("%us", _info.TrimStart().Value()));
            }

            if (_info.TrimEnd().HasValue())
            {
                trimParameters.emplace_back(Baroque::RawStringFormat("=%us", _info.TrimEnd().Value()));
            }

            sox_effect_options(effect, trimParameters.size(), trimParameters.data());

            sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);

            free(effect);
        }

        if (_info.Normalization() != 0)
        {
            effect = sox_create_effect(sox_find_effect("nongnunormalize"));
            args[0] = Baroque::RawStringFormat("%d", _info.Normalization());
            sox_effect_options(effect, 1, args);
            sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);

            free(effect);
        }

        if (_info.UseDithering())
        {
            effect = sox_create_effect(sox_find_effect("dither"));
            args[0] = "-s";
            sox_effect_options(effect, 1, args);
            sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);
        }

        effect = sox_create_effect(sox_find_effect("output"));
        args[0] = reinterpret_cast<char*>((sox_format_t*)memOut);
        sox_effect_options(effect, 1, args);
        sox_add_effect(chain, effect, &fileIn->signal, &memOut->signal);
        free(effect);

        sox_flow_effects(chain, nullptr, nullptr);

        sox_sample_t rawSamples[BufferSize];

        auto memIn = SoxFormatWrapper(sox_open_read(tempWavFilename, nullptr, nullptr, nullptr));

        if (!memIn)
        {
            return;
        }

        char pcmFilename[1024];
        sprintf(pcmFilename, "%s-%d.pcm", _info.OutputPrefix().c_str(), _info.TrackIndex());

        auto outputFile = Baroque::CFileWrapper(pcmFilename, "wb");
        if (outputFile)
        {
            fwrite("MSU1", 4, 1, outputFile);

            int loopPoint = _info.Loop();
            if (loopPoint > 0 && _info.LoopIsRelativeToSource() && _info.TrimStart().HasValue())
            {
                loopPoint -= _info.TrimStart().Value();
            }
            fwrite(reinterpret_cast<void*>(&loopPoint), sizeof(int), 1, outputFile);

            size_t samplesRead = 0;
            while (samplesRead = sox_read(memIn, rawSamples, BufferSize))
            {
                int16_t samples[BufferSize];

                for (size_t i = 0; i < samplesRead; ++i)
                {
                    SOX_SAMPLE_LOCALS;
                    int clips = 0;

                    samples[i] = SOX_SAMPLE_TO_SIGNED_16BIT(rawSamples[i],clips);
                }

                fwrite(samples, sizeof(int16_t), samplesRead, outputFile);
            }
        }

        if (!_info.KeepWav())
        {
            remove(tempWavFilename);
        }
    }
}
