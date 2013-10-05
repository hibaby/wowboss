--全局函数导入到模块中
local string		= string
local dbgmessage	= dbgmessage
local dbgerror		= dbgerror

--模块定义
local base = _ENV
local _mode = {}
local _ENV = _mode

------------------------------------------------------------------------
-- module

dbg = {
}
function dbg.message(fmt, ...)
	dbgmessage( string.format( fmt, ... ) )
end

function dbg.error(fmt, ...)
	dbgerror( string.format( fmt, ... ) )
end

------------------------------------------------------------------------

--返回模块
return _mode