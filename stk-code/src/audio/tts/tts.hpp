#pragma once

#include <sapi.h>
#include <iostream>
#include <string>

class Tts {
    protected:
    int choice;
    ISpVoice* pVoice;
    HRESULT hr,a;
    std::wstring input;

    public:
    Tts() {
        pVoice = NULL;
        input = L"";
        a = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        //HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
        if (FAILED(a)){
            std::cout << "ERROR 404 FAILED INITIALIZING COM\n";
            exit(1);
        }
        
        hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    }
    
    void setSpeech(std::string s);
    
    ~Tts() {
        ::CoUninitialize();
        delete pVoice;
    }
};