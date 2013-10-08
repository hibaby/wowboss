
--全局函数导入到模块中
local common	= require "me.common"
local dbg		= common.dbg
local stock		= require "me.stock"
local alligator	= require "me.alligator"


--模块定义
local base = _ENV
local _mode = {}
local _ENV = _mode

------------------------------------------------------------------------
-- module
function OnStkUpdate()
	dbg.message("OnStkUpdate\n")
	alligator.Update()
end

function OnCallFunc()
	stock.UpdateStk( OnStkUpdate )
end

------------------------------------------------------------------------

--返回模块
return _mode
