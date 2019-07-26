#ifndef JOYPAD_H
#define JOYPAD_H


extern unsigned char joypad_dir; // direction
extern unsigned char joypad_btn; // button

extern void toggle_dir_up(unsigned char status);
extern void toggle_dir_down(unsigned char status);
extern void toggle_dir_left(unsigned char status);
extern void toggle_dir_right(unsigned char status);
extern void toggle_btn_select(unsigned char status);
extern void toggle_btn_start(unsigned char status);
extern void toggle_btn_B(unsigned char status);
extern void toggle_btn_A(unsigned char status);

#endif