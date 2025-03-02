
unsigned int LCD_BGR2RGB(unsigned int c);
void Gui_Circle(unsigned int X,unsigned int Y,unsigned int R,unsigned int fc); 
void Gui_DrawLine(unsigned int x0, unsigned int y0,unsigned int x1, unsigned int y1,unsigned int Color);  
void Gui_box(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned int bc);
void Gui_box2(unsigned int x,unsigned int y,unsigned int w,unsigned int h, unsigned char mode);
void DisplayButtonDown(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void DisplayButtonUp(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Gui_DrawFont_GBK16(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, char *s);
void Gui_DrawFont_GBK24(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned char *s);
void Gui_DrawFont_Num32(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num) ;



