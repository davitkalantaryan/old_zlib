
#include <decompress_common_in_versions_private.hpp>

void SwapStatic(uint16_t* a_pDataToSwap)
{
	uint8_t* pBuff = reinterpret_cast<uint8_t*>(a_pDataToSwap);
	uint8_t tmpElement = pBuff[0];
	
	pBuff[0]=pBuff[1];
	pBuff[1]=tmpElement;
}


void SwapStatic(uint32_t* a_pDataToSwap)
{
	uint8_t* pBuff = reinterpret_cast<uint8_t*>(a_pDataToSwap);
	uint8_t tmpElement = pBuff[0];
	
	pBuff[0]=pBuff[3];
	pBuff[3]=tmpElement;
	
	tmpElement = pBuff[1];
	pBuff[1]=pBuff[2];
	pBuff[2]=tmpElement;
}


void SwapStatic(uint64_t* a_pDataToSwap)
{
	uint8_t* pBuff = reinterpret_cast<uint8_t*>(a_pDataToSwap);
	uint8_t tmpElement = pBuff[0];
	
	pBuff[0]=pBuff[7];
	pBuff[7]=tmpElement;
	
	tmpElement = pBuff[1];
	pBuff[1]=pBuff[6];
	pBuff[6]=tmpElement;
	
	tmpElement = pBuff[2];
	pBuff[2]=pBuff[5];
	pBuff[5]=tmpElement;
	
	tmpElement = pBuff[3];
	pBuff[3]=pBuff[4];
	pBuff[4]=tmpElement;
}
