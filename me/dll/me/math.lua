--全局函数导入到模块中
local common	= require "me.common"
local dbg		= common.dbg

--模块定义
local base = _ENV
local _mode = {}
local _ENV = _mode

------------------------------------------------------------------------
-- module
function Max(a, b)
	if a >= b then
		return a
	end
	
	return b
end

function Average(datas, num, index)
--	dbg.message("Average enter(%d,%d)\n", num, index)
	local result = 0
	
	local begin = Max(0, index-num+1)
	
	for i=begin,index,1 do
--		dbg.message("Average add %d,%f\n", i, datas[i])
		result = result + datas[i]
	end
	
--	dbg.message("Average exit(%f / %d = %f)\n", result, index-begin+1, result / (index-begin+1) )
	return result / (index-begin+1)
end

function MA(datas, num, period)
	local ma = {}
	for i=0,num-1,1 do
		ma[i] = Average(datas, period, i)
	end
	return ma
end

------------------------------------------------------------------------

--返回模块
return _mode