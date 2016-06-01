#pragma once
class ViewPort
{
private:
     //The size is 64x32
     //4x5 pixel font (0-9, A-F)
     //drawing done is XOR
     //origin is upper left
     //8x1 to 15 sprites
     //when one or more pixels are erased while a sprite is drawn, the VF register is set to 0x01, otherwise it is set to 0x00
     
public:
     ViewPort();
     ~ViewPort();
};

