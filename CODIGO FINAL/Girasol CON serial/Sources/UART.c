/*
 * UART.c
 *
 *  Created on: Nov 8, 2014
 *      Author: JuanPablo
 */
#include "UART.h"
/**HABILITAR FIFO READ y WRITE
 * LEERLO BIEN A VER DONDE LO ESCRIBE
 *  **/

uint8_t dataIndex = 0;
uint8_t dataFlag = 0;
uint8_t dataBuffer[30];

void UART_init(void){
	
	// Habilito la llave general de las interrupciones
	
	NVICISER0 |= NVIC_ISER_SETENA(1<<31);
	
	//Habilito los cuatro modulos de UART
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	// Habilito el pin PTC1 como salida del canal 0
	PORTB_PCR16 = (PORT_PCR_ISF_MASK |
					              PORT_PCR_MUX(0x03));	//Tx
	PORTB_PCR17 = (PORT_PCR_ISF_MASK |
					              PORT_PCR_MUX(0x03)); //Rx
	
	//Habilito por interrupcion, un bit de stop y baudrate
	UART_C1_REG(UART0_BASE_PTR) = 0x00;
	
	UART_BDH_REG(UART0_BASE_PTR) = /*UART_BDH_RXEDGIE_MASK | */UART_BDH_SBR(BAUDRATEHIGH);
	UART_BDL_REG(UART0_BASE_PTR) = UART_BDL_SBR(BAUDRATELOW);
	
	UART_C2_REG(UART0_BASE_PTR) = UART_C2_TIE_MASK | UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK;
	UART_C3_REG(UART0_BASE_PTR) = UART_C3_TXDIR_MASK;
	UART_PFIFO_REG(UART0_BASE_PTR) = UART_PFIFO_TXFE_MASK | UART_PFIFO_TXFIFOSIZE(0x06) | UART_PFIFO_RXFE_MASK | UART_PFIFO_RXFIFOSIZE(0x01);
	UART_S2_REG(UART0_BASE_PTR) = 0x00;
}

void UART_writeByte(uint8_t data){
	UART_D_REG(UART0_BASE_PTR) = data;
	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_TIE_MASK;
}

uint8_t UART_readByte(void){
	return UART_D_REG(UART0_BASE_PTR);
}

void UART_TransferComplete(){
	UART_C2_REG(UART0_BASE_PTR) &= ~UART_C2_TIE_MASK;
}

void UART_ReceptionComplete(){
	if(dataFlag == 0){
		dataBuffer[dataIndex] = UART_readByte();
		if(dataBuffer[dataIndex] == 10){
			dataFlag = 1;
			dataIndex = 0;
		} else {
			dataIndex++;
		}
			
	}
}

ISR_t UART0_Status_IRQHandler(void){
	uint8_t uartFlag = UART_S1_REG(UART0_BASE_PTR);
	UART_S1_REG(UART0_BASE_PTR) = 0x00;
	
	if(uartFlag & UART_S1_TC_MASK){
			UART_TransferComplete();
		}
	if(uartFlag & UART_S1_RDRF_MASK){
			UART_ReceptionComplete();
			
	}
}

uint8_t UART_isDataAvailable(void){
	return dataFlag;
}

