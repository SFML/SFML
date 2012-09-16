#include "XDisplay.hpp"

static Display *Dpy = NULL;
static int ref_count = 0;

Display *SXOpenDisplay() {
  if (!Dpy) {
    Dpy = XOpenDisplay(NULL);
  }
  ref_count++;
  return Dpy;
}

void SXCloseDisplay() {
  if (--ref_count == 0) {
    XCloseDisplay(Dpy);
  }
}
