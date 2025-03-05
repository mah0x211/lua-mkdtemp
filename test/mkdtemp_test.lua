local assert = require('assert')
local errno = require('errno')
local rmdir = require('rmdir')
local mkdtemp = require('mkdtemp')

--- check if a file or directory exists in this path
local function exists(file)
    local ok, err, code = os.rename(file, file)
    if not ok then
        if code == 13 then
            -- Permission denied, but it exists
            return true
        end
    end
    return ok, err
end

--- check if a directory exists in this path
local function isdir(path)
    return exists(path .. "/")
end

-- test that create tempdir
local tmpl = './tempdir_XXXXXX'
local pathname, err = assert(mkdtemp(tmpl))
assert.is_string(pathname)
assert.is_nil(err)
assert.not_equal(pathname, tmpl)
assert(isdir(pathname))
assert(rmdir(pathname, true))

-- test that returns an error
pathname, err = mkdtemp('./foo/bar/baz/tempfile')
assert.is_nil(pathname)
assert.equal(err.type, errno[err.code])

-- test that return ENAMETOOLONG error
local arr = {}
for i = 1, 1024 do
    arr[i] = 'foo'
end
tmpl = './' .. table.concat(arr, '/') .. '/tempdir_XXXXXX'
pathname, err = mkdtemp(tmpl)
assert.is_nil(pathname)
assert.equal(err.type, errno.ENAMETOOLONG)

-- test that throws an error
err = assert.throws(mkdtemp, {})
assert.match(err, '#1 .+ [(]string expected', false)
