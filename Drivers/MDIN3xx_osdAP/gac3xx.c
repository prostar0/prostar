//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2010  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	GAC3xx.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

//--------------------------------------------------------------------------------------------------------------------------
// You must make drive functions for I2C read & I2C write.
//--------------------------------------------------------------------------------------------------------------------------
// static BYTE MDINI2C_Write(BYTE nID, WORD rAddr, PBYTE pBuff, WORD bytes)
// static BYTE MDINI2C_Read(BYTE nID, WORD rAddr, PBYTE pBuff, WORD bytes)

//--------------------------------------------------------------------------------------------------------------------------
// If you use MDIN380, you must make drive functions for BUS read & BUS write.
//--------------------------------------------------------------------------------------------------------------------------
// static BYTE MDINBUS_Write(BYTE nID, WORD rAddr, PBYTE pBuff, WORD bytes)
// static BYTE MDINBUS_Read(BYTE nID, WORD rAddr, PBYTE pBuff, WORD bytes)

//--------------------------------------------------------------------------------------------------------------------------
// If you use DMA-function, you must make drive functions for DMA read & DMA write.
//--------------------------------------------------------------------------------------------------------------------------
// MDIN_ERROR_t MDINBUS_DMAWrite(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes)
// MDIN_ERROR_t MDINBUS_DMARead(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes)

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"mdin3xx.h"
#include	"mdintrue.h"

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static SPRITE_CTL_INFO stOSD[12];
static LAYER_CTL_INFO stLayer[3];

static CHAR Hello[]		= "      Hello World!      ";
static CHAR MacroIT[]	= " Macro Image Technology ";
static BYTE i, pSTR[32];

