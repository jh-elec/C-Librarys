
void TSL2561_init(int address);
int read_tsl2561_adc_raw (void);
int get_real_brightness_raw (int address);
int get_tsl2561_vis_lux (int address)
int get_tsl2561_vis_lux (int address);

#define TSL2561_Addr_GND 0x52;
#define TSL2561_Addr_OPEN 0x72;
#define TSL2561_Addr_VCC 0x92;

enum //adc channels
{
	adc_channel_ir_vis = 0,
	adc_channel_ir
};