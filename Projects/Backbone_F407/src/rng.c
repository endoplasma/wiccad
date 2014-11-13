#include "rng.h"

RNG_HandleTypeDef RngHandle;

void rng_init()
{
	__RNG_CLK_ENABLE();
	RngHandle.Instance = RNG;
	if(HAL_RNG_Init(&RngHandle) != HAL_OK)
	{
		Error_Handler();
	}
}


uint32_t rng()
{
	return HAL_RNG_GetRandomNumber(&RngHandle);
}
