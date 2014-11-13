#include "adc.h"

ADC_HandleTypeDef			ADC_temperature_handler;



void adc_init()
{
	ADC_ChannelConfTypeDef		ADC_Channel_Conf;
	__ADC1_CLK_ENABLE();

	ADC_temperature_handler.Instance = ADC1;
	ADC_temperature_handler.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	ADC_temperature_handler.Init.Resolution = ADC_RESOLUTION12b;
	ADC_temperature_handler.Init.ScanConvMode = DISABLE;
	ADC_temperature_handler.Init.ContinuousConvMode = ENABLE;
	ADC_temperature_handler.Init.DiscontinuousConvMode = DISABLE;
	ADC_temperature_handler.Init.NbrOfDiscConversion = 0;
	ADC_temperature_handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_temperature_handler.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADC_temperature_handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_temperature_handler.Init.NbrOfConversion = 1;
	ADC_temperature_handler.Init.DMAContinuousRequests = ENABLE;
	ADC_temperature_handler.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&ADC_temperature_handler);

	ADC_Channel_Conf.Channel = ADC_CHANNEL_TEMPSENSOR;
	ADC_Channel_Conf.Rank = 1;
	ADC_Channel_Conf.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	ADC_Channel_Conf.Offset = 0;
	HAL_ADC_ConfigChannel(&ADC_temperature_handler, &ADC_Channel_Conf);

	HAL_ADC_Start(&ADC_temperature_handler);
}



#define AVERAGE_HISTORY	32
float adc_get_temp_in_celsius()
{
	static float past[AVERAGE_HISTORY];
	static uint8_t idx = 0;

	float TemperatureValue = HAL_ADC_GetValue(&ADC_temperature_handler);
	TemperatureValue *= 2.87;
	TemperatureValue /= 0xfff;
	TemperatureValue -= 0.760; // reference voltage at 25°C
	TemperatureValue /= .0025; // slope 2.5mV/°C
	TemperatureValue += 25.0; // Add 25°C

	past[idx++] = TemperatureValue;
	if(idx == AVERAGE_HISTORY)
		idx = 0;

	TemperatureValue = 0;
	uint8_t i;
	for(i=0;i<AVERAGE_HISTORY;i++)
		TemperatureValue += past[i];

	return TemperatureValue/AVERAGE_HISTORY;
}
