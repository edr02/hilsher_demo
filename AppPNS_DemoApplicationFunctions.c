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
/* Includes */

#include "AppPNS_DemoApplication.h"

/*****************************************************************************************************************************************/
/* Configuration related definitions */

#define APP_PNS_SUBMODULE_COUNT    6 //With 18, the configuration packet is to bit an will be send fragmented

/**************************************************************************************
 function:    SetConfigParams
 description: Fills all necessary device parameters in set configuration packet.

 global:      none
 input:       PNS_IF_DEVICE_PARAMETER_T FAR* ptSetConfig    - pointer to the packet
 UINT16 usDeviceClass                          - netX device class

 output:      none
 return:      long ( size of pointer to the packet )
 **************************************************************************************/
long SetConfigParams(void* pvPck, uint16_t usDeviceClass)
{
  PNS_IF_DEVICE_PARAMETER_T *ptSetConfig = pvPck;

  /* First set arrays to zero */
  memset(ptSetConfig->abNameOfStation, 0x00, 240); /* The NameOfStation as ASCII char-array  */
  memset(ptSetConfig->abTypeOfStation, 0x00, 240); /* The TypeOfStation as ASCII char-array  */
  memset(ptSetConfig->abDeviceType, 0x00, 28); /* The DeviceType as ASCII char-array     */
  memset(ptSetConfig->abOrderId, 0x00, 20); /* The OrderId as ASCII char-array        */

  ptSetConfig->ulSystemFlags = (PNS_IF_SYSTEM_START_AUTO_START |
      PNS_IF_SYSTEM_STACK_HANDLE_I_M_ENABLED |
      PNS_IF_SYSTEM_ARDY_WOUT_APPL_REG_ENABLED |
      PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_ENABLED);

  ptSetConfig->ulWdgTime = 1000; /* Watchdog time (in milliseconds)        */
  ptSetConfig->ulVendorId = 0x011E; /* Vendor ID                              */
  ptSetConfig->ulMaxAr = 0; /* Currently not used                     */
  ptSetConfig->ulCompleteInputSize = 0; /* Maximum amount of allowed input data   */
  ptSetConfig->ulCompleteOutputSize = 0; /* Maximum amount of allowed output data  */

  ptSetConfig->ulDeviceId = 0x1003; /* Device ID                              */
  #if !defined( DEMO_QUIET )
  printf("DeviceID: %u\r\n", (unsigned int) ptSetConfig->ulDeviceId);
  #endif
  /*Set Device Name and IP with configuration packet works only if Flag:
   PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_ENABLED is disabled. */
  memcpy(ptSetConfig->abNameOfStation, "netxv5repns", sizeof("netxv5repns")); /* The NameOfStation as ASCII char-array  */
  ptSetConfig->ulNameOfStationLen = sizeof("netxv5repns") - 1; /* Length of NameOfStation                */

  memcpy(ptSetConfig->abTypeOfStation, "NETX V5 RE/PNS", sizeof("NETX V5 RE/PNS")); /* The TypeOfStation as ASCII char-array  */
  ptSetConfig->ulTypeOfStationLen = sizeof("NETX V5 RE/PNS") - 1; /* Length of TypeOfStation                */
  memcpy(ptSetConfig->abDeviceType, "NETX V5 RE/PNS", sizeof("NETX V5 RE/PNS") - 1); /* The DeviceType as ASCII char-array     */

  ptSetConfig->ulIpAddr = 0; /* IP address, default: 0.0.0.0                         */
  ptSetConfig->ulNetMask = 0; /* Network mask, default: 0.0.0.0                       */
  ptSetConfig->ulGateway = 0; /* Gateway address, default: 0.0.0.0                    */
  ptSetConfig->usSwRevision1 = 5; /* Software Revision 5, default: 0                      */
  ptSetConfig->usSwRevision2 = 5; /* Software Revision 5, default: 0                      */
  ptSetConfig->usSwRevision3 = 0; /* Software Revision 0, default: 0                      */
  ptSetConfig->bSwRevisionPrefix = 'V'; /* Software Revision Prefix, default: 0                 */
  ptSetConfig->bReserved = 0; /* Reserved, set to zero                                */
  ptSetConfig->usMaxDiagRecords = 16; /* The number of diagnosis records, default: 256        */
  ptSetConfig->usInstanceId = 1; /* Instance ID, GSDML ObjectUUID_LocalIndex, default: 1 */
  ptSetConfig->usReserved = 0; /* Reserved for future use, set to zero                 */

  return sizeof(*ptSetConfig);
} /* SetConfigParams */