static GACMAP_FONT_INFO stFONT;		// for GAC-FONT
static GACCOPY_BMP_INFO stCOPY;		// for GAC-COPY
static GACFILL_BOX_INFO stFILL;		// for GAC-FILL

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void CreateMDIN3xxGACFontInstance(void)
{
	DWORD e_addr = (DWORD)MDIN3xx_GetSizeOfBank()*2*8192;			// SDRAM end address
	DWORD b_addr = (DWORD)e_addr - sizeof(mdin3xx_font_cnv_bmp);	// fontBMP address [e_addr-BMP size]
	DWORD s_addr = (DWORD)b_addr - (DWORD)(12*32)*(16*2)/2;			// sprite address [b_addr-SPRITE size]

	// load font bitmap data
	MDINOSD_SetBitmapData(b_addr, sizeof(mdin3xx_font_cnv_bmp), (PBYTE)mdin3xx_font_cnv_bmp);

	// set sprite0 in main-OSD display => SPRITE_INDEX0
	memset(&stOSD[SPRITE_INDEX0], 0, sizeof(SPRITE_CTL_INFO));	// clear stOSD[SPRITE_INDEX0]
	stOSD[SPRITE_INDEX0].ctrl			= 0;				// font palette addr = 0
	stOSD[SPRITE_INDEX0].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX0].x				= 104;
	stOSD[SPRITE_INDEX0].y				= 60;
	stOSD[SPRITE_INDEX0].stBMP.w		= 12*32;			// stFONT.w * 32-column
	stOSD[SPRITE_INDEX0].stBMP.h		= 16*2;				// stFONT.h * 2-line
	stOSD[SPRITE_INDEX0].stBMP.addr		= s_addr;			//
	MDINOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX0], SPRITE_INDEX0);

	// set sprite0 in aux-OSD display => SPRITE_INDEX8
	memset(&stOSD[SPRITE_INDEX8], 0, sizeof(SPRITE_CTL_INFO));	// clear stOSD[SPRITE_INDEX8]
	stOSD[SPRITE_INDEX8].ctrl			= 0;				// font palette addr = 0
	stOSD[SPRITE_INDEX8].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX8].x				= 104;
	stOSD[SPRITE_INDEX8].y				= 60;
	stOSD[SPRITE_INDEX8].stBMP.w		= 12*32;			// stFONT.w * 32-column
	stOSD[SPRITE_INDEX8].stBMP.h		= 16*2;				// stFONT.h * 2-line
	stOSD[SPRITE_INDEX8].stBMP.addr		= s_addr;			// 
	MDINOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX8], SPRITE_INDEX8);

	// set palette in main-OSD display => LAYER_INDEX0
	memset(&stLayer[LAYER_INDEX0], 0, sizeof(LAYER_CTL_INFO));	// clear stLayer[LAYER_INDEX0]
	MDINOSD_SetLayerConfig(&stLayer[LAYER_INDEX0], LAYER_INDEX0);
	MDINOSD_SetLayerPalette(&stLayer[LAYER_INDEX0], 0, sizeof(mdin3xx_font_rgb_pal), (PBYTE)mdin3xx_font_rgb_pal);

	// set palette in aux-OSD display => LAYER_INDEX2
	memset(&stLayer[LAYER_INDEX2], 0, sizeof(LAYER_CTL_INFO));	// clear stLayer[LAYER_INDEX2]
	MDINOSD_SetLayerConfig(&stLayer[LAYER_INDEX2], LAYER_INDEX2);
	MDINOSD_SetLayerPalette(&stLayer[LAYER_INDEX2], 0, sizeof(mdinaux_font_rgb_pal), (PBYTE)mdinaux_font_rgb_pal);

	// set GAC charater mode with sprite0 & sprite8
	stFONT.mode		= MDIN_GAC_DRAW_CHMODE;
	stFONT.attb		= GACMAP_1BITPALT_COLOR | MDIN_GAC_16Vx32H_GRID;
	stFONT.w		= 12;			// size of 1-fontBMP
	stFONT.h		= 16;
	stFONT.addr		= b_addr;		// fontBMP address
	MDINGAC_SetFontMode(&stFONT, &stOSD[SPRITE_INDEX0]);
	MDINGAC_SetFontMode(&stFONT, &stOSD[SPRITE_INDEX8]);

	// display "Hello World!" (background: blue, font: white)
	for (i=0; i<strlen(Hello); i++) pSTR[i] = Hello[i]-0x20;	// ascii offset
	MDINGAC_SetDrawCHMode(0, 0, pSTR, strlen(Hello), 0x71);

	// clear remainder of "Hello World!"
	for (i=0; i<32-strlen(Hello); i++) pSTR[i] = 0x00;	// blank font & transparency attb
	MDINGAC_SetDrawCHMode(0, strlen(Hello), pSTR, 32-strlen(Hello), 0x00);

	// display "Macro Image Technology" (background: blue, font: white)
	for (i=0; i<strlen(MacroIT); i++) pSTR[i] = MacroIT[i]-0x20;	// ascii offset
	MDINGAC_SetDrawCHMode(1, 0, pSTR, strlen(MacroIT), 0x71);

	// clear remainder of "Macro Image Technology"
	for (i=0; i<32-strlen(MacroIT); i++) pSTR[i] = 0x00;	// blank font & transparency attb
	MDINGAC_SetDrawCHMode(1, strlen(MacroIT), pSTR, 32-strlen(MacroIT), 0x00);

	// enable sprite0 & sprite8
	MDINOSD_EnableSprite(&stOSD[SPRITE_INDEX0], ON);	// main-OSD display ON
	MDINOSD_EnableSprite(&stOSD[SPRITE_INDEX8], ON);	// aux-OSD display ON
}

