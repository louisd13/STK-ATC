#include "audio/tts/tts.hpp"

void Tts::setSpeech(std::string toSay) {
    std::wstring input(toSay.size(), L' '); // Overestimate number of code points.
    input.resize(std::mbstowcs(&input[0], toSay.c_str(), toSay.size()));

    hr = pVoice->Speak((L"<voice required='Gender = Male;'>" + input).c_str(), 0, NULL);
}