/**************************************************************************************
 function:    SetModulConfig
 description: Fills all necessary information about module configuration.

 global:      none
 input:       PNS_IF_MODULE_CFG_REQ_DATA_T FAR* ptModule    - pointer to the packet

 output:      none
 return:      long (Pointer to the packet)
 **************************************************************************************/
long SetModulConfig(void* pvPck)
{

  PNS_IF_MODULE_CFG_REQ_DATA_T *ptModule = pvPck;

  PNS_IF_API_STRUCT_T *ptApi = NULL;
  PNS_IF_SUBMODULE_STRUCT_T *ptSubmod = NULL;

  ptModule->ulNumApi = 0x01; /* Number of API elements to follow */
  ptApi = (PNS_IF_API_STRUCT_T *) (ptModule + 1); /* Pointer to Module configuration  */

  ptApi->ulApi = 0x00; /* Number of the API profile to be configured   */
  if(ptApi->ulNumSubmodItems != APP_PNS_SUBMODULE_COUNT) /* Number of submodule-items this API contains  */
  {
    while(1)
    {
    }
  }

  ptSubmod = (PNS_IF_SUBMODULE_STRUCT_T *) (ptApi + 1); /* Pointer to Submodule configuration           */

  /* Configure the DAP NETX RE/PNS V5.x */
  ptSubmod->usSlot = 0x0000; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x0001; /* Subslot of this submodule                                              */
  ptSubmod->ulModuleId = 0x00003095; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00003094; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000000; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++; /* Increment submodule                                                    */

  /* Configure PDEV PN-IO */
  ptSubmod->usSlot = 0x0000; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x8000; /* Subslot of this submodule                                              */
  ptSubmod->ulModuleId = 0x00003095; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00003095; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000000; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++; /* Increment submodule                                                    */

  /* Configure PDEV Port 1 */
  ptSubmod->usSlot = 0x0000; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x8001; /* Subslot of this submodule                                              */
  ptSubmod->ulModuleId = 0x00003095; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00003096; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000000; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++; /* Increment submodule                                                    */

  /* Configure PDEV Port 2 */
  ptSubmod->usSlot = 0x0000; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x8002; /* Subslot of this submodule                                              */
  ptSubmod->ulModuleId = 0x00003095; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00003096; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000000; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++;

  /* Configure 10 Byte Input module (Device to PLC) */
  ptSubmod->usSlot = 0x0001; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x0001; /* Subslot of this submodule                                              */
  /* PNS SET CONFIG Dirk */
  ptSubmod->ulModuleId = 0x00000001; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00000001; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x0000000A; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000000; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++; /* Increment submodule                                                    */

  /* Configure 6 Byte Output module (PLC to Device) */
  ptSubmod->usSlot = 0x0002; /* Slot this submodule belongs to                                         */
  ptSubmod->usSubslot = 0x0001; /* Subslot of this submodule                                              */
  /* PNS SET CONFIG Dirk */
  ptSubmod->ulModuleId = 0x00000002; /* Module ID to which this submodule belongs                              */
  ptSubmod->ulSubmodId = 0x00000001; /* Submodule ID                                                           */
  ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
  ptSubmod->ulConsDataLen = 0x00000006; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
  ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
  /* submodule shall be taken from                                          */
  ptSubmod->ulDPMOffsetIn = 0x00000000; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
  /* submodule shall be copied to                                           */
  ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
  /* submodule relative to beginning of IOPS block in DPM output area from  */
  ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
  /* submodule relative to beginning of IOPS block in DPM input area to     */
  ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
  /* submodule relative to beginning of IOCS block in DPM output area from  */
  ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
  /* submodule relative to beginning of IOCS block in DPM input area to     */
  ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
  ptSubmod++; /* Increment submodule                                                    */

  for(int i = 0; i < (APP_PNS_SUBMODULE_COUNT - 6u); i++)
  {
    /* Configure 6 Byte Output module (PLC to Device) */
    ptSubmod->usSlot = 0x0003 + i; /* Slot this submodule belongs to                                         */
    ptSubmod->usSubslot = 0x0001; /* Subslot of this submodule                                              */
    /* PNS SET CONFIG Dirk */
    ptSubmod->ulModuleId = 0x00000002; /* Module ID to which this submodule belongs                              */
    ptSubmod->ulSubmodId = 0x00000001; /* Submodule ID                                                           */
    ptSubmod->ulProvDataLen = 0x00000000; /* Length of provider data (sent by IO-Device, received by IO-Controller) */
    ptSubmod->ulConsDataLen = 0x00000006; /* Length of consumer data (sent by IO-Controller, received by IO-Device) */
    ptSubmod->ulDPMOffsetOut = 0x00000000; /* Offset in DPM OutputArea or in Output-image, where provided data of the*/
    /* submodule shall be taken from                                          */
    ptSubmod->ulDPMOffsetIn = 0x00000006 + 6u * i; /* Offset in DPM InputArea or in Input-image, where consumed data for the */
    /* submodule shall be copied to                                           */
    ptSubmod->usOffsetIOPSProvider = 0x0000; /* Offset where the stack shall take the IOPS provider state for this     */
    /* submodule relative to beginning of IOPS block in DPM output area from  */
    ptSubmod->usOffsetIOPSConsumer = 0x0000; /* Offset where the stack shall put the IOPS consumer state of this       */
    /* submodule relative to beginning of IOPS block in DPM input area to     */
    ptSubmod->usOffsetIOCSProvider = 0x0000; /* Offset where the stack shall take the IOCS provider state for this     */
    /* submodule relative to beginning of IOCS block in DPM output area from  */
    ptSubmod->usOffsetIOCSConsumer = 0x0000; /* Offset where the stack shall put the IOCS consumer state for this      */
    /* submodule relative to beginning of IOCS block in DPM input area to     */
    ptSubmod->ulReserved = 0x00000000; /* Reserved for future usage                                              */
    ptSubmod++; /* Increment submodule                                                    */
  }
  return (long) ptSubmod - (long) ptModule;
} /* SetModulConfig */

