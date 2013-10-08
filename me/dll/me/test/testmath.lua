local common	= require "me.common"
local dbg		= common.dbg
local stock		= require "me.stock"
local math		= require "me.math"

function testMA()
	local d		= {1,  2, 3, 4,  5,  6, 7,  8, 9, 10, 11, 12, 13}
	local r5	= {1,1.5, 2,2.5, 3,  4, 5,  6, 7,  8,  9, 10, 11}
	local r10	= {1,1.5, 2,2.5, 3,3.5, 4,4.5, 5,5.5,6.5,7.5,8.5}
	
	local dd	= {}
	local rr5	= {}
	local rr10	= {}
	local num = 13
	for i=0,num-1,1 do
		--dbg.message("testMA %d, %f, %f, %f\n", i, d[i+1], r5[i+1], r10[i+1])
		dd[i]	= d[i+1]
		rr5[i]	= r5[i+1]
		rr10[i]	= r10[i+1]
	end
	
	
	local ma5	= math.MA( dd, num, 5 )
	local ma10	= math.MA( dd, num, 10 )
	for i=0,stock.DataNum()-1,1 do
		assert( ma5[i] == rr5[i] )
		assert( ma10[i] == rr10[i] )
	end
	
end

function testMath()
	testMA()
	dbg.message("testMath OK\n")
end

testMath()