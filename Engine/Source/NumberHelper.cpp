#include "NumberHelper.h"

ENGINE_API inline bool within(float number, float amount, float value) {
	return value < number + amount && value > number - amount;
}