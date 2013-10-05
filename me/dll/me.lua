--ZJH_LUA_PATH = "D:/doc/zjh/code/zjh_client/trunk/djh/Src/lua/"
--package.path = ZJH_LUA_PATH .. "?.lua;" .. package.path
print("Enter me.lua\n")

dbg = {
}

function dbg.message(fmt, ...)
	dbgmessage( string.format( fmt, ... ) )
end

function dbg.error(fmt, ...)
	dbgerror( string.format( fmt, ... ) )
end

local	DT_TICK_DATA	= 2					--分笔
local	DT_MIN1_DATA	= 3					--1
local	DT_MIN5_DATA	= 4					--5					
local	DT_MIN15_DATA	= 5					--15
local	DT_MIN30_DATA	= 6					--30
local	DT_MIN60_DATA	= 7					--60
local	DT_DAY_DATA		= 8					--日
local	DT_WEEK_DATA	= 9					--周
local	DT_MONTH_DATA	= 10				--月
local	DT_MULTI_DATA	= 11				--多日

local	FI_TEST			= 1
local	FI_TEST2		= 2



local	lastStkLabel	= ""
local	lastDataType	= 0
local	lastDatas		= {}
--A:"me@CALLFUNCVAR"(CLOSE,1,2,3);	
function CallFunc()
	dbg.message("Enter CallFunc()\n")
	
	UpdateStk()
	DoFunc()
	dbg.message("Exit CallFunc()\n")
end

function UpdateStk()
	dbg.message("Enter UpdateStk()\n")
	local stkLabel = EnvGetStkLabel()
	local dataType = EnvGetDataType()
	if lastStkLabel == stkLabel then
		if lastDataType == dataType then
			dbg.message("Exit UpdateStk()\n")
			return
		end
	end
	lastStkLabel = stkLabel
	lastDataType = dataType
	
	local nNumData = EnvGetNumData()
	for i=0,nNumData-1,1 do
		local data = {}
		data.open		= EnvGetDataOpen(i)
		data.high		= EnvGetDataHigh(i)
		data.low		= EnvGetDataLow(i)
		data.close		= EnvGetDataClose(i)
		data.volume		= EnvGetDataVolume(i)
		lastDatas[i]	= data
	end
	
	Normalize()
	Fractal()
--[[	
	for i=0,nNumData-1,1 do
		local data = lastDatas[i]		
		dbg.message("UpdateStk[%d]: %0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n", i, data.open, data.high, data.low, data.close, data.volume)
	end
--]]
	
	dbg.message("Exit UpdateStk()\n")
end

function DoFunc()
	dbg.message("Enter DoFunc()\n")
	
	if EnvIsParamValid(2) == false then
		dbg.message("Exit DoFunc()\n")
	end
	
	local funcId = EnvGetParamValueInt(2)
	dbg.message("funcId:%d\n", funcId)
	
	if funcId == FI_TEST then
		Test1()
	end
	if funcId == FI_TEST2 then
		Test2()
	end
	
	dbg.message("Exit DoFunc()\n")
end

function Test1()
	dbg.message("Enter Test1()\n")
	local nNumData = EnvGetNumData()
	for i=0,nNumData-1,1 do
		local data = lastDatas[i]
		--EnvSetDataResult(i,data.volume)
		EnvSetDataResult(i,data.normal.high)
	end
	
	
	dbg.message("Exit Test1()\n")	
end

function Test2()
	dbg.message("Enter Test2()\n")
	local nNumData = EnvGetNumData()
	for i=0,nNumData-1,1 do
		local data = lastDatas[i]
		EnvSetDataResult(i,data.normal.low)
	end	
	dbg.message("Exit Test2()\n")	
end

function sample()
	dbg.message("Enter sample()\n")
	dbg.message("Exit sample()\n")
end

function Normalize()
	dbg.message("Enter Normalize()\n")
	
	local nNumData = EnvGetNumData()
	if nNumData > 2 then
		local indx = 0
		
		--find first normalize point
		for i=indx,nNumData-3,1 do
			local d0 = lastDatas[i]
			local d1 = lastDatas[i+1]
			
			if d0.high < d1.high and d0.low < d1.low then
				break;
			end
			if d0.high > d1.high and d0.low > d1.low then
				break;
			end
			AddNormalData(i, d0.high, d0.low, -1)
			indx = i+1
		end
		
		if indx < nNumData then
			AddNormalData(indx, lastDatas[indx].high, lastDatas[indx].low, 0)
		end
		
		for i=indx,nNumData-3,1 do
			local d0 = lastDatas[i]
			local d1 = lastDatas[i+1]
			local d2 = lastDatas[i+2]
			
			if d1.high <= d2.high and d1.low >= d2.low then
				AddNormalData(i+1, d1.high, d1.low, 1)
				if d0.high > d1.high then
					d2.high = d1.high
				else
					d2.low = d1.low
				end
			elseif d1.high >= d2.high and d1.low <= d2.low then
				AddNormalData(i+1, d1.high, d1.low, 1)
				if d0.high > d1.high then
					d2.low = d1.low
				else
					d2.high = d1.high
				end
			else
				AddNormalData(i+1, d1.high, d1.low, 0)
			end	
			indx = i+1			
		end
		
		for i=indx+1,nNumData-1,1 do
			AddNormalData(i, lastDatas[i].high, lastDatas[i].low, 0)
		end
	end
		
	
	dbg.message("Exit Normalize()\n")