void UART_readDataBuffer(uint8_t *dataStore){
	uint8_t index = 0;
	// dataBuffer[index] != 10 && 
	for(index = 0; index < 30; index++){
		dataStore[index] = dataBuffer[index];
	}
	dataFlag = 0;
}
/**
 * UARTx_BDH
RXEDGIE
	RxD Input Active Edge Interrupt Enable
	Enables the receive input active edge, RXEDGIF, to generate interrupt requests.
	0 Hardware interrupts from RXEDGIF disabled using polling.
	1 RXEDGIF interrupt request enabled.
SBNS
	Stop Bit Number Select
	SBNS selects the number of stop bits present in a data frame. This field valid for all 8, 9 and 10 bit data
	formats available. This field is not valid when C7816[ISO7816E] is enabled.
	0 Data frame consists of a single stop bit.
Baudrate
	UART module clock / (16 × (SBR[12:0] + BRFD))
* UARTx_BDL
SBR
	UART_BDH_SBR
	UART_BDL_SBR
* UARTx_C1
LOOPS
	Loop Mode Select
	When LOOPS is set, the RxD pin is disconnected from the UART and the transmitter output is internally
	connected to the receiver input. The transmitter and the receiver must be enabled to use the loop function.
	0 Normal operation.
	1 Loop mode where transmitter output is internally connected to receiver input. The receiver input is
	determined by RSRC.
UARTSWAI
	UART Stops in Wait Mode
	0 UART clock continues to run in Wait mode.
	1 UART clock freezes while CPU is in Wait mode.
RSRC
	Receiver Source Select
	This field has no meaning or effect unless the LOOPS field is set. When LOOPS is set, the RSRC field
	determines the source for the receiver shift register input.
	0 Selects internal loop back mode. The receiver input is internally connected to transmitter output.
	1 Single wire UART mode where the receiver input is connected to the transmit pin input signal.
M
	9-bit or 8-bit Mode Select
	This field must be set when C7816[ISO_7816E] is set/enabled.
	0 Normal—start + 8 data bits (MSB/LSB first as determined by MSBF) + stop.
	1 Use—start + 9 data bits (MSB/LSB first as determined by MSBF) + stop.
WAKE
	Receiver Wakeup Method Select
	Determines which condition wakes the UART:
	• Address mark in the most significant bit position of a received data character, or
	• An idle condition on the receive pin input signal.
	0 Idle line wakeup.
	1 Address mark wakeup.
ILT
	Idle Line Type Select
	Determines when the receiver starts counting logic 1s as idle character bits. The count begins either after
	a valid start bit or after the stop bit. If the count begins after the start bit, then a string of logic 1s preceding
	the stop bit can cause false recognition of an idle character. Beginning the count after the stop bit avoids
	false idle character recognition, but requires properly synchronized transmissions.
	NOTE: • In case the UART is programmed with ILT = 1, a logic of 1'b0 is automatically shifted after a
	received stop bit, therefore resetting the idle count.
	• In case the UART is programmed for IDLE line wakeup (RWU = 1 and WAKE = 0), ILT has
	no effect on when the receiver starts counting logic 1s as idle character bits. In idle line
	wakeup, an idle character is recognized at anytime the receiver sees 10, 11, or 12 1s
	depending on the M, PE, and C4[M10] fields.
	0 Idle character bit count starts after start bit.
	1 Idle character bit count starts after stop bit.
PE
	Parity Enable
	Enables the parity function. When parity is enabled, parity function inserts a parity bit in the bit position
	immediately preceding the stop bit. This field must be set when C7816[ISO_7816E] is set/enabled.
	0 Parity function disabled.
	1 Parity function enabled.
PT
	Parity Type
	Determines whether the UART generates and checks for even parity or odd parity. With even parity, an
	even number of 1s clears the parity bit and an odd number of 1s sets the parity bit. With odd parity, an odd
	number of 1s clears the parity bit and an even number of 1s sets the parity bit. This field must be cleared
	when C7816[ISO_7816E] is set/enabled.
	0 Even parity.
	1 Odd parity.
* UARTx_C2
TIE
	Transmitter Interrupt or DMA Transfer Enable.
	Enables S1[TDRE] to generate interrupt requests or DMA transfer requests, based on the state of
	C5[TDMAS].
	NOTE: If C2[TIE] and C5[TDMAS] are both set, then TCIE must be cleared, and D[D] must not be written
	unless servicing a DMA request.
	0 TDRE interrupt and DMA transfer requests disabled.
	1 TDRE interrupt or DMA transfer requests enabled
TCIE
	Transmission Complete Interrupt or DMA Transfer Enable
	Enables the transmission complete flag, S1[TC], to generate interrupt requests . or DMA transfer requests
	based on the state of C5[TCDMAS]
	NOTE: If C2[TCIE] and C5[TCDMAS] are both set, then TIE must be cleared, and D[D] must not be
	written unless servicing a DMA request.
	0 TC interrupt and DMA transfer requests disabled.
	1
	TC interrupt or DMA transfer requests enabled.
RIE
	Receiver Full Interrupt or DMA Transfer Enable
	Enables S1[RDRF] to generate interrupt requests or DMA transfer requests, based on the state of
	C5[RDMAS].
	0 RDRF interrupt and DMA transfer requests disabled.
	1 RDRF interrupt or DMA transfer requests enabled.
ILIE
	Idle Line Interrupt DMA Transfer Enable
	Enables the idle line flag, S1[IDLE], to generate interrupt requestsor DMA transfer requests based on the
	state of C5[ILDMAS].
	0 IDLE interrupt requests disabled. and DMA transfer
	1 IDLE interrupt requests enabled. or DMA transfer
TE
	Transmitter Enable
	Enables the UART transmitter. TE can be used to queue an idle preamble by clearing and then setting TE.
	When C7816[ISO_7816E] is set/enabled and C7816[TTYPE] = 1, this field is automatically cleared after
	the requested block has been transmitted. This condition is detected when TL7816[TLEN] = 0 and four
	additional characters are transmitted.
	0 Transmitter off.
	1 Transmitter on.
RE
	Receiver Enable
	Enables the UART receiver.
	0 Receiver off.
	1 Receiver on.
RWU
	Receiver Wakeup Control
	This field can be set to place the UART receiver in a standby state. RWU automatically clears when an
	RWU event occurs, that is, an IDLE event when C1[WAKE] is clear or an address match when C1[WAKE]
	is set. This field must be cleared when C7816[ISO_7816E] is set.
	NOTE: RWU must be set only with C1[WAKE] = 0 (wakeup on idle) if the channel is currently not idle.
	This can be determined by S2[RAF]. If the flag is set to wake up an IDLE event and the channel
	is already idle, it is possible that the UART will discard data. This is because the data must be
	received or a LIN break detected after an IDLE is detected before IDLE is allowed to reasserted.
	0 Normal operation.
	1 RWU enables the wakeup function and inhibits further receiver interrupt requests. Normally, hardware
	wakes the receiver by automatically clearing RWU.
SBK
	Send Break
	Toggling SBK sends one break character from the following: See Transmitting break characters for the
	number of logic 0s for the different configurations. Toggling implies clearing the SBK field before the break
	character has finished transmitting. As long as SBK is set, the transmitter continues to send complete
	break characters (10, 11, or 12 bits, or 13 or 14 bits, or 15 or 16 bits). Ensure that C2[TE] is asserted
	atleast 1 clock before assertion of this bit.
	• 10, 11, or 12 logic 0s if S2[BRK13] is cleared
	• 13 or 14 logic 0s if S2[BRK13] is set.
	• 15 or 16 logic 0s if BDH[SBNS] is set.
	This field must be cleared when C7816[ISO_7816E] is set.
	0 Normal transmitter operation.
	1 Queue break characters to be sent.
* UARTx_D
	Reads return the contents of the read-only receive data register and writes go to the write-only transmit
	data register.
* UARTx_S1

 */

