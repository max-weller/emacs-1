/* Declarations useful when processing input.
   Copyright (C) 1985, 1986, 1987, 1992 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Total number of times read_char has returned.  */
extern int num_input_chars;

/* Total number of times read_char has returned, outside of macros.  */
extern int num_nonmacro_input_chars;

/* Nonzero means polling for input is temporarily suppresed.  */
extern int poll_suppress_count;

/* Keymap mapping ASCII function key sequences onto their preferred forms.
   Initialized by the terminal-specific lisp files.  */
extern Lisp_Object Vfunction_key_map;


/* Macros for dealing with lispy events.  */

/* True iff EVENT has data fields describing it (i.e. a mouse click).  */
#define EVENT_HAS_PARAMETERS(event) \
  (XTYPE (event) == Lisp_Cons)

/* Return the head of an event.  This works on composite and simple events.  */
#define EVENT_HEAD(event) \
  (EVENT_HAS_PARAMETERS (event) ? XCONS (event)->car : (event))

/* Return the starting and ending locations of a composite event.  */
#define EVENT_START(event) (XCONS (XCONS (event)->cdr)->car)
#define EVENT_END(event) (XCONS (XCONS (XCONS (event)->cdr)->cdr)->car)

/* Return the fields of a position.  */
#define POSN_WINDOW(posn) (XCONS (posn)->car)
#define POSN_BUFFER_POSN(posn) (XCONS (XCONS (posn)->cdr)->car)
#define POSN_SCROLLBAR_BUTTON POSN_BUFFER_POSN
#define POSN_WINDOW_POSN(posn) (XCONS (XCONS (XCONS (posn)->cdr)->cdr)->car)
#define POSN_TIMESTAMP(posn) \
  (XCONS (XCONS (XCONS (XCONS (posn)->cdr)->cdr)->cdr)->car)

/* Some of the event heads.  */
extern Lisp_Object Qvscrollbar_part, Qvslider_part;
extern Lisp_Object Qvthumbup_part, Qvthumbdown_part;
extern Lisp_Object Qhscrollbar_part, Qhslider_part;
extern Lisp_Object Qhthumbleft_part, Qhthumbright_part;

/* Properties on event heads.  */
extern Lisp_Object Qevent_kind, Qevent_symbol_elements;

/* Getting an unmodified version of an event head.  */
#define EVENT_HEAD_UNMODIFIED(event_head) \
  (Fcar (Fget ((event_head), Qevent_symbol_elements)))

/* The values of Qevent_kind properties.  */
extern Lisp_Object Qfunction_key, Qmouse_click, Qmouse_movement;
extern Lisp_Object Qscrollbar_click;

/* Getting the kind of an event head.  */
#define EVENT_HEAD_KIND(event_head) \
  (Fget ((event_head), Qevent_kind))

/* Symbols to use for non-text mouse positions.  */
extern Lisp_Object Qmode_line, Qvertical_line;

/* User-supplied string to translate input characters through.  */
extern Lisp_Object Vkeyboard_translate_table;

