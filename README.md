# lua-mkdtemp

[![test](https://github.com/mah0x211/lua-mkdtemp/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-mkdtemp/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-mkdtemp/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-mkdtemp)

create a unique temporary directory.


## Installation

```
luarocks install mkdtemp
```


## Usage

```lua
local mkdtemp = require('mkdtemp')
local pathname = assert(mkdtemp('/tmp/lua_XXXXXX'))
print(pathname) -- /tmp/lua_Wo6kHb
```


## Error Handling

the following functions return the `error` object created by https://github.com/mah0x211/lua-errno module.


## pathname, err = mkdtemp( tmpl )

this function takes the given file name template `tmpl` and overwrites a portion of it to create a directory name and the template directory.

see `man mkdtemp` for more details.

**Parameters**

- `tmpl:string`: filename template.

**Returns**

- `pathname:string`: pathname of the created directory.
- `err:error`: `error` object on failure.
