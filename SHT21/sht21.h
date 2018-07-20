
// SHT21 I2C address
#define SHT21_ADDR      ( 0x40 << 1 )

// Sensor commands
#define CMD_TMP_HLD   0xE3
#define CMD_HUM_HLD   0xE5
#define CMD_TMP_NOHLD 0xF3
#define CMD_HUM_NOHLD 0xF5
#define CMD_WR_REG    0xE6
#define CMD_RD_REG    0xE7
#define CMD_SOFT_RST  0xFE


uint8_t sht21Read( int16_t *temp , uint16_t *humidity );

uint8_t sht21GetSerialNumber( uint8_t numb[8] );
