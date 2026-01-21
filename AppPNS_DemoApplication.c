/**************************************************************************************
Exclusion of Liability for this demo software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

#include "AppPNS_DemoApplication.h"

/*******************************************************************************/

PROTOCOL_IDENTIFICATION_T g_tProtocolIdentification =
{
  .szFirmwareName = "PROFINET IO Device",
  .usFirmwareMajor = PNS_FIRMWARE_VERSION_MAJOR,
  .usFirmwareMinor = PNS_FIRMWARE_VERSION_MINOR,
};

/*******************************************************************************/

static MY_PNS_COMM_CHANNEL_HANDLER_RSC_T s_tPnsRsc;

/**************************************************************************************
*   Initialize PNS Application
*
*   \param phStackRsc
*   \param hCifXChannel
*   \param ptCifXChannelInfo
*
*   \return CIFX_NO_ERROR on success.
**************************************************************************************/
static int AppPNS_Initialize(APP_COMM_CHANNEL_HANDLER_RSC_H *phStackRsc, CIFXHANDLE hCifXChannel, CHANNEL_INFORMATION *ptCifXChannelInfo )
{
  uint32_t ulRet = CIFX_NO_ERROR;
  MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc = &s_tPnsRsc;

  ptRsc->hCifXChannel      = hCifXChannel;
  ptRsc->ptCifXChannelInfo = ptCifXChannelInfo;

  *phStackRsc = (APP_COMM_CHANNEL_HANDLER_RSC_H)ptRsc;

  if( !Pkt_Init( &(ptRsc->hPktIfRsc), ptRsc->hCifXChannel))
    return -1;

  if( !Pkt_RegisterPacketHandler(ptRsc->hPktIfRsc, AppPNS_PacketHandler, ptRsc))
    return -1;

#ifdef HOST_APPLICATION_SETS_MAC_ADDRESS
  /* optional when initial DDP state is passive: set DDP base device parameters: MAC address */
  ulRet = AppPNS_SetMacAddress( ptRsc );
  if( ulRet != CIFX_NO_ERROR )
    return (int)ulRet;
#endif

#ifdef HOST_APPLICATION_SETS_SERIAL_NUMBER
  /* optional when initial DDP state is passive: set additional (OEM) DDP base device parameters: serial number */
  ulRet = AppPNS_SetOemDeviceData( ptRsc );
  if(ulRet != CIFX_NO_ERROR)
    return (int)ulRet;
#endif

#if defined(HOST_APPLICATION_SETS_MAC_ADDRESS) || defined(HOST_APPLICATION_SETS_SERIAL_NUMBER)
  ulRet = AppPNS_ActivateDdp( ptRsc );
  if(ulRet != CIFX_NO_ERROR)
    return (int)ulRet;
#endif

  return (int)ulRet;
}

/**************************************************************************************
*   PNS Application Setup
*
*   \param hRsc
*
*   \return CIFX_NO_ERROR on success.
**************************************************************************************/
static int AppPNS_Setup(APP_COMM_CHANNEL_HANDLER_RSC_H hRsc )
{
  MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc = (MY_PNS_COMM_CHANNEL_HANDLER_RSC_T*)hRsc;
  uint32_t ulRet = CIFX_NO_ERROR;

  /* Register application */
  App_SysPkt_AssembleRegisterAppReq( &(ptRsc->tPacket) );
  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
  if( ulRet != CIFX_NO_ERROR )
    return (int)ulRet;

  /* Download the configuration */
  ulRet = AppPNS_ConfigureStack( ptRsc );
  if( ulRet != CIFX_NO_ERROR )
    return (int)ulRet;

  return 0;
}

/**************************************************************************************
*   PNS Application routine task
*
*   \param hRsc
**************************************************************************************/
static void AppPNS_RoutineTask(APP_COMM_CHANNEL_HANDLER_RSC_H hRsc)
{
  MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc = (MY_PNS_COMM_CHANNEL_HANDLER_RSC_T*)hRsc;
  Pkt_CheckReceiveMailbox( ptRsc->hPktIfRsc , &(ptRsc->tPacket) );
}

/**************************************************************************************
*   PNS Application IO Task
*
*   \param hRsc
*
*   \return CIFX_NO_ERROR on success.
**************************************************************************************/
static void AppPNS_IOTask(APP_COMM_CHANNEL_HANDLER_RSC_H hRsc)
{
  MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc = (MY_PNS_COMM_CHANNEL_HANDLER_RSC_T*)hRsc;
  int32_t lRet = CIFX_NO_ERROR;

  /** INPUT DATA ***************************************************************/
  lRet = xChannelIORead( ptRsc->hCifXChannel, 0, 0, sizeof(ptRsc->abActorData), ptRsc->abActorData, 0 );
  if( lRet != CIFX_NO_ERROR )
  {
    /** Something failed?
     * Reason for error could be:
     * 1) netX is not "ready" yet. May happen during startup.
     * 2) netX is not "running" yet. May happen during startup in case the netX is not fully configured yet.
     * 3) netX has not yet established an IO connection. */

  }
  else
  {
    /** process newly received input data image */

  }

  ptRsc->abSensorData[0] = ptRsc->abActorData[0] + 1;


  /** OUTPUT DATA ***************************************/
  lRet = xChannelIOWrite( ptRsc->hCifXChannel, 0, 0, sizeof(ptRsc->abSensorData), ptRsc->abSensorData, 0 );
  if( lRet != CIFX_NO_ERROR )
  {
    /** Something failed?
     * Reason for error could be:
     * 1) netX is not "ready" yet. May happen during startup.
     * 2) netX is not "running" yet. May happen during startup in case the netX is not fully configured yet.
     * 3) netX has not yet established an IO connection. */
  }

}

/*******************************************************************************/

APP_COMM_CHANNEL_HANDLER_T g_tRealtimeProtocolHandler =
{
  .pfnInitialize  = AppPNS_Initialize,
  .pfnSetup       = AppPNS_Setup,
  .pfnRoutineTask = AppPNS_RoutineTask,
  .pfnTimerTask   = AppPNS_IOTask,
};

/*******************************************************************************/
