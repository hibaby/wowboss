--ZJH_LUA_PATH = "D:/doc/zjh/code/zjh_client/trunk/djh/Src/lua/"
--package.path = ZJH_LUA_PATH .. "?.lua;" .. package.path
print("Enter me.lua\n")

local common	= require "me.common"
local dbg		= common.dbg
local m			= require "me.main"

--A:"me@CALLFUNCVAR"(CLOSE,1,2,3);	
function CallFunc()
	dbg.message("Enter CallFunc()\n")
	m.OnCallFunc()
	dbg.message("Exit CallFunc()\n")
end