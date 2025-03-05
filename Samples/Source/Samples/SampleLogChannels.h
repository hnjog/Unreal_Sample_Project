#pragma once

#include"Containers/UnrealString.h"
#include"Logging/LogMacros.h"

// extern 이기에 구조체의 정의가 다른 파일에 있다고 하므로
// cpp 쪽에 선언을 해준다
DECLARE_LOG_CATEGORY_EXTERN(LogSample, Log, All);
