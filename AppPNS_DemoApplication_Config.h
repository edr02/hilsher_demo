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


/*************************************************************************************
 Note: All features of the Profibus Host Example Project can enable in this central
       configuration file. Per default most of the are disabled.
*************************************************************************************/

#ifndef PNS_DEMOAPPLICATION_CONFIG_H_
#define PNS_DEMOAPPLICATION_CONFIG_H_


/*************************************************************************************
 Enable one of the following macro shows the different use cases for IP address
 configuration.
*************************************************************************************/


/*************************************************************************************
 Set in Tag List the DDP to Passive. Enable this macro will send OEM Data like
 Serial Number, Order Number and Hardware Revison the firmware and set DDP to Active
**************************************************************************************/
//#define HOST_APPLICATION_SETS_OEM_DATA

/*************************************************************************************
 Set in Tag List the DDP to Passive. Enable this macro will send MAC_ADDRESS
 and set DDP to Active
**************************************************************************************/
//#define HOST_APPLICATION_SETS_MAC_ADDRESS


#endif /* PNS_DEMOAPPLICATION_CONFIG_H_ */
