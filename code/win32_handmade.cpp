/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Heikki Siikavirta $
   $Notice: (C) Copyright 2016 by Heikki Siikavirta, Inc. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>

#define local_persist static
#define global_variable static
#define internal static 

//TODO(heikki): This is globar for now.
global_variable bool Running;

internal void
Win32ResizeDIBSection( int Width, int Height)
{
    HBITMAP CreateDIBSection(
        HDC        hdc,
        const BITMAPINFO *pbmi,
        UINT       iUsage,
        VOID       **ppvBits,
        HANDLE     hSection,
        DWORD      dwOffset);


}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y,  int Width, int Height)
{
    StretchDIBits(DeviceContext,
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  const VOID   *lpBits,
                  const  BITMAPINFO *lpBitsInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);    
    
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND hwnd,
                        UINT   uMsg,
                        WPARAM wParam,
                        LPARAM lParam)
{

    LRESULT Result = 0;

    
    switch(uMsg)
    {
        

        case WM_SIZE: //When user resizes the window
        {
            RECT ClientRect
            GetClientRect(hwmd, &ClientRect );
            int Height = ClientRect.bottom - ClientRect.top;
            int Width = ClientRect.right - ClientRect.left;
            ResizeDIBSection(Width, Height);
            OutputDebugStringA("WM_SIZE\n");
        } break;

    
        case WM_DESTROY: //When windows deletes our window
        {
            OutputDebugStringA("WM_DESTROY\n");
            //TODO(heikki): Handle this as an error - recreate window? 
            Running = false;
        } break;

        case WM_CLOSE: // When user click c button
        {
            //TODO(heikki): Handle this with message to user?
            Running = false;
            
            OutputDebugStringA("WM_CLOSE\n");
            PostQuitMessage(0);
            
        } break;

        case WM_ACTIVATEAPP: // When user has clicked uss
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT:
        {
            
            PAINTSTRUCT Paint;
            HDC DeviceContext =  BeginPaint(hwnd, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            Win32UpdateWindow(DeviceContext , X, Y, Width, Height);
     
        } break;
        
        default:
        {

            Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
        
    }


    return(Result);
}



int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nCmdShow)
{
    
    WNDCLASS WindowClass = {};

    // TODO(heikki): check if HREDRAW/VREDRAW/OWNDC still matter
    WindowClass.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = hInstance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass"; //Our windows class name

     
    if(RegisterClass(&WindowClass))
    {

        HWND WindowHandle =
            CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                "Handmade Hero",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                hInstance,
                0);

        if(WindowHandle)
        {

            Running = true;
            
            while(Running) // ;; means infinite loop
            {
                MSG Message;
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
                if(MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
            
            
            
        }
        else
        {
            //TODO(heikki): Logging
        }
    }
    else
    {
        //TODO(heikki): Logging
    }

   
return(0);
}
