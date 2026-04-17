#include <reg52.h>
#include "Key.h"

sbit KEY_GAME_FORWARD = P3^5;
sbit KEY_GAME_BACK   = P3^4;
sbit KEY_GAME_TURN   = P3^3;
sbit KEY_GAME_BREAK  = P2^7;
sbit KEY_MENU_SWITCH = P2^5;
sbit KEY_MENU_SELECT = P2^4;

// ?????
unsigned char cnt_menu_switch = 0;
unsigned char cnt_menu_select = 0;
unsigned char cnt_game_forward = 0;
unsigned char cnt_game_back = 0;
unsigned char cnt_game_turn = 0;
unsigned char cnt_game_break = 0;

// ????(1=??,0=??)
unsigned char state_menu_switch = 1;
unsigned char state_menu_select = 1;
unsigned char state_game_forward = 1;
unsigned char state_game_back = 1;
unsigned char state_game_turn = 1;
unsigned char state_game_break = 1;

// ?????



void key_scan(void)
{
    // ===================== ???? =====================
    if(KEY_MENU_SWITCH != state_menu_switch)
    {
        cnt_menu_switch++;
        if(cnt_menu_switch >= 3)
        {
            state_menu_switch = KEY_MENU_SWITCH;
            if(state_menu_switch == 0)
            {
                key_menu_switch_flag = 1;
            }
            cnt_menu_switch = 0;
        }
    }
    else cnt_menu_switch = 0;

    // ===================== ???? =====================
    if(KEY_MENU_SELECT != state_menu_select)
    {
        cnt_menu_select++;
        if(cnt_menu_select >= 3)
        {
            state_menu_select = KEY_MENU_SELECT;
            if(state_menu_select == 0)
            {
                key_menu_select_flag = 1;
            }
            cnt_menu_select = 0;
        }
    }
    else cnt_menu_select = 0;

    // ===================== ?? =====================
    if(KEY_GAME_FORWARD != state_game_forward)
    {
        cnt_game_forward++;
        if(cnt_game_forward >= 3)
        {
            state_game_forward = KEY_GAME_FORWARD;
            if(state_game_forward == 0)
            {
                key_game_forward_flag = 1;
            }
            cnt_game_forward = 0;
        }
    }
    else cnt_game_forward = 0;

    // ===================== ?? =====================
    if(KEY_GAME_BACK != state_game_back)
    {
        cnt_game_back++;
        if(cnt_game_back >= 3)
        {
            state_game_back = KEY_GAME_BACK;
            if(state_game_back == 0)
            {
                key_game_back_flag = 1;
            }
            cnt_game_back = 0;
        }
    }
    else cnt_game_back = 0;

    // ===================== ?? =====================
    if(KEY_GAME_TURN != state_game_turn)
    {
        cnt_game_turn++;
        if(cnt_game_turn >= 3)
        {
            state_game_turn = KEY_GAME_TURN;
            if(state_game_turn == 0)
            {
                key_game_turn_flag = 1;
            }
            cnt_game_turn = 0;
        }
    }
    else cnt_game_turn = 0;

    // ===================== ?? =====================
    if(KEY_GAME_BREAK != state_game_break)
    {
        cnt_game_break++;
        if(cnt_game_break >= 3)
        {
            state_game_break = KEY_GAME_BREAK;
            if(state_game_break == 0)
            {
                key_game_break_flag = 1;  // ? ??!
            }
            cnt_game_break = 0;
        }
    }
    else cnt_game_break = 0;
}

MenuKeyState get_menuKey_state()
{
    if(key_menu_switch_flag)
    {
        key_menu_switch_flag = 0;
        return KEY_DOWN;
    }
    else if(key_menu_select_flag)
    {
        key_menu_select_flag = 0;
        return KEY_OK;
    }
    return NONEM;
}

GameKeyState get_gameKey_state()
{
    if(key_game_forward_flag)
    {
        key_game_forward_flag = 0;
        return KEY_FORWARD;
    }
    else if(key_game_back_flag)
    {
        key_game_back_flag = 0;
        return KEY_BACK;
    }
    else if(key_game_turn_flag)
    {
        key_game_turn_flag = 0;
        return KEY_TURN;
    }
    else if(key_game_break_flag)
    {
        key_game_break_flag = 0;
        return KEY_BREAK;
    }
    return NONEG;
}