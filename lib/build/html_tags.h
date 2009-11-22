#pragma once

#include "common.h"

#define STRINGIZE(x) #x
#define _$(x) STRINGIZE(x)

#define TAG1_A(tag, attr) _TAG1A(tag, attr)
#define _TAG1A(x, attr) <x attr />

#define TAG1(tag) _TAG1(tag)
#define _TAG1(x) <x />

#define TAG(tag, content) _TAG(tag, content)
#define _TAG(x, c) <x>c</x>

#define TAG_A(tag, content, attr) _TAGA(tag, content, attr)
#define _TAGA(x, c, attr) <x attr>c</x>

#define DOCTYPE <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 

#define HTML_BEGIN <html xmlns="http://www.w3.org/1999/xhtml"> 
#define BODY_BEGIN <body>
#define HEAD_BEGIN <head>
#define HTML_END </html>
#define BODY_END </body>
#define HEAD_END </head>

#define COMMENT(content) <!-- content -->

#define TITLE(content) TAG(title, content)
#define H1(content) TAG(h1, content)
#define H2(content) TAG(h2, content)
#define H3(content) TAG(h3, content)

#define P(content) TAG(p, content)
#define BR TAG1(br)
#define EM(content) TAG(em, content)
#define IMG(source, alt_text) TAG1_A(img, src=#source alt=#alt_text)

#ifdef CONTEXT	
#define _A(link, name) TAG_A(a, name, href=#/##CONTEXT##link)
#else
#define _A(link, name) TAG_A(a, name, href=#link)
#endif

#define OL <ol>
#define UL <ul>
#define OL_END </ol>
#define UL_END </ul>
#define LI(content) TAG(li, content)

#define DIV(content) TAG(div, content)
#define DIV_ID(id_value, content) TAG_A(div, content, id=#id_value)
#define DIV_C(class_value, content) TAG_A(div, content, class=#class_value)
#define DIV_ID_C(id_value, class_value, content) TAG_A(div, content, id=#id_value class=#class_value)

#define TABLE <table>
#define TABLE_END </table>
#define TR(content) TAG(tr, content)
#define TD(content) TAG(td, content)