/**************************************************************************************/
/**************************************************************************************/

/* Download the configuration */
uint32_t AppPNS_ConfigureStack(MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc)
{
  uint32_t     ulRet            = CIFX_NO_ERROR;
  CIFX_PACKET* ptPacket         = &ptRsc->tPacket;

  uint16_t     usDeviceClass    = 0;

  struct
  {
    /** Data part */
    PNS_IF_SET_CONFIGURATION_REQUEST_DATA_T tSetConfigReq;
    /** Submodule part */
    PNS_IF_API_STRUCT_T tAPI;
    /** Submodule part */
    PNS_IF_SUBMODULE_STRUCT_T tSubmodules[APP_PNS_SUBMODULE_COUNT];
  } tConfiguration;

  /* Clear packet */
  memset( ptPacket, 0, sizeof(*ptPacket) );

  tConfiguration.tSetConfigReq = (PNS_IF_SET_CONFIGURATION_REQUEST_DATA_T ) { 0 };
  tConfiguration.tSetConfigReq.tModuleConfig.ulNumApi = 0x01u;
  tConfiguration.tAPI.ulNumSubmodItems = APP_PNS_SUBMODULE_COUNT;

  tConfiguration.tSetConfigReq.ulTotalConfigPckLen = sizeof(tConfiguration);

  ptPacket->tHeader.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptPacket->tHeader.ulId = 0x1u;
  ptPacket->tHeader.ulCmd = PNS_IF_SET_CONFIGURATION_REQ;
  (void) SetConfigParams(&tConfiguration.tSetConfigReq.tDeviceParameters, usDeviceClass);
  (void) SetModulConfig(&tConfiguration.tSetConfigReq.tModuleConfig);

  if(sizeof(tConfiguration) < sizeof(ptPacket->abData))
  {
    // No Fragmentation
    ptPacket->tHeader.ulLen = sizeof(tConfiguration);
    memcpy(ptPacket->abData, &tConfiguration, sizeof(tConfiguration));
  }
  else
  {
    // Fragmentation
    // First Packet
    size_t ulIndex = 0;
    size_t ulOffset = 0;
    ptPacket->tHeader.ulLen = sizeof(ptPacket->abData);
    ptPacket->tHeader.ulCmd = PNS_IF_SET_CONFIGURATION_REQ;
    ptPacket->tHeader.ulExt &= ~(HIL_PACKET_SEQ_MASK | HIL_PACKET_SEQ_NR_MASK);
    ptPacket->tHeader.ulExt |= HIL_PACKET_SEQ_FIRST | ulIndex;
    memcpy(ptPacket->abData, &((uint8_t*) &tConfiguration)[ulOffset], ptPacket->tHeader.ulLen);

    ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

    if(ulRet != CIFX_NO_ERROR)
      return ulRet;
    ulIndex++;
    ulOffset = sizeof(ptPacket->abData) * ulIndex;
    while(ulOffset < (sizeof(tConfiguration) - sizeof(ptPacket->abData)))
    {
      // Middle Packets
      ptPacket->tHeader.ulLen = sizeof(ptPacket->abData);
      ptPacket->tHeader.ulCmd = PNS_IF_SET_CONFIGURATION_REQ;
      ptPacket->tHeader.ulExt &= ~(HIL_PACKET_SEQ_MASK | HIL_PACKET_SEQ_NR_MASK);
      ptPacket->tHeader.ulExt |= HIL_PACKET_SEQ_MIDDLE | ulIndex;
      memcpy(ptPacket->abData, &((uint8_t*) &tConfiguration)[ulOffset], ptPacket->tHeader.ulLen);

      ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
      if(ulRet != CIFX_NO_ERROR)
        return ulRet;
      ulIndex++;
      ulOffset = sizeof(ptPacket->abData) * ulIndex;
    }
    // Final Packet
    ptPacket->tHeader.ulLen = sizeof(tConfiguration) - ulOffset;
    ptPacket->tHeader.ulCmd = PNS_IF_SET_CONFIGURATION_REQ;
    ptPacket->tHeader.ulExt &= ~(HIL_PACKET_SEQ_MASK | HIL_PACKET_SEQ_NR_MASK);
    ptPacket->tHeader.ulExt |= HIL_PACKET_SEQ_LAST | ulIndex;
    memcpy(ptPacket->abData, &((uint8_t*) &tConfiguration)[ulOffset], sizeof(tConfiguration) - ulOffset);
  }

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
  if(ulRet != CIFX_NO_ERROR)
    return ulRet;

  App_SysPkt_AssembleChannelInitReq(ptPacket);
  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
  if(ulRet != CIFX_NO_ERROR)
    return ulRet;


  App_SysPkt_AssembleStartStopCommReq(ptPacket, true);
  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
  if(ulRet != CIFX_NO_ERROR)
    return ulRet;

  return ulRet;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppPNS_SetOemData(MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc)
{
  uint32_t                   ulRet = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq = (HIL_DDP_SERVICE_SET_REQ_T*)&ptRsc->tPacket;
  char*                      szSerialNumber     = "20000           ";
  char*                      szOrderNumber      = "1234.567            ";
  char*                      szHardwareRevision = "2";

  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + strlen(szSerialNumber) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_SERIALNUMBER;

  memcpy(ptReq->tData.uDataType.szString, szSerialNumber, strlen(szSerialNumber) + 1);

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  /* Check status of cifX function and confirmation packet */
  ulRet = (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
  if (CIFX_NO_ERROR != ulRet)
  {
    return ulRet;
  }

  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + strlen(szOrderNumber) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_ORDERNUMBER;

  memcpy(ptReq->tData.uDataType.szString, szOrderNumber, strlen(szOrderNumber) + 1);

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  /* Check status of cifX function and confirmation packet */
  ulRet = (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
  if (CIFX_NO_ERROR != ulRet)
  {
    return ulRet;
  }

  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + strlen(szHardwareRevision) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_HARDWAREREVISION;

  memcpy(ptReq->tData.uDataType.szString, szHardwareRevision, strlen(szHardwareRevision) + 1);

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  /* Check status of cifX function and confirmation packet */
  ulRet = (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
  if (CIFX_NO_ERROR != ulRet)
  {
    return ulRet;
  }

  /* also render the OEM serial number "valid" in the corresponding bitfield */
  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + sizeof(ptReq->tData.uDataType.ulValue);
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_OPTIONS;
  ptReq->tData.uDataType.ulValue = HIL_PRODUCT_DATA_OEM_IDENTIFICATION_FLAG_ORDERNUMBER_VALID
                                 | HIL_PRODUCT_DATA_OEM_IDENTIFICATION_FLAG_HARDWAREREVISION_VALID
                                 | HIL_PRODUCT_DATA_OEM_IDENTIFICATION_FLAG_SERIALNUMBER_VALID
                                 | HIL_PRODUCT_DATA_OEM_IDENTIFICATION_FLAG_PRODUCTIONDATA_VALID;   /* set bit zero to set OEM serial number valid */

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  /* Check status of cifX function and confirmation packet */
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppPNS_ActivateDdp(MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc)
{
  uint32_t                   ulRet = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq = (HIL_DDP_SERVICE_SET_REQ_T*)&ptRsc->tPacket;

  /* required when inital DPP state is passive: Set DDP active now */
  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + sizeof(ptReq->tData.uDataType.ulValue);
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_STATE;
  ptReq->tData.uDataType.ulValue = HIL_DDP_SERVICE_STATE_ACTIVE;

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  /* Check status of cifX function and confirmation packet */
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppPNS_SetMacAddress( MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc )
{
  uint32_t                   ulRet = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq = (HIL_DDP_SERVICE_SET_REQ_T*)&ptRsc->tPacket;

  uint8_t abMyComMacAddresses[8][6] =
  {
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x0 },   /* This is the first chassis MAC address which is used by EtherNet/IP */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x1 },   /* Port 0 MAC Address used for LLDP                                   */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x2 },   /* Port 1 MAC Address used for LLDP                                   */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x3 },   /* This is the second chassis MAC (RawEthernet/NDIS)                  */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x4 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x5 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x6 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x7 },
  };

  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd  = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen  = sizeof(ptReq->tData.ulDataType) + sizeof(abMyComMacAddresses);

  /* Set MAC adrress for the protocol stack (override pre-defined MAC address from FDL) */
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_MAC_ADDRESSES_COM;

  memcpy(ptReq->tData.uDataType.atMacAddress, abMyComMacAddresses, sizeof(abMyComMacAddresses));

  ulRet = Pkt_SendReceivePacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptRsc->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************
*   Return ParameterEndResponse with application ready.
*
*   \param ptRsc  Pointer to the communication channel handle
*
*   \return:      void
***************************************************************************************/
void AppPNS_HandleParameterEndInd(MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc) {
  PNS_IF_PARAM_END_RSP_T *ptReq = (PNS_IF_PARAM_END_RSP_T *)&ptRsc->tPacket;

  ptReq->tHead.ulCmd |= 0x01; // PNS_IF_PARAM_END_RES;
  ptReq->tHead.ulLen = 8;
  ptReq->tHead.ulSta = SUCCESS_HIL_OK;
  ptReq->tData.fSendApplicationReady = 1;

  Pkt_SendReceivePacket(ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);

  return;
}
/**************************************************************************************
**************************************************************************************/

bool AppPNS_PacketHandler( void* pvUserData, CIFX_PACKET* ptPacket )
{
  MY_PNS_COMM_CHANNEL_HANDLER_RSC_T *ptRsc  = pvUserData;
  bool        fPacketCouldBeHandled = false;


  if( ptPacket != &(ptRsc->tPacket) )
  {
    PRINTF("Unexpected packet resource received!!!" NEWLINE);
    return false;
  }

  switch (ptPacket->tHeader.ulCmd)
  {
#if 0
  case HIL_LINK_STATUS_CHANGE_IND:
     (void)App_SysPkt_HandleLinkStatusChangeInd( ptRsc, &(ptRsc->tPacket) );
     fPacketCouldBeHandled = true;
     break;
#endif

  case PNS_IF_PARAM_END_IND:
    // mandatory, if Register Application is used
    //  This function is necessary because it returns application ready = 1.
    //  If application ready = 0 will be used, the application needs to send
    //  application ready request.
    (void)AppPNS_HandleParameterEndInd(ptRsc);
    fPacketCouldBeHandled = true;
    break;
  case PNS_IF_AR_SET_STATUS_IND:
  case PNS_IF_AR_CHECK_IND:
  case PNS_IF_RECONFIG_IND:
  case PNS_IF_AR_INDATA_IND:
  case PNS_IF_AR_ABORT_IND:
  case PNS_IF_APDU_STATUS_IND:
  case PNS_IF_ALARM_IND:
  case PNS_IF_PARAM_BEGIN_IND:
  case PNS_IF_GET_ASSET_IND:
  case PNS_IF_CONNECT_REQ_DONE_IND:
  case PNS_IF_USER_ERROR_IND:
  case PNS_IF_STORE_REMANENT_DATA_IND:
  case PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND:
  case PNS_IF_EVENT_IND:

    // Standard answer for not handled indications by application
    ptRsc->tPacket.tHeader.ulCmd |= 0x01;
    ptRsc->tPacket.tHeader.ulLen = 0;
    ptRsc->tPacket.tHeader.ulState =
        0xC0000202; // TLR_E_NO_APPLICATION_REGISTERED
    (void)Pkt_SendPacket(ptRsc->hPktIfRsc, &(ptRsc->tPacket), TXRX_TIMEOUT);
    fPacketCouldBeHandled = true;
    break;

  default:
    if( (ptRsc->tPacket.tHeader.ulCmd & 0x1) == 0 ) /* received an indication*/
    {
      PRINTF("Warning: Disregarded indication packet received!" NEWLINE);
      ptRsc->tPacket.tHeader.ulLen    = 0;
      ptRsc->tPacket.tHeader.ulState  = ERR_HIL_UNKNOWN_COMMAND;
      ptRsc->tPacket.tHeader.ulCmd   |= 0x01; /* Make it a response */

      (void)Pkt_SendPacket( ptRsc->hPktIfRsc, &(ptRsc->tPacket), TX_TIMEOUT );
      fPacketCouldBeHandled  = true;
    }
    else
    { /* received a confirmation */
      PRINTF("Warning: Disregarded confirmation packet received!" NEWLINE);
    }
    break;
  } /*switch*/

  return fPacketCouldBeHandled;
}
