--全局函数导入到模块中
local common	= require "me.common"
local dbg		= common.dbg

--模块定义
local base = _ENV
local _mode = {}
local _ENV = _mode

------------------------------------------------------------------------
-- module


------------------------------------------------------------------------

--返回模块
return _mode