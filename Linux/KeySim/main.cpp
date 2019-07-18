#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <thread>
#include <chrono>

XKeyEvent createKeyEvent(Display *display, Window win, Window winRoot,
						 int press, int keycode, int modifiers) {
	XKeyEvent event;

	event.display = display;
	event.window = win;
	event.root = winRoot;
	event.subwindow = None;
	event.time = CurrentTime;
	event.x = 1;
	event.y = 1;
	event.x_root = 1;
	event.y_root = 1;
	event.same_screen = True;
	event.keycode = XKeysymToKeycode(display, keycode);
	char *str = XKeysymToString(keycode);
	qDebug() << str;
	event.state = modifiers;

	if (press)
		event.type = KeyPress;
	else
		event.type = KeyRelease;

	return event;
}


int main() {
	int argc = 0;
	QCoreApplication aa(argc, nullptr);
	Display *disp;
	Window win, root;
	GC gc;
	int scr;
	// Atom WM_DELETE_WINDOW;
	XEvent ev;
	// XEvent ev2;
	// KeySym keysym;
	// int loop;

	/* open connection with the server */
	disp = XOpenDisplay(nullptr);
	if (disp == nullptr) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	scr = XDefaultScreen(disp);

	root = RootWindow(disp, scr);

	Atom a = XInternAtom(disp, "_NET_ACTIVE_WINDOW", true);

	qDebug() << a;


	auto l = [&](int code) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		int hz;
		Window top;
		qDebug() << "triggered " << code;
		XGetInputFocus(disp, &top, &hz);
		auto event = createKeyEvent(disp, top, root, 1, code, 0);
		XSendEvent(event.display, event.window, True, KeyPressMask,
				   (XEvent *)&event);
		// XFlush(disp);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		event = createKeyEvent(disp, top, root, 0, code, XK_Meta_L);
		XSendEvent(event.display, event.window, True, KeyPressMask,
				   (XEvent *)&event);
		XFlush(disp);

		XTestFakeKeyEvent(disp, event.keycode, True, 0);
		XTestFakeKeyEvent(disp, event.keycode, True, 1);

		XFlush(disp);
	};

	QThread worker;
	QTimer timer;
	// qDebug() << QObject::connect(&timer, &QTimer::timeout, l, 5);
	timer.setSingleShot(false);
	timer.start(500);

	// QObject::connect(&worker, SIGNAL(started()), &timer, SLOT(start()));

	// timer.setInterval(1000);
	// timer.moveToThread(&worker);

	worker.start();

	auto ww =
		XCreateSimpleWindow(disp, root,
							/* x, y, width, height, border_width */
							10, 10, 300, 200, 1,
							/* border, background */
							XBlackPixel(disp, scr), XWhitePixel(disp, scr));
	XStoreName(disp, ww, "Hello");
	/* map (show) the window */
	XMapWindow(disp, ww);
	XFlush(disp);
	auto WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(disp, ww, &WM_DELETE_WINDOW, 1);

	std::async(std::launch::async, l, 269025043);

	std::async(std::launch::async, l, 269025043);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	XTestFakeButtonEvent(disp, 1, True, CurrentTime);
	XTestFakeButtonEvent(disp, 1, False, CurrentTime);
	XFlush(disp);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	for (int i = 269025030; i < 269025090; i++)
		std::async(std::launch::async, l, i);

	bool loop = 1;
	while (loop) {
		XNextEvent(disp, &ev);
		switch (ev.type) {
		case Expose:
			/* draw or redraw the window */
			{
				char msg1[] = "Clic in the window to generate";
				char msg2[] = "a key press event";
				XDrawString(disp, ww, gc, 10, 20, msg1, sizeof(msg1) - 1);
				XDrawString(disp, ww, gc, 10, 35, msg2, sizeof(msg2) - 1);
			}
			break;

		case ClientMessage:
			/* delete window event */
			if (ev.xclient.data.l[0] == WM_DELETE_WINDOW)
				loop = 0;
			break;
		}
	}
	XDestroyWindow(disp, ww);
	/* close connection to server */
	XCloseDisplay(disp);
	// return 1973;
	return aa.exec();
}
