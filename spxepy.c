/*

Copyright (c) 2023 Eugenio Arteaga A.

Permission is hereby granted, free of charge, to any 
person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <spxe.h>

/* main spxe function interfaces */

static PyObject* spxepyStart(PyObject* self, PyObject* args)
{
    PyObject* fb;
    Px* pixbuf;
    const char* wintitle;
    int winwidth, winheight, scrwidth, scrheight;
    if (!PyArg_ParseTuple(args, "siiii", 
        &wintitle, &winwidth, &winheight, &scrwidth, &scrheight))
        return NULL;
    pixbuf = spxeStart(wintitle, winwidth, winheight, scrwidth, scrheight),
    fb = Py_BuildValue(
        "y#",
        (char*)pixbuf,
        (Py_ssize_t)(scrwidth * scrheight * sizeof(Px))
    ); 
    
    free(pixbuf);
    return fb;
}

static PyObject* spxepyRun(PyObject* self, PyObject* args)
{
    char* pixbuf;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "y#", &pixbuf, &size))
        return NULL;
    return Py_BuildValue("i", spxeRun((Px*)pixbuf));
}

static PyObject* spxepyEnd(PyObject* self, PyObject* args)
{
    return Py_BuildValue("i", spxeEnd((Px*)NULL));
}

/* keyboard controls python wrappers */

static PyObject* spxepyKeyDown(PyObject* self, PyObject* args)
{
    int key;
    if (!PyArg_ParseTuple(args, "i", &key))
        return NULL;
    return Py_BuildValue("i", spxeKeyDown(key));
}

static PyObject* spxepyKeyPressed(PyObject* self, PyObject* args)
{
    int key;
    if (!PyArg_ParseTuple(args, "i", &key))
        return NULL;
    return Py_BuildValue("i", spxeKeyPressed(key));
}

static PyObject* spxepyKeyReleased(PyObject* self, PyObject* args)
{
    int key;
    if (!PyArg_ParseTuple(args, "i", &key))
        return NULL;
    return Py_BuildValue("i", spxeKeyReleased(key));
}

static PyObject* spxepyKeyChar(PyObject* self, PyObject* args)
{
    return Py_BuildValue("C", (int)spxeKeyChar());
}

/* mouse control python wrappers */

static PyObject* spxepyMouseDown(PyObject* self, PyObject* args)
{
    int button;
    if (!PyArg_ParseTuple(args, "i", &button))
        return NULL;
    return Py_BuildValue("i", spxeMouseDown(button));
}

static PyObject* spxepyMousePressed(PyObject* self, PyObject* args)
{
    int button;
    if (!PyArg_ParseTuple(args, "i", &button))
        return NULL;
    return Py_BuildValue("i", spxeMousePressed(button));
}

static PyObject* spxepyMouseReleased(PyObject* self, PyObject* args)
{
    int button;
    if (!PyArg_ParseTuple(args, "i", &button))
        return NULL;
    return Py_BuildValue("i", spxeMouseReleased(button));
}

static PyObject* spxepyMousePos(PyObject* self, PyObject* args)
{
    int x, y;
    spxeMousePos(&x, &y);
    return Py_BuildValue("(ii)", x, y);
}

static PyObject* spxepyMouseVisible(PyObject* self, PyObject* args)
{
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    spxeMouseVisible(n);
    Py_RETURN_NONE;
}

/* time wrapper */

static PyObject* spxepyTime(PyObject* self, PyObject* args)
{
    double t = spxeTime();
    return Py_BuildValue("d", t);
}

/* screen and window size */

static PyObject* spxepyScreenSize(PyObject* self, PyObject* args)
{
    int w, h;
    spxeScreenSize(&w, &h);
    return Py_BuildValue("(ii)", w, h);
}

static PyObject* spxepyWindowSize(PyObject* self, PyObject* args)
{
    int w, h;
    spxeWindowSize(&w, &h);
    return Py_BuildValue("(ii)", w, h);
}

/* get and set functions */

