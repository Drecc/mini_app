#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>

#include "base/logging.h"
#include "base/command_line.h"

#include "ui/gfx/x/x11.h"
#include "ui/gfx/x/x11_switches.h"
#include "ui/gfx/x/x11_types.h"
#include "ui/gfx/x/x11_connection.h"
#include "ui/base/x/x11_util_internal.h"

int main(int argc, char** argv)
{
    base::CommandLine::Init(argc, argv);
    base::CommandLine::ForCurrentProcess()->AppendSwitch(switches::kX11Display);
    XDisplay *display;
    Window window;
    XEvent event;

    if(!gfx::InitializeThreadedX11()) {
        LOG(ERROR) << "get display error!";
        return 0;
    }

    display = gfx::GetXDisplay();

    XWindowAttributes rootWindowAttributes;
    if (!XGetWindowAttributes(display, DefaultRootWindow(display), &rootWindowAttributes)) {
        LOG(ERROR) << "Error: XGetWindowAttributes failed!";
        return 0;
    }


    // XVisualInfo info;
    // if (!XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &info)) {
    //     LOG(ERROR) << "Error: XMatchVisualInfo failed!";
    //     return 0;
    // }



    unsigned long attribute_mask = CWBorderPixel | CWBackPixel;
    XSetWindowAttributes swa;
    memset(&swa, 0, sizeof(swa));
    swa.background_pixmap = x11::None;
    // swa.bit_gravity = NorthWestGravity;
    swa.border_pixel = 0;
    swa.background_pixel = WhitePixel(display, DefaultScreen(display));

    Visual* visual = CopyFromParent;
    int depth = CopyFromParent;
    VisualID visual_id_ = 0;
    Colormap colormap = CopyFromParent;
    bool visual_has_alpha_ = false;
    ui::XVisualManager* visual_manager = ui::XVisualManager::GetInstance();
    if (!visual_manager->GetVisualInfo(visual_id_, &visual, &depth, &colormap,
                                        &visual_has_alpha_)) {
        visual_manager->ChooseVisualForWindow(true, &visual,
                                            &depth, &colormap,
                                            &visual_has_alpha_);
        LOG(ERROR) << "GET";
    }

    attribute_mask |= CWColormap;
    swa.colormap = colormap;

    LOG(ERROR) << "visual_has_alpha_ " << visual_has_alpha_;

    window = XCreateWindow(display, DefaultRootWindow(display), 0, 0, rootWindowAttributes.width, rootWindowAttributes.height, 0,
                            depth, InputOutput, visual, attribute_mask, &swa);
    // window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, rootWindowAttributes.width, rootWindowAttributes.height, 0,
    //                         0, WhitePixel(display, DefaultScreen(display)));

    // XserverRegion region = XFixesCreateRegion(display, 0, 0);
    // XFixesSetWindowShapeRegion(display, window, ShapeBounding, 0, 0, 0);
    // XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
    // XFixesDestroyRegion(display, region);

    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | Button1MotionMask);

    XMapWindow(display, window);

    LOG(INFO) << "start!!";
    int x = 0;
    int y = 0;
    bool is_pressed = false;

    GC gc;
    XGCValues values;
    values.line_style = LineSolid;
    values.line_width = 5;
    // values.background = 5656;
    unsigned long valuemask = GCLineWidth | GCLineStyle;
    gc = XCreateGC(display, window, valuemask, &values);

    for (;;)
    {
        XNextEvent(display, &event);
 
        if (event.type == Expose) {

        }

        if(event.type == ButtonPress) {
            is_pressed = true;
            x = event.xbutton.x;
            y = event.xbutton.y;
        }

        if(event.type == ButtonRelease) {
            is_pressed = false;
        }

        if(event.type == MotionNotify) {
            // LOG(INFO) << "draw line";
            if(is_pressed) {
                int temp_x = event.xmotion.x;
                int temp_y = event.xmotion.y;
                Window the_win = event.xbutton.window;
                XDrawLine(display, the_win, gc, x, y, temp_x, temp_y);
                x = temp_x;
                y = temp_y;
            }
        }

        if (event.type == KeyPress)
            break;
    }


    XCloseDisplay(display);


    return 0;

}