//hoja 1609
/**
 * To initiate a UART transmission:
1. Configure the UART.
a. Select a baud rate. Write this value to the UART baud registers (BDH/L) to
begin the baud rate generator. Remember that the baud rate generator is disabled
when the baud rate is zero. Writing to the BDH has no effect without also
writing to BDL.
b. Write to C1 to configure word length, parity, and other configuration bits
(LOOPS, RSRC, M, WAKE, ILT, PE, and PT). Write to C4, MA1, and MA2 to
configure.
c. Enable the transmitter, interrupts, receiver, and wakeup as required, by writing to
C2 (TIE, TCIE, RIE, ILIE, TE, RE, RWU, and SBK), S2 (MSBF and BRK13),
and C3 (ORIE, NEIE, PEIE, and FEIE). A preamble or idle character is then
shifted out of the transmitter shift register.
2. Transmit procedure for each byte.
a. Monitor S1[TDRE] by reading S1 or responding to the TDRE interrupt. The
amount of free space in the transmit buffer directly using TCFIFO[TXCOUNT]
can also be monitored.
b. If the TDRE flag is set, or there is space in the transmit buffer, write the data to
be transmitted to (C3[T8]/D). A new transmission will not result until data exists
in the transmit buffer.
3. Repeat step 2 for each subsequent transmission.
 */

