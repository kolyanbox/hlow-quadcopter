/*****************************************************************************
 *      U S B  -  C O X - C O M P O N E N T
 ****************************************************************************/
/**
 * @file	: cox_usb.h
 * @brief	: COX USB Peripheral Interface
 * @version	: 1.1
 * @date	: 3. Mar. 2010
 * @author	: CooCox
 ****************************************************************************/

#ifndef __COX_USB_H
#define __COX_USB_H

#include "cox_types.h"



#define USB_EVT_SETUP       1   /* Setup Packet                             */
#define USB_EVT_OUT         2   /* OUT Packet                               */
#define USB_EVT_IN          3   /*  IN Packet                               */
#define USB_EVT_OUT_NAK     4   /* OUT Packet - Not Acknowledged            */
#define USB_EVT_IN_NAK      5   /*  IN Packet - Not Acknowledged            */
#define USB_EVT_OUT_STALL   6   /* OUT Packet - Stalled                     */
#define USB_EVT_IN_STALL    7   /*  IN Packet - Stalled                     */

/**
 * @brief USB Standard Endpoint Descriptor
 */
typedef struct _USB_ENDPOINT_DESCRIPTOR {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
} __attribute__((packed)) USB_ENDPOINT_DESCRIPTOR;

/** RTC event call-back function definition */
typedef void (*USB_Event_Handler)(void *,uint32_t Event);

/**
 * @brief COX USB Peripheral Interface Definition
 */
typedef struct {
	COX_Status (*USBIOClkConfig)  (void);
	COX_Status (*USB_Init)        (void);
	COX_Status (*USB_Connect)     (uint32_t con);
	COX_Status (*USB_Reset)       (void);
	COX_Status (*USB_Suspend)     (void);
	COX_Status (*USB_Resume)      (void);
	COX_Status (*USB_WakeUp)      (void);
	COX_Status (*USB_WakeUpCfg)   (uint32_t cfg);
	COX_Status (*USB_SetAddress)  (uint32_t adr);
	COX_Status (*USB_Configure)   (uint32_t cfg);
	COX_Status (*USB_ConfigEP)    (USB_ENDPOINT_DESCRIPTOR *cfg);
	COX_Status (*USB_DirCtrlEP)   (uint32_t dir);
	COX_Status (*USB_EnableEP)    (uint32_t EPNum);
	COX_Status (*USB_DisableEP)   (uint32_t EPNum);
	COX_Status (*USB_ResetEP)     (uint32_t EPNum);
	COX_Status (*USB_SetStallEP)  (uint32_t EPNum);
	COX_Status (*USB_ClrStallEP)  (uint32_t EPNum);
	COX_Status (*USB_ControlOutEn)(void);
	COX_Status (*USB_ClearEPBuf)  (uint32_t EPNum);
	uint32_t   (*USB_ReadEP)      (uint32_t EPNum, uint8_t *pData);
	uint32_t   (*USB_WriteEP)     (uint32_t EPNum, uint8_t *pData, uint32_t cnt);
	uint32_t   (*USB_GetFrame)    (void);
	COX_Status (*USB_Event)       (uint8_t event, USB_Event_Handler handler, void *data);
} COX_USB_PI_Def;

typedef const COX_USB_PI_Def COX_USB_PI;


#endif  /* __COX_USB_H */
