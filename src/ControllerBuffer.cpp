#include "ControllerBuffer.h"
#include "PinMappings.h"
#include "Debug.h"

// buffer to hold data being read from controller 
bool buffer[DATA_SIZE + DATA_OFFSET];

// bit resolution and offsets
int bitOffsets[32];
int bitResolution;

/** Function to send a Command to the attached N64-Controller.
 *  Must be run from RAM to defy timing differences introduced from 
 *  reading Code from ESP32's SPI Flash Chip.
*/
void IRAM_ATTR sendCommand(byte command)
{
    // the current bit to write 
    bool bit;

    // clear output buffer 
    memset(buffer,0,DATA_SIZE + DATA_OFFSET);

    // for each bit
    for (int i = 0; i < 8; i++)
    {
        // get value
        bit = (1 << (7 - i)) & command;

        // write data
        LINE_WRITE_LOW;
        delayMicroseconds((3 - 2 * bit));
        LINE_WRITE_HIGH;
        delayMicroseconds((1 + 2 * bit));
    }

    // console stop bit
    LINE_WRITE_LOW;
    delayMicroseconds(1);
    LINE_WRITE_HIGH;
    delayMicroseconds(2);

    // read returned data as fast as possible 
    for(int i = 0;i < DATA_SIZE + DATA_OFFSET;i++)
    {
        buffer[i] = digitalRead(DATA_PIN);
    }

    // plot polling process from controller if unstructed to 
    #ifdef PLOT_CONSOLE_POLLING
    for(int i = 0; i < DATA_SIZE + DATA_OFFSET;i++)
    {
        Serial.println(buffer[i]*2500);
    }
    #endif
}

/* Function to extract a controller bit from the buffer of returned data */
void getBit(bool *bit,int offset,bool *data)
{
    // sanity check offset 
    if(offset < 0) offset = 0;

    // count 
    short count = 0;
  
    // get count from offset to offset + length
    for(int i = offset + DATA_OFFSET;i < offset + bitResolution;i++)
    {
        count += *(data + i);
    }

    // if offset surpasses threshold set bit 
    *bit = false;
    if(count > BIT_THRESHOLD) *bit = true;
}

/** Function to populate the controller struct if command 0x01 was sent.
 *  Buttons are set according to data in buffer, raw axis data is written,
 *  Axis Data is correctly decoded from raw axis data by taking two's complement
 *  and checking if value if below 'MAX_INCLINE_AXIS_X' or 'MAX_INCLINE_AXIS_Y'.
 *  If values surpass the maximum incline they are set to match those values.
 */
