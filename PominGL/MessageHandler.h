#pragma once

///处理菜单消息
void handleMenuMessage(int menuID);

///处理按键消息
void handleKeyMessage(int key);

///处理鼠标消息
void handleMouseMessage(UINT message, int x, int y, int det);

///处理绘制消息
void display();

///处理所有消息入口
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


