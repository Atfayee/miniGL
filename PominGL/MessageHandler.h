#pragma once

///����˵���Ϣ
void handleMenuMessage(int menuID);

///��������Ϣ
void handleKeyMessage(int key);

///���������Ϣ
void handleMouseMessage(UINT message, int x, int y, int det);

///���������Ϣ
void display();

///����������Ϣ���
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


