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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// lua
#include <lua_errno.h>

static int mkdtemp_lua(lua_State *L)
{
    size_t len = 0;
    char *tmpl = (char *)luaL_checklstring(L, 1, &len);
    char *buf  = lua_newuserdata(L, len + 1);

    memcpy(buf, tmpl, len);
    buf[len] = 0;
    if (!mkdtemp(buf)) {
        lua_pushnil(L);
        lua_errno_new(L, errno, "mkdtemp");
        return 2;
    }

    lua_settop(L, 0);
    lua_pushlstring(L, buf, len);
    return 1;
}

LUALIB_API int luaopen_mkdtemp(lua_State *L)
{
    lua_errno_loadlib(L);
    lua_pushcfunction(L, mkdtemp_lua);
    return 1;
}
