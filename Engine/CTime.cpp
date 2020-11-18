#include "CTime.h"
#include <Windows.h>
#include <string>

int CTime::Init()
{
    m_lastTimeStamp = timeGetTime();

    return 0;
}

void CTime::Update()
{
    double actualTimestamp = timeGetTime();
    m_deltaTime = actualTimestamp - m_lastTimeStamp;
    m_lastTimeStamp = actualTimestamp;
    m_deltaTime *= 0.001;
    m_time += m_deltaTime;
    m_fps = roundf(1 / m_deltaTime);

    std::wstring output = TEXT("deltaTime: ") + std::to_wstring(m_fps) + TEXT("\n");

    OutputDebugString(output.c_str());
}

void CTime::Release()
{
}