//--------------------------------------------------------------------------------------------------------------------------
static void CreateMDIN3xxGACCopyInstance(void)
{
//	Suppose srcBMP is 144x128 24bit-color, dstBMP is 1280x960 24bit-color.
//	Suppose the start address of srcBMP data is mdin3xx_true_cnv_bmp.

	MDIN_AREA_BOX stSRC, stDST;

	DWORD e_addr = MDIN3xx_GetSizeOfBank()*2*8192;			// SDRAM end address
	DWORD b_addr = e_addr - sizeof(mdin3xx_true_cnv_bmp);	// srcBMP address [e_addr-BMP size]
	DWORD s_addr = b_addr - 1280*960*3;						// sprite address [b_addr-SPRITE size]

	// load source bitmap data
	MDINOSD_SetBitmapData(b_addr, sizeof(mdin3xx_true_cnv_bmp), (PBYTE)mdin3xx_true_cnv_bmp);

	// set sprite3 in dspBMP-OSD display => SPRITE_INDEX3
	memset(&stOSD[SPRITE_INDEX3], 0, sizeof(SPRITE_CTL_INFO));	// clear stOSD[SPRITE_INDEX3]
	stOSD[SPRITE_INDEX3].ctrl			= SPRITE_aRGB0888_24BIT;
	stOSD[SPRITE_INDEX3].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX3].x				= 100;
	stOSD[SPRITE_INDEX3].y				= 60;
	stOSD[SPRITE_INDEX3].stBMP.w		= 1280;			// width of dstBMP
	stOSD[SPRITE_INDEX3].stBMP.h		= 960;			// height of dstBMP
	stOSD[SPRITE_INDEX3].stBMP.addr		= s_addr;		// address of dstBMP 
	MDINOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX3], SPRITE_INDEX3);

	// set GAC copy mode with sprite3
	stCOPY.attb							= GACMAP_24BITFULL_COLOR;
	stCOPY.w							= 144;			// width of srcBMP
	stCOPY.h							= 128;			// height of srcBMP
	stCOPY.addr							= b_addr;		// address of srcBMP
	MDINGAC_SetCopyMode(&stCOPY, &stOSD[SPRITE_INDEX3]);

	// copy operation - srcBMP(0,0,144,128) to dstBMP(0,0,144,128) 
	stSRC.x =   0; stSRC.y =   0; stSRC.w = 144; stSRC.h = 128;
	stDST.x =   0; stDST.y =   0; stDST.w = 144; stDST.h = 128;
	MDINGAC_CopyRectangle(&stSRC, &stDST);

	// copy operation - srcBMP(0,0,144,128) to dstBMP(480,0,144,128) 
	stSRC.x =   0; stSRC.y =   0; stSRC.w = 144; stSRC.h = 128;
	stDST.x = 480; stDST.y =   0; stDST.w = 144; stDST.h = 128;
	MDINGAC_CopyRectangle(&stSRC, &stDST);

	// enable sprite3
	MDINOSD_EnableSprite(&stOSD[SPRITE_INDEX3], ON);	// dstBMP-OSD display ON
}

//--------------------------------------------------------------------------------------------------------------------------
static void CreateMDIN3xxGACFillInstance(void)
{
//	Suppose dstBMP(sprite) is 480x512 aRGB0565 16bit-color.

	DWORD e_addr = MDIN3xx_GetSizeOfBank()*2*8192;			// SDRAM end address
	DWORD s_addr = e_addr - 480*512*2;						// sprite address [e_addr-SPRITE size]

	// set sprite4 in dspBMP-OSD display => SPRITE_INDEX4
	memset(&stOSD[SPRITE_INDEX4], 0, sizeof(SPRITE_CTL_INFO));	// clear stOSD[SPRITE_INDEX4]
	stOSD[SPRITE_INDEX4].ctrl			= SPRITE_aRGB0565_16BIT;
	stOSD[SPRITE_INDEX4].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX4].x				= 100;
	stOSD[SPRITE_INDEX4].y				= 60;
	stOSD[SPRITE_INDEX4].stBMP.w		= 480;			// width of dstBMP
	stOSD[SPRITE_INDEX4].stBMP.h		= 512;			// height of dstBMP
	stOSD[SPRITE_INDEX4].stBMP.addr		= s_addr;		// address of dstBMP 
	MDINOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX4], SPRITE_INDEX4);

	// set GAC fill mode with sprite4
	memset(&stFILL, 0, sizeof(GACFILL_BOX_INFO));		// clear stFILL
	stFILL.attb		= GACMAP_16BITFULL_COLOR;
	MDINGAC_SetFillMode(&stFILL, &stOSD[SPRITE_INDEX4]);

	// fill operation - dstBMP(40,40,128,128), colorRGB(0,128,128)
	stFILL.x =  40; stFILL.y =  40; stFILL.w = 128; stFILL.h = 128;
	MDINGAC_FillRectangle(&stFILL, aRGB(0,0,128,128));

	// fill operation - dstBMP(100,200,128,128), colorRGB(255,0,0)
	stFILL.x = 100; stFILL.y = 200; stFILL.w = 128; stFILL.h = 128;
	MDINGAC_FillRectangle(&stFILL, aRGB(0,255,0,0));

	// enable sprite4
	MDINOSD_EnableSprite(&stOSD[SPRITE_INDEX4], ON);	// dstBMP-OSD display ON
}

/*  FILE_END_HERE */