end

function Fractal()
--[[
0	非顶底
-1	底
1	顶
-2	底中继
2	顶中继
--]]
	dbg.message("Enter Fractal()\n")
	local validNum = 0
	local validData = {}
	local nNumData = EnvGetNumData()
	for i=0,nNumData-1,1 do
		local data		= lastDatas[i]
		if data.normal.type ~= -1 then
			validData[validNum] = i
			validNum = validNum + 1
		end
		lastDatas[i].fractal = 0
	end
	
	for i=0+1,validNum-1-1,1 do
		local index0	= validData[i-1]
		local index1	= validData[i]
		local index2	= validData[i+1]
		local data0		= lastDatas[index0]
		local data1		= lastDatas[index1]
		local data2		= lastDatas[index2]
		
		if data1.high > data0.high and data1.high > data2.high then
			lastDatas[index1].fractal = 1
		end
		if data1.low < data0.low and data1.low < data2.low then
			lastDatas[index1].fractal = -1
		end
	end
	
	validNum = 0
	for i=0,nNumData-1,1 do
		local data		= lastDatas[i]
		if data.fractal ~= 0 then
			validData[validNum] = i
			validNum = validNum + 1
		end
	end
	
	local lastFractalIndex = validData[0]
	for i=0+1,validNum-1,1 do
		local index	= validData[i]		
		local lastFractal	= lastDatas[lastFractalIndex]
		local thisFractal	= lastDatas[index]
		
		if lastFractal.fractal == thisFractal.fractal then
			if lastFractal.fractal == 1 then
				if lastFractal.high > thisFractal.high then
					lastDatas[index].fractal = 2
				else
					lastDatas[lastFractalIndex].fractal = 2
					lastFractalIndex = index
				end
			else
				if lastFractal.low < thisFractal.low then
					lastDatas[index].fractal = -2
				else
					lastDatas[lastFractalIndex].fractal = -2
					lastFractalIndex = index
				end
			end		
		else
			lastFractalIndex = validData[i]
		end
	end
	
	dbg.message("Exit Fractal()\n")
end

function AddNormalData(index, high, low, normalType)
--[[
0	不变
-1	被合并
1	合并后
--]]
	lastDatas[index].normal = {}
	lastDatas[index].normal.high	= high
	lastDatas[index].normal.low		= low
	lastDatas[index].normal.type	= normalType
end

function MakeBI()
	dbg.message("Enter MakeBI()\n")
	for i=0,nNumData-1,1 do
		local data = lastDatas[i]		
		dbg.message("UpdateStk[%d]: %0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n", i, data.open, data.high, data.low, data.close, data.volume)
	end
	
	dbg.message("Exit MakeBI()\n")	
end


function Test()
	dbg.message("Enter Test()\n")
	local stkLabel = EnvGetStkLabel()
	local dataType = EnvGetDataType()
	local nNumData = EnvGetNumData()
	dbg.message("[%s][%d][%d]\n", stkLabel, dataType, nNumData)
	for i=0,nNumData-1,1 do
		local nOpen		= EnvGetDataOpen(i)
		local nHigh		= EnvGetDataHigh(i)
		local nLow		= EnvGetDataLow(i)
		local nClose	= EnvGetDataClose(i)
		local nVolume	= EnvGetDataVolume(i)
		
		EnvSetDataResult(i,nVolume)
		
		dbg.message("ohlcv[%d]: %0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n", i, nOpen, nHigh, nLow, nClose, nVolume)
		
		if EnvIsParamValid(1) then
			local param1Start = EnvGetParam1Start()
			dbg.message("param1Start:%d\n", param1Start)
			dbg.message("Param1:%0.2f\n", EnvGetParamValue(1,0))
		end
		
		if EnvIsParamValid(2) then
			dbg.message("Param2:%0.2f\n", EnvGetParamValue(2))
		end
		
		if EnvIsParamValid(3) then
			dbg.message("Param3:%0.2f\n", EnvGetParamValue(3))
		end
		
		if EnvIsParamValid(4) then
			dbg.message("Param4:%0.2f\n", EnvGetParamValue(4))
		end
	end
	dbg.message("Exit Test()\n")
end