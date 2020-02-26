/*
 * i2c.c
 *
 *  Created on:
 *      Author:  Sprints wave4
 */
/*================================================*
 * DEFINES
 * ==============================================*/
#define I2C_PRIVATE_CODE
#define I2C_NUM_OF_CHANNEL              4
/*================================================*
 * INCLUDES
 * ==============================================*/
#include "../../utils/Bit_Math.h"
#include "../../utils/STD_Types.h"
#include "../mcu_hw.h"

#include "../../config/i2c_cfg.h"
#include "i2c_types.h"
#include "i2c.h"

/*================================================*
 * EXTERNS
 * ==============================================*/
extern I2C_CfgType I2C_CfgArr[];

/*================================================*
 * LOCAL VARIABLES
 * ==============================================*/
static const uint32 I2c_BaseAddrArr[] = {I2C_0_BASE_ADDRESS, I2C_1_BASE_ADDRESS, I2C_2_BASE_ADDRESS, I2C_3_BASE_ADDRESS};
static I2C_ChannelParamType I2C_ChannelParam[I2C_NUM_OF_CHANNEL];
static I2C_StatusType I2C_ChannelStatus[I2C_NUM_OF_CHANNEL];
volatile static I2C_MasterModeType I2C_MasterMode;

/*================================================*
 * FUNCTIONS DEFINTIONS
 * ==============================================*/

void I2C_init(void)
{
    uint8 CfgIndex;
    uint32 TPR_value;
    I2C_ChannelType ChannelId;
    uint32 BaseAddress;

    for(CfgIndex=0; CfgIndex<I2C_NUM_OF_ACTIVATED_CHANNEL; CfgIndex++)
    {
        ChannelId  =  I2C_CfgArr[CfgIndex].I2C_ChannelId;
        I2C_ChannelStatus[ChannelId] = I2C_Status_Idle;
        BaseAddress = I2c_BaseAddrArr[ChannelId];

        /* Initialize the I2C Mode Master/slave Mode**/
        if(I2C_CfgArr[CfgIndex].I2C_Mode==I2C_Mode_Master)
        {
            I2CMCR(BaseAddress).B.MFE = STD_high;
        }
        else
        {
            I2CMCR(BaseAddress).B.MFE = STD_low;
        }

        I2CMCR(BaseAddress).B.LPBK =I2C_CfgArr[CfgIndex].I2C_LoopbackEn;

        /* value to be assigned to I2CMTPR register  */
        TPR_value= (uint32)((((uint32)80000000)/((uint32)(20*I2C_CfgArr[CfgIndex].I2C_BaudRate)))-1);

        if(I2C_CfgArr[CfgIndex].I2C_SpeedMode==I2C_SpeedMode_Standard)
        {
            /****Standard speed mode *****/
            I2CMTPR(BaseAddress).B.HS=STD_low;

        }
        else if(I2C_CfgArr[CfgIndex].I2C_SpeedMode==I2C_SpeedMode_HighSpeed)
        {
            /****High speed mode *****/
            I2CMTPR(BaseAddress).B.HS=STD_high;
        }

        /*****Set the desired SCL clock speed*****/

        I2CMTPR(BaseAddress).B.TPR=TPR_value;

        /*Enable Interrupt mask*/
        I2CMIMR(BaseAddress).B.IM=STD_high;

    }
}



