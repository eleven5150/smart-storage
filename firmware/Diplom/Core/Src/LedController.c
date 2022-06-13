#include "LedController.h"

uint8_t ledData[MAX_LED][AMOUNT_OF_DATA] = {0};

//bool dataSentFlag = false;
uint16_t pwmData[(WS2812_DATA_BIT_WIDTH * MAX_LED) + RESERVE] = {0};

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//    dataSentFlag = true;
}

void LedController_SetLED (int ledNum, int red, int green, int blue)
{
    ledData[ledNum][NUMBER_OF_LED] = ledNum;
    ledData[ledNum][GREEN_INTENSITY] = green;
    ledData[ledNum][RED_INTENSITY] = red;
    ledData[ledNum][BLUE_INTENSITY] = blue;
}


void LedController_WS2812Send (void)
{
    uint32_t indx = 0;
    uint32_t color;

    for (int i = 0; i < MAX_LED; i++)
    {

        color = ((ledData[i][1]<<SHIFT_FOR_GEEN_LED) | (ledData[i][2]<<SHIFT_FOR_RED_LED) | (ledData[i][3]));

        for (int i = WS2812_DATA_BIT_WIDTH - 1; i >= 0; i--)
        {
            if (color&(1<<i))
            {
                pwmData[indx] = 40;  // 2/3 of 90
            }
            else
            {
                pwmData[indx] = 20;  // 1/3 of 90
            }
            indx++;
        }

    }

    for (int i = 0; i < RESERVE; i++)
    {
        pwmData[indx] = 0;
        indx++;
    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
//    while (!dataSentFlag){
//
//    };
//    dataSentFlag = false;
}


void LedController_OnLed (int ledNumber){
    if (ledNumber <= MAX_LED)
    {
        LedController_SetLED(ledNumber, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY, BLUE_COLOR_INTENSITY);
    }

    LedController_WS2812Send();
    HAL_Delay(TIME_FOR_SENDING_DATA);
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
}

void LedController_OffAllLeds(void){
    for (int m = 0; m < MAX_LED; m++)
    {
        LedController_SetLED(m, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY);
    }
    LedController_WS2812Send();
    HAL_Delay(TIME_FOR_SENDING_DATA);
}

void LedController_OnXY (int xLedCoordinate, int yLedCoordinate)
{
    LedController_OffAllLeds();
    if(xLedCoordinate % 2 == UNEVEN_CELL)
    {
        switch(xLedCoordinate)
        {
            case(FIRST_CELL):
                LedController_OnLed(21);
                LedController_OnLed(20);
                LedController_OnLed(19);
                LedController_OnLed(18);
                break;
            case(FIRST_CELL+2):
                LedController_OnLed(13);
                LedController_OnLed(12);
                LedController_OnLed(11);
                break;
            case(FIRST_CELL+4):
                LedController_OnLed(6);
                LedController_OnLed(5);
                LedController_OnLed(4);
                break;
        }
    }
    else
    { //even cell
        switch(xLedCoordinate)
        {
            case(FIRST_CELL+1):
                LedController_OnLed(FIRST_LED+17);
                LedController_OnLed(FIRST_LED+16);
                LedController_OnLed(FIRST_LED+15);
                LedController_OnLed(FIRST_LED+14);
                break;
            case(FIRST_CELL+3):
                LedController_OnLed(FIRST_LED+10);
                LedController_OnLed(FIRST_LED+9);
                LedController_OnLed(FIRST_LED+8);
                LedController_OnLed(FIRST_LED+7);
                break;
            case(FIRST_CELL+5):
                LedController_OnLed(FIRST_LED+3);
                LedController_OnLed(FIRST_LED+2);
                LedController_OnLed(FIRST_LED+1);
                LedController_OnLed(FIRST_LED);
                break;
        }
    }
    if(yLedCoordinate % 2 == UNEVEN_CELL){
            switch(yLedCoordinate){
                case(FIRST_CELL):
                    LedController_OnLed(FIRST_LED+22);
                    LedController_OnLed(FIRST_LED+23);
                    LedController_OnLed(FIRST_LED+24);
                    break;
                case(FIRST_CELL+2):
                    LedController_OnLed(FIRST_LED+27);
                    LedController_OnLed(FIRST_LED+28);
                    LedController_OnLed(FIRST_LED+29);
                    break;
                case(FIRST_CELL+4):
                    LedController_OnLed(FIRST_LED+32);
                    LedController_OnLed(FIRST_LED+33);
                    LedController_OnLed(FIRST_LED+34);
                    break;
                }
        }else{ //even cell
            switch(yLedCoordinate){
                    case(FIRST_CELL+1):
                        LedController_OnLed(FIRST_LED+25);
                        LedController_OnLed(FIRST_LED+26);
                        break;
                    case(FIRST_CELL+3):
                        LedController_OnLed(FIRST_LED+30);
                        LedController_OnLed(FIRST_LED+31);
                        break;
                    case(FIRST_CELL+5):
                        LedController_OnLed(FIRST_LED+35);
                        LedController_OnLed(FIRST_LED+36);
                        LedController_OnLed(FIRST_LED+37);
                        break;
            }
        }
}