static PyObject* spxepyGet(PyObject* self, PyObject* args)
{
    char* pixbuf;
    int w, h, x, y;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "y#ii", &pixbuf, &size, &x, &y))
        return NULL;
    
    spxeScreenSize(&w, &h);
    Px p = *(Px*)(pixbuf + ((y * w) + x) * sizeof(Px));
    return Py_BuildValue("(iiii)", (int)p.r, (int)p.g, (int)p.b, (int)p.a);
}

static PyObject* spxepySet(PyObject* self, PyObject* args)
{
    char* pixbuf;
    Py_ssize_t size, index;
    int w, h, x, y, r, g, b, a;
    if (!PyArg_ParseTuple(args, "y#iiiiii", &pixbuf, &size, &x, &y, &r, &g, &b, &a))
        return NULL;
    
    spxeScreenSize(&w, &h);
    index = ((y * w) + x) * sizeof(Px);
    pixbuf[index + 0] = (uint8_t)r;
    pixbuf[index + 1] = (uint8_t)g;
    pixbuf[index + 2] = (uint8_t)b;
    pixbuf[index + 3] = (uint8_t)a;
    
    Py_RETURN_NONE;
}

static PyObject* spxepyClear(PyObject* self, PyObject* args)
{
    char* buf;
    Px* pixbuf, p;
    Py_ssize_t size;
    int w, h, x, y, r, g, b, a;
    if (!PyArg_ParseTuple(args, "y#iiii", &buf, &size, &r, &g, &b, &a))
        return NULL;
    
    pixbuf = (Px*)buf;
    spxeScreenSize(&w, &h);
    p.r = (uint8_t)r;
    p.g = (uint8_t)g;
    p.b = (uint8_t)b;
    p.a = (uint8_t)a;
    for (x = 0; x < w; ++x)
        pixbuf[x] = p;
    for (y = 1; y < h; ++y)
        memcpy(pixbuf + y * w, pixbuf, w * sizeof(Px));
    
    Py_RETURN_NONE;
}

/* python module C API setup */

static PyMethodDef spxeMethods[] = {
    {"start", spxepyStart, METH_VARARGS, "Initialize spxe and retrieve a pixel buffer"},
    {"run", spxepyRun, METH_VARARGS, "Render pixel buffer and check if still running"},
    {"end", spxepyEnd, METH_VARARGS, "Uninitialize spxe and close window"},
    {"keyDown", spxepyKeyDown, METH_VARARGS, "Check if key was held down"},
    {"keyPressed", spxepyKeyPressed, METH_VARARGS, "Check if key was pressed"},
    {"keyReleased", spxepyKeyReleased, METH_VARARGS, "Check if key was released"},
    {"keyChar", spxepyKeyChar, METH_VARARGS, "Get last pressed character in keyboard"},
    {"mouseDown", spxepyMouseDown, METH_VARARGS, "Check if mouse button was held down"},
    {"mousePressed", spxepyMousePressed, METH_VARARGS, "Check if mouse button was pressed"},
    {"mouseReleased", spxepyMouseReleased, METH_VARARGS, "Check if mouse button was released"},
    {"mousePos", spxepyMousePos, METH_VARARGS, "Get pixel position of mouse in the screen"},
    {"mouseVisible", spxepyMouseVisible, METH_VARARGS, "Switch visibility of mouse cursor"},
    {"time", spxepyTime, METH_VARARGS, "Get time since spxe was initialized in seconds"},
    {"screenSize", spxepyScreenSize, METH_VARARGS, "Get size of screen pixel buffer"},
    {"windowSize", spxepyWindowSize, METH_VARARGS, "Get size of rendering window in pixels"},
    {"get", spxepyGet, METH_VARARGS, "Get RGBA value of pixel at position x, y"},
    {"set", spxepySet, METH_VARARGS, "Set RGBA value of pixel at position x, y"},
    {"clear", spxepyClear, METH_VARARGS, "Clear pixel buffer to RGBA values"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef spxePy = {
    PyModuleDef_HEAD_INIT,
    "spxepy",
    "",
    -1,
    spxeMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyObject* PyInit_spxepy(void)
{
    return PyModule_Create(&spxePy);
}

