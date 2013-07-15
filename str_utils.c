#include "str_utils.h"

uint8_t strLen(const char* str)
{
	uint8_t len = 0;
	while (*(str + len) != '\0')
	{
		len++;
	}
	return len;
}

