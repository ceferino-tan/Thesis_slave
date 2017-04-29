#include <PROTOCOL_TIMER.h>
#include <inttypes.h>
#include <RX_REC.h>
#include <TX_SEND.h>

// Defining RTS Parameters
#define RTS_length 0x04
#define RTS_max 0x03

// Defining Packet Paramaters
#define packet_length 0x09
#define packet_max 0x02
#define packet_ack_counter_max 0x03

int state=1;      
int wait_cts_status=0;
//PROTOCOL_TIMER timer;
TX_SEND tx;
RX_REC rx;
uint8_t modbus_address = 0x01;
uint8_t function_code = 0x02;
uint8_t modbus_data = 0x03;
uint16_t crc_byte = 0xF0;
uint8_t RTS_current = 1;
uint8_t packet_ack_counter = 1;
uint8_t packet_current;

uint8_t cts_timer;
uint8_t cts_received;

unsigned long packet_ack_timer;
uint8_t ack_received;

//Kevin Added these:
uint8_t cts_length;
uint8_t rts_current = 1;
uint8_t packet_received;
int wait_rts_status=0;

void setup() {
// put your setup code here, to run once:
//volatile unsigned long z;

Serial.begin(9600);
}

void loop(){

switch (state)
{
case 1:        //listen
Serial.println("Initializing Slave-side Transceiver");

RTS_current = 0;
wait_cts_status = 0;
state++;
break;

case 2: //wait_rts
Serial.println("Awaiting RTS");
rx.rx_rec_RTS();
//int rts_received = rx.rx_rec_RTS();
//wait_rts_status, 0-reset,1-timeout or not received,2-received
if(wait_rts_status == 1)
{
Serial.println("RTS timeout, we keep waiting.");
state = 1;
}
else if(wait_cts_status == 2)
{
Serial.println("RTS Received, Ready to send CTS.");
state = 3;
}
else{
Serial.println("We keep waiting for RTS.");
state = 1;
}
break;

case 3: //send CTS
//tx.tx_send_CTS(uint8_t cts_length);
tx.tx_send_CTS(cts_length);
state=4;
break;

case 4: //wait packet

rx.rx_rec_Packet();

break;

case 5: //send packetAck
tx.tx_send_PacketAck(packet_received);
//uint8_t current_ACK = tx.tx_send_PacketAck(uint8_t packet_received);
/*
if(current_ACK == 1){
state = 4;
}else{
state = 6;
}*/
break;

case 6: 
Serial.println("Message Successfully Received!");
break;

case 7: //response_fully_received
Serial.println("Fail Handling Case");
wait_cts_status == 0;
RTS_current == 0;
state = 1;
break;

default:
state = 1;
break;
}
}
