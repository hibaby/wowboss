--全局函数导入到模块中
local common	= require "me.common"
local dbg		= common.dbg
local math		= require "me.math"
local stock		= require "me.stock"

--模块定义
local base = _ENV
local _mode = {}
local _ENV = _mode

------------------------------------------------------------------------
-- module
function Update()
	dbg.message("alligator.Update %d\n", stock.Close()[0])
	local ma5 = math.MA( stock.Close(), stock.DataNum(), 5 )
	
	local c = stock.Close()
	for i=0,stock.DataNum()-1,1 do
		dbg.message("alligator.Update %f, %f\n", c[i], ma5[i])
	end
end

------------------------------------------------------------------------

--返回模块
return _mode