void populateControllerStruct(ControllerData *data)
{
    // first byte 
    getBit(&(data->buttonA) ,    bitOffsets[0]  ,&buffer[0]);
    getBit(&(data->buttonB),     bitOffsets[1]  ,&buffer[0]);
    getBit(&(data->buttonZ),     bitOffsets[2]  ,&buffer[0]);
    getBit(&(data->buttonStart), bitOffsets[3]  ,&buffer[0]);
    getBit(&(data->DPadUp),      bitOffsets[4] ,&buffer[0]);
    getBit(&(data->DPadDown),    bitOffsets[5] ,&buffer[0]);
    getBit(&(data->DPadLeft),    bitOffsets[6] ,&buffer[0]);
    getBit(&(data->DPadRight),   bitOffsets[7] ,&buffer[0]);

    // second byte, first two bits are unused 
    getBit(&(data->buttonL), bitOffsets[10]  ,&buffer[0]);
    getBit(&(data->buttonR), bitOffsets[11]  ,&buffer[0]);
    getBit(&(data->CUp),     bitOffsets[12] ,&buffer[0]);
    getBit(&(data->CDown),   bitOffsets[13] ,&buffer[0]);
    getBit(&(data->CLeft),   bitOffsets[14] ,&buffer[0]);
    getBit(&(data->CRight),  bitOffsets[15] ,&buffer[0]);

    // third byte 
    getBit(&(data->xAxisRaw[0]), bitOffsets[16], &buffer[0]);
    getBit(&(data->xAxisRaw[1]), bitOffsets[17], &buffer[0]);
    getBit(&(data->xAxisRaw[2]), bitOffsets[18], &buffer[0]);
    getBit(&(data->xAxisRaw[3]), bitOffsets[19], &buffer[0]);
    getBit(&(data->xAxisRaw[4]), bitOffsets[20], &buffer[0]);
    getBit(&(data->xAxisRaw[5]), bitOffsets[21], &buffer[0]);
    getBit(&(data->xAxisRaw[6]), bitOffsets[22], &buffer[0]);
    getBit(&(data->xAxisRaw[7]), bitOffsets[23], &buffer[0]);

    // fourth byte 
    getBit(&(data->yAxisRaw[0]), bitOffsets[24], &buffer[0]);
    getBit(&(data->yAxisRaw[1]), bitOffsets[25], &buffer[0]);
    getBit(&(data->yAxisRaw[2]), bitOffsets[26], &buffer[0]);
    getBit(&(data->yAxisRaw[3]), bitOffsets[27], &buffer[0]);
    getBit(&(data->yAxisRaw[4]), bitOffsets[28], &buffer[0]);
    getBit(&(data->yAxisRaw[5]), bitOffsets[29], &buffer[0]);
    getBit(&(data->yAxisRaw[6]), bitOffsets[30], &buffer[0]);
    getBit(&(data->yAxisRaw[7]), bitOffsets[31], &buffer[0]);

    // sum up bits to get axis bytes
    data->xAxis = 0;
    data->yAxis = 0; 
    for(int i = 0;i < 8;i++)
    {
        data->xAxis += (data->xAxisRaw[i] * (0x80 >> (i)));
        data->yAxis += (data->yAxisRaw[i] * (0x80 >> (i)));

        // print y axis values 
        #ifdef PRINT_Y_AXIS_VALUES
        Serial.printf("%i %i %i %i %i %i %i %i\n",data->yAxisRaw[0],data->yAxisRaw[1],data->yAxisRaw[2],data->yAxisRaw[3],data->yAxisRaw[4],data->yAxisRaw[5],data->yAxisRaw[6],data->yAxisRaw[7]);
        Serial.printf("yAxis: %i \n",data->yAxis);
        #endif

        // print x axis values 
        #ifdef PRINT_X_AXIS_VALUES
        Serial.printf("%i %i %i %i %i %i %i %i\n",data->xAxisRaw[0],data->xAxisRaw[1],data->xAxisRaw[2],data->xAxisRaw[3],data->xAxisRaw[4],data->xAxisRaw[5],data->xAxisRaw[6],data->xAxisRaw[7]);
        Serial.printf("xAxis: %i \n",data->xAxis);
        #endif
    }

    // decode xAxis two's complement 
    if(data->xAxis & 0x80)
    {
        data->xAxis = -1 * (0xff - data->xAxis);
    }

    // decode yAxis two's complement 
    if(data->yAxis & 0x80)
    {
        data->yAxis = -1 * (0xff - data->yAxis);
    }

    // keep x axis below maxIncline 
    if(data->xAxis >  MAX_INCLINE_AXIS_X) data->xAxis =  MAX_INCLINE_AXIS_X;
    if(data->xAxis < -MAX_INCLINE_AXIS_X) data->xAxis = -MAX_INCLINE_AXIS_X;

    // keep y axis below maxIncline
    if(data->yAxis >  MAX_INCLINE_AXIS_Y) data->yAxis =  MAX_INCLINE_AXIS_Y;
    if(data->yAxis < -MAX_INCLINE_AXIS_Y) data->yAxis = -MAX_INCLINE_AXIS_Y;

    //Serial.printf("xaxis: %-3i yaxis: %-3i \n",data->xAxis,data->yAxis);
}

void updateOffsetsAndResolution()
{
    // the current bit counter 
    int bitCounter = 0;

    // to hold the offset of A Button's falling edge 
    int bitAfallingOffset = 0;

    // iterate over buffer  
    for(int i = 0;i < DATA_SIZE + DATA_OFFSET - 1;i++)
    {
        // if a falling edge is detected 
        if(buffer[i] == true && buffer[1+i] == false)
        {
            // store bit's end offset 
            bitOffsets[bitCounter] = i+1;

            // if it's the A button store offset of the falling edge 
            if(bitCounter == 0) bitAfallingOffset = i+1;

            // if it's the B button calculate the bit Resolution 
            if(bitCounter == 1) bitResolution = (i+1) - bitAfallingOffset;

            // increment bit counter 
            bitCounter++;
        }
    }

    Serial.printf("Bit resolution is %i \n",bitResolution);

    // calculate bit's beginning offsets by subtracting resolution
    for(int i = 0;i < 32;i++)
    {
        bitOffsets[i] -= bitResolution;
        Serial.printf("beginning of bit %i detected @ begin+%i \n",i+1,bitOffsets[i]);
    }

    
}