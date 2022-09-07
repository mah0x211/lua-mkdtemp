/**
 *  Copyright (C) 2022 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include <errno.h>
#include <stdio.h>
// lua
#include <lua_errno.h>

static size_t TMPL_BUFSIZ = PATH_MAX;
static char *TMPL_BUF     = NULL;

static int mkdtemp_lua(lua_State *L)
{
    size_t len = 0;
    char *tmpl = (char *)luaL_checklstring(L, 1, &len);

    if (len > TMPL_BUFSIZ) {
        lua_pushnil(L);
        errno = ENAMETOOLONG;
        lua_errno_new(L, errno, "mkdtemp");
        return 2;
    }
    tmpl      = memcpy(TMPL_BUF, tmpl, len);
    tmpl[len] = 0;

    if (!mkdtemp(tmpl)) {
        lua_pushnil(L);
        lua_errno_new(L, errno, "mkdtemp");
        return 2;
    }

    lua_settop(L, 0);
    lua_pushlstring(L, tmpl, len);
    return 1;
}

LUALIB_API int luaopen_mkdtemp(lua_State *L)
{
    long pathmax = pathconf(".", _PC_PATH_MAX);

    lua_errno_loadlib(L);

    // set the maximum number of bytes in a pathname
    if (pathmax != -1) {
        TMPL_BUFSIZ = pathmax;
    }
    // allocate the buffer for mkdtemp
    TMPL_BUF = lua_newuserdata(L, TMPL_BUFSIZ + 1);
    // holds until the state closes
    luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushcfunction(L, mkdtemp_lua);
    return 1;
}
