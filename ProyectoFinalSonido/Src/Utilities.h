#pragma once

inline bool ERRCHECK(const FMOD_RESULT & res)
{
	if (res != FMOD_OK)
	{
		throw new std::exception(FMOD_ErrorString(res));
	}
}
