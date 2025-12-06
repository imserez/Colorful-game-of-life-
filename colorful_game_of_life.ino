#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

typedef struct Color 
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
} Color;

const uint8_t GRID_W = 32;
const uint8_t GRID_H = 40;
const uint8_t CELL_SIZE = 4;
const uint8_t LIFE_INIT = 30;
const uint32_t RULE = 0x1808;

Color grid[GRID_W][GRID_H];
Color next_grid[GRID_W][GRID_H];

void init_grid()
{
  for (uint8_t x = 0; x < GRID_W; x++)
  {
    for (uint8_t y = 0; y < GRID_H; y++)
    {
      if (random(100) < LIFE_INIT)
      {
        grid[x][y].R = random(256);
        grid[x][y].G = random(256);
        grid[x][y].B = random(256);
      }
      else
      {
        grid[x][y].R = 0;
        grid[x][y].G = 0;
        grid[x][y].B = 0;
      }
    }
  }
}

void render_grid()
{
  tft.startWrite();
  for (uint8_t x = 0; x < GRID_W; x++)
  {
    for (uint8_t y = 0; y < GRID_H; y++)
    {
      uint16_t c = tft.color565(grid[x][y].R, grid[x][y].G, grid[x][y].B);
      tft.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, c);
    }
  }
  tft.endWrite();
}

uint8_t get_color_bits(int offset, int x, int y)
{
  uint8_t i;
  uint8_t count;

  uint8_t dead_change = 0;
  uint8_t alive_change = 0;
  uint8_t new_color = 0;

  for (i = 0; i < 8; i++)
  {
    count = 0;

    int up_row    = (y == 0) ? GRID_H - 1 : y - 1;
    int down_row  = (y == GRID_H - 1) ? 0 : y + 1;
    int left_col  = (x == 0) ? GRID_W - 1 : x - 1;
    int right_col = (x == GRID_W - 1) ? 0 : x + 1;

    if ( (*(((uint8_t*)&grid[left_col][up_row])  + offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[x][up_row])         + offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[right_col][up_row]) + offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[right_col][y])      + offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[right_col][down_row])+offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[x][down_row])       + offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[left_col][down_row])+offset) >> i ) & 1 ) count++;
    if ( (*(((uint8_t*)&grid[left_col][y])       + offset) >> i ) & 1 ) count++;

    dead_change = (RULE >> count) & 1;
    alive_change = (RULE >> (count + 9)) & 1;

    uint8_t current_bit = ( (*(((uint8_t*)&grid[x][y]) + offset)) >> i ) & 1;

    if (current_bit == 0)
      new_color |= (dead_change << i);
    else
      new_color |= (alive_change << i);
  }

  return new_color;
}


void new_generation()
{
  for (uint8_t x = 0; x < GRID_W; x++)
  {
    for (uint8_t y = 0; y < GRID_H; y++)
    {
      next_grid[x][y].R = get_color_bits(0, x, y);
      next_grid[x][y].G = get_color_bits(1, x, y);
      next_grid[x][y].B = get_color_bits(2, x, y);
    }
  }

  memcpy(grid, next_grid, sizeof(grid));
}

void setup() 
{
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  randomSeed(analogRead(A0));
  init_grid();
}

void loop() 
{
  new_generation();
  render_grid();
  delay(100);
}