Std_ReturnType I2C_writeIB(I2C_ChannelType Channel, uint8* Data, uint8 SizeCpy)
{
    Std_ReturnType Ret = E_OK;
    uint8 i;

    /*
         * TODO Check if  Channel status is Idle
         * */
    /* never write the user's buffer to the internal buffer unless the i2c status is idle */
    if( I2C_ChannelStatus[Channel] == I2C_Status_Idle)
    {
        /*
             * TODO Set Channel status to Busy
             * */
        /* make status of the i2c channel busy */
        I2C_ChannelStatus[Channel] = I2C_Status_BUSY;

        /* assign the user's buffer limit to the internal tx buffer */
        I2C_ChannelParam[Channel].TxMsgSize = SizeCpy;

        for (i = 0; i < SizeCpy; ++i)
        {
            /* fill the internal tx buffer with the user's buffer data */
            I2C_ChannelParam[Channel].TxBuffer[i] = Data[i];
        }

        /*
             * TODO Set Channel status to Idle
             * */
        I2C_ChannelStatus[Channel] = I2C_Status_Idle;
    }
    else
    {
        Ret = E_NOT_OK;
    }



    return Ret;

}
Std_ReturnType I2C_readIB(I2C_ChannelType Channel, uint8* Data, uint8* Size)
{
    Std_ReturnType Ret = E_OK;


    return Ret;

}
Std_ReturnType I2C_syncTransmit( I2C_ChannelType Channel, uint8 Sla, I2C_OperationType Operation, /*Read\Write*/ uint8 RxSize, I2C_StopFrameEnType StopFrameEn)
{
    Std_ReturnType Ret = E_OK;
    uint32 BaseAddress = I2c_BaseAddrArr[Channel];
    I2C_ChannelParam[Channel].TxBufferIndex = 0;

    /*
     * TODO Check if  Channel status is Idle
     * */
    if( I2C_ChannelStatus[Channel] == I2C_Status_Idle)
    {

    /*
     * TODO Set Channel status to Busy
     * */

        I2C_ChannelStatus[Channel] = I2C_Status_BUSY;
    /*
        TODO Specify the slave address of the master and that the next operation is a Transmit by writing the I2CMSA register with a value of 0x0000.0076. This sets the slave address to 0x3B.
     */
        /* clear the R/W bit before setting or clearing it */
        I2CMSA(BaseAddress).B.RS = 0;

        /* assign the R/W an the slave address */
        I2CMSA(BaseAddress).R = (uint32)(Sla | Operation);

        /*
               TODOPlace data (byte) to be transmitted in the data register by writing the I2CMDR register with the desired data.
            */

        /* if we are going to write(Master Transmit), then prepare the 1st byte */
        if( Operation == I2C_Operation_Write )
        {
            I2C_MasterMode = I2C_MasterMode_Transmitter;
            I2CMDR(BaseAddress) = I2C_ChannelParam[Channel].TxBuffer[I2C_ChannelParam[Channel].TxBufferIndex];
            I2C_ChannelParam[Channel].TxBufferIndex++;
        }

        /* if we are going to read(Master Receive) */
        else
        {
            I2C_MasterMode = I2C_MasterMode_Receiver;
        }


    /*
        TODO Initiate a single byte transmit of the data from Master to Slave by writing the I2CMCS register with desired value of (STOP, START, RUN)
     */
        I2CMCS_Write(BaseAddress).B.RUN = 1;
        I2CMCS_Write(BaseAddress).B.START = 1;
        I2CMCS_Write(BaseAddress).B.STOP = 0;
        I2CMCS_Write(BaseAddress).B.HS = 0;


    }
    else
    {
        Ret = E_NOT_OK;
    }


    return Ret;

}

I2C_StatusType I2C_getStatus(I2C_ChannelType Channel)
{
    /*TODO Return Channel Status*/


}




/*==================================================================
 * INTERRUPT HANDLERS
 * ================================================================*/
static inline void I2c_IntRoutine(I2C_ChannelType Channel)
{


    /* get the base address of the i2c channel */
    uint32 BaseAddress = I2c_BaseAddrArr[Channel];

    /* if the i2c is configured as master mode */
    if( I2CMCR(BaseAddress).B.MFE == 1 )
    {
        /* if the i2c is configured as master transmitter */
        if( I2C_MasterMode == I2C_MasterMode_Transmitter )
        {
            /* check if we received an ACK for the previous transmitted byte */
            if( I2CMCS_Read(BaseAddress).B.DATACK == 1 )
            {
                /* if there are still bytes in the tx buffer, send the next byte */
                if( I2C_ChannelParam[Channel].TxBufferIndex <= I2C_ChannelParam[Channel].TxMsgSize-1  )
                {
                    I2CMDR(BaseAddress) = I2C_ChannelParam[Channel].TxBuffer[I2C_ChannelParam[Channel].TxBufferIndex];
                    I2C_ChannelParam[Channel].TxBufferIndex++;
                    I2CMCS_Write(BaseAddress).B.RUN = 1;
                    I2CMCS_Write(BaseAddress).B.START = 0;
                    I2CMCS_Write(BaseAddress).B.STOP = 0;
                    I2CMCS_Write(BaseAddress).B.HS = 0;
                }
                /* if no more data in the buffer to be sent, send the stop condition */
                else
                {
                    I2CMCS_Write(BaseAddress).B.RUN = 1;
                    I2CMCS_Write(BaseAddress).B.STOP = 0;
                }
            }

            /* if received NOT ACK, send the stop condition */
            else
            {
                I2CMCS_Write(BaseAddress).B.RUN = 1;
                I2CMCS_Write(BaseAddress).B.STOP = 0;
            }

        }
        /* if the i2c is configured as master receiver */
        else
        {
            /* further implementation */
        }
    }

    /* else if i2c is configured as slave */
    else if( I2CMCR(BaseAddress).B.SFE == 1 )
    {
        /* further implementation */
    }

    /* clear the interrupt flag */
    I2CMICR(BaseAddress).B.IC = 1;
}

void I2c0_IntHandler(void)
{
    I2c_IntRoutine(I2C_Channel_0);
}
void I2c1_IntHandler(void)
{
    I2c_IntRoutine(I2C_Channel_1);
}
void I2c2_IntHandler(void)
{
    I2c_IntRoutine(I2C_Channel_2);
}
void I2c3_IntHandler(void)
{
    I2c_IntRoutine(I2C_Channel_3);
}
