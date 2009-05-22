/*
 *  A 'window' is simply a canvas that can receive events.
 *  It can be anything from a simple message box to the
 *	game screen when playing.
 *
 *  See event.c for event handling code.
 *
 *	-kreator 2009-05-06
 */

#include "gr.h"
#include "window.h"
#include "u_mem.h"
#include "error.h"

struct window
{
	grs_canvas w_canv;					// the window's canvas to draw to
	int (*w_callback)(window *wind, d_event *event, void *data);	// the event handler
	int w_visible;						// whether it's visible
	void *data;							// whatever the user wants (eg menu data for 'newmenu' menus)
	struct window *prev;				// the previous window in the doubly linked list
	struct window *next;				// the next window in the doubly linked list
};

static window *FrontWindow = NULL;
static window *FirstWindow = NULL;

window *window_create(grs_canvas *src, int x, int y, int w, int h, int (*event_callback)(window *wind, d_event *event, void *data), void *data)
{
	window *wind;
	
	wind = d_malloc(sizeof(window));
	if (wind == NULL)
		return NULL;

	Assert(src != NULL);
	Assert(event_callback != NULL);
	gr_init_sub_canvas(&wind->w_canv, src, x, y, w, h);
	wind->w_callback = event_callback;
	wind->w_visible = 1;	// default to visible
	wind->data = data;

	if (FirstWindow == NULL)
		FirstWindow = wind;
	wind->prev = FrontWindow;
	FrontWindow->next = wind;
	wind->next = NULL;
	FrontWindow = wind;

	return wind;
}

void window_close(window *wind)
{
	if (wind == FrontWindow)
		FrontWindow = wind->prev;
	if (wind == FirstWindow)
		FirstWindow = wind->next;
	if (wind->next)
		wind->next->prev = wind->prev;
	if (wind->prev)
		wind->prev->next = wind->next;

	d_free(wind);
}

// Get the top window that's visible
window *window_get_front(void)
{
	window *wind;

	for (wind = FrontWindow; (wind != NULL) && !wind->w_visible; wind = wind->prev) {}

	return wind;
}

window *window_get_first(void)
{
	return FirstWindow;
}

window *window_get_next(window *wind)
{
	return wind->next;
}

// Make wind the front window
void window_select(window *wind)
{
	Assert (wind != NULL);

	if (wind == FrontWindow)
		return;
	if ((wind == FirstWindow) && FirstWindow->next)
		FirstWindow = FirstWindow->next;

	if (wind->next)
		wind->next->prev = wind->prev;
	if (wind->prev)
		wind->prev->next = wind->next;
	wind->prev = FrontWindow;
	wind->next = NULL;
	FrontWindow = wind;
}

void window_set_visible(window *wind, int visible)
{
	wind->w_visible = visible;
}

int window_is_visible(window *wind)
{
	return wind->w_visible;
}

grs_canvas *window_get_canvas(window *wind)
{
	return &wind->w_canv;
}

int window_send_event(window *wind, d_event *event)
{
	return wind->w_callback(wind, event, wind->data);
